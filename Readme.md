## RISC-V Assembler

This repository contains the code to convert a assembly code to a machine code .

### Building the project

---

To be able to compile the code, you need:

- A C++14 compiler (clang >= 6 or g++ >= 7 should be fine)

- CMake build system

To build the project, you need to use the `cmake` utility to generate the makefile .

- Clone the repository and go inside the folder path.
- Create a `build` folder by :

  ```
  mkdir build
  ```

- Go inside the `build` folder by :

  ```
  cd build
  ```

- Issue the following command to generate the makefile :

  ```
  cmake ..
  ```

- Run `make` to build the binaries . The main executable is called `assembler`.

### Running the Assembler

---

Once compiled, you should be ready to use this implementation of conversion of .asm file to .mc file. If you execute the binary `assembler` without any arguments, it will present you the command line usage :

```
./assembler
Usage: ./assembler <1: input file path>
```

Following the list of parameters you need to specify :

- `<1. input file path>` The path to the input file for which .mc file needs to be generated.

Example :

```
./assembler ../input/example1.asm
```
