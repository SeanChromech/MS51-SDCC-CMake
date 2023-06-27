# MS51-SDCC-CMake
A setup for using SDCC and CMake to develop on the Nuvoton MS51 microcontroller.
The secret sauce is some CMake-fu that transforms the official SDK/BSP to something digestible by SDCC on build.

## Why, What, and How
### Why
Compilers should be free.  Unless you're working for a huge corporation, it doesn't make much sense to program an ultra-cheap microcontroller using thousands of dollars worth of software.

The official SDK/BSP for the MS51 is designed to work with Keil's uVision IDE/compiler, which is not free.  SDCC is a free compiler that supports the 8051 architecture, but it doesn't support the Nuvoton-provided SDK/BSP as-is.

### What
To get around this, we can use CMake to transform the SDK/BSP into something that SDCC can understand.  There are also a few files built into Keil that the SDK/BSP assumes exist, but don't exist in SDCC.  We provide a shim version that works well enough to get the job done.

### How
Other attempts I've seen at getting the SDK/BSP working on SDCC have involved modifying the SDK/BSP itself.  This is a bad idea, because it makes it difficult to update the SDK/BSP in the future.  Instead, we use CMake to transform the SDK/BSP on build, and then use it to build our project.

Most of the magic happens in CMakeLists.txt.  The sdcc_compat folder contains the shim files that make the SDK/BSP work with SDCC.

## Getting Started
Coming soon...