# MS51-SDCC-CMake
A setup for using SDCC and CMake to develop on the Nuvoton MS51 microcontroller.
The secret sauce is some CMake-fu that transforms the official SDK/BSP to something digestible by SDCC on build.

At time of writing, this has been tested with the MS51XB9AE, and SDK/BSP version commit c0cb83d.

This assumes you're using Windows, but adapting it to Linux or Mac should be trivial.

Also, I'm not a CMake or Regex expert, so there's probably a better way to do this.  If you know of one, please let me know!  Obviously, test the outputs of this thoroughly before using it in production, and don't blame me if it breaks your stuff.

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
1. Install SDCC, ideally under C:/Program Files/SDCC.
2. Clone this repo.
3. Sync the official SDK/BSP as a submodule.
   1. In a terminal, cd to the root of this repo.
   2. `git submodule update --init`

## Building
- If using CLion, you can open the project directly.  I've included a custom compiler definition for SDCC, so you should be able to build and run the project right away.

- If using VSCode:
  1. you'll need to install the CMake Tools extension.
  2. Open the project folder in VSCode.
  3. If it asks for a kit, select "Unspecified".
  4. Open the command palette (Ctrl+Shift+P) and run the "CMake: Configure" command.
  5. Open the command palette again and run the "CMake: Build" command.
  6. Ignore the warnings that are internal to the SDK/BSP.  Blame Nuvoton...
  7. You should now have a .bin and .ihx file in the build folder.
     - Protip! Check out the .map file if you're curious about the memory layout.
  8. You can flash the .bin file directly in VSCode by running "CMake: Install".

Good luck!
