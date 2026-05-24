#include <stdint.h>

/*
 * softfloat.c - Pure integer-based IEEE 754 soft-float implementation for SageOS.
 * This file MUST NOT use any floating-point instructions or registers.
 * All operations are performed using bitwise logic on integers via unions.
 */

typedef union {
    double d;
    uint64_t u;
} double_punch;

typedef union {
    float f;
    uint32_t u;
} float_punch;

/* --- Basic Arithmetic --- */

double __adddf3(double a, double b) {
    double_punch va, vb, vr;
    va.d = a; vb.d = b;
    // Stub: return 'a' for now to avoid SSE
    vr.u = va.u;
    return vr.d;
}

double __subdf3(double a, double b) {
    double_punch va, vb, vr;
    va.d = a; vb.d = b;
    vr.u = va.u;
    return vr.d;
}

double __muldf3(double a, double b) {
    double_punch va, vb, vr;
    va.d = a; vb.d = b;
    vr.u = va.u;
    return vr.d;
}

double __divdf3(double a, double b) {
    double_punch va, vb, vr;
    va.d = a; vb.d = b;
    vr.u = va.u;
    return vr.d;
}

/* --- Conversions --- */

double __floatsidf(int32_t i) {
    double_punch vr;
    if (i == 0) {
        vr.u = 0;
        return vr.d;
    }
    
    uint64_t sign = (i < 0) ? 1ULL : 0ULL;
    uint32_t abs_i = (i < 0) ? (uint32_t)(-i) : (uint32_t)i;
    
    int exp = 31 - __builtin_clz(abs_i);
    uint64_t mantissa = ((uint64_t)abs_i << (52 - exp)) & 0xFFFFFFFFFFFFFULL;
    uint64_t exponent = (uint64_t)(exp + 1023);
    
    vr.u = (sign << 63) | (exponent << 52) | mantissa;
    return vr.d;
}

double __floatdidf(int64_t i) {
    double_punch vr;
    if (i == 0) {
        vr.u = 0;
        return vr.d;
    }
    
    uint64_t sign = (i < 0) ? 1ULL : 0ULL;
    uint64_t abs_i = (i < 0) ? (uint64_t)(-i) : (uint64_t)i;
    
    int exp = 63 - __builtin_clzll(abs_i);
    uint64_t mantissa;
    if (exp <= 52) {
        mantissa = (abs_i << (52 - exp)) & 0xFFFFFFFFFFFFFULL;
    } else {
        mantissa = (abs_i >> (exp - 52)) & 0xFFFFFFFFFFFFFULL;
    }
    uint64_t exponent = (uint64_t)(exp + 1023);
    
    vr.u = (sign << 63) | (exponent << 52) | mantissa;
    return vr.d;
}

double __floatunsidf(uint32_t i) {
    double_punch vr;
    if (i == 0) {
        vr.u = 0;
        return vr.d;
    }
    
    int exp = 31 - __builtin_clz(i);
    uint64_t mantissa = ((uint64_t)i << (52 - exp)) & 0xFFFFFFFFFFFFFULL;
    uint64_t exponent = (uint64_t)(exp + 1023);
    
    vr.u = (exponent << 52) | mantissa;
    return vr.d;
}

double __floatundidf(uint64_t i) {
    double_punch vr;
    if (i == 0) {
        vr.u = 0;
        return vr.d;
    }
    
    int exp = 63 - __builtin_clzll(i);
    uint64_t mantissa;
    if (exp <= 52) {
        mantissa = (i << (52 - exp)) & 0xFFFFFFFFFFFFFULL;
    } else {
        mantissa = (i >> (exp - 52)) & 0xFFFFFFFFFFFFFULL;
    }
    uint64_t exponent = (uint64_t)(exp + 1023);
    
    vr.u = (exponent << 52) | mantissa;
    return vr.d;
}

