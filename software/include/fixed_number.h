#ifdef FIXED_POINT_SHIFT

typedef int fixed;

#define FIXED_MUL(a, b) ((fixed)(((long long)(a) * (b)) >> FIXED_POINT_SHIFT))
#define FIXED_DIV(a, b) ((fixed)(((long long)(a) << FIXED_POINT_SHIFT) / (b)))

#define FLOAT_TO_FIXED(x) ((fixed)((x) * (1 << FIXED_POINT_SHIFT)))
#define FIXED_TO_INT(x) ((x) >> FIXED_POINT_SHIFT)

#endif
