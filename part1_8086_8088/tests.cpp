#include <iostream>

#include "ntest.hpp"
#include "term.hpp"
#include "primitives.hpp"
#include "decode.hpp"

int main() {
  using namespace term;

  // ntest init
  {
    auto const res = ntest::init();
    u64 const total = res.num_files_removed + res.num_files_failed_to_remove;

    if (total > 0)
      printf(FG_MAGENTA, "ntest: ");
    if (res.num_files_removed)
      printf(FG_MAGENTA, "%zu residual files removed ", res.num_files_removed);
    if (res.num_files_failed_to_remove > 0)
      printf(FG_YELLOW, "(%zu residual files failed to be removed)", res.num_files_failed_to_remove);
    if (total > 0)
      std::printf("\n");
  }

  ntest::assert_cstr(
    "mov cx, bx\n",

    decode_8086_instruction_stream(
      ntest::extract_binary_file("bin/single_register_mov.bin")).c_str()
  );

  ntest::assert_cstr(
    "mov cx, bx\n"
    "mov ch, ah\n"
    "mov dx, bx\n"
    "mov si, bx\n"
    "mov bx, di\n"
    "mov al, cl\n"
    "mov ch, ch\n"
    "mov bx, ax\n"
    "mov bx, si\n"
    "mov sp, di\n"
    "mov bp, ax\n",

    decode_8086_instruction_stream(
      ntest::extract_binary_file("bin/many_register_mov.bin")).c_str()
  );

  // ntest output
  {
    auto const res = ntest::generate_report("decode_8086", [](ntest::assertion const &a, bool const passed) {
      if (!passed)
        printf(FG_RED, "failed: %s:%zu\n", a.loc.file_name(), a.loc.line());
    });

    if ((res.num_fails + res.num_passes) == 0) {
      printf(FG_BRIGHT_YELLOW, "No tests defined");
    } else {
      if (res.num_fails > 0) {
        printf(FG_BRIGHT_RED,   "%zu failed", res.num_fails);
        std::printf(" | ");
        printf(FG_BRIGHT_GREEN, "%zu passed", res.num_passes);
      } else
        printf(FG_BRIGHT_GREEN, "All %zu tests passed", res.num_passes);
    }
    std::printf("\n\n");
  }
}