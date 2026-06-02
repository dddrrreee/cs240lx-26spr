## Preface for the crazy ones

DMA is Turing complete. If you want to just completely daniel mode this and figure everything out
on your own, just ignore this README and do your own thing, since it's a really fun
trick and this README kinda spoils the surprise ;)

## DMA (by Max Cura)

Today we're going to talk about DMA is, and what you can do with it.

*CHECKOFF*:
 - `code/5-dma-add.c`
 - `code/6-dma-blink.c`

## Background - DMA

DMA (Direct Memory Access) refers to a fairly common technique where hardware will provide dedicated
 accelerators for `memcpy`-shaped operations. This is because in general computers spend quite a lot
of time doing `memcpy`-shaped things, and tying up the CPU doing `ldr`/`str` during all of these is
not a very good use of the hardware. Thus, DMA engines are basically a way to kick off asynchronous
`memcpy` operations.

The basic DMA usage looks something like:
 1. Configure the DMA engine (source/dest/length)
 2. Start the DMA engine
 3. Optionally wait for the DMA engine to finish running, while doing something else on the side

Since the BCM has a lot of different peripherals with very different interfaces and operating
procedures, for the DMA to be able to interact with these requires the DMA system to have a great
deal of flexibility. Therefore, the BCM has a very versatile DMA engine that implements a
"chained `memcpy`" primitive, similar to many other SoCs of its sort.

> [!NOTE]
> We're talking about the BCM2835 peripheral; the ARM CPU has a little tiny vestigial DMA system in
> it as well, but it's very weak and we're not going to be using it.

The DMA engine is directed by a linked list of "control blocks" that reside in main memory.
It runs like so:
 1. Load current control block (source/dest/length/next control block/+ some other config) from RAM
 2. Perform `memcpy`
 3. Repoint the engine's current control block pointer at the next control block specified
 4. Repeat until some condition is met (on the BCM, control block pointer = 0)

This turns out to actually be an extremely powerful system because control blocks are able to modify
each other. This system is, in fact, Turing-complete, which we'll demonstrate later in this lab by
loading a full RISC-V emulator in the DMA.

## Background - DMA computation

The basic idea of doing computation with DMA is that you can modify the source/destination/next
control block pointer of other control blocks. In particular, this allows us to do
**table-based computation**.

> [!NOTE]
> It is important to note that if a control block modifies itself, the modifications won't take
> effect until the NEXT time it's run, since the DMA controller pulls control blocks from DRAM and
> into the controller's internal registers before executing them.

Consider the following C program that calculates the `x % 3`:

```c
uint8_t mod_3(uint8_t input) {
  return input % 3.
}
```

This is in fact equivalent to the following:
```c
static volatile MOD_3_TABLE[256];
for(int i=0;i<256;i++)
  MOD_3_TABLE[i] = i % 3;

uint8_t mod_3(uint8_t input) {
  uint8_t output;
  memcpy(&output, &MOD_3_TABLE[input], 1);
  return output;
}
```

This is super useful for us! DMA doesn't have any kind of notion of modulo or remainders, but it
does, as a matter of fact, have an excellent understanding of the `memcpy` operation. Therefore,
all you need to do to be able to perform computations with DMA is figure out how to index into
tables with it.

Useful:
 - Think about how you could use DMA to do these kinds of lookup operations!
   Under what conditions can DMA let you do these things?

## Miscellanea

Important things to note:
 1. bus address != ARM address
 2. watch out for MEMORY CORRUPTION - it's super easy to run into issues with this!!!!!
 3. be VERY CAREFUL if you turn the ARM caches on

## Part 1 - Example

Take a look at `0-dma-example.c` to familiarize yourself with making the DMA do things.

> [!NOTE]
> `0-dma-example.c` and `0-ctx-example.c` are different things!

Also good to check out `dma-impl.h` to get a sense of where things are.

## Part 2 - IO with DMA

