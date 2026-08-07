#ifndef ASM_H
#define ASM_H

#include "../Miscs/FlagFormat.h"
#include "../Core/InstructionFormat.h"
#include "../Miscs/Utils.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

class Assembler
{
private:
    void TrimSpaces         (std::string &line);
    void TrimLabel          (std::string &line);
    void TrimComments       (std::string &line);
    void CapitalizeOpCode   (std::string& operand);
    int ManualStringToInt   (const std::string &token);
    int GetFlagValueByToken (char &token);
    int ParseFlagMask       (const std::string &token);
    int parseOperand        (const std::string& token, const std::map<std::string,int>& labelTable);

    std::vector<Instruction> ParseFile (const std::string &filepath);
public:
    std::vector<Instruction> GetFile   ();
};

#endif