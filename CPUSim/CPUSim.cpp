#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;

struct Instruction
{
    string op;
    int r0;
    int r1;
    int r2;
};

enum FlagCode {
    Equal = 1 << 0,  // 0001
    Greater = 1 << 1, // 0010
    Less = 1 << 2,    // 0100
};

struct ScreenPixel
{
    int x;
    int y;
    int color;
};

class BitMaskFlag{

    private:
        int flagValue;
    public:
        BitMaskFlag() : flagValue(0) {}

        void setFlag(FlagCode flag) {
            flagValue |= flag;
        }

        void clearFlag(FlagCode flag) {
            flagValue &= ~flag;
        }

        bool isFlagSet(FlagCode flag) const {
            return (flagValue & flag) != 0;
        }

        int getFlagValue() const {
            return flagValue;
        }
};

class CPU {
    private:
        BitMaskFlag Flag;
        vector<int> memory;
        vector<Instruction> prog;
        vector<vector<int>> ScreenDisplay;
        int programCounter;
        bool running;
        int StackMem[100] = {0};

        void SetColor(int textColor, int bgColor){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, ((bgColor << 4) | textColor));
        }

        void ClearScreen(){
            #ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            DWORD count;
            DWORD cellCount;
            COORD homeCoords = {0, 0};

            if (hConsole == INVALID_HANDLE_VALUE) return;

            if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
            cellCount = csbi.dwSize.X * csbi.dwSize.Y;

            //Fill entire screen with spaces
            if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

            //Fill the entire screen with the current colors and attributes
            if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;

            //Move the cursor home
            SetConsoleCursorPosition(hConsole, homeCoords);
            #else
                std::cout << "\033[2J\033[H";
            #endif // _WIN32
        }

        void Delay(int milliseconds){
            #ifdef _WIN32
                Sleep(milliseconds);
            #else
                usleep(milliseconds * 1000); // usleep takes microseconds
            #endif
        }

    public:
    CPU(size_t memorySize = 256) : memory(memorySize, 0), programCounter(0), running(true), ScreenDisplay(10, vector<int>(20, 0)) {}

    void LoadProgram (const vector<Instruction>&program)
    {
        if (program.size() > memory.size()){
            throw runtime_error("Program too large for memory");
        }

        prog = program;
    };

    Instruction fetch ()
    {
        if(programCounter < 0 || programCounter >= (int)prog.size())
        {
            throw runtime_error("Program Counter out of range");
        }

        return prog[programCounter++];
    }

    int PopStack ()
    {
        int topStack = StackMem[0];

        for (int i = 0; i < 99; ++i)
        {
            StackMem[i] = StackMem[i + 1];
        }

        return topStack;
    };

    void PushStack ()
    {
        int temp = programCounter;

        for (int i = 99; i > 0; --i){
            StackMem[i] = StackMem[i - 1];
        }
        StackMem[0] = temp;
    };

    void DecodeAndExecute (const Instruction& instr)
    {
        string op = instr.op;

        if (op == "LDTM"){ ///Load to Memory with value of r1 at address r0
            memory[instr.r0] = instr.r1;
        }
        else if (op == "MOV"){ ///Move value of r1 to address r0
            memory[instr.r0] = memory[instr.r1];
        }
        else if (op == "ADD"){ ///Calculate with value of r0 and r1, store in r2
            memory[instr.r2] = memory[instr.r0] + memory[instr.r1];
        }
        else if (op == "SUB"){ ///Calculate with value of r0 and r1, store in r2
            memory[instr.r2] = memory[instr.r0] - memory[instr.r1];
        }
        else if (op == "MUL"){ ///Calculate with value of r0 and r1, store in r2
            memory[instr.r2] = memory[instr.r0] * memory[instr.r1];
        }
        else if(op == "DIV"){ ///Calculate with value of r0 and r1, store in r2
            if (memory[instr.r1] != 0)
                memory[instr.r2] = memory[instr.r0] / memory[instr.r1];
            else
                memory[instr.r2] = 0;
        }
        else if (op == "INC") { ///Increment value at address r0
            memory[instr.r0] = memory[instr.r0] + 1;
        }
        else if (op == "DEC"){ ///Decrement value at address r0
            memory[instr.r0] = memory[instr.r0] - 1;
        }
        else if (op == "AND"){
            memory[instr.r2] = memory[instr.r0] & memory[instr.r1];
        }
        else if (op == "OR"){ ///Calculate with value of r0 and r1, store in r2
            memory[instr.r2] = memory[instr.r0] | memory[instr.r1];
        }
        else if (op == "XOR"){ ///Calculate with value of r0 and r1, store in r2
            memory[instr.r2] = memory[instr.r0] ^ memory[instr.r1];
        }
        else if (op == "NOT"){ ///Calculate with value of r0, store in r1
            memory[instr.r1] = ~memory[instr.r0];
        }
        else if (op == "SHL"){ ///Shift left value at address r0, store in r2
            memory[instr.r2] = memory[instr.r0] << (instr.r1);
        }
        else if (op == "SHR"){ ///Shift right value at address r0, store in r2
            memory[instr.r2] = memory[instr.r0] >> (instr.r1);
        }
        else if (op == "CMP"){ ///Compare value at address r0 and r1, set flag accordingly
            if (memory[instr.r0] == memory[instr.r1]){
                Flag.setFlag(Equal);
                Flag.clearFlag(Greater);
                Flag.clearFlag(Less);
            }
            else if (memory[instr.r0] > memory[instr.r1]){
                Flag.clearFlag(Equal);
                Flag.setFlag(Greater);
                Flag.clearFlag(Less);
            }
            else{
                Flag.clearFlag(Equal);
                Flag.clearFlag(Greater);
                Flag.setFlag(Less);
            }
        }
        else if (op == "DLY"){ //Delay execution for r0 milliseconds
            Delay(instr.r0);
        }
        else if (op =="JMP"){ ///Jump to address r0
            programCounter = instr.r0;
        }
        else if (op == "BRH"){ /*Branch if flag is equal to r1, jump to address r0, bit masking for flag comparison:
                                 0001 (1) equal, 0010 (2) greater, 0100 (4) less, 0011 (3) greater or equal, 0101 (5) less or equal, 0110 (6) not equal*/
            if (static_cast<bool>(Flag.getFlagValue() & instr.r1))
                programCounter = instr.r0;
        }
        else if (op == "CALL"){ ///Call subroutine at address r0, push current PC to stack
            PushStack();
            programCounter = instr.r0;
        }
        else if (op == "CALLC"){ //Call subroutine with condition, push current PC to stack if flag matches r1, jump to address r0
            if (static_cast<bool>(Flag.getFlagValue() & instr.r1)){
                PushStack();
                programCounter = instr.r0;
            }
        }
        else if (op == "RET"){ ///Return from subroutine, pop address from stack to PC
            programCounter = PopStack();
        }
        else if (op == "HLT"){ ///Halt the CPU
            running = false;
        }
        else if (op == "SCR"){
            if (memory[instr.r0] < 0 || memory[instr.r0] >= (int)ScreenDisplay.size() || memory[instr.r1] < 0 || memory[instr.r1] >= (int)ScreenDisplay[0].size()){
                throw runtime_error("Invalid screen coordinates");
            }
            int y = memory[instr.r0];
            int x = memory[instr.r1];
            ScreenDisplay[y][x] = memory[instr.r2];
        }
        else if (op == "DSCR"){ ///Display screen data
            for (const auto& row : ScreenDisplay){
                for (const auto& pixel : row){
                    SetColor(0, pixel);
                    cout << "  ";
                    SetColor(7, 0); ///Reset to default color
                }
                cout << endl;
            }
        }
        else if (op == "CLS"){ ///Clear screen data
            for (auto& row : ScreenDisplay){
                for (auto& pixel : row){
                    pixel = 0; ///Reset to default color
                }
            }
            ClearScreen();
        }
        else if (op == "INPUT"){ ///Input data to memory at address r0
            if(kbhit()){
                char c = getch();
                memory[instr.r0] = static_cast<int>(c);
            }
        }
        else if (op == " "){
                //NOP
        }
        else{
            throw runtime_error("Unknown operator: " + op);
        }
    };

    void runCPU ()
    {
        while (running)
        {
            Instruction instr = fetch();
            DecodeAndExecute(instr);
        }
    };

    int GetMemoryData (int addr)
    {
        if (addr < 0 || addr >= (int)memory.size()){
            throw runtime_error("Invalid memory address");
        }
        return memory[addr];
    };
};

