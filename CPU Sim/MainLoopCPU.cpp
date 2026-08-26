#include "Core/LoopState.h"
#include "Core/CPUCore.h"
#include "FileManager/Assembler.h"
#include "Display/DebugRenderer.h"
#include "Miscs/Utils.h"

int main(){
    CPU cpu0(121, 10, 60);
    Assembler build;
    RenderDebug debug;
    LoopState state;
    state.SetUpState(LoopStageFlag::MainLoop, true);
    auto program = build.GetFile();
    cpu0.LoadProgram(program);
    while (state.HasState(LoopStageFlag::MainLoop) && cpu0.running)
    {
        if(!state.HasState(LoopStageFlag::UpdateRender)){
            try
            {
                cpu0.RunCPU();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }

        if(!state.HasState(LoopStageFlag::UpdateRender) && cpu0.executed){
            state.SetUpState(LoopStageFlag::UpdateRender, true);
            cpu0.posRelative = cpu0.ScrWidth + cpu0.posScrX + 1;
            debug.OutputFrameBufferMemory(cpu0.memory, cpu0.posRelative, cpu0.posScrY);
            cpu0.posRelative = cpu0.ScrHeight + cpu0.posScrX + 8;
            uint16_t currentPC = cpu0.programCounter;
            debug.OutputFrameBufferOther(debug.PadString("PC: " + std::to_string(currentPC), 7), cpu0.posScrX, cpu0.posRelative);
            debug.OutputFrameBufferOther(debug.PadString("Opcode: " + cpu0.programMemory[currentPC].op, 14), cpu0.posScrX, cpu0.posRelative + 1);
            debug.OutputFrameBufferOther(debug.PadString("R0: " + std::to_string(cpu0.programMemory[currentPC].r0), 7), cpu0.posScrX, cpu0.posRelative + 2);
            debug.OutputFrameBufferOther(debug.PadString("R1: " + std::to_string(cpu0.programMemory[currentPC].r1), 7), cpu0.posScrX, cpu0.posRelative + 3);
            debug.OutputFrameBufferOther(debug.PadString("R2: " + std::to_string(cpu0.programMemory[currentPC].r2), 7), cpu0.posScrX, cpu0.posRelative + 4);
            Delay(700);
            state.SetUpState(LoopStageFlag::UpdateRender, false);
        }
    }
    
    return 0;
}