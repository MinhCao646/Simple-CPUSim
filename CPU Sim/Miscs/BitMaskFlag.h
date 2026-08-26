#ifndef BIT_MASK_H
#define BIT_MASK_H

class BitMaskFlag
{
private:
    int flagCurrentValue;
public:
    BitMaskFlag() : flagCurrentValue(0) {}
    
    void SetFlag    (int flag);
    void RemoveFlag (int flag);
    bool HasFlag    (int flag);
};

#endif