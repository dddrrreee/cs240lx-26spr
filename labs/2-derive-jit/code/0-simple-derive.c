// simple solver the <add rdst, src1, src2> instruction.  
// solve for <srcc> by varying the registers from r0..r15 
// and solve for the bits.
#include "rpi.h"

#define SENTINAL ".word 0x12345678;"

// vary one source register: can then use linear solving 
// on the bits.  
// since we try all possible registers we know that:
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


// look for SENTINAL in the code pointed to by <p>
// error if not found within <len> words.
static uint32_t *find_sentinal(uint32_t *p, unsigned max_len) {
    for(int i = 0; i < max_len; i++)
        if(p[i] == 0x12345678)
            return &p[i];
    panic("did not find sentinal after %d instructions\n", max_len);
    return 0;
}

void notmain() { 
    enum { MAX_INST = 128 };
    // get where instructions start.
    uint32_t * inst = find_sentinal((void*)derive_add_src2, MAX_INST) + 1;
    // end.
    uint32_t * e = find_sentinal(inst, MAX_INST);
    uint32_t n = e - inst;
    demand(n==16, should have 16 instructions!);

    // print out the raw encodings.
    output("inst\thex encoding\tbinary encoding\n");
    for(int i = 0; i < 15; i++) 
        output("%d:\t[%x]\t[%b]\n", i,inst[i], inst[i]);

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
    // - bits that change are the negation 
    //   of these.
    uint32_t always_0 = ~0;
    uint32_t always_1 = ~0;
    for(int i = 0; i < 15; i++) {
        always_0 &= ~inst[i];
        always_1 &= inst[i];
    }

    // anything that wasn't always 0 or always 1 changed.
    // since only the <src2> was varied --- these are exactly
    // the bits used to encode it.
    uint32_t changed = ~(always_0 | always_1);
    for(unsigned i = 0; i < 32; i++) {
        if(changed & (1<<i))
            output("\tbit=%d changed: part of reg <src2>\n",i);
    }
}
