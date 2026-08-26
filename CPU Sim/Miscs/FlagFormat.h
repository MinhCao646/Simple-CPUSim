#ifndef FLAG_H
#define FLAG_H

enum FlagCode{
    Equal = 1 << 0,  // 0001
    Greater = 1 << 1, // 0010
    Less = 1 << 2,    // 0100
};

#endif