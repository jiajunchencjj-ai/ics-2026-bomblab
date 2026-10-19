#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    uint32_t low = (uint32_t)challenge;
    uint32_t middle = (uint32_t)(challenge >> 16);
    uint32_t high = (uint32_t)(challenge >> 32);

    uint32_t A = (low * 5u) ^ (high + 0xb514u);
    uint32_t B = (high + middle * 2u) ^ 0xb3c9u;
    uint32_t C = (low + 0x4ccfu) ^ B;
    values[0] = A ^ C;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    uint32_t a = (uint32_t)((challenge >> 13) & 0x7fu);
    uint32_t b = a + 0xbu;
    values[0] = b;

    uint32_t prev = values[0], current, part;
    for (int i = 1; i < 6; i++) {
        current = (prev + i) * 3u;
        part = (uint32_t)(challenge >> (14 + 2 * (i - 1)));
        part &= 0x1fu;
        current -= part;
        values[i] = current;
        prev = current;
    }
    count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    (void)challenge;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。

    uint32_t mixed = (uint32_t)(challenge >> 3) ^ (uint32_t)(challenge >> 32);
    uint32_t index = (uint32_t)challenge &(0x7);

    values[0]=index;
    switch (index){
        case 4: {values[1]=(mixed^0xb342u) + (mixed>>1);break;}
        case 3: {values[1]=3*mixed+0x2ac2;break;}
        case 1:{values[1]=(4*mixed)^(0x4c33);break;}
        case 7 :{values[1]=5*mixed+0x230d7;break;}
        case 5:{values[1]=5*mixed+0x56db;break;}
        case 2: {values[1]=5*mixed+0x162b;break;}
        case 0 :{values[1]=(2*mixed)^0x6a72;break;}
        case 6: {values[1]=(mixed^0x20f3)+(mixed>>1);break;}
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    return 1;
}
