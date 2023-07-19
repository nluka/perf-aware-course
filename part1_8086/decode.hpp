#ifndef DECODE_HPP
#define DECODE_HPP

#include <string>
#include <vector>

#include "primitives.hpp"

std::string decode_8086_instruction_stream(std::vector<u8> const &instruction_stream);

#endif // DECODE_HPP
