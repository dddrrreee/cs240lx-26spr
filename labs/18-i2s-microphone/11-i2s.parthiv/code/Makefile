# list out the driver program source
# TEST_SRC = $(wildcard tests/*.c)
PROGS = tests/1-i2s-test.c
# PROGS = tests/2-i2s-dump.c
# PROGS = tests/3-i2s-audiovis.c
# PROGS = tests/4-fft-test.c
# PROGS = tests/5-fft-time.c
# PROGS = tests/6-fft-freqout.c
# PROGS = tests/7-fft-freqviz.c
# PROGS = tests/8-fft-buckets.c

OPT_LEVEL = -O3

# your source, shared by driver code.
#   if you want to use our staff-hc-sr04.o,
#   comment SRC out and uncomment STAFF_OBJS
COMMON_SRC = i2s.c fft.c

# define this if you need to give the device for your pi
TTYUSB = 

# set RUN = 1 if you want the code to automatically run after building.
RUN = 1

include $(CS240LX_2023_PATH)/libpi/mk/Makefile.template-v2
