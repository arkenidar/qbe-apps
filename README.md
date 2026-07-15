# qbe-apps — graphical demos in QBE IL and C

Welcome! This is a little playground for learning [QBE](https://c9x.me/compile/),
the lightweight compiler backend, by writing **graphical SDL2 demos directly in
QBE's intermediate language** — and pairing each one with an equivalent C program
so you can compare the two side by side, source and generated assembly alike.

## The demos

Every demo exists twice: hand-written IL in `ssa-lang/` and a C twin in `c-lang/`.
In rough learning order:

| demo | what it shows | what it teaches |
|------|---------------|-----------------|
| `blank` | empty window | the starter skeleton: init, frame loop, event handling |
| `circle-loop` | checkerboard + red circle (static) | nested loops, branches, scanline circle |
| `bounce` | ball bouncing off the walls | real animation loop, loop-carried state, defining a second function |
| `plasma` | shimmering old-school plasma | double math, libm `sin`, a value-returning helper |
| `spin` | rotating triangle + wireframe 3D cube | 2D/3D rotation, perspective projection, manual array addressing |

All windows are 800×600 and quit on **ESC** or window close.

## Building and running

You need `gcc`, SDL2 dev headers (`libsdl2-dev`), and `qbe` — see
[docs/texts/000-instructions.txt](docs/texts/000-instructions.txt) for how qbe
was installed here (clone from `git://c9x.me/qbe.git`, `make`, `sudo make install`).

If `qbe` is not on your PATH — e.g. on Termux, where you just build it in place —
the Makefile falls back to `~/qbe/qbe`; any other location works with
`make QBE=/path/to/qbe`.

```sh
make all        # build everything
make spin       # build one demo, both versions: out/spin and out/spin-c
./out/spin      # run the QBE version
./out/spin-c    # run the C version
make clean
```

The pipeline for the IL versions is two steps — QBE compiles IL to assembly,
gcc assembles and links it:

```sh
qbe -o out/spin.s ssa-lang/spin.ssa
gcc out/spin.s -o out/spin -lSDL2 -lm
```

## Comparing the two compilers

For every C demo, make also emits gcc's `-O1` assembly next to QBE's output:

```sh
make bounce
diff out/bounce.s out/bounce-c.s   # QBE codegen vs gcc codegen
```

The `plasma` pair is the most interesting for float codegen (sin calls,
int↔double conversions); `spin` shows how C arrays map to QBE's manual
pointer arithmetic.

## Starting your own demo

1. Copy the starter pair: `ssa-lang/blank.ssa` and `c-lang/blank.c`
   (say, to `mydemo.ssa` / `mydemo.c`).
2. Add `mydemo` to `SSA_DEMOS` and `C_DEMOS` in the [Makefile](Makefile).
3. Fill in the three `PLACEHOLDER` sections in each file: persistent state,
   per-frame update, drawing.
4. `make mydemo` and run `./out/mydemo`.

## Learning notes

[docs/texts/md/notes.md](docs/texts/md/notes.md) is a one-page QBE IL
cheatsheet keyed to these demos: types, temporaries and control flow, memory
and address arithmetic, float conversions, the call ABI, and the SDL constants
spelled as integers in the IL (including the ESC-key trick — reading
`keysym.sym` straight out of the event buffer at byte offset 20).

The full IL specification lives at <https://c9x.me/compile/doc/il.html>.

## Layout

```
ssa-lang/   hand-written QBE IL demos (*.ssa)
c-lang/     equivalent C programs (*.c)
out/        build artifacts: binaries and .s assembly (git-ignored)
docs/       install notes and the IL cheatsheet
Makefile    builds every demo in both flavors
LICENSE     MIT
```

## License

[MIT](LICENSE) — do what you like, keep the notice.
