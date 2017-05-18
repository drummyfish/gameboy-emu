/*
  Original gameboy (NOT color) emulator intended as a simple
  learning project. Readability is preferred to efficiency.

  resources:
    http://www.chrisantonellis.com/files/gameboy/gb-programming-manual.pdf
    http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define KBYTE             1024
#define DISPLAY_WIDTH     160
#define DISPLAY_HEIGHT    144
#define SCREEN_XY(x,y)    x + y * DISPLAY_WIDTH
#define DISPLAY_PIXELS    DISPLAY_WIDTH * DISPLAY_HEIGHT
#define CPU_FREQUENCY     1050000

// memory map:

// the memory starts with cartridge data
#define CARTRIDGE_START   0

  #define INTERRUPT_START   CARTRIDGE_START
  #define INTERRUPT_SIZE    256

  #define ROM_DATA_START    INTERRUPT_START + INTERRUPT_SIZE
  #define ROM_DATA_SIZE     79     

  #define PROGRAM_START     ROM_DATA_START + ROM_DATA_SIZE
  #define PROGRAM_SIZE      32432

#define CARTRIDGE_SIZE      INTERRUPT_SIZE + ROM_DATA_SIZE + PROGRAM_SIZE

// video ram - it stores sprite data, not pixels
#define VRAM_START          CARTRIDGE_START + CARTRIDGE_SIZE 
#define VRAM_SIZE           8 * KBYTE

// external extension RAM
#define EXTERNAL_RAM_START  VRAM_START + VRAM_SIZE
#define EXTERNAL_RAM_SIZE   8 * KBYTE

// main RAM
#define RAM_START           EXTERNAL_RAM_START + EXTERNAL_RAM_SIZE
#define RAM_SIZE            8 * KBYTE

// this memory mustn't be accessed
#define PROHIBITED_START    RAM_START + RAM_SIZE
#define PROHIBITED_SIZE     7679

// for CPU
#define CPU_RAM_START       PROHIBITED_START + PROHIBITED_SIZE

  // OAM = object attribute memory
  #define OAM_START         CPU_RAM_START
  #define OAM_SIZE          160

  // this part of memory is unused
  #define EMPTY_START       OAM_START + OAM_SIZE
  #define EMPTY_SIZE        96

  // various registers and flags
  #define REGISTER_START    EMPTY_START + EMPTY_SIZE
  #define REGISTER_SIZE     128

  // CPU work RAM and stack RAM
  #define STACK_START       REGISTER_START + REGISTER_SIZE
  #define STACK_SIZE        126 

#define CPU_RAM_SIZE        OAM_SIZE + EMPTY_SIZE + REGISTER_SIZE + STACK_SIZE 

#define TOTAL_MEMORY_SIZE   CARTRIDGE_START + VRAM_SIZE + EXTERNAL_RAM_SIZE + RAM_SIZE + PROHIBITED_SIZE + CPU_RAM_SIZE

#include "gui.c"         // we don't need no shitty header files

unsigned char *memory;   // main memory pointer
unsigned char *screen;   // actual screen pixels

int main()
  {
    memory = (unsigned char *) malloc(TOTAL_MEMORY_SIZE);
    screen = (unsigned char *) malloc(DISPLAY_PIXELS);

    gui_init();

    for (uint i = 0; i < 20; i++)
      screen[SCREEN_XY(i,i)] = i * 12;

    gui_update_window(screen);
    gui_wait(2000);
    gui_end();

    free(memory);

    return 0;
  }
