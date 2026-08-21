#include "Core/CPUCore.h"
#include "FileManager/Assembler.h"

int main(){
    try
    {
        CPU cpu0(256, 10, 20);
        Assembler build;

        auto program = build.GetFile();
        cpu0.LoadProgram(program);
        cpu0.RunCPU();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
