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

uint32_t walk(uint32_t target,uint32_t low,uint32_t high,uint32_t salt){
    uint32_t mid = low+((high-low)>>1);

    if(target==mid) return (mid+3) ^salt;
    else if(target < mid){
        uint32_t child = walk(target,low,mid-1,salt);
        return ((5*child)^(mid+salt))^0x9;
    }
    else{
        uint32_t child = walk(target,mid+1,high,salt);
        return ((5*child)^(mid+salt))^0x15;
    }
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    (void)challenge;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。

    uint32_t low,high,target,salt;
    salt = (uint32_t)(challenge>>7)^(uint32_t)(challenge>>32);
    target = ((uint32_t)challenge)& 0x7f;
    high = 0x7f;low=0;

    values[0]=walk(target,low,high,salt);
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。

    typedef struct {
    int next;          
    uint32_t value;
    uint32_t mask;
} Node;

static const int heads[8] = {
    1, 6, 9, 3, 0, 11, 8, 7
};

static const Node nodes[12] = {
    /*  0 */ {11, 0x82f, 0x5f8d},
    /*  1 */ { 6, 0x01a, 0x1d59},
    /*  2 */ { 5, 0x94c, 0xaa0d},
    /*  3 */ { 0, 0x517, 0x5a82},
    /*  4 */ {10, 0x27a, 0x09e4},
    /*  5 */ {-1, 0x361, 0xeacc},
    /*  6 */ { 9, 0x73c, 0x5d12},
    /*  7 */ { 4, 0x457, 0xfae4},
    /*  8 */ { 7, 0xb49, 0xda18},
    /*  9 */ { 3, 0x170, 0xf6ce},
    /* 10 */ { 2, 0xa0f, 0x425a},
    /* 11 */ { 8, 0x650, 0xc7e7},
};

int node_index= heads[(uint32_t)(challenge & 0x7)];
uint32_t result,last_value;
result = (uint32_t)(challenge >> 6);
result ^= (uint32_t)(challenge >> 32);
result ^= nodes[node_index].mask;
result ^= 0x3491;
last_value = nodes[node_index].value;
result += last_value;
uint32_t steps = (uint32_t)((challenge >> 3) & 3u);

for(uint32_t i = 0;i<steps;i++){
    node_index = nodes[node_index].next;
    result ^= nodes[node_index].mask;
    last_value = nodes[node_index].value;
    result += last_value;
}
values[0] = result;
values[1] = last_value;
count = 2;    
    return write_answer(out, cap, values, count);
}


static uint32_t p6_op_0(uint32_t x) {
    return (x + 0x2763u) ^ (x >> 5);
}

static uint32_t p6_op_1(uint32_t x) {
    return (4u * x + 0xa7edu) ^ (5u * x);
}

static uint32_t p6_op_2(uint32_t x) {
    return (x ^ 0x88f1u) + 8u * x;
}

static uint32_t p6_op_3(uint32_t x) {
    uint32_t rotated = (x << 2) | (x >> 30);
    return rotated ^ 0x4e95u;
}

static uint32_t p6_op_4(uint32_t x) {
    return (x >> 5) | (x << 27);
}

static uint32_t p6_op_5(uint32_t x) {
    return ((x >> 1) ^ x) + 0x1d86u;
}
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。

    
uint32_t first_input =(uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);
uint32_t first_index = (uint32_t)(challenge & 7u);
uint32_t second_index = (uint32_t)((challenge >> 3) & 7u);

    static uint32_t (*const ops[8])(uint32_t) = {
    p6_op_0, p6_op_4, p6_op_0, p6_op_5,
    p6_op_3, p6_op_2, p6_op_1, p6_op_2
};

uint32_t first_result = ops[first_index](first_input);
uint32_t second_input = first_result ^ 0x6d6u;
uint32_t second_result = ops[second_index](second_input);
second_result+=0x9a23;
values[0]=second_result;
count=1;
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint32_t first = (uint32_t)(context&0x3);
    uint32_t esi = (uint32_t)(challenge >> 32) ^ (uint32_t)challenge;
    uint32_t r8d = (uint32_t)(context >> 32) ^ (uint32_t)context;

    uint32_t r9d = first * 0xefdeu;
    r9d ^= (uint32_t)(context >> 16);
    uint32_t edi = esi ^ r8d; 

    uint32_t second = (((edi>>1)|(edi<<31))^0x29af)^r9d;
    uint32_t shifted = esi >> 1;
    uint32_t third = second ^ (r8d + first + 0xc671u + shifted);

    int n = snprintf(out, cap,
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN %" PRIu32 " %" PRIu32 " %" PRIu32,
    first, second, third);
    if (n < 0 || (size_t)n >= cap) return 1;
    return 0;
}