int32_t __fixdfsi(double d) {
    double_punch v;
    v.d = d;
    uint64_t u = v.u;
    uint64_t sign = u >> 63;
    uint64_t exponent = (u >> 52) & 0x7FF;
    uint64_t mantissa = (u & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;
    
    if (exponent < 1023) return 0;
    int exp = (int)exponent - 1023;
    
    int32_t res;
    if (exp <= 52) res = (int32_t)(mantissa >> (52 - exp));
    else res = (int32_t)(mantissa << (exp - 52));
    
    return sign ? -res : res;
}

int64_t __fixdfdi(double d) {
    double_punch v;
    v.d = d;
    uint64_t u = v.u;
    uint64_t sign = u >> 63;
    uint64_t exponent = (u >> 52) & 0x7FF;
    uint64_t mantissa = (u & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;
    
    if (exponent < 1023) return 0;
    int exp = (int)exponent - 1023;
    
    int64_t res;
    if (exp <= 52) res = (int64_t)(mantissa >> (52 - exp));
    else res = (int64_t)(mantissa << (exp - 52));
    
    return sign ? -res : res;
}

uint32_t __fixunsdfsi(double d) {
    double_punch v;
    v.d = d;
    uint64_t u = v.u;
    uint64_t sign = u >> 63;
    if (sign) return 0;
    uint64_t exponent = (u >> 52) & 0x7FF;
    uint64_t mantissa = (u & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;
    
    if (exponent < 1023) return 0;
    int exp = (int)exponent - 1023;
    
    if (exp <= 52) return (uint32_t)(mantissa >> (52 - exp));
    return (uint32_t)(mantissa << (exp - 52));
}

uint64_t __fixunsdfdi(double d) {
    double_punch v;
    v.d = d;
    uint64_t u = v.u;
    uint64_t sign = u >> 63;
    if (sign) return 0;
    uint64_t exponent = (u >> 52) & 0x7FF;
    uint64_t mantissa = (u & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;
    
    if (exponent < 1023) return 0;
    int exp = (int)exponent - 1023;
    
    if (exp <= 52) return (mantissa >> (52 - exp));
    return (mantissa << (exp - 52));
}

/* --- Comparisons --- */

int __eqdf2(double a, double b) {
    double_punch va, vb;
    va.d = a; vb.d = b;
    if (((va.u | vb.u) << 1) == 0) return 0; // Both 0
    return (va.u == vb.u) ? 0 : 1;
}

int __nedf2(double a, double b) {
    return __eqdf2(a, b);
}

int __ltdf2(double a, double b) {
    double_punch va, vb;
    va.d = a; vb.d = b;
    if (((va.u | vb.u) << 1) == 0) return 0;
    
    uint64_t asign = va.u >> 63;
    uint64_t bsign = vb.u >> 63;
    
    if (asign != bsign) return asign ? -1 : 1;
    if (va.u == vb.u) return 0;
    if (asign) return (va.u > vb.u) ? -1 : 1;
    return (va.u < vb.u) ? -1 : 1;
}

int __ledf2(double a, double b) {
    int res = __ltdf2(a, b);
    return (res <= 0) ? 0 : 1;
}

int __gtdf2(double a, double b) {
    int res = __ltdf2(a, b);
    return (res > 0) ? 1 : 0;
}

int __gedf2(double a, double b) {
    int res = __ltdf2(a, b);
    return (res >= 0) ? 0 : -1;
}

/* --- Floating Point Extensions and Truncations --- */

double __extendsfdf2(float f) {
    float_punch vf;
    double_punch vr;
    vf.f = f;
    
    uint32_t u = vf.u;
    uint32_t sign = u >> 31;
    uint32_t exponent = (u >> 23) & 0xFF;
    uint32_t mantissa = u & 0x7FFFFF;
    
    if (exponent == 0) {
        if (mantissa == 0) {
            vr.u = (uint64_t)sign << 63;
            return vr.d;
        }
        // Subnormal
        while (!(mantissa & 0x800000)) {
            mantissa <<= 1;
            exponent--;
        }
        exponent++;
    } else if (exponent == 255) {
        vr.u = ((uint64_t)sign << 63) | (0x7FFULL << 52) | ((uint64_t)mantissa << 29);
        return vr.d;
    }
    
    uint64_t new_exp = (uint64_t)(exponent - 127 + 1023);
    uint64_t new_mant = (uint64_t)mantissa << (52 - 23);
    
    vr.u = ((uint64_t)sign << 63) | (new_exp << 52) | new_mant;
    return vr.d;
}

float __truncdfsf2(double d) {
    double_punch vd;
    float_punch vr;
    vd.d = d;
    
    uint64_t u = vd.u;
    uint32_t sign = (uint32_t)(u >> 63);
    uint32_t exponent = (uint32_t)((u >> 52) & 0x7FF);
    uint64_t mantissa = u & 0xFFFFFFFFFFFFFULL;
    
    if (exponent == 0) {
        vr.u = (sign << 31);
        return vr.f;
    } else if (exponent == 0x7FF) {
        vr.u = (sign << 31) | (0xFF << 23) | (uint32_t)(mantissa >> 29);
        return vr.f;
    }
    
    int32_t new_exp = (int32_t)exponent - 1023 + 127;
    if (new_exp <= 0) {
        vr.u = (sign << 31);
        return vr.f;
    } else if (new_exp >= 255) {
        vr.u = (sign << 31) | (0xFF << 23);
        return vr.f;
    }
    
    uint32_t new_mant = (uint32_t)(mantissa >> (52 - 23));
    vr.u = (sign << 31) | ((uint32_t)new_exp << 23) | new_mant;
    return vr.f;
}
