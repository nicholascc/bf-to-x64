# Brainf*** to x64
A just-in-time [brainf***](https://en.wikipedia.org/wiki/Brainfuck) compiler for Windows [x86-64](https://en.wikipedia.org/wiki/X86-64).

###### Note: this program will *only* work on x86-64 Windows machines.

### Usage

The executable takes an input file path as a command line argument, and executes that file, providing some extra diagnostic information (a hex dump of the first 256 memory cells).

Currently this JIT compiler only supports the +, -, <, >, and . instructions. The loop and input instructions will be added soon!

### Details

The tape is 30000 cells long, and each cell is a single byte. The tape is not bounds-checked, so the head can overflow the tape and write to unknown memory without causing an error.

Code generation (done in `bf_to_x64.c`) uses functions specified in `instruction_encoding.c`, which expose an assembly-language-like interface. `main.c` uses `bf_to_x64.c` to generate the assembly code and calls it as a function (no run-time optimization is done on the generated machine language).

### Compiling

Compilation has only been tested using the Microsoft C compiler (MSVC).
