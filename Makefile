override PROG := bad_atomic

CC    := gcc-11
CXX   := g++-11
LD    := $(CC)
STRIP := strip
RM    := rm

BASEFLGS := -Og -ggdb -march=native -mtune=native -flto=32 -fuse-linker-plugin

DEBUG :=
ifneq (1,$(strip $(DEBUG)))
override BASEFLGS += -DNDEBUG
endif

LFLAGS   := -pthread
CFLAGS   := -std=gnu18 $(BASEFLGS)
CXXFLAGS := -std=gnu++17 $(BASEFLGS)

Q :=
ifeq (1,$(strip $(Q)))
override Q := @
else
override Q :=
endif

override SRC := $(wildcard *.c *.cpp)

.PHONY: all clean
all: $(PROG)

$(PROG): $(patsubst %,%.o,$(SRC))
	$(Q)$(CC) $(BASEFLGS) -o $@ $^ $(LFLAGS)
ifneq (1,$(strip $(DEBUG)))
	$(Q)$(STRIP) $@
endif

%.c.o : %.c
	$(Q)$(CC) $(CFLAGS) -c -o $@ $^

%.cpp.o : %.cpp
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	$(Q)$(RM) -f $(PROG) $(SRC:%=%.o)
