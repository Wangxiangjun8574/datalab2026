/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    // 德摩根定律: x & y = ~( ~x | ~y )
    // 先对x,y各自取反，或运算，整体再取反，等价于按位与
    return ~(~x | ~y);
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    // XOR: 两位不相同则为1；相同为0
    // 相同分为两种：都为1 (x&y) 、都为0 (~x&~y)
    // 把两种相同的位取反后相与，保留不相同的位
    return ~(~x & ~y) & ~(x & y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    // 算术右移31位，拿到符号位，负数全1(0xFFFFFFFF)，正数/0全0
    int sx = x >> 31;
    int sy = y >> 31;

    // 情况1：两个都是负数，符号相同，返回1
    if (sx && sy) {
        return 1;
    }
    // 情况2：符号位不一样，一个负一个非负，返回0
    if (sx ^ sy) {
        return 0;
    }
    // 剩下：两者都不是负数（>=0）
    // 规则：0既不是正数也不是负数，只有两个都等于0才返回1
    if (!x) {
        // x是0：只有y也是0才返回1
        return !y;
    } else {
        // x>0，y>=0：y必须>0才算同符号
        return !!y;
    }
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    // 找到最高位1所在位置，就是log2(v)
    // 二分法：先判断高16位有没有1，再8位，4位，2位，1位
    int b16 = v > 0xFFFF;        // b16=1 代表高16位存在1，结果+16
    int v1 = v >> (b16 << 4);   // 如果高16有1，右移16位，只看剩下高位
    int b8 = v1 > 0xFF;         // 判断剩余部分高8位是否有1，+8
    int v2 = v1 >> (b8 << 3);
    int b4 = v2 > 0xF;          // 判断剩余高4位，+4
    int v3 = v2 >> (b4 << 2);
    int b2 = v3 > 0x3;          // 判断剩余高2位，+2
    int v4 = v3 >> (b2 << 1);
    int b1 = v4 > 0x1;          // 判断剩余最高1位，+1

    // 合并所有位，得到最高位的下标
    return (b16 << 4) | (b8 << 3) | (b4 << 2) | (b2 << 1) | b1;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    // 1字节 = 8bit，第n个字节偏移 = n*8
    int shift_n = n << 3;
    int shift_m = m << 3;

    // 提取n位置的1字节，掩码0xFF取出低8位
    int byte_n = (x >> shift_n) & 0xFF;
    int byte_m = (x >> shift_m) & 0xFF;

    // 掩码：把n和m两个字节位置全部置0
    int mask = (0xFF << shift_n) | (0xFF << shift_m);
    int x_cleared = x & ~mask;

    // 将两个字节交换回填到对应位置
    int result = x_cleared | (byte_n << shift_m) | (byte_m << shift_n);
    return result;
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    // 循环逐位取出最低位，放到结果的最低位，循环32次完成反转
    unsigned r = 0;
    int i;
    for (i = 32; i; i = i - 1) {
        r = (r << 1) | (v & 1); // 结果左移，把v当前最低位放进来
        v = v >> 1;             // v右移，处理下一位
    }
    return r;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    // 算术右移会在高位补符号位；逻辑右移高位补0
    int shifted = x >> n;
    // 构造掩码：高n位为0，剩下低位全1，用来把符号带来的高位1清零
    int mask = ~((1 << 31) >> n << 1);
    return shifted & mask;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    // 从最高bit开始，连续有多少个1
    // y = ~x，原bit是1的位置y就是0；找到第一个0的位置，就是连续1的个数
    int y = ~x;
    int b16 = !!(y >> 16) ^ 1; // 判断高16位是否全部是1（y>>16全0）
    int r = b16 << 4;
    y = y << (b16 << 4);       // 如果高16全1，把y左移16位，继续看后面

    int b8 = !!(y >> 24) ^ 1;  // 判断接下来8位是否全部1
    r = r | (b8 << 3);
    y = y << (b8 << 3);

    int b4 = !!(y >> 28) ^ 1;  // 判断接下来4位是否全部1
    r = r | (b4 << 2);
    y = y << (b4 << 2);

    int b2 = !!(y >> 30) ^ 1;  // 判断接下来2位是否全部1
    r = r | (b2 << 1);
    y = y << (b2 << 1);

    int b1 = !!(y >> 31) ^ 1;  // 判断剩下1位是否是1
    r = r | b1;

    // 如果y全0，代表原x全部是1，结果+1
    return r + !y;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    // 单精度浮点数：1符号位 | 8指数位 | 23尾数位，偏移量127
    int sign = (x >> 31) & 1; // 提取符号位，0正1负
    int exp = 158;            // 初始指数：127 + 31
    int frac = 0;
    if (x == 0) return 0;     // 0直接返回0浮点表示
    if (sign) x = -x;         // 负数转为正数处理

    // 左移直到最高位到达bit31，找到最高1的位置
    while (!(x & 0x80000000)) {
        x = x << 1;
        exp = exp - 1;
    }

    // 舍入：guard/round/sticky 3位，就近舍入到偶数
    int guard = (x >> 7) & 1;
    int round = (x >> 6) & 1;
    int sticky = !!(x & 0x3F);
    frac = (x >> 8) & 0x7FFFFF; // 提取23位尾数，丢弃隐藏的最高位1
    int inc = guard & (round | sticky | (frac & 1));
    frac = frac + inc;

    // 尾数进位溢出，尾数清零，指数+1
    if (frac >> 23) {
        frac = 0;
        exp = exp + 1;
    }
    // 拼接符号、指数、尾数
    return (sign << 31) | (exp << 23) | frac;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    unsigned sign = uf & 0x80000000;   // 符号位
    unsigned exp = uf & 0x7F800000;     // 8位指数部分
    unsigned frac = uf & 0x7FFFFF;      // 23位尾数
    // NaN / 无穷大，直接返回原值
    if (exp == 0x7F800000) return uf;
    // 非规格化数：指数=0，直接尾数左移一位 ×2
    if (exp == 0) {
        frac = frac << 1;
        // 左移后产生隐藏位，升级为规格化数
        if (frac & 0x800000) {
            return sign | 0x00800000 | (frac & 0x7FFFFF);
        }
        return sign | frac;
    }
    // 规格化数：乘以2等价于指数+1
    exp = exp + 0x00800000;
    // 指数变成全1，结果为无穷
    if (exp == 0x7F800000) return sign | exp;
    return sign | exp | frac;
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    // double 64bit：1符号，11指数(偏移1023)，52尾数
    unsigned sign = uf2 >> 31;
    unsigned exp = (uf2 >> 20) & 0x7FF;
    // 拼接隐藏位1 + 52位尾数
    unsigned val = 0x80000000u | ((uf2 & 0xFFFFF) << 11) | (uf1 >> 21);

    // 指数小于1023，绝对值小于1，向零舍入 → 0
    if (exp < 1023) return 0;
    // 指数太大，超过32位int范围 → 溢出返回0x80000000
    if (exp > 1054) return 0x80000000u;

    // 移位，把小数点放到整数位置
    val = val >> (1054 - exp);

    // 负数
    if (sign) {
        if (val > 0x80000000u) return 0x80000000u;
        return -val;
    }
    // 正数溢出判断
    if (val > 0x7FFFFFFF) return 0x80000000u;
    return val;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    // 2^x 用float表示：
    // 规格化范围：指数 -126 ~ 127
    int exp = x + 127;
    // x>127，超出最大规格化指数，返回正无穷
    if (x >= 128) return 0x7F800000;
    // 规格化区间，尾数全0，只需要设置指数
    if (x >= -126) return exp << 23;
    // 非规格化区间：-149 ~ -127，指数强制0，尾数放2^x
    if (x >= -149) return 1 << (x + 149);
    // 太小，低于最小可表示非规格数，返回0
    return 0;
}
