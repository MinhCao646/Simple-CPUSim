#include "LoopState.h"

bool LoopState::HasState(LoopStageFlag state){
    return (currentState & state) != 0;
}

void LoopState::SetUpState(LoopStageFlag state, bool set){
    if(set){
        currentState |= static_cast<uint8_t>(state);
    }
    else
        currentState &= ~static_cast<uint8_t>(state);
}