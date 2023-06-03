CXX = g++
CXXFLAGS = -std=c++20 -Werror -Wall -Wextra -Wpedantic -Wformat -Wmissing-include-dirs -Wuninitialized -Wunreachable-code -Wshadow -Wconversion -Wsign-conversion -Wredundant-decls -Winit-self -Wswitch-default -Wfloat-equal -Wunused-parameter
LDFLAGS = -lstdc++

# 8086:
# 	@$(CXX) $(CXXFLAGS) -o 8086.elf decode_8086.cpp $(LDFLAGS)

tests:
	@$(CXX) $(CXXFLAGS) -o tests.elf tests.cpp decode.cpp ntest.cpp term.cpp $(LDFLAGS)
