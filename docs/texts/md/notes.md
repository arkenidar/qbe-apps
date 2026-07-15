# QBE IL notes — as used by the demos in `ssa-lang/`

QBE (<https://c9x.me/compile/>) compiles a small SSA-style intermediate language
to assembly. Full spec: <https://c9x.me/compile/doc/il.html>.

## Build pipeline

```sh
qbe -o out/bounce.s ssa-lang/bounce.ssa      # IL -> assembly
gcc out/bounce.s -o out/bounce -lSDL2 -lm   # assemble + link
```

Or just `make bounce` (see the root `Makefile`). Every demo also has a C twin in
`c-lang/`; `make` drops its `-O1` assembly next to QBE's (`out/bounce-c.s` vs
`out/bounce.s`) so you can diff the two compilers' codegen.

## Types

| type | meaning            | used for                          |
|------|--------------------|-----------------------------------|
| `w`  | 32-bit word        | ints, SDL flags, colors, booleans |
| `l`  | 64-bit long        | pointers (SDL_Window*, buffers)   |
| `s`  | 32-bit float       | (not used here)                   |
| `d`  | 64-bit double      | angles, sin/cos math              |

Constants: plain integers (`32`, `-1`), doubles with a prefix: `d_3.14159`.

## Temporaries and control flow

- `%name =w add %a, %b` — assign a temporary. QBE IL does **not** have to be
  strict SSA: reassigning `%x` in a loop is fine, qbe inserts phis itself.
- Blocks start with `@label`. Terminators: `jmp @l`, `jnz %cond, @then, @else`,
  `ret [%val]`.
- Comparisons produce a `w` 0/1: `csltw` (signed <), `cslew` (<=), `csgew` (>=),
  `ceqw` (==)… doubles use `cltd`, `cled`, etc.
- There is no `!`/`&&`; combine conditions with `and`/`or` on the 0/1 results
  (see the wall-bounce checks in `bounce.ssa`).

## Memory

- `%p =l alloc4 16` — 16 stack bytes, 4-aligned (`alloc8` for 8-aligned).
  Used for SDL_Rect (4 words) and SDL_Event (64 bytes, first word = event type).
- `storew %val, %addr` / `%v =w loadw %addr`; doubles: `stored`/`loadd`.
- No indexing syntax — compute addresses by hand:
  `%off =l mul %il, 8` + `%p =l add %base, %off` (see cube vertices in `spin.ssa`).
  Word-typed indexes must be widened first: `%il =l extsw %i`.

## Floats

- `%d =d swtof %w` — signed word → double; `%w =w dtosi %d` — double → int
  (truncates).
- libm is just a call: `%s =d call $sin(d %a)` — hence `-lm` at link time.

## Calls and functions

- `%r =w call $SDL_Init(w 32)` — args are `type value` pairs; the result type
  annotation must match the C prototype (`l` for returned pointers).
- Defining functions: `function w $channel(d %v, d %phase) { ... ret %c }`
  (see `plasma.ssa`); omit the return type for void. `export` makes it visible
  to the linker — `$main` needs it.
- Globals: `data $title = { b "qbe bounce", b 0 }` — a NUL-terminated byte string.

## SDL constants used (SDL2 headers, spelled as integers in IL)

| constant                | value        |
|-------------------------|--------------|
| `SDL_INIT_VIDEO`        | `32`         |
| `SDL_WINDOWPOS_CENTERED`| `805240832`  |
| `SDL_RENDERER_ACCELERATED` | `1`       |
| `SDL_QUIT` (event type) | `256`        |
| `SDL_KEYDOWN` (event type) | `768`     |
| `SDLK_ESCAPE` (`keysym.sym`, at SDL_Event byte offset 20) | `27` |

## The demos, in learning order

1. `circle-loop.ssa` — static frame, nested loops, `SDL_WaitEvent`.
2. `bounce.ssa` — real animation loop (`SDL_PollEvent` + `SDL_Delay 16`),
   loop-carried state, a second function (`$fillcircle`) and the call ABI.
3. `plasma.ssa` — double math, libm `sin`, a helper returning a value.
4. `spin.ssa` — 2D rotation (triangle), then 3D rotation + perspective
   projection (cube) with manual array address arithmetic.
