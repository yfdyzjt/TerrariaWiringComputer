void reset() __attribute__((section(".text.reset")));
void reset()
{
    extern int main();
    main();
    *(volatile unsigned char *)0xFFFFFFFF = 0;
}