Okay, so: DMA can access memory, and since MMIO acts like memory, DMA can also do MMIO.
Traditionally, this is used for things like quick UART FIFO fills and whatnot (our UARTs don't
actually have full support for this unfortunately). We won't be bitbanging UART with the DMA
(unless you want to-then it's an extension!)

Specifically, we'll be making an LED blink with (mostly) DMA. We'll gradually shave off the parts
that aren't DMA, though we won't finish that process until Part 3.

Parts:
 1. in `1-dma-blink.c`, you'll learn how to use DMA to do MMIO
 2. in `2-dma-delay.c`, you'll figure out how to emulate `delay_ms()` in DMA
 3. in `3-dma-blink.c`, you'll use your `dma_delay` to space your blinking

## Part 3 - Small compute with DMA

I said before that DMA on the BCM is Turing-complete, but talk is cheap, so let's prove it. If we
can do addition and loops with arbitrary conditions, then we're Turing complete.

> [!NOTE]
> For the theory people: this is by reduction to Brainfuck (https://en.wikipedia.org/wiki/Brainfuck).

While I was writing the RISC-V emulator, I ended up coming up with a set of helper functions for
generating DMA programs. It's currently under the rather un-descriptive name `ctx`, but I think it's
pretty useful for writing long-form DMA stuff. Check out `0-ctx-example.c` for some usage examples,
and definitely look at the full API in `ctx.h`.

Parts:
 1. in `4-dma-inc.c`, you'll figure out how to do simple table-based computation to make a DMA chain
    that increments a `uint8_t` by 1.
 2. in `5-dma-add.c`, you'll extend that to adding two `uint32_t`s together.

    **IMPORTANT**: `5-dma-add.c` has three stages with separate `todo`s.

## Part 4 - Finishing our blinky

In part 3, we said that we could demonstrate Turing completeness with addition and while loops.
So far, we've done additions, but we haven't done while loops yet. We'll finish off by completely
transforming our original blink example into DMA by moving the `for` loop that counts the number of
blinks completely into DMA.

Parts:
 1. complete `6-dma-blink.c`

## Part 5 - Getting RISC-y (optional)

This is more of a victory lap than anything else, but it's kinda cool, so here it is anyway:

In the lecture, I demo'd a RISC-V emulator that runs in the DMA. If you want, you can ship-of-theseus
your way through the operations that make it up. In particular, you can replace the staff `stamp_add`
function with your own and see if you can get the performance on par with the staff one.

You can do this by replacing the `stamp_add` in `stamps.h` with your own.

## Extensions

Lot of extensions here. If you can think of a computer doing a thing, you can do that thing in DMA,
and then see how fast you can make it, and BOOM: extension 🤯.

Couple things I thought of while writing this up:
 1. **Minor**: better carries - it's possible to what we did much faster and with minimal extra
    memory. Useful:
      - what patterns do you see about the carried values in add/sub? how much information is
        actually being carried?
      - use this to do 3-way addition (carry + lhs + rhs) with minimal lookups
    How fast can you do an add?
 2. **Minor**: fast 32-bit signed integer multiplication stamp - I recommend taking a look at
    https://en.wikipedia.org/wiki/Binary_multiplier for this.
 3. **Major**: fast 32-bit signed integer division stamp. This one's a lot harder that
    multiplication, because you can't just make everything table based. I would recommend looking
    at libgcc or compiler-rt's __divdi implementation for inspiration. You'll probably need
    bit shift ops (in both directions) and subtraction to make this work.

    Not for the faint of heart. If you do this, I would recommend tweaking your DMA setup so that
    you can single-step through a DMA chain.
 4. **Major**: bitbanged UART with DMA
 5. **Minor**: use the DMA for what it was actually intended for - maybe for I2C or SPI on a
    previous device lab.
 
## Acknowledgements

 - Some of the code from this lab is adapted from Dawson's 340LX lab last year (in particular: the
   section "ACTUAL DMA STRUCTURES" in `dma-impl.h`)
 - `printf.h` and `printf.c` are from https://github.com/mpaland/printf

