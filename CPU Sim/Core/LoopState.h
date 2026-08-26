#ifndef STATE_H
#define STATE_H

#include <cstdint>

enum LoopStageFlag {
    MainLoop = 1 << 0,
    UpdateRender = 1 << 1
};

class LoopState
{
private:
    /* data */
public:
    uint8_t currentState = 0;

    bool HasState(LoopStageFlag state);
    void SetUpState(LoopStageFlag state, bool set);
};

#endif