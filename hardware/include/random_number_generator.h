#ifndef RANDOM_NUMBER
#define RANDOM_NUMBER

union RandomNumber
{
    unsigned int u32;
    unsigned short u16;
    unsigned char u8;
    int i32;
    short i16;
    char i8;
};

volatile const union RandomNumber rand __attribute__((section(__FILE_NAME__)));

#endif
