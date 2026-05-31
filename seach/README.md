# searching_for_tchisla (HASH & RBT, fix_factor_bug)

This is a small standalone search program for the *Tchisla* style repeated-digit game.
It enumerates rational values reachable from a base digit using a limited set of
operations and reports how to build a requested target value.

The original C++ code was written in 2016 by **Indestinee** (see `search.cpp` header).
This repository simply repackages that code (plus the original Windows binary and
PDF) into a Git-friendly layout.

> **Note**: Licensing terms are not explicitly stated in the source.
> Treat this as archival / research material unless you have explicit
> permission from the original author.


## Contents

- `search.cpp`  
  Core search engine implementation (C++).
- `search.exe`  
  Prebuilt Windows executable from 2016 (likely 32‑bit, as compiled at the time).
- `readme.pdf`  
  Original description / notes from the author (binary PDF, not modified here).


## What the program does

The program searches expressions built from repeated copies of a **base digit**
(e.g. `7`, `77`, `777`, …) up to a given maximum **step** count.

Each expression is represented as a rational number using a custom `frac` type.
The search explores expressions reachable using the following operations:

- Addition: `a + b`
- Subtraction: `a - b` (absolute value is taken internally)
- Multiplication: `a * b`
- Division: `a / b` and `b / a`
- Power: `a ^ b` and `b ^ a` (integer exponents only)
- Power of reciprocal: `(1/a) ^ b` and `(1/b) ^ a`
- Factorial: `a!` (precomputed for small integers only)
- Square root: `sqrt(a)` (only when result stays rational)

The state space is kept finite by:

- Working in rationals with bounded numerator/denominator
  (`infd = 1e12`, `infn = 1e3` in `search.cpp`).
- Rejecting negative or obviously invalid intermediate values.
- Recording the best (lowest‑cost) construction for each reachable value.

Search results and reconstruction information are stored in either:

- `unordered_map` / `unordered_set` with a custom hash (`HASH` mode), or
- `std::map` / `std::set` based on red‑black trees (RBT mode).

The shipped source defines `HASH` by default.


## Building from source

### Prerequisites

- A C++11‑capable compiler (GCC or Clang recommended).
- Standard C++ library.

### macOS / Linux (GCC or Clang)

From the `seach/` directory:

```bash
g++ -O2 -std=c++11 search.cpp -o search
# or with Clang
clang++ -O2 -std=c++11 search.cpp -o search
```

This produces an executable named `search` in the same directory.

### Windows (MinGW)

From a MinGW shell in the `seach/` directory:

```bash
g++ -O2 -std=c++11 search.cpp -o search.exe
```

You can also run the original `search.exe` that ships with this archive, but
rebuilding from source is recommended so you know exactly what binary you have.


## Running the search

After building, run the program from a terminal:

```bash
./search
```

On Windows (MinGW):

```bat
search.exe
```

The program is interactive and uses standard input/output.

### 1. Precomputation phase

On startup you will see a line like:

```text
    Use HASH..
Please input base and steps, for prepare the progress:
```

Enter two integers:

- `base` – the digit used to build repeated numbers (e.g. `7`).
- `steps` – maximum number of digit positions (expression "length").

Example:

```text
Please input base and steps, for prepare the progress: 7 4
```

The program then builds a search frontier from:

- Step 1: `7`
- Step 2: `77`
- Step 3: `777`
- … up to `steps` digits, combining previously found values with the
  operations listed above.

Progress is printed as:

```text
Step[1] finished..
Step[2] finished..
...
Step[steps] finished..
Start..
```

The larger `steps` is, the more memory and time this phase will take.

### 2. Query phase

After `Start..`, you can query target values, one per line. Input format is:

- Integer: `42`
- Rational: `a/b` (e.g. `3/7`)

For each non‑zero value `v` that is reachable within the given step limit,
the program prints a reconstruction of how to build `v` using the allowed
operations. The output is text‑only and shows both the final expression and
its decomposition into sub‑expressions.

If a value cannot be found, it prints a message like:

```text
Not find (x | y) within N steps..
```

Send EOF (Ctrl‑D on Unix, Ctrl‑Z then Enter on Windows) to terminate.


## Internal details and options

Some notable implementation details from `search.cpp`:

- Values are represented by the `frac` class (`d` for numerator, `n` for
  denominator) with automatic reduction and sanity checks.
- Factorials are precomputed up to `cnt_f = 12` in the `f[]` array, and
  only small integer arguments are allowed.
- Square roots only succeed when both numerator and denominator are
  perfect squares; otherwise `sqrt(a)` is treated as invalid and
  replaced by zero.
- The global `mp` map tracks, for each reachable value, its minimum
  cost (`step`) and the operation used to produce it so that full
  expressions can be reconstructed.

### Switching between HASH and RBT modes

By default, the source is compiled in **HASH** mode:

```cpp
#define HASH
```

To switch to red‑black tree structures (`std::map` / `std::set`), comment out
or remove that macro and recompile:

```cpp
// #define HASH
```

HASH mode is usually faster for large searches; RBT mode can be helpful for
more deterministic iteration order or debugging.


## Files in this repository

- `search.cpp` – C++11 source code for the search engine.  
- `search.exe` – prebuilt Windows executable from 2016.  
- `readme.pdf` – original PDF documentation from the author.  
- `README.md` – this file, added to make the project easy to browse on GitHub.


## Credits

- Original algorithm and implementation: **Indestinee** (2016), see header in
  `search.cpp`.
- This repository layout and `README.md`: packaging only, no functional
  changes to the original code.
