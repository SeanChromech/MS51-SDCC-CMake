cmake_minimum_required(VERSION 3.23)
set(CMAKE_SYSTEM_NAME Generic)
set(DEVICE MS51XB9AE)
set(PROJECT_NAME MyProject)

# Find which bsp folder to use, by the name of the device.
file(GLOB BSP "${CMAKE_CURRENT_LIST_DIR}/MS51BSP_KEIL/*${DEVICE}*/Library")

if ("${BSP}" STREQUAL "")
    message(ERROR " No BSP found for ${DEVICE}")
    return()
else()
    message(STATUS "Found BSP at: ${BSP}")
endif ()

#Find the compiler
find_program(SDCC_COMPILER sdcc)
find_program(SDCC_OBJCOPY sdobjcopy)
if (${CMAKE_HOST_WIN32})
    if (NOT EXISTS "${SDCC_COMPILER}")
        list(APPEND CMAKE_PROGRAM_PATH "C:/Program Files(86)/SDCC/bin")
        find_program(SDCC_COMPILER sdcc)
        find_program(SDCC_OBJCOPY sdobjcopy)
    endif ()
    if (NOT EXISTS "${SDCC_COMPILER}")
        list(APPEND CMAKE_PROGRAM_PATH "C:/Program Files/SDCC/bin")
        find_program(SDCC_COMPILER sdcc)
        find_program(SDCC_OBJCOPY sdobjcopy)
    endif ()
endif ()
if (EXISTS "${SDCC_COMPILER}")
    set(CMAKE_C_COMPILER "${SDCC_COMPILER}")
else ()
    message(FATAL_ERROR "SDCC compiler is not found on your computer.")
endif ()

project(${PROJECT_NAME} C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

# Rewrite MS51_16K.h to work with SDCC
file(READ "${BSP}/Device/Include/MS51_16K.h" MS51_16K_H)

# Format everything correctly
# In MS51_16K.h, we need to replace all of the sfr definitions with __at() definitions.
# That is, we need to replace "sfr P0 = 0x80;" with "__sfr __at(0x80) P0;"
string(REGEX REPLACE "([a-z]+) ([A-Z0-9_]+) += ([A-Zx0-9^_]+);" "__\\1 __at(\\3) \\2;" MS51_16K_H_NEW "${MS51_16K_H}")

# We need to remove the math from the sbit definitions, as it is not supported by SDCC.
string(REGEX MATCHALL "[A-Z0-9_]+\\^[0-9]" SBIT_TARGETS "${MS51_16K_H_NEW}")
foreach(SBIT_TARGET IN LISTS SBIT_TARGETS)
    string(LENGTH ${SBIT_TARGET} LEN)
    math(EXPR LEN ${LEN}-2)
    string(SUBSTRING ${SBIT_TARGET} 0 ${LEN} SFR_NAME)
    string(REGEX REPLACE ".*sfr ${SFR_NAME}?[ =]+([0-9A-Fx]*).*" "\\1" SFR_LOC "${MS51_16K_H}")
    # Okay, so now we have both a register name and a value that it should be.
    # We need to replace the sbit sfr reference with the value.
    # Technically, we're doing this multiple times and we don't really need to, but it's easier to do this way.
    # This only actually happens once, on a PC, so who cares.
    string(REGEX REPLACE "__at\\(${SFR_NAME}" "__at(${SFR_LOC}" MS51_16K_H_NEW "${MS51_16K_H_NEW}")
endforeach()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/MS51_16K.h" "${MS51_16K_H_NEW}")

add_compile_options(-mmcs51 --model-medium)
add_link_options(-mmcs51
                 --iram-size 256
                 --xram-size 1024
                 --model-medium)

add_compile_definitions(bit=__bit
                        code=__code
                        data=__data
                        xdata=__xdata
                        interrupt=__interrupt)

add_compile_definitions(FOSC_240000)
# Nuvoton's commit 6f3f4f4 changed the way the timer divider is calculated, which breaks everything.
# This is a workaround for that.
add_compile_definitions(TIMER_DIV4_VALUE_10us=TIMER_DIV4_VALUE_10us_FOSC_24000000)
add_compile_definitions(TIMER_DIV12_VALUE_100us=TIMER_DIV12_VALUE_100us_FOSC_24000000)
add_compile_definitions(TIMER_DIV4_VALUE_500us=TIMER_DIV4_VALUE_500us_FOSC_24000000)
add_compile_definitions(TIMER_DIV4_VALUE_1ms=TIMER_DIV4_VALUE_1ms_FOSC_24000000)
add_compile_definitions(TIMER_DIV12_VALUE_1ms=TIMER_DIV12_VALUE_1ms_FOSC_24000000)
add_compile_definitions(TIMER_DIV12_VALUE_10ms=TIMER_DIV12_VALUE_10ms_FOSC_24000000)
add_compile_definitions(TIMER_DIV128_VALUE_100ms=TIMER_DIV128_VALUE_100ms_FOSC_24000000)

# I know, this should be a library, but I can't get it to work for some reason.
# I think SDCC is doing something weird.

# Add your source files here, preferably under main.c.
# Next, uncomment the parts of the BSP you're using.
# SDCC has pretty poor dead code elimination, so it's best to only include what you need.
add_executable(${PROJECT_NAME}
              main.c
              sdcc_compat/putchar.c
#              ${BSP}/StdDriver/src/bod.c
              ${BSP}/StdDriver/src/common.c
              ${BSP}/StdDriver/src/delay.c
#              ${BSP}/StdDriver/src/eeprom.c
#              ${BSP}/StdDriver/src/eeprom_sprom.c
#              ${BSP}/StdDriver/src/i2c.c
#              ${BSP}/StdDriver/src/IAP.c
#              ${BSP}/StdDriver/src/IAP_SPROM.c
#              ${BSP}/StdDriver/src/isr.c
#              ${BSP}/StdDriver/src/pwm.c
#              ${BSP}/StdDriver/src/spi.c
              ${BSP}/StdDriver/src/sys.c
              ${BSP}/StdDriver/src/timer.c
              ${BSP}/StdDriver/src/uart.c
#              ${BSP}/StdDriver/src/watchdog.c)
               )

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${BSP}/StdDriver/inc
                           ${CMAKE_CURRENT_LIST_DIR}/sdcc_compat
                           BEFORE ${CMAKE_CURRENT_BINARY_DIR}
                           ${BSP}/Device/Include)

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${SDCC_OBJCOPY} ARGS -I ihex -O binary ${PROJECT_NAME}.ihx ${PROJECT_NAME}.bin
)

install(SCRIPT "FlashDevice.txt")