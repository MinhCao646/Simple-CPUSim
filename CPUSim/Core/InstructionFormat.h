#ifndef INSTR_H
#define INSTR_H

#include<string>

struct Instruction{
    std::string op;
    int r0, r1, r2;
};

#endif