#include "Assembler.h"

void Assembler::TrimSpaces         (std::string &line){
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
}

void Assembler::TrimLabel          (std::string &line){
    size_t pos = line.find('.');
    if (pos != std::string::npos){
        line.erase(pos, 1);
    }
}

void Assembler::TrimComments       (std::string &line){
    size_t pos = line.find('/');
    if (pos != std::string::npos){
        line.erase(pos);
    }
}

void Assembler::CapitalizeOpCode   (std::string& operand){
    std::transform(operand.begin(), operand.end(), operand.begin(), ::toupper);
}

int Assembler::ManualStringToInt   (const std::string &token){
    int value = 0;
    if (token.empty()) return 0;
    for (char c : token) {
        if (!std::isdigit(c) && c != '.' && c != '/') {
            throw std::runtime_error("Invalid integer: " + token);
        }
        else{
            value = value * 10 + (c - '0');
        }
    }
    return value;
}

int Assembler::GetFlagValueByToken (char &token){
    switch (token)
    {
    case 'e': case 'E':
        return static_cast<int>(FlagCode::Equal);
    case 'g': case 'G':
        return static_cast<int>(FlagCode::Greater);
    case 'l': case 'L':
        return static_cast<int>(FlagCode::Less);
    default:
        std::cerr << "\nUnknown token.";
        return 0;
    }
}

int Assembler::ParseFlagMask       (const std::string &token){
    int tokenSize = token.length();
    if(tokenSize > 1){
        int mask = 0;
        for (auto &s : token){
            char getChar = s;
            mask |= GetFlagValueByToken(getChar);
        }
        return mask;
    }
    else{
        char getChar = token.at(0);
        return GetFlagValueByToken(getChar);
    }
}

int Assembler::parseOperand        (const std::string& token, const std::map<std::string,int>& labelTable){
    if(token.empty()) return 0;

    if (token[0] == '.'){
        auto it = labelTable.find(token);
        if (it != labelTable.end()){
            return it->second;
        } else {
            throw std::runtime_error("Undefined label: " + token);
        }
    } else {
        if (token.at(0) >= '0' && token.at(0) <= '9')
            return ManualStringToInt(token);
        else
            return ParseFlagMask(token);
    }
}


std::vector<Instruction> Assembler::ParseFile(const std::string &filepath){
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    file.close();
    TrimComments(line);
    TrimSpaces(line);

    // Pass 1: collect labels
    std::map<std::string,int> labelTable;
    int instrIndex = 0;
    for (size_t i=0; i<lines.size(); ++i) {
        std::istringstream iss(lines[i]);
        std::string first;
        iss >> first;
        if (!first.empty() && (first[0] == '.' || first[0] == '/')) {
            labelTable[first] = instrIndex; // store label -> current instruction index
        } else {
            instrIndex++; // count actual instructions
        }
    }

    // Pass 2: build instructions
    std::vector<Instruction> program;
    for (size_t i=0; i<lines.size(); ++i) {
        std::istringstream iss(lines[i]);
        std::string op;
        iss >> op;
        if (!op.empty() && (op[0] == '.' || op[0] == '/')) continue; // skip labels
        Instruction instr;
        CapitalizeOpCode(op);
        instr.op = op;
        std::string r0, r1, r2;
        iss >> r0 >> r1 >> r2;
        // If operand is a label, replace with numeric address
        instr.r0 = parseOperand(r0, labelTable);
        instr.r1 = parseOperand(r1, labelTable);
        instr.r2 = parseOperand(r2, labelTable);
        program.push_back(instr);
    }
    TrimLabel(line);
    lines.clear();
    return program;
}


std::vector<Instruction> Assembler::GetFile(){
    std::string path;
    std::cout << "\nInput the asm file path: ";
    std::getline(std::cin, path);
    if(path.empty()){
        //Fallback to default
        path = "ASMCode.txt";
    }
    
    ClearConsole();
    return ParseFile(path);
}