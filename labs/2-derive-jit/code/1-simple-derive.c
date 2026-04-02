// simple solver for add instruction.  vary the last register
// from r0..r15 and solve for the bits.
#include "rpi.h"

#define SENTINAL ".word 0x12345678;"

// vary one source register: can then use
// linear solving on the bits.  since we try all possible
// registers we know that:
//  1. anything that changed was part of the register field.
//  2. anything that never changed was not.
void derive_add_src2(void) {
    asm volatile (
        // use an unlikely sentinal to mark the start
        SENTINAL
        "add r0, r0, r0;"
        "add r0, r0, r1;"
        "add r0, r0, r2;"
        "add r0, r0, r3;"
        "add r0, r0, r4;"
        "add r0, r0, r5;"
        "add r0, r0, r6;"
        "add r0, r0, r7;"
        "add r0, r0, r8;"
        "add r0, r0, r9;"
        "add r0, r0, r10;"
        "add r0, r0, r11;"
        "add r0, r0, r12;"
        "add r0, r0, r13;"
        "add r0, r0, r14;"
        "add r0, r0, r15;"
        // use an unlikely sentinal to mark the end
        SENTINAL
    );
}

// vary rs1, keep everything else the same.
static void derive_add_src1(void) {
    asm volatile (
        SENTINAL
        "add r0, r0, r0;"
        "add r0, r1, r0;"
        "add r0, r2, r0;"
        "add r0, r3, r0;"
        "add r0, r4, r0;"
        "add r0, r5, r0;"
        "add r0, r6, r0;"
        "add r0, r7, r0;"
        "add r0, r8, r0;"
        "add r0, r9, r0;"
        "add r0, r10, r0;"
        "add r0, r11, r0;"
        "add r0, r12, r0;"
        "add r0, r13, r0;"
        "add r0, r14, r0;"
        "add r0, r15, r0;"
        SENTINAL
    );
}

// vary dst, keep everything else the same.
static void derive_add_dst(void) {
    asm volatile (
        SENTINAL
        "add r0, r0, r0;"
        "add r1, r0, r0;"
        "add r2, r0, r0;"
        "add r3, r0, r0;"
        "add r4, r0, r0;"
        "add r5, r0, r0;"
        "add r6, r0, r0;"
        "add r7, r0, r0;"
        "add r8, r0, r0;"
        "add r9, r0, r0;"
        "add r10, r0, r0;"
        "add r11, r0, r0;"
        "add r12, r0, r0;"
        "add r13, r0, r0;"
        "add r14, r0, r0;"
        "add r15, r0, r0;"
        SENTINAL
    );
}

// exploit linear: use largest and smallest reg.
static void derive_add_dst_cheat(void) {
    asm volatile (
        SENTINAL
        "add r0, r0, r0;"
        "add r15, r0, r0;"
        SENTINAL
    );
}

static void derive_mov_imm8(void) {
    asm volatile (
        SENTINAL
        "mov r0, #0;"
        "mov r0, #255;"
        SENTINAL
    );
}

// look for SENTINAL in the code pointed to by <p>
// error if not found within <len> words.
static uint32_t *find_sentinal(uint32_t *p, unsigned max_len) {
    for(int i = 0; i < max_len; i++)
        if(p[i] == 0x12345678)
            return &p[i];
    panic("did not find sentinal after %d instructions\n", max_len);
    return 0;
}

#define BITS_STR(_x) ({                            \
    uint32_t b = _x;                                \
    static char buf[33]={0};                        \
    for(uint32_t i = 0; i < 32; i++)                \
        buf[i] = "01"[((b) >> i) & 1];               \
    buf;                                            \
})

