#include "BitMaskFlag.h"

void BitMaskFlag::SetFlag(int flag){
    flagCurrentValue |= flag;
}

void BitMaskFlag::RemoveFlag(int flag){
    flagCurrentValue &= ~flag;
}

bool BitMaskFlag::HasFlag(int flag){
    return (flagCurrentValue & flag) != 0;
}