## CS240lx, Spr 2026 (rm: Y2E2-111 6pm)

-----------------------------------------------------------------
### tl;dr:  getting setup.

  - get the class repo:

        git clone git@github.com:dddrrreee/cs240lx-26spr.git

  - define the `CS240LX_2026_PATH` environment variable to 
    point to where you put the class repo.  

        # for tcsh in .tcshr
        setenv CS240LX_2026_PATH <absolute path to your repo>

        # for bash (in .bashrc or similar)
        export CS240LX_2026_PATH=<absolute path to your repo>

  - check that everything worked: plug in your pi, and run hello:

        % pwd
        /home/engler/class/cs240lx-26spr
        % cd labs/0-pi-setup
        % make

        ... alot of stuff ...
        listening on ttyusb=</dev/ttyUSB0>
        hello world from the pi
        DONE!!!
        
        Saw done

  - [look at the labs](./labs/README.md)
  - write code!

-----------------------------------------------------------------
### Overview

  - If you liked 140e, consider taking cs240lx.
  - It's fun.
  - we have record enrollment so by the birthday paradox 
    / pigeon hole principle, your fav people will be there.

Same rough format: 
  - still 2 days a week.
  - still hardware, low level code.
  - still pizza.

Contents:
  - cool tricks I picked up over 3+ decades. or seem not well covered.
  - depends alot on what people are interested in.  can be
    OS heavy, or more device heavy, or more project heavy
    depending.  let us know what kind of stuff you are into.
  - Probably at least one new board (pico, different riscv).

Differences:
  - usually stop around 10:30pm or so, vs 1:00am.
  - is more conceptual:
      - 140e must cover threads, interrupts, VM, FS, some network.
      - 240lx assumes you know that stuff, and use it to do
        other stuff.
  - class can replace cs240 (paper reading).
  - usually has the people who find 140e fun (+/-).
  - the final projects are wild.

