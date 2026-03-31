## CS240lx Spr'26 (Y2E2-111 530pm)

This is a implementation-heavy, lab-based class.  We assume 140e as
a baseline and use this knowledge as a springboard to advance quickly
through a set of powerful, useful techniques that even many advanced
practioners don't know.

Like 140e you'll write custom code from scratch for the widely-used,
ARM-based raspberry pi; our code will run "bare-metal" without an
operating system, which means we can do interesting things without
constantly fighting with a lumbering OS that cannot get out of its
own way.

The course workload is significant, but we aim to not waste your
time. CS140E is strongly encouraged as a prerequisite, but a sufficiently
talented and motivated implementor can make up for its lack (as
calibration we usually have two or three people per year do this route).

### 240lx vs 140e 

Same rough format: 
  - still 2 days a week.
  - still hardware, low level code.
  - still pizza.

Contents:
  - Various cool tricks from 3+ decades of low level code.
  - Devices, speed, dynamic tools, code generation, OS tricks.  
    - Depends alot on what people are interested in.  
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

Older versions:
  - [cs240lx-22](https://github.com/dddrrreee/cs240lx-22spr/tree/main/labs)
  - [cs240lx-23](https://github.com/dddrrreee/cs240lx-23spr/tree/main/labs)
  - [cs240lx-25](https://github.com/dddrrreee/cs240lx-25spr/tree/main/labs)


Alot of your favorite staff!
  - Head TA: Joseph Shetaye (rockets, booms, winning bets)
  - Maximilien Cura (Rust + insane hacks)
  - Aditya Sriram (ox64 riscv + bass)
  - Dawson Engler (sup)

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
    output compared to the work and expense of this class.
  - WTS: we are developing "LLMs for low-level hardware" lab classes.  But
    they are intended as follow-on for those that understand how things
    work.

Sorry for the negative message. We live in interesting times. The path
ahead is unclear other than the neon glare of what has not worked well.

-----------------------------------------------------------------
### Possible labs

We have 50+ possible labs, so actual contents vary depending on
interest.

Always do: Generate executable code at runtime (today)
  - even more low level than inline assembly.
  - self-modifying code is the ultimate sleazy hack.

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

-----------------------------------------------------------------
### tl;dr:  getting setup.

  - get the class repo:

        git clone git@github.com:dddrrreee/cs240lx-26spr.git

  - define the `CS240LX_2026_PATH` environment variable to 
    point to where you put the class repo.  

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

<p align="center">
<img src="labs/lab-memes/chaotic-study.jpg" width="400" />
</p>