// return 1 if x has (1) at least one bit set to 1, (2) all 
// one bits are contiguous.
static inline int iscontig32(uint32_t x) {
    if(!x)
        return 0;

    // cute trick to test for contige bits from "hacker's delight"
    // https://stackoverflow.com/a/62714182
    //
    // let lb = lowest 1 bit.
    // let ub = highest 1 bit.
    // 1. x & -x ==> get low bit lb
    //    - common trick to detect if power-of-2: (x&-x == x)
    // 2. if x only has contig bits, adding (1) to x clears all bits
    //    in the contig block, leaving only a 1 at ub+1.
    // 3. x & (2) = 0, then the 1s were all together.  otherwise there
    //    will be at least one a 1 bit.
    return (x & (x + (x & -x))) == 0;
}

static uint32_t 
solve_for_reg(const char *name, 
              void (*fn)(void), 
              uint32_t expected_inst) 
{
    enum { MAX_INST = 128 };
    uint32_t * inst = find_sentinal((void*)fn, MAX_INST) + 1;
    uint32_t * e = find_sentinal(inst, MAX_INST);
    unsigned n = e-inst;
    assert(n == expected_inst);

    output("------------------------------------------------\n");
    output("%s: %d instructions total\n", name, n);
    output("inst\thex encoding\tbinary encoding\n");
    for(int i = 0; i < n; i++) 
        output("%d:\t[%x]\t[%s]\n", i,inst[i], BITS_STR(inst[i]));

    // solve.
    output("-------------------------------------\n");
    output("solve for changed bits\n");

    // solve for register bits by computing which
    // bits change.
    //
    // we do so by computing which bits never change:
    // everything else must belong to the register
    // field.
    //
    // - compute which bits are always 0.
    // - compute which bits are always 1.
    // - bits that change (the negation of these) are
    //   the bits used to encode the register
    uint32_t always_0 = ~0;
    uint32_t always_1 = ~0;
    for(int i = 0; i < n; i++) {
        always_0 &= ~inst[i];
        always_1 &= inst[i];
    }

    // bits that changed were (definitionally): not always
    // always 0 or always 1.
    uint32_t changed = ~(always_0 | always_1);
    // bits that never changed: union of always 0 and always 1.
    uint32_t unchanged = (always_0 | always_1);

    // various tautologies (but good to check to ensure they are :).
    assert(unchanged == ~changed);
    assert(changed == ~unchanged);
    assert((changed & unchanged) == 0);
    assert((changed | unchanged) == ~0);

    output("always_0   = [%s]\n", BITS_STR(always_0));
    output("always_1   = [%s]\n", BITS_STR(always_1));
    output("changed    = [%s]\n", BITS_STR(changed));
    output("unchanged  = [%s]\n", BITS_STR(unchanged));

    // 32 bits: print which ones changed.
    for(unsigned i = 0; i < 32; i++) {
        if(changed & (1<<i))
            output("bit=%d changed: part of %s\n",i, name);
    }

    if(!iscontig32(changed))
        panic("reg should be contig: is [%s]\n", BITS_STR(changed));
    return changed;
}

void notmain() { 
    uint32_t src2 = solve_for_reg("src2", derive_add_src2, 16);
    uint32_t src1 = solve_for_reg("src2", derive_add_src1, 16);
    uint32_t dst  = solve_for_reg("src2", derive_add_dst, 16);
    uint32_t opcode = ~(src2|src1|dst);

    output("src2    = [%s]\n", BITS_STR(src2));
    output("dst     = [%s]\n", BITS_STR(dst));
    output("src1    = [%s]\n", BITS_STR(src1));
    output("opcode  = [%s]\n", BITS_STR(opcode));

    output("going to try cheating\n");
    uint32_t dst_cheat  = solve_for_reg("src2", derive_add_dst_cheat, 2);
    if(dst_cheat != dst)
        panic("cheat dst=[%s], dst=[%s]\n",  
            BITS_STR(dst_cheat),
            BITS_STR(dst));
    output("SUCCESS: dst==dst_cheat\n"); 

    uint32_t imm8 = solve_for_reg("imm8", derive_mov_imm8, 2);
}