class GetCodeFromFile{
    private:
        void TrimSpaces (std::string &line){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        }

        void TrimLabel (string &line){
            size_t pos = line.find('.');
            if (pos != string::npos){
                line.erase(pos, 1);
            }
        }

        void TrimComments (string &line){
            size_t pos = line.find('/');
            if (pos != string::npos){
                line.erase(pos);
            }
        }

        void CapitalizeOpCode (std::string& operand){
            std::transform(operand.begin(), operand.end(), operand.begin(), ::toupper);
        }

        int ManualStringToInt (const string &token){
            int value = 0;
            if (token.empty()) return 0;
            for (char c : token) {
                if (!std::isdigit(c) && c != '.' && c != '/') {
                    throw runtime_error("Invalid integer: " + token);
                }
                else{
                    value = value * 10 + (c - '0');
                }
            }
            return value;
        }

        int GetFlagValueByToken(char &token){
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

        int ParseFlagMask (const std::string &token){
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

        int parseOperand (const std::string& token, const std::map<std::string,int>& labelTable){
            if(token.empty()) return 0;

            if (token[0] == '.'){
                auto it = labelTable.find(token);
                if (it != labelTable.end()){
                    return it->second;
                } else {
                    throw runtime_error("Undefined label: " + token);
                }
            } else {
                if (token.at(0) >= '0' && token.at(0) <= '9')
                    return ManualStringToInt(token);
                else
                    return ParseFlagMask(token);
            }
        }
    public:
    std::vector<Instruction> ReadCodeWithLabels(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Could not open file");

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) lines.push_back(line);
        }
        file.close();
        TrimSpaces(line);
        TrimComments(line);

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
};

int main() {

    try
    {
        GetCodeFromFile codeReader;
        std::vector<Instruction> program = codeReader.ReadCodeWithLabels("ASMCode.txt");

        CPU cpu0;
        cpu0.LoadProgram(program);
        cpu0.runCPU();
    } catch (const exception& e){
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
