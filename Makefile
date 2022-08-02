override PROG := bad_atomic

CC    := gcc
CXX   := g++
LD    := $(CC)
STRIP := strip

BASEFLGS := -mtune=native -flto

NDEBUG :=
ifeq (1,$(strip $(NDEBUG)))
override BASEFLGS += -O3 -DNDEBUG
else
override BASEFLGS += -Og -ggdb
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
ifeq (1,$(strip $(NDEBUG)))
	$(Q)$(STRIP) $@
endif

%.c.o : %.c
	$(Q)$(CC) $(CFLAGS) -c -o $@ $^

%.cpp.o : %.cpp
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	$(Q)$(RM) $(PROG) $(SRC:%=%.o)
