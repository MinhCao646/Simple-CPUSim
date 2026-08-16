#include "CPUCore.h"

#include <iostream>
#include <string>

void CPU::LoadProgram(const std::vector<Instruction> &program){
    programMemory = program;
}

Instruction CPU::Fetch(){
    if(programCounter > programMemory.size() - 1 || programCounter < 0)
        throw std::runtime_error("Error, program counter out of bound.");

    return programMemory.at(programCounter++);
}

void CPU::DecodeAndExecute(const Instruction &instr){
    std::string op = instr.op;

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
            bit.SetFlag(static_cast<int>(FlagCode::Equal));
            bit.RemoveFlag(static_cast<int>(FlagCode::Greater));
            bit.RemoveFlag(static_cast<int>(FlagCode::Less));
        }
        else if (memory[instr.r0] > memory[instr.r1]){
            bit.RemoveFlag(static_cast<int>(FlagCode::Equal));
            bit.SetFlag(static_cast<int>(FlagCode::Greater));
            bit.RemoveFlag(static_cast<int>(FlagCode::Less));
        }
        else{
            bit.RemoveFlag(static_cast<int>(FlagCode::Equal));
            bit.RemoveFlag(static_cast<int>(FlagCode::Greater));
            bit.SetFlag(static_cast<int>(FlagCode::Less));
        }
    }
    else if (op == "DLY"){ //Delay execution for r0 milliseconds
        //Delay(instr.r0);
    }
    else if (op =="JMP"){ ///Jump to address r0
        programCounter = instr.r0;
    }
    else if (op == "BRH"){ /*Branch if flag is equal to r1, jump to address r0, bit masking for flag comparison:
                             0001 (1) equal, 0010 (2) greater, 0100 (4) less, 0011 (3) greater or equal, 0101 (5) less or equal, 0110 (6) not equal*/
        if (bit.HasFlag(instr.r1))
            programCounter = instr.r0;
    }
    else if (op == "CALL"){ ///Call subroutine at address r0, push current PC to stack
        stack.PushStack(programCounter);
        programCounter = instr.r0;
    }
    else if (op == "CALLC"){ //Call subroutine with condition, push current PC to stack if flag matches r1, jump to address r0
        if (bit.HasFlag(instr.r1)){
            stack.PushStack(programCounter);
            programCounter = instr.r0;
        }
    }
    else if (op == "RET"){ ///Return from subroutine, pop address from stack to PC
        programCounter = stack.PopStack();
    }
    else if (op == "HLT"){ ///Halt the CPU
        running = false;
    }
    else if (op == "GSCRY"){ ///Get screen height (y-axis)
        memory[instr.r0] = rend.GetScreenY();
    }
    else if (op == "GSCRX"){ //Get screen width (x-axis)
        memory[instr.r0] = rend.GetScreenX();
    }
    else if (op == "SCR"){
        if (memory[instr.r0] < 0 || memory[instr.r0] >= (int)rend.GetScreenY() || memory[instr.r1] < 0 || memory[instr.r1] >= (int)rend.GetScreenX()){
            throw std::runtime_error("Invalid screen coordinates");
        }
        int y = memory[instr.r0];
        int x = memory[instr.r1];
        //char symbol = '#';
        //rend.AssignPixel(&y, &x, symbol, memory[instr.r2]);
        wchar_t symbol = L'█';
        rend.AssignPixelForUnicode(&y, &x, symbol, memory[instr.r2]);
    }
    else if (op == "DSCR"){ ///Display screen data
        //rend.OutputFrameBuffer();
        rend.OutputFrameBufferUnicode();
    }
    else if (op == "CLS"){ ///Clear screen data
        rend.ResetPixelValue();
    }
    else if (op == "INPUT"){ ///Input data to memory at address r0
        memory[instr.r0] = GetInput();
    }
    else if (op == " "){
            //NOP
    }
    else{
        throw std::runtime_error("Unknown operator: " + op);
    }
}

void CPU::RunCPU(){
    Instruction instr;
    while (running)
    {
        instr = Fetch();
        DecodeAndExecute(instr);
    }
}
