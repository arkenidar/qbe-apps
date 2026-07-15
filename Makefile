# QBE + SDL2 demos
#   make all            build every demo (QBE and C versions)
#   make bounce         build out/bounce (from .ssa) and out/bounce-c (from .c)
#   make plasma / spin / circle-loop
#   make clean

# qbe from PATH if installed, else a local build at ~/qbe/qbe
# (e.g. on Termux, where "make install" isn't used); override with make QBE=...
QBE    := $(shell command -v qbe 2>/dev/null || echo $(HOME)/qbe/qbe)
CC     := gcc
LIBS   := -lSDL2 -lm

SSA_DEMOS := blank circle-loop bounce plasma spin
C_DEMOS   := blank circle-loop bounce plasma spin

all: $(SSA_DEMOS:%=out/%) $(C_DEMOS:%=out/%-c)

# qbe path: ssa-lang/x.ssa -> out/x.s -> out/x
out/%.s: ssa-lang/%.ssa
	$(QBE) -o $@ $<

out/%: out/%.s
	$(CC) $< -o $@ $(LIBS)

# gcc path: c-lang/x.c -> out/x-c (plus out/x-c.s for codegen comparison)
out/%-c: c-lang/%.c
	$(CC) $< -o $@ $(LIBS)
	$(CC) -S -O1 $< -o out/$*-c.s

# convenience per-demo targets: make bounce == out/bounce + out/bounce-c
$(C_DEMOS): %: out/% out/%-c

clean:
	rm -f out/*

.PRECIOUS: out/%.s
.PHONY: all clean $(SSA_DEMOS)