Several old, partially overlapping offerings:
  - [cs240lx-22](https://github.com/dddrrreee/cs240lx-22spr/tree/main/labs)
  - [cs240lx-23](https://github.com/dddrrreee/cs240lx-23spr/tree/main/labs)
  - [cs240lx-25](https://github.com/dddrrreee/cs240lx-25spr/tree/main/labs)

---------------------------------------------------------------
### Class rules: no llm usage, no late labs.

This class is a brutal staff load.  140E last quarter:
 - Almost 2,000 checkoffs; 
 - Many (unpaid!) staff staying at lab from 530-midnight;
 - Everyone still has PTSD.
240lx has conceptually harder labs than 140e, much smaller staff, and
almost 2x last years enrollment.

So unfortunately we do have two draconian rules for survival.

*Rule 1: absolutely no labs accepted*:
  - You have 1 week to turn in each lab (earlier is better!).
  - We will not accept them after this.  
  - TAs can't deal with the load of late turn-ins.
  - Possible exceptions: you are presenting at a conference, 
    competing at a national championship, or doing a huge
    interesting extension.

*Rule 2: Absolutely no LLM usage for labs or extensions*:
  - You might not get caught, but if you do so we'll try to get you
    kicked out if you don't drop the class.
  - After the past year's experience, we've been forced into fascism.
  - The alternative is in-class blue book exams.  No one wants that.
  - If you want to use robots, *please* take the many other classes that
    are ok with it.  
  - There are much higher fidelity and much easier ways for us to see LLM
    output.
  - WTS: we are developing LLM for low-level hardware classes.  But
    they are intended as follow-on for those that understand the hardware.

Sorry for the negative message. We live in interesting times. The path
ahead is unclear other than the neon glare of what doesn't work well.

-----------------------------------------------------------------

This is a implementation-heavy, lab-based class that will cover similar
topics as CS240 --- threads, virtual memory, file systems and distributed
systems --- but by writing code versus discussing papers. After taking an
initial operating systems course you are often left in the odd situation
of having spent significant time getting the background knowledge needed
to do interesting actions, but not being left with enough time to actually
do them. This course attacks this problem by assuming basic knowledge and
using it as a springboard to advance quickly through a set of powerful,
useful techniques that even advanced practioners can be ignorant of.

We will write custom code from scratch for the widely-used, ARM-based
raspberry pi; our code will run "bare-metal" without an operating system,
which means we can do interesting things without constantly fighting
with a lumbering OS that cannot get out of its own way.

By combining research insights (many unpublished) and our our lightweight
bare-metal code we will be in the unusual position of having foundational
abilities that most people have assumed are in-practice impossible. As
one example, we will implement fast, flexible exception handling and then
use this ability to build a variety of tools that find race conditions,
check code correctness, and memory corruption. By using exceptions rather
than binary rewriting we can build tools that find similar errors as
Purify / Valgrind, but can be implemented in merely hundreds rather than
hundreds of thousands of lines of code; our tools will also be faster
and more extensible.

The course workload is significant, but we aim to not waste your
time. CS140E is strongly encouraged as a prerequisite, but a sufficiently
talented and motivated implementor can make up for its lack (as calibration
we usually have two or three people per year do this route).

### Possible labs

Inspired by 140e final projects:
  - Stuart doing some kind of elf loader.
  - Aditya, Rohan, Sebastian: an ox64 lab?
  - Asanshay and Rohan: GPU + fractals for speedup.
  - Max: riscv + ppp + smi + rust?
  - others?

Always do: Generate executable code at runtime
  - even more low level than inline assembly.
  - self-modifying code is the ultimate sleazy hack.
  - used to make fast(er) interpreted languages (JIT for
    javascript, etc), fast dynamic intstrumentation (valgrind),
    and old school virtual machines (vmware).
  - can do all sorts of speed hacks by specializing using runtime
    information.  compiling data structures to code.  etc.

Always do: Custom pcb:
  - parthiv historically comes in to do a week of custom pcb labs so
    can make your own cool boards.

Always do: Build a bunch of tools using memory faults and single-stepping.
  - Eraser race detector 
  - Purify memory checker 
  - Advanced interleave checker.  
  - instruction level profiler measuring cycles, cache misses,
    etc.

Always do: Speed:
  - make low level operations (exceptions, fork/wait, pipe
    ping pong) 10-100x faster than laptop.  i think this is
    feasible but haven't done, so am interested.
  - overclocking the pi and seeing how fast can push it.
  - push the NRF using interrupts and concurrent tx, rx to 
    see how close can get to the hw limit.
  - make a digital analyzer (printk for electricity) where
    you get the error rate down to a small number of nanoseconds

And a few different communication protocols: 
  - how fast can send/recv data over gpio pins b/n two pis?
  - over IR
  - over speaker/mic.
  - over light and camera (?)
  - lora?



Device labs are fun.  So we do those too:
  - accelerometer, gyro
  - lidar
  - addressable light array
  - analog to digital converter.
  - lora so can send bytes 1km+

More systems-y device labs, by combining into
standalone tools:
  - acoustically reactive light display using mic, adc, 
    addressable light array.  extend to multiple systems.
  - little oscilliscope using oled display, mic, adc.

If we have more kernel hackers some subset of:
  - network bootloader
  - make an actual OS that ties all the stuff together.
  - small distributed system
  - distributed file system
  - actual clean fat32 r/w so can do distributed file system,
    firmware updates.
  - processes that migrate from one pi to another.
  - FUSE file system interface.

Big stuff missing from 140e:
  - makefiles
  - crazy C macro tricks.

Maybe other boards:
  - pico 2?  (slow, but small+cheap: has arm-m33 and risv)
  - ox64?  (risv)
  - if we do riscv: can write a simulator that will simulate itself
    on riscv in a couple of days.  can also make it symbolic.

Possible other stuf:
  - e.g., maybe some other languages (rust?  zig?)
  - static bug finder.

---------------------------------------------------------------------------
### What we need

To get a better feel or what the labs look like, you can look at:
  - [2023's repo](https://github.com/dddrrreee/cs240lx-23spr/tree/main/labs)
  - [2022's repo](https://github.com/dddrrreee/cs240lx-22spr/tree/main/labs)

There are way more possible labs than class slots so if you can please
look through these and let us know some rough preferences.

In addition, let us know:
  - stuff missing from 140e.
  - stuff that you saw in the wild that looks cool.

<p align="center">
<img src="labs/lab-memes/chaotic-study.jpg" width="400" />
</p>
