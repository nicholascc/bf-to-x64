# Brainf*** to x64
A just-in-time [brainf***](https://en.wikipedia.org/wiki/Brainfuck) compiler for Windows [x86-64](https://en.wikipedia.org/wiki/X86-64).

###### Note: this program will *only* work on x86-64 Windows machines.

### Usage

The executable takes an input file path as a command line argument, and executes that file. Add a second verbose flag "-v" to get extra diagnostic information about the file's execution (a hex dump of the generated assembly code and a hex dump of the first 64 cells in memory after program completion).

Try out the examples in the `examples\` directory, such as `examples\mandelbrot.bf` which prints out an ASCII representation of the Mandelbrot set!

Currently this JIT compiler does not support the ',' (input) instruction, and does not perform any optimizations on the input (e.g. folding long strings of increments into a single addition).

### Details

The tape is 30000 cells long, and each cell is a single byte. The tape is not bounds-checked, so the head can overflow the tape and write to unknown memory without causing an error.

Code generation (done in `bf_to_x64.c`) uses functions specified in `instruction_encoding.c`, which exposes an assembly-language-like interface. `main.c` uses `bf_to_x64.c` to generate the assembly code and calls it as a function (no run-time optimization is done on the generated machine language).

### Compiling

Compilation has only been tested using the Microsoft C compiler (MSVC).
