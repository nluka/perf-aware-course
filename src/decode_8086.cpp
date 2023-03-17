#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#define CRASH_IF(cond) \
   if (cond) { \
      std::cout << "error, " << #cond << '\n'; \
      std::exit(1); \
   }

int main(int argc, char const **argv) {
   if (argc != 2) {
      puts("usage: decode_8086 <file_path>");
      return 1;
   }

   std::vector<uint8_t> instruction_stream;

   {
      char const *file_path = argv[1];
      [[maybe_unused]] auto const cwd = fs::current_path();
      if (!fs::is_regular_file(file_path)) {
         std::cout << "error, not a regular file\n";
         std::exit(1);
      }
      size_t const file_size = static_cast<size_t>(fs::file_size(file_path));
      instruction_stream.resize(file_size);
      std::ifstream file(file_path, std::ios::binary);
      CRASH_IF(!file);
      try {
         file.read(reinterpret_cast<char *>(instruction_stream.data()), file_size);
      } catch (std::exception const &except) {
         std::cout << "error reading file, " << except.what() << '\n';
         std::exit(1);
      }
      CRASH_IF(static_cast<size_t>(file.gcount()) != file_size);
   }

   CRASH_IF(instruction_stream.empty());
   CRASH_IF(instruction_stream.size() & 1);

   for (size_t i = 0; i < instruction_stream.size(); i += 2) {
      uint8_t const byte1 = instruction_stream[i];
      uint8_t const byte2 = instruction_stream[i + 1];

      bool const goodOpcode = (byte1 >> 2) & 0b100010;
      bool const destComesFirst = (byte1 & 0b10);
      bool const wide = (byte1 & 0b1);
      bool const registerToRegister = (byte2 >> 6) & 0b11;

      if (!goodOpcode) {
         std::cout << "error, bad opcode for instruction " << i << '\n';
         std::exit(1);
      }

      auto const decode_register = [](bool const wide, uint8_t const code, char *out) {
         if (wide) {
            char const *reg;
            if      ((code & 0b111) == 0b111) reg = "di";
            else if ((code & 0b110) == 0b110) reg = "si";
            else if ((code & 0b101) == 0b101) reg = "bp";
            else if ((code & 0b100) == 0b100) reg = "sp";
            else if ((code & 0b011) == 0b011) reg = "bx";
            else if ((code & 0b010) == 0b010) reg = "dx";
            else if ((code & 0b001) == 0b001) reg = "cx";
            else    /* 0b000 */               reg = "ax";
            memcpy(out, reg, 2);
         } else {
            if      ((code & 0b11) == 0b11) out[0] = 'b';
            else if ((code & 0b10) == 0b10) out[0] = 'd';
            else if ((code & 0b01) == 0b01) out[0] = 'c';
            else    /* 0b00 */              out[0] = 'a';

            bool const highestBitSet = code & 0b100;
            if (highestBitSet) out[1] = 'h';
            else               out[1] = 'l';
         }
      };

      uint8_t const dst_register_code = (destComesFirst ? (byte2 >> 3) : byte2) & 0b00000111;
      uint8_t const src_register_code = (destComesFirst ? (byte2) : (byte2 >> 3)) & 0b00000111;

      char dst_register_str[3] { '\0' };
      char src_register_str[3] { '\0' };
      decode_register(wide, dst_register_code, dst_register_str);
      decode_register(wide, src_register_code, src_register_str);

      std::cout << "mov " << dst_register_str << ", " << src_register_str << '\n';
   }

   return 0;
}
