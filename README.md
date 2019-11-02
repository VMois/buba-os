# Buba OS

The simple OS is written from scratch in Assembly and C. Project for "Basic of programming 1" course at Budapest University of Technology and Economics (BME).

## Goal

The goal is to learn how operating systems are working by making a simple OS.
The development includes using Assembly and C programming languages, and concepts like boot sector, stack, interrupt, kernel. No advanced OS development techniques/tools are used, everything is built from scratch. The system is not indented for production use and it does not serve any other goal except educational.

## System structure and features

The system will be able to run using QEMU (virtual machine) or live USB stick. The system will consist of:

- boot sector, written in Assembly
- kernel + shell, written in C
- simple filesystem (?)

The shell will have a few simple commands that are able to print on the screen or read user input.

## Development (Work in progress)

Details about how to develop, compile and run the system.

### how to compile

```bash
nasm -f bin [filname].asm -o [filename].bin
```

### how to run

```bash
qemu-system-x86_64 [something].bin
```

## Resources

Used material and tutorials:

- https://github.com/cfenollosa/os-tutorial
- http://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
- https://wiki.osdev.org/Main_Page
