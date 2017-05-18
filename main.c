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
#define CPU_FREQUENCY     4194304

// memory map:

// the memory starts with cartridge data
#define CARTRIDGE_START     0

  #define INTERRUPT_START   CARTRIDGE_START
  #define INTERRUPT_SIZE    256

  #define ROM_DATA_START    INTERRUPT_START + INTERRUPT_SIZE
    #define GAME_TITLE      308
    #define GAME_TITLE_SIZE 14
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

    #define REG_P1          65280    /* controller register of bits 76543210, contains
                                        the state of buttons, which form a matrix
                                        (bits 6 and 7 are unused):

                                            4       5
                                        0  RIGHT  A
                                        1  LEFT   B
                                        2  UP     SELECT
                                        3  DOWN   START */
    #define REG_DIV         65284    /* upper 8 bits of the counter that counts the
                                        clock frequency, if LD is executed, the bits
                                        are cleared */
    #define REG_TIMA        65285    /* main timer counter, generates interrupt on
                                        overflow */
    #define REG_TMA         65286    /* timer modulo, when TIMA overflows, this value
                                        is loaded into TIMA */
    #define REG_TAC         65287    /* timer control register, writing 1 to the bit
                                        2 starts the timer, bits 0 and 1 define a
                                        number n that defines the speed of counting as
                                        f/(2^(2^n)) */

  #define REGISTER_SIZE     128

  // CPU work RAM and stack RAM
  #define STACK_START       REGISTER_START + REGISTER_SIZE
  #define STACK_SIZE        126 

#define CPU_RAM_SIZE        OAM_SIZE + EMPTY_SIZE + REGISTER_SIZE + STACK_SIZE 

#define TOTAL_MEMORY_SIZE   CARTRIDGE_START + VRAM_SIZE + EXTERNAL_RAM_SIZE + RAM_SIZE + PROHIBITED_SIZE + CPU_RAM_SIZE

#include "gui.c"         // we don't need no shitty header files

unsigned char *memory;   // main memory pointer
unsigned char *screen;   // actual screen pixels

/*
  Loads ROM from given filename into memory.
*/

int load_rom(char *filename)
  {
    FILE *input_file = fopen(filename,"rb");

    if (!input_file)
      {
        printf("Error: could not open file: %s.\n",filename);
        return 0;
      }

    fread(memory + CARTRIDGE_START,1,CARTRIDGE_SIZE,input_file); 

    fclose(input_file);
    return 1;
  }

int main()
  {
    memory = (unsigned char *) malloc(TOTAL_MEMORY_SIZE);
    screen = (unsigned char *) malloc(DISPLAY_PIXELS);

    load_rom("tetris.gb");
    printf("%s\n",(char *) memory + GAME_TITLE);

    gui_init();

    for (uint i = 0; i < 20; i++)
      screen[SCREEN_XY(i,i)] = i * 12;

    gui_update_window(screen);
    gui_wait(2000);
    gui_end();

    free(memory);

    return 0;
  }
