#ifndef INSTR_H
#define INSTR_H

#include <string>
#include <cstdint>

struct Instruction{
    std::string op;
    uint8_t r0, r1, r2;
};

#endif