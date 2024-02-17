CC := gcc

# Set to TRUE to enable logging functions.
LOG_ENABLE = FALSE

CORE_INCLUDE_PATHS = -I../uld-driver
PLATFORM_INCLUDE_PATHS = -I../platform
EXAMPLES_INCLUDE_PATHS = -I../examples

BASE_CFLAGS = -Wall -Werror -Wno-missing-braces
CFLAGS_RELEASE = -Os -g0
CFLAGS_RELEASE += -DUSE_INTERRUPTS
#CFLAGS_RELEASE += -DSTMVL53L4CD_KERNEL

LIB_CORE_SOURCES =\
	$(wildcard ../uld-driver/*.c)

LIB_PLATFORM_SOURCES =\
	$(wildcard ../platform/*.c)

LIB_EXAMPLES_SOURCES =\
	$(wildcard ../examples/*.c)

LIB_SOURCES := $(LIB_CORE_SOURCES) $(LIB_PLATFORM_SOURCES) $(LIB_EXAMPLES_SOURCES)
INCLUDE_PATH = $(CORE_INCLUDE_PATHS) $(PLATFORM_INCLUDE_PATHS) $(EXAMPLES_INCLUDE_PATHS)

CFLAGS = $(BASE_CFLAGS) $(CFLAGS_RELEASE) $(INCLUDE_PATH)

all:
	$(CC) $(CFLAGS) $(LIB_FLAGS) -o menu ./menu.c $(LIB_SOURCES) -lm

clean:
	rm -f menu
