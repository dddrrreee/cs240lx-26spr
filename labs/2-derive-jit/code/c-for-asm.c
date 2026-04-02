// we want to figure out assembly: how big immediates are, the
// encoding arithmetic:
//   1. write code to try different permutations.
//   2. look at the machine code.
//
// you can absolutely use the instruction encodings in the armv6 manual.
// however, doing sleazy reverse engineering teaches you about 
// invariants, relations, tendencies, which will lead to a deeper
// understanding.  it also leads to a deeper insight b/c the machine
// code is not just some inert substance.  
//
// useful:
// using gcc to figure out assembly:
//   https://github.com/dddrrreee/cs140e-26win/tree/main/notes/using-gcc-for-asm
// 
// callee-caller (for the methods)
//    https://github.com/dddrrreee/cs140e-26win/tree/main/notes/caller-callee
#include "rpi.h"

// 1. jump to self (0 back)
void derive_bl_self(void) {
    asm volatile (".Lbl_b0: bl .Lbl_b0;");
}
// 2. jump to 1 back
void derive_bl_1_back(void) {
    asm volatile (".Lbl_b1: nop; bl .Lbl_b1;");
}
// 3. jump to 2 back
void derive_bl_2_back(void) {
    asm volatile (".Lbl_b2: nop; nop; bl .Lbl_b2;");
}
// 4. jump to 1 forward
void derive_bl_1_forward(void) {
    asm volatile ("bl .Lbl_f1; .Lbl_f1: nop; ");
}
// 5. jump to 2 forward
void derive_bl_2_forward(void) {
    asm volatile ("bl .Lbl_f2; nop; .Lbl_f2:");
}


// you should use the instruction manual.  we are doing things
// the wrong way so you can think more about how to figure stuff
// out.
int ret_256() { return 256; }
int ret_257() { return 257; }
int ret_neg_257() { return -257; }
int ret_neg_256() { return -256; }

// look at different branching.
void derive_bne(int x) {
    asm volatile ("beq l1; l1:");
    asm volatile ("beq l2; nop; l2:");
    asm volatile ("beq l3; nop; l3:");

    asm volatile ("b1: beq b1;");
    asm volatile ("b2: beq b2;");
    asm volatile ("b3: nop; beq b3;");
}

void notmain(void) {
    output("make sure you look at the <c-for-asm.list> file!\n");
}
