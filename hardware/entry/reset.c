void reset() __attribute__((section(".text.reset")));
void reset()
{
    extern int main();
    main();
}
