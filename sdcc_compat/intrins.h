/* intrins.h - Keil intrinsics
 * This is a stub that emulates the Keil intrinsics.
 *
 * These are the only ones implemented by https://github.com/danchouzhou/MS51BSP_SDCC.
 * More could be added in the future, if needed.
 */

#ifndef COMPAT_INTRINS_H
#define COMPAT_INTRINS_H

#define _push_(x)   __asm push _##x __endasm
#define _pop_(x)    __asm pop _##x __endasm
#endif
