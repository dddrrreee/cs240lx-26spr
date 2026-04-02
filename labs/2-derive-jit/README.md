## Reverse engineering instruction encodings.

The last thing we need to tell you after the previous lab: Encoding
binary instructions can be error-prone and is definitely tedious.



As you look at the encodings you will realize two things:
   1. We already have tools that understand them (e.g., the assembler).
   2. While the encodings have weird fields at weird offsets each field
      is almost always independent of the others.

As a result, you can use a form of linear equation solving to efficiently
reverse engineer how to encode an instruction.

By simply:
  1. emitting the instruction in a file (possibly with placeholders
     to bracket it)
  2. Run the assembler on it to produce a binary version.
  3. Read in the result of (2): that is the encoding!

To find all register encodings:
  1. Hold `rdst` and `rsrc1` fixed and iterate over all possible
     registers for `rsrc2`.
  2. Hold `rdst` and `rsrc2` fixed and iterate over all possible
     registers for `rsrc1`.
  3. Hold `rsrc1` and `rsrc1` fixed and iterate over all possible
     registers for `rdst`.
  4. All the bits that never change during (1)---(3) are part of the opcode.

If the instruction fields are idependent, then the bits that change for
any step (the changset) should be disjoint from the changeset of any
other step.  We can check this and panic if it's not true.

An example is easier than words.  The program `code/0-simple-derive.c`
shows how to do this solving to figure out the encoding for the `src2`
register for the add instruction.

First, to figure out the register encoding, we write out one add
instruction, one for each possible register (`r0..r15`):

```c

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

```

After compiling, if you look at the `.list` file you see these become:


```
00008038 <derive_add_src2>:
    8038:   12345678    .word   0x12345678
    803c:   e0800000    add r0, r0, r0
    8040:   e0800001    add r0, r0, r1
    8044:   e0800002    add r0, r0, r2
    8048:   e0800003    add r0, r0, r3
    804c:   e0800004    add r0, r0, r4
    8050:   e0800005    add r0, r0, r5
    8054:   e0800006    add r0, r0, r6
    8058:   e0800007    add r0, r0, r7
    805c:   e0800008    add r0, r0, r8
    8060:   e0800009    add r0, r0, r9
    8064:   e080000a    add r0, r0, sl
    8068:   e080000b    add r0, r0, fp
    806c:   e080000c    add r0, r0, ip
    8070:   e080000d    add r0, r0, sp
    8074:   e080000e    add r0, r0, lr
    8078:   e080000f    add r0, r0, pc
    807c:   12345678    .word   0x12345678
    8080:   e12fff1e    bx  lr
```

If you look at the add instructions themselves, you can see that only
the lower bits change as we iterate and, in fact it is identical to the
register value.  (This identity map is common for register encodings):

```
    #              src2
    803c:   e08000  00    add r0, r0, r0    
    8040:   e08000  01    add r0, r0, r1
    8044:   e08000  02    add r0, r0, r2
    8048:   e08000  03    add r0, r0, r3
    804c:   e08000  04    add r0, r0, r4
    8050:   e08000  05    add r0, r0, r5
    .. .
    8074:   e08000  0e    add r0, r0, lr
    8078:   e08000  0f    add r0, r0, pc
```

Given this we do the second step: iterate over all instructions,
computing which bits change --- these are exactly those used to encode
the register number.

Since its cleaner we just compute which bits are always 0, which are always
1, and the bits that change are the complement of this:

```c
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
```

You can generalize this to solve for multiple registers and immediates.
The program `code/1-simple-derive.c` shows how to do this.


A long time ago, we built a tool, Derive, that used this approach
and it worked pretty well.  (It came about because I refused to figure out
how to encode `x86` instructions because they were so disgusting.)  We'll
do a simple version in this lab.  The papers:
   - a [short version](docs/derive-short.pdf)
   - a [longer more short version](docs/derive-usenix.pdf)

With all that said, you should know how to use the instruction manual.  
However, doing sleazy reverse engineering teaches you about 
invariants, relations, tendencies, which will lead to a deeper
understanding.  It also leads to a deeper insight because the machine
code is not just some inert substance.  

We are doing things this sleazy "wrong way" because it makes encoding
an interesting puzzle.

Useful readings:
  - [using gcc to figure out assembly](https://github.com/dddrrreee/cs140e-26win/tree/main/notes/using-gcc-for-asm).
  - [callee-caller (for the methods)](https://github.com/dddrrreee/cs140e-26win/tree/main/notes/caller-callee)

### What to do

1. You should look at the code in `code/` and modify it to reverse engineer
   the code you hand-coded in lab 1.  There is an example of how to do 
   immediates.  You're strongly encouraged to write a power, clean
   system that can succinctly do many instructions easily.
2. You should add code that will print C encoder routines that 
   will use your reverse engineered constraints to implement the 
   encoding.  E.g., you should emit the implementation of:

    // returns the uint32_t encoding of <add dst, src1, src2>
    uint32_t gen_add(uint32_t dst, uint32_t src1, uint32_t src2);

Extensions:
 - There are tons of fun tricks.    Will add.  But the easiest to just
   figure out the encoding for as many things as you see in the list
   file as you can.
 - You can also use the cross compiler (better to do so in some ways)
   by emitting dynamically and reading back in.  If you do so you can
   detect illegal registers or constrants (by detecting when the assembler
   fails) or bifurcation constraints (when it makes more instructions
   than expected).
