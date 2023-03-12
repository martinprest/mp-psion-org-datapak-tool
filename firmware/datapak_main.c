
////////////////////////////////////////////////////////////////////////////////
//
// Psion Organiser Datapack Tool Control Program
//
// This is an early version of the code which is a copy
// of the pak_gadget code with enough changes to test the
// first version of the hardware wrt the display, keys and
// SD card.
//
//
////////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"

#include "f_util.h"

#include "ff.h"
#include "ff_stdio.h"
#include "hw_config.h"
#include "my_debug.h"
#include "rtc.h"
#include "sd_card.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef int boolean;


boolean paged_addr                  = true;     // true for paged addressing, false for linear addressing - paged is default
boolean datapak_mode              = true;     // true for datapaks, false for rampaks, mode can be changed by command option
boolean program_low                 = false;    // will be set true when SLOT_SPGM_PIN is low during datapak write, so page counter can be pulsed accordingly
const boolean force_write_cycles    = false;    // set true to perform max write cycles, without break for confirmed write
const boolean overwrite             = false;    // set true to add a longer overwite after confirmed write
const byte max_datapak_write_cycles = 5;        // max. no. of write cycle attempts before failure
const byte datapak_write_pulse      = 100;      // datapak write pulse in us, 1000 us = 1 ms, 10us write can be read by Arduino, but not Psion!
word current_address                = 0;
#define max_eprom_size              0x8000      // max eprom size - 32k - only used by Matt's code

boolean read_fixed_size             = false;    // true for fixed size
//boolean read_fixed_size           = true;     // true for fixed size
word read_pack_size                 = 0x7e9b;   // set a fixed pack size for read
//word read_pack_size               = 0x0100; 

byte CLK_val                        = 0;        // flag to indicate CLK state


// Use this if breakpoints don't work
#define DEBUG_STOP {volatile int x = 1; while(x) {} }

// Are we compiling for the gadget breakout or the picopak?
#define PICOPAK 0

// Drop into a loop that displays key states and does nothing else
#define KEY_DEBUG_ONLY                0

// Interrupts may muck up the pack interface, but it does seem to run with them enabled.
// If USB is ever to work then interrupts need to be enabled.
#define NO_INTERRUPTS_WHILE_POLLING   0
#define TEST_STDIO                    0

// Redefine pins to match our hardware
// datapack tool port note:
//
// We have SD clock defined as pin 5 here, but that is wired to SW3 so
// this doesn't make sense. It does seem to work, though.
// The actual clock pin is on GPIO18, which is CMD here, so I don't know what's
// going on

#define PICO_SD_CLK_PIN        5
#define PICO_SD_CMD_PIN       18
#define PICO_SD_DAT0_PIN      19
#define ENABLE_4_PIN           0

#define SUPPORT_ID_BYTE        1
#define PAK_ID_BYTE         0x01
#define READ_ONLY              0
#define INIT_PAK_MEMORY        0
#define FF_FIRST_BYTES         0

// Direct access to GPIO registers is faster, and we need speed
#define DIRECT_GPIO            1

bool sd_ok_flag = false;

// Do we use a polling loop of interrupts?
#define USE_INTERRUPTS         0
#define USE_POLLING            1

// For added speed, we poll the address counter stuff on the other core
#define MULTICORE_POLL         1

// The address into pak memory
#define PAK_ADDRESS (ss_address | ss_page)
//#define PAK_ADDRESSxxx (ss_address)

// Pack ID byte
#define PACK_ID_PAGED  0x04

// For the OLED display
const uint SDA_PIN             = 15;
const uint SCL_PIN             = 14;


//#ifndef I2C_FUNCTIONS_H_
#define I2C_FUNCTIONS_H_

typedef unsigned char BYTE;

// Buttons got changed for PCB layout reasons
#if 0
const int SW0_PIN       = 0;
const int SW1_PIN       = 1;
const int SW2_PIN       = 5;
const int SW3_PIN       = 26;
#else
const int SW0_PIN       = 28;
const int SW1_PIN       = 27;
const int SW2_PIN       = 26;
//const int SW3_PIN       = 5;
#endif

const int SLOT_SPGM_PIN = 10;
const int SLOT_SS_PIN   = 11;
const int SLOT_SCLK_PIN = 12;

const int SLOT_SOE_PIN  = 9;
const int SLOT_SMR_PIN  = 8;

const int SLOT_SD0_PIN  = 0;
const int SLOT_SD1_PIN  = 1;
const int SLOT_SD2_PIN  = 2;
const int SLOT_SD3_PIN  = 3;
const int SLOT_SD4_PIN  = 4;
const int SLOT_SD5_PIN  = 5;
const int SLOT_SD6_PIN  = 6;
const int SLOT_SD7_PIN  = 7;

const byte data_pin[8] =
  {
   SLOT_SD0_PIN,
   SLOT_SD1_PIN,
   SLOT_SD2_PIN,
   SLOT_SD3_PIN,
   SLOT_SD4_PIN,
   SLOT_SD5_PIN,
   SLOT_SD6_PIN,
   SLOT_SD7_PIN
  }; // pins D0 to D7 on Datapak

const int data_gpio[8] =
  {
   SLOT_SD0_PIN,
   SLOT_SD1_PIN,
   SLOT_SD2_PIN,
   SLOT_SD3_PIN,
   SLOT_SD4_PIN,
   SLOT_SD5_PIN,
   SLOT_SD6_PIN,
   SLOT_SD7_PIN,
  };

const int LS_DIR_PIN    = 13;
const int VPP_ON_PIN    = 17;

volatile int ss_count = 0;
volatile int soe_state = 1;
volatile int ss_address = 0;
volatile int ss_page = 0;


// Memory that emulates a pak
typedef unsigned char BYTE;
typedef void (*FPTR)();
typedef void (*CMD_FPTR)(char *cmd);

// The tracing was used for low level analysis of the protocol
#define TRACE_LENGTH 2
BYTE trace0[TRACE_LENGTH];
BYTE trace1[TRACE_LENGTH];

void nextPage();

volatile int trace_i = 0;
#define TRACE0(XX) if(trace_i != (TRACE_LENGTH-1)) {trace0[trace_i++] = XX; /*trace_i %= TRACE_LENGTH;*/}
#define TRACE1(XX) if(trace_i != (TRACE_LENGTH-1)) {trace1[trace_i++] = XX; /*trace_i %= TRACE_LENGTH;*/}

//#define TRACE(XX)

//#define WRITE_TRAP if( (PAK_ADDRESS + data) == 0)  while(1);

#define WRITE_TRAP

//#define PAK_MEMORY_SIZE  65536
#define PAK_MEMORY_SIZE  (128*1024)
//#define PAK_MEMORY_SIZE  32768


// Number of buttons used for the menu system. The 'exit polling' button is not in this
// list, it is coded as a GPIO line as we don't want to waste time processing the menu
// buttons in the fast polling loop

#define NUM_BUTTONS            3

// All organiser files are in this subdirectory on the SD card, just to keep things tidy
// and allow the card to be used for other things if needed.
#define PAK_DIR                "/PAK"

// Debounce
#define MAX_BUT_COUNT          6

#if PICOPAK
#define MENU_MAX  3
#else
#define MENU_MAX  6
#endif

int menu_offset = 0;


////////////////////////////////////////////////////////////////////////////////
//
// Prototypes
//
////////////////////////////////////////////////////////////////////////////////

word read_dir(void);
bool write_opk_file(I2C_SLAVE_DESC *slave, char *filename);
bool read_opk_file(I2C_SLAVE_DESC *slave, char *filename);
void button_compare_file(struct MENU_ELEMENT *e);
void compare_opk_file(I2C_SLAVE_DESC *slave, char *filename);
byte readByte();


bool logger_enabled;
const uint32_t period = 1000;
absolute_time_t next_time;





////////////////////////////////////////////////////////////////////////////////
//
// Read and process the config file
//
////////////////////////////////////////////////////////////////////////////////

void process_config_file(I2C_SLAVE_DESC *slave)
{
  char line[40];
  char fileline[80];

  //DEBUG_STOP;
  
  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }

  
  oled_clear_display(slave);
  oled_set_xy(slave, 0, 0);
  sprintf(line, "Processing");
  oled_display_string(slave, line);
      
  oled_set_xy(slave, 0, 8);
  sprintf(line, "config.txt");
  oled_display_string(slave, line);
  
  loop_delay(1000000);

  // Read the file from the SD card into the pak memory
  FF_FILE *fp = ff_fopen("config.txt", "r");

  if (fp == NULL)
    {
      sprintf(line, "Failed to open:");
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      oled_display_string(slave, line);
      
      oled_set_xy(slave, 0, 7);
      sprintf(line, "config.txt");
      oled_display_string(slave, line);

      loop_delay(3000000);
      unmount_sd();
      return;
    }

 
  // Get lines from the file
  while( ff_fgets(&(fileline[0]), sizeof(fileline)-1, fp) != NULL )
    {
      char keyword[80];
      char name[80];

      // Remove trailing newline
      fileline[strlen(fileline)-1] = '\0';
      
      keyword[0] = '\0';
      name[0] = '\0';

      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      sprintf(line, "[%s]", fileline);
      oled_display_string(slave, line);
      loop_delay(3000000);
      
      sscanf(fileline, "%[^=]=%s", keyword, name);

      if( strcmp(keyword, "startfile") == 0 )
	{
	  strcpy(current_file, name);
	  
	  // read the file
	  core_read(&oled0, current_file);
	}
      
      if( strcmp(keyword, "brightness") == 0 )
	{
	  sscanf(name, "%d", &brightness_percent);
	  oled_set_brightness(&oled0, brightness_percent);
	  
	  oled_clear_display(slave);
	  oled_set_xy(slave, 0, 0);
	  sprintf(line, "Brightness: %d%%", brightness_percent);
	  oled_display_string(slave, line);
	  loop_delay(3000000);
	}
      
    }
  
  ff_fclose(fp);
  unmount_sd();
}

// Read the file with the given name into the buffer
// Pak images are in a directory called PAK

void core_read(I2C_SLAVE_DESC *slave, char * arg)
{
  char line[40];

  //DEBUG_STOP;

  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }
  
  oled_clear_display(slave);
  oled_set_xy(slave, 0, 0);
  sprintf(line, "Reading file");
  oled_display_string(slave, line);
      
  oled_set_xy(slave, 0, 8);
  sprintf(line, "%s", arg);
  oled_display_string(slave, line);
  
  loop_delay(1000000);

  // Read the file from the SD card into the pak memory
  FIL fil;
  FRESULT fr = f_open(&fil, arg, FA_READ);

  if (FR_OK != fr)
    {
      sprintf(line, "Failed to open:");
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      oled_display_string(slave, line);
      
      oled_set_xy(slave, 0, 7);
      sprintf(line, "%s", arg);
      oled_display_string(slave, line);

      loop_delay(3000000);
      unmount_sd();
      return;
    }
  
  char buf[1];
  int pak_i = 0;
  int br = 0;
  bool done = false;
  char name[80];
  char extension[20];
  bool modify_header = false;
  
  // If the extension is .opk then we drop the first 6 bytes
  if( sscanf(arg, "%[^.].%s", name, extension) == 2 )
    {
      if( strcmp(extension, "opk") == 0 )
	{
	  // Drop 6 bytes from start of file
	  for(int i=0; i<6; i++)
	    {
	      f_read(&fil, buf, sizeof buf, &br);
	    }

	  // We need to modify the header, so set flag telling us to do
	  // that later
	  modify_header = true;
	}
    }
  
  while( !done )
    {
      f_read(&fil, buf, sizeof buf, &br);
      
      if( br == 0 )
	{
	  done = true;
	  continue;
	}
      
      for(int i=0; i<br; i++)
	{
	  pak_memory[pak_i++] = buf[i];
	}
    }
  f_close(&fil);
  
  oled_set_xy(slave, 0, 16);
  sprintf(line, "%d bytes read", pak_i);
  oled_display_string(slave, line);

  // Do we modify the header?

  if( modify_header )
    {
      word csum = 0;
      
      // For reference, this arrangement works as 32K pack
      //  pak_memory[0] = 0x78;
      //  pak_memory[1] = 0x04; //32K, could allow original length through
      //  pak_memory[2] = 0x56;
      //  pak_memory[3] = 0x00;
      //  pak_memory[4] = 0x03;
      //  pak_memory[5] = 0x02;
      //  pak_memory[6] = 0x35;
      //  pak_memory[7] = 0x46;
      //  pak_memory[8] = 0x06;
      //  pak_memory[9] = 0x4c;

      // Pass flags through as we can now handle paged packs
#if 0
      pak_memory[0] = 0x68;
#endif

      // We can allow sizes up to and including 128K
      if( pak_memory[1] > 0x10 )
	{
	  pak_memory[1] = 0x10; //128K
	}
      
      // Pass next bytes on unaltered
#if 0      
      pak_memory[2] = 0x56;
      pak_memory[3] = 0x00;
      pak_memory[4] = 0x03;
      pak_memory[5] = 0x02;
      pak_memory[6] = 0x35;
      pak_memory[7] = 0x46;
#endif
      
      // Calculate correct checksum
      // The next two bytes are a cheksum of the first four WORDS
      for(int i = 0; i<8; i+=2)
	{
	  csum += pak_memory[i]*256+pak_memory[i+1];
	}
      pak_memory[8] = csum >> 8;
      pak_memory[9] = csum & 0xFF;
      
    }
  
  loop_delay(3000000);
  unmount_sd();
}

// Writes the buffer to a file.
// Deletes any file that exists with the same name so that the resulting
// file is the same size as the buffer
// Searches the card directory for the highest file number and uses the next one
//

void core_writefile(bool oled_nserial)
{
  char filename[20];
  int i;
  FRESULT fr;
  FIL fp;
  int bytes_written = 0;
  char line[40];

  //DEBUG_STOP

  mount_sd();
  
  if( cd_to_pak_dir(&oled0) )
    {
      unmount_sd();
      return;
    }

  find_next_file_number();
  sprintf(filename, PAK_FILE_NAME_FORMAT, ++max_filenum);
  
  oled_clear_display(&oled0);
  oled_set_xy(&oled0, 0, 0);
  oled_display_string(&oled0, "Writing");
  oled_set_xy(&oled0, 0, 8);
  oled_display_string(&oled0, filename);
  
  // Open file for writing
  fr = f_open(&fp, filename, FA_CREATE_NEW | FA_WRITE);

  if (FR_OK != fr && FR_EXIST != fr)
    {
      oled_error("Open error");
      unmount_sd();
      return;
    }

  // As we are writing an OPK file, we add a header in the first 6 bytes. We don't set the size yet
  BYTE hdr[6] = {'O', 'P', 'K', 0, 0, 0};

  f_write (&fp, &(hdr[0]), 6, &bytes_written); // set size
  
  // Write the entire buffer in one go
  f_write (&fp, &(pak_memory[0]), PAK_MEMORY_SIZE, &bytes_written);

  bytes_written += 6;
  
  fr = f_close(&fp);
  if (FR_OK != fr)
    {
      printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
      unmount_sd();
      return;
    }

  oled_set_xy(&oled0, 0, 3*8);
  sprintf(line, "%d bytes", bytes_written);
  oled_display_string(&oled0, line);

  unmount_sd();

}



////////////////////////////////////////////////////////////////////////////////
//
// Auto size a datapack
//
////////////////////////////////////////////////////////////////////////////////

// This will not work with a blank datapack.

uint8_t page0[256];
int paged_size = 0;
int linear_size = 0;
uint32_t i;

int check_for_page0(void)
{

  uint8_t byte;
  
  ArdDataPinsToInput();          // ensure Arduino data pins are set to input
  packOutputAndSelect();         // Enable pack data bus output then select it
  resetAddrCounter();            // reset counters

  // Skip page 0
  for(int x=0; x<256; x++)
    {
      nextAddress();
    }
  
  // Go to every page over the possible address range
  for(int i=256; i<128*1024-256; i+=256)
    {
      // See if page0 is here
      boolean matched = true;
      
      for(int j=0; j<256; j++)
	{
	  byte = readByte();
	  
	  //printf("\n Checking %02X against %02X at %06X", page0[j], byte, i);		 

	  if( page0[j] == byte )
	    {
	      // Match
	    }
	  else
	    {
	      // No match
	      matched = false;
	    }
	  nextAddress();
	}

      if( matched )
	{
	  // Page 0 has appeared here, so assume address range has wrapped
	  // return start of matchinmg wrapped page
	  packDeselectAndInput();
	  return(i);
	}
    }

  packDeselectAndInput();
  return(i);

}

////////////////////////////////////////////////////////////////////////////////
//
// Auto size
//
////////////////////////////////////////////////////////////////////////////////


void auto_size(int oled_nserial)
{
  // Take whatever datapack is attached and attempt to work out
  // the size and addressing mode

  // Check the first 256 bytes against other 256 byte pages. When we get a math,
  // assume that is the same page and addressing has wrapped.

  // Read the first page
  ArdDataPinsToInput();          // ensure Arduino data pins are set to input
  packOutputAndSelect();         // Enable pack data bus output then select it
  resetAddrCounter();            // reset counters

  if( oled_nserial )
    {
      oled_clear_display(&oled0);
      oled_set_xy(&oled0, 0, 0);
      oled_printf(&oled0, "Auto Sizing Datapack");
    }
  
  printf("\nAuto sizing datapack\n");
  
  printf("\nReading page 0");
  
  for(int i=0; i<256; i++)
    {
      if( (i%16)==0 )
	{
	  printf("\n%02X: ", i);
	}
      
      page0[i] = readByte();
      printf("%02X ", page0[i]);
      
      nextAddress();
    }

  packDeselectAndInput();

  // We have a page0 that we can check for throughout the
  // pack address range...
  
  // First, addressing mode
  paged_addr = false;
  linear_size = check_for_page0();
  paged_addr = true;
  paged_size = check_for_page0();


  printf("\nPaged  addressing wrap:0x%06X (Size %d)", paged_size, paged_size);
  printf("\nLinear addressing wrap:0x%06X (Size %d)", linear_size, linear_size);
  int datapack_length;
  if( paged_size > linear_size )
    {
      datapack_length = paged_size;
      printf("\nPage addressed datapack");
      paged_addr = true;
    }
  else
    {
      datapack_length = linear_size;
      printf("\nLinear addressed datapack");
      paged_addr = false;
    }
  printf("\n");

  if( oled_nserial )
    {
      oled_set_xy(&oled0, 0, 8);
      oled_printf(&oled0, "%d bytes", datapack_length);
      oled_set_xy(&oled0, 0, 16);
      oled_printf(&oled0, "%s", paged_addr?"Page":"Linear");
      oled_set_xy(&oled0, 0, 24);
      oled_printf(&oled0, " addressed pack");
      loop_delay(5000000);
    }
}


////////////////////////////////////////////////////////////////////////////////

// Ported code from Martin Prest's datapack reader/writer
//
// https://github.com/martinprest/Psion2-Datapak-Rampak-reader-writer
//
// (git@github.com:martinprest/Psion2-Datapak-Rampak-reader-writer.git)
//
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void delayShort()
{
  // 1 us delay
  loop_delay(10);
}

//------------------------------------------------------------------------------------------------------

void delayLong()
{ // 3 us delay
  loop_delay(30);
}

// Switch data lines to inputs
// Also swap level shifters to match

void ArdDataPinsToInput(void)
{
  int i;

  gpio_put(LS_DIR_PIN, 0);
  
  for(i=0; i<8; i++)
    {
      if (datapak_mode)
	{
	  // Pull ups
	  gpio_set_pulls(data_pin[i], 1, 0);
	}
      else
	{
	  // No pull ups
	  gpio_set_pulls(data_pin[i], 0, 0);
	}
      
      gpio_set_dir(data_pin[i], GPIO_IN);
    }

  // 
} 

//------------------------------------------------------------------------------------------------------

void ArdDataPinsToOutput()
{
  // set Arduino data pins to output
  gpio_put(LS_DIR_PIN, 1);
  for (byte i = 0; i <= 7; i += 1)
    {
      gpio_set_dir(data_pin[i], GPIO_OUT);
    }

  delayShort();
}

//------------------------------------------------------------------------------------------------------

void packOutputAndSelect()
{
  // sets pack data pins to output and selects pack memory chip (EPROM or RAM)
  gpio_put(SLOT_SOE_PIN, 0);        // enable output - pack ready for read
  delayShort();                     // delay whilst pack data pins go to output
  
  gpio_put(SLOT_SS_PIN, 0);         // take memory chip CE_N low - select pack
  delayShort();
  delayShort();
}

//------------------------------------------------------------------------------------------------------

void packDeselectAndInput()
{
  // deselects pack memory chip and sets pack pins to input
  gpio_put(SLOT_SS_PIN, 1); // take memory chip select high - deselect pack
  delayShort();
  gpio_put(SLOT_SOE_PIN, 1); // disable output - pack ready for write
  delayShort(); // don't do anything until output disabled & pack data pins become input
  delayShort();
}

//------------------------------------------------------------------------------------------------------

byte readByte()
{
  // Reads Arduino data pins, assumes pins are in the input state

  byte data = 0;
  
  for (int8_t i = 7; i >= 0; i -= 1)
    {
      // int8 type to allow -ve 8 bit numbers, so loop can end at -1
      // read each pin and shift pin values into data, starting at MSB
      
      data = (data << 1) + gpio_get(data_pin[i]); 
    }
  
  return data;
}

//------------------------------------------------------------------------------------------------------

void writeByte(byte data)
{
  // Writes to Arduino data pins, assumes pins are in the output state

  for (byte i = 0; i <= 7; i += 1)
    {
      gpio_put(data_pin[i], data & 1);    // write data bits to data_pin[i], starting at LSB 
      data = data >> 1;                   // shift data right, so can AND with 1 to read next bit
    }
}

//------------------------------------------------------------------------------------------------------

void resetAddrCounter()
{
  // Resets pack counters

  gpio_put(SLOT_SCLK_PIN, 0);          // start with clock low, CLK is LSB of address
  delayShort();
  
  CLK_val = 0;                         // set CLK state low

  gpio_put(SLOT_SMR_PIN, 1);           // reset address counter - asynchronous, doesn't require SLOT_SS_PIN or OE_N
  delayShort();

  gpio_put(SLOT_SMR_PIN, 0);
  delayShort();

  //delayLong();
  current_address = 0;
}

//------------------------------------------------------------------------------------------------------

void nextAddress()
{
  // toggles CLK to advance the address, CLK is LSB of address and triggers the counter
  if (CLK_val == 0)
    {
      gpio_put(SLOT_SCLK_PIN, 1);
      CLK_val = 1;
    }
  else if (CLK_val == 1)
    {
      gpio_put(SLOT_SCLK_PIN, 0);
      CLK_val = 0;
    }

  delayShort(); // settling time, let datapak catch up with address

  current_address++;

  if (paged_addr && ((current_address & 0xFF) == 0))
    {
      nextPage(); // if paged mode and low byte of addr is zero (end of page) - advance page counter
    }
}

//------------------------------------------------------------------------------------------------------

void nextPage()
{
  // pulses PGM low, -ve edge advance page counter
  if (program_low)
    {
      // if SLOT_SPGM_PIN low, pulse high then low
      gpio_put(SLOT_SPGM_PIN, 1);
      delayShort();
      gpio_put(SLOT_SPGM_PIN, 0);
      delayShort();
    }
  else
    {
      // if SLOT_SPGM_PIN high, pulse low then high
      gpio_put(SLOT_SPGM_PIN, 0); // -ve edge advances counter
      delayShort();
      gpio_put(SLOT_SPGM_PIN, 1);
      delayShort();
    }
}

//------------------------------------------------------------------------------------------------------

void setAddress(word addr)
{
  // resets counter then toggles counters to reach address, <word> so max address is 64k
  resetAddrCounter(); // reset counters
  byte page = (addr & 0xFF00) >> 8; // high byte of address
  byte addr_low = addr & 0xFF; // low byte of address
  if (paged_addr)
    { // if paged addressing
      for (byte p = 0; p < page; p++)
	{
	  nextPage();
	} // call nextPage, until page reached
    
      for (byte a = 0; a < addr_low; a++)
	{
	  nextAddress();
	} // call nextAddress, until addr_low reached  
    }
  else
    { // else linear addressing
      for (word a = 0; a < addr; a++)
	{
	  nextAddress();
	} // call nextAddress, until addr reached 
    }
  delayShort(); // extra delay, not needed?
  current_address = addr;
}

//------------------------------------------------------------------------------------------------------

void printPageContents(byte page)
{

  // set address to start of page and print contents of page (256 bytes) to serial (formatted with addresses)
  
  ArdDataPinsToInput(); // ensure Arduino data pins are set to input
  packOutputAndSelect(); // Enable pack data bus output, then select it
  resetAddrCounter(); // reset address counter
  
  printf("%s\n", "addr  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  -------TEXT-------");
  printf("%s\n", "------------------------------------------------------  01234567  89ABCDEF"); // comment out to save memory
  
  for (byte p = 0; p < page; p++)
    { // page counter
      if (paged_addr)
	{ // paged addressing
	  nextPage(); // call nextPage(), until page reached      
	}
      else
	{ // linear addressing
	  for (word a = 0; a <= 0xFF; a++)
	    {
	      nextAddress(); // call nextAddress() for every address in page, including 0xFF - so will advance to 0x100
	    }
	}
    }
  
  for (word base = 0; base <= 255; base += 16)
    {
      // loop through 0 to 255 in steps of 16, last step to 256

      byte data;
      char str[19] = ""; // fill with zeros, 16+2+1, +1 for char zero terminator

      str[8] = 32; str[9] = 32; // gap in middle, 2 spaces

      byte pos = 0;
      char buf[6]; // buffer for sprintf: 5 chars + terminator

      sprintf(buf, "%04x ", base + page * 0x100); // format page in hex

      printf(buf);

      for (byte offset = 0; offset <= 15; offset += 1)
	{
	  // loop through 0 to 15

	  if ((offset == 0) || (offset == 8))
	    {
	      printf(" "); // at 0 or 7 print an extra spaces
	    }
	
	  data = readByte(); // read byte from pack
	  sprintf(buf, "%02x ", data); // format data byte in hex
	  printf(buf);
      
	  if ((data > 31) && (data < 127))
	    {
	      // if printable char, put in str        
	      str[pos] = data;
	    }
	  else
	    {
	      str[pos] = '.'; // else use '.'
	    }
      
	  pos++;

	  if (pos == 8)
	    {
	      pos += 2; // jump 2 spaces after 8th char in str
	    }
      
	  nextAddress();
	}

      printf(" ");
      printf("%s\n", str);
    
      /*
	char buf[80]; // buffer for sprintf - used too much memory
	sprintf(buf, "%04x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %s",
	base + page * 0x100, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
	data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
	printf("%s\n", buf);*/
    }
  
  packDeselectAndInput(); // deselect pack, then set pack data bus to input
}

//------------------------------------------------------------------------------------------------------

byte readAddr(word addr, bool output) { // set address, read byte, print if output true, and return value

  ArdDataPinsToInput(); // ensure Arduino data pins are set to input
  packOutputAndSelect(); // Enable pack data bus output, then select it
  setAddress(addr);

  byte dat = readByte(); // read Arduino data bus
  
  if (output == true)
    {
      char buf[15];
      sprintf(buf, "(Ard) %04x  %02x", addr, dat); // print to buf with 3 digits hex, then 2 digits hex, with leading zeros
      printf("%s\n", buf);
    }
  
  packDeselectAndInput(); // deselect pack, then set pack data bus to input

  return dat;
}

////////////////////////////////////////////////////////////////////////////////

void printPackMode()
{
  if (datapak_mode)
    {
      printf("\n(Ard) Now in Datapak mode (Arduino input pullups)"); // sets Arduino input pullups for datapack mode in ArdDataPinsToInput()
    }
  
  else
    {
      printf("\n(Ard) Now in Rampak mode (No Arduino input pullups)"); // no Arduino input pullups
    }
}

void printAddrMode()
{
  if (paged_addr)
    {
      printf("(Ard) Now in paged addressing mode");
    }
  else
    {
      printf("(Ard) Now in linear addressing mode");
    }
}
void printCommands()
{
  printf("\n(Ard) datapak_read_write_v1.3\n");
  printPackMode();
  printAddrMode();
  printf("\n(Ard) Select a command:\ne - erase\nr - read pack\nw - write pack");
  printf("\n0 - print page 0\n1 - print page 1\n2 - print page 2\n3 - print page 3");
  printf("\nt - write TEST record to main\nm - rampak (or datapak) mode\nl - linear (or paged) addressing");
  printf("\ni - print pack id byte flags\nd - directory and size pack\nb - check if pack is blank");
  printf("\n? - list commands\nx - exit");
}

////////////////////////////////////////////////////////////////////////////////
//
// read all pack data, output if selected and return address of 1st free byte (with value 0xFF)
// output: 0 - none, 1 - print address & byte value, 2 - send bytes
//
////////////////////////////////////////////////////////////////////////////////

word readAll(byte output)
{
  word endAddr = read_dir(); // size pack - max is 64k

  printf("\nSize: %08X", endAddr);

  ArdDataPinsToInput();          // ensure Arduino data pins are set to input
  packOutputAndSelect();         // Enable pack data bus output then select it
  resetAddrCounter();            // reset counters

  if (output == 2)
    {
      // tell PC to read data
#if 0      
      printfln("XXRead"); // send "XXRead" to PC to tell it to receive read data
      Serial.write(0); // size bytes, highest is zero, so max is 64k
      Serial.write(highByte(endAddr));
      Serial.write(endAddr & 0xFF);
#endif
    }  
  
  bool quit = false;
  word addr_tot = 0; // total
  
  while  (quit == false)
    {
      byte dat = readByte(); // read Datapak byte at current address
      
      if (output == 1)
	{
	  // print to serial
	  byte addr_low = addr_tot & 0xFF;
	  printf("\n(Ard) %04x: %02x", addr_low, dat); 
	}

#if 0
      if (output == 2)
	{
	  // send to serial as bytes

	  Serial.write(dat); // send byte only
	  unsigned long t = millis();

	  while (!(Serial.available() > 0))
	    {
	      // wait for data echo back, if no data: loop until there is, or timeout

	      if (millis()-t > 1000)
		{                                      // if timeout
		  printf("\n(Ard) Timeout!");
		  return false;
		}
	    }                                          // end of while loop delay for data echo back from PC

	  byte datr = Serial.read(); // read byte value from serial

	  if (datr != dat)
	    {
	      // if echo datr doesn't match dat sent

	      delay(600);                        // delay to force timeout on PC !!
	      
	      printf("\n(Ard) Read data not verified by PC!");
	      return false;        
	    }
	}
#endif
      
      if (addr_tot >= endAddr)
	{
	  quit = true;
	}
      
      if ((read_fixed_size == true) && (addr_tot >= read_pack_size))
	{
	  quit = true;                 // quit if reach packSize and readFixedsize is true
	}
      
    if (addr_tot >= 0xFF) // should be 0xFFFF
      {
	quit = true; // break loop if reach max size: (65536-1 bytes) 64k !!
      }
    
    if (quit != true)
      {      
	nextAddress();
	addr_tot++;
      }
    }
  
  packDeselectAndInput();       // deselect pack, then set pack data bus to input
  return addr_tot;              // returns end address
}

//------------------------------------------------------------------------------------------------------

bool writePakByteRampak(byte val) { // writes val to current address, returns true if written ok - no longer used

  packDeselectAndInput(); // deselect pack, then set pack data bus to input (OE_N = high)
  ArdDataPinsToOutput(); // set Arduino data pins to output - can't do this with Datapak also output! OE_N must be high
  
  writeByte(val); // put value on Arduino data bus

  delayShort();

  gpio_put(SLOT_SS_PIN, 0); // take CE_N low - select
  delayShort(); // delay for write
  gpio_put(SLOT_SS_PIN, 1); // take CE_N high - deselect
  delayShort(); 

  ArdDataPinsToInput(); // set Arduino data pins to input
  packOutputAndSelect(); // Enable pack data bus output then select it
  byte dat = readByte(); // read byte from datapak
  packDeselectAndInput(); // deselect pack, then set pack data bus to input

  if (dat == val) return true; // true if value written ok
  else return false; // false if write cannot be verified
}

//------------------------------------------------------------------------------------------------------

bool writePakByte(byte val, bool output)

{
  // writes val to current address, returns false if write failed
  // returns no. of cycles if write ok
  // needs both SLOT_SPGM_PIN low and CE_N low for Eprom write
  
  byte write_cycles = 1;
  byte dat = 0;
  byte i = 1;
  
  if (datapak_mode)
    {
      write_cycles = max_datapak_write_cycles;
    }
  
  for (i = 1; i <= write_cycles; i++)
    {
      
      packDeselectAndInput(); // deselect pack, then set pack data bus to input (CE_N high, OE_N high)
      
      if (datapak_mode)
	{
	  if (output)
	    {
	      printf("\n(Ard) Datapak write VPP on");
	    }
	  
	  gpio_put(VPP_ON_PIN, 1); // turn on VPP, 5V VCC is on already
	  delayLong();      
	}
      else
	{
	  delayShort();
	}
  
      ArdDataPinsToOutput();       // set Arduino data pins to output - can't do this with Datapak also output! OE_N must be high
      
      writeByte(val);              // put value on Arduino data bus
      
      if (datapak_mode)
	{
	  delayLong();
	}
      else
	{
	  delayShort();
	}
      
      gpio_put(SLOT_SS_PIN, 0); // take CE_N low - select
      
      if (datapak_mode)
	{
	  loop_delay(datapak_write_pulse); // delay for write
	}
      else
	{
	  delayShort();
	}
      
      gpio_put(SLOT_SS_PIN, 1); // take CE_N high - deselect
      if (datapak_mode)
	{
	  delayLong();
	}
      else
	{
	  delayShort();
	}
      
    if (datapak_mode)
      {
	gpio_put(VPP_ON_PIN, 0); // turn off VPP, 5V VCC goes off later
	delayLong();
	if (output)
	  {
	    printf("\n(Ard) Datapak write VPP off");
	  }
      }
  
    ArdDataPinsToInput();         // set Arduino data pins to input - for read    
    packOutputAndSelect();        // Enable pack data bus output then select it
  
    dat = readByte();             // read byte from datapak
  
    if (output)
      {
	printf("(Ard) Cycle: %02d, Write: %02x, Read: %02x", i, val, dat);
      }
    
    if ((!force_write_cycles) && (dat == val))
      {
	break; // written ok, so break out of for-loop
      }
    }
  
  packDeselectAndInput(); // deselect pack, then set pack data bus to input (CE_N high, OE_N high)

  if ((overwrite) == true && (dat == val) && (datapak_mode))
    {
      // for non-CMOS EPROM write again to make sure - overwrite
      if (output)
	{
	  printf("\n(Ard) Datapak write VPP on");
	}
      
    gpio_put(VPP_ON_PIN, 1); // turn on VPP, 5V VCC is on already
    delayLong();

    ArdDataPinsToOutput(); // set Arduino data pins to output - can't do this with Datapak also output! OE_N must be high

    writeByte(val); // put value on Arduino data bus
    delayLong();  

    gpio_put(SLOT_SS_PIN, 0); // take CE_N low - select
    loop_delay(datapak_write_pulse*3); // 3*delay for overwrite

    gpio_put(SLOT_SS_PIN, 1); // take CE_N high - deselect
    delayLong();  

    gpio_put(VPP_ON_PIN, 0); // turn off VPP, 5V VCC goes off later
    delayLong();

    if (output)
      {
	printf("(Ard) Datapak write VPP off");
      }
  }
  
  if (dat == val)
    {
    return i;              // return no. of cycles if value written ok
    }
  else
    {
      return false;        // false if write cannot be verified
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Compares an OPK file from the SD card to the currently attached datapak
//
////////////////////////////////////////////////////////////////////////////////

void compare_opk_file(I2C_SLAVE_DESC *slave, char *filename)
{
  bool done_w = false;
  int addr = 0;
  char line[200];
  int datr = 0;
  
  // Mount SD card
  // Files are in PAK directory (like the pak-gadget)
  
  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }

  oled_clear_display(slave);
  oled_set_xy(slave, 0, 0);
  sprintf(line, "Reading file");
  oled_display_string(slave, line);
  
  oled_set_xy(slave, 0, 8);
  sprintf(line, "%s", filename);
  oled_display_string(slave, line);

  loop_delay(3000000);
  
  // Open file
  FIL fil;
  FRESULT fr = f_open(&fil, filename, FA_READ);
  
  if (FR_OK != fr)
    {
      sprintf(line, "Failed to open:");
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      oled_display_string(slave, line);
      
      oled_set_xy(slave, 0, 7);
      sprintf(line, "%s", filename);
      oled_display_string(slave, line);

      loop_delay(3000000);
      unmount_sd();
      return;
    }

  char opk_header[6];
  char buf[100];
  int pak_i = 0;
  int br = 0;
  bool done = false;
  char name[80];
  char extension[20];
  bool modify_header = false;
  int numBytes = 0;
  
  // If the extension is .opk then read the header and get the file size
  if( sscanf(filename, "%[^.].%s", name, extension) == 2 )
    {
      if( strcmp(extension, "opk") == 0 )
	{
	  // Read header to get pack size
	  f_read(&fil, opk_header, sizeof(opk_header), &br);

	  numBytes = opk_header[3]*256*256+opk_header[4]*256+opk_header[5];

	  printf("\nOPK file data block length = 0x%06X (%d) bytes", numBytes, numBytes);
	  loop_delay(3000000);
	}
    }

  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 0); // take SLOT_SPGM_PIN low - select & program - need SLOT_SPGM_PIN low for CE_N low if OE_N high
      program_low = true;
    }

  resetAddrCounter(); // reset address counters, after SLOT_SPGM_PIN low

  // Compare the data
  // Assume the datapack data is the same as the SD file data
  bool data_identical = true;
  
  for (addr = 0; addr <= numBytes;)
    {
      printf("\naddr:%d", addr);
      
      // Bytes from file
      f_read(&fil, buf, sizeof buf, &br);

      printf("\nbr=%d", br);

      if( br==0 )
	{
	  break;
	}
      
      // Compare bytes
      for(int j=0; j<br; j++)
	{
	  // Byte from SD card
	  datr = readByte();
	  
	  // Check byte
	  if( j==50 )
	    {
	      printf("\n%d %d", buf[j], datr);
	    }
	  
	  if( buf[j] != datr )
	    {
	      // Not the same, we have our answer...
	      data_identical = false;
	      addr = numBytes+1;
	      break;
	    }
	  
	  addr++;
	  nextAddress();
	}
    }
  
  f_close(&fil);
  
  oled_set_xy(slave, 0, 16);
  oled_printf(&oled0, "%d bytes read", numBytes);
  
  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 1); // take SLOT_SPGM_PIN high
      program_low = false;
    }
  
  if( data_identical )
    {
      oled_set_xy(slave, 0, 24);
      oled_printf(&oled0, "Data is identical");
      printf("\nData is identical\n");
    }
  else
    {
      oled_set_xy(slave, 0, 24);
      oled_printf(&oled0, "Data not identical", numBytes);
      printf("\nData not identical\n");
    }
  
  unmount_sd();

  return;
}

////////////////////////////////////////////////////////////////////////////////
//
// Writes an OPK file from the SD card to the currently attached datapak
//
////////////////////////////////////////////////////////////////////////////////

bool write_opk_file(I2C_SLAVE_DESC *slave, char *filename)
{
  // write PC serial data to pack
  bool done_w = false;
  word addr = 0;
  char line[200];
  
  // Mount SD card
  // Files are in PAK directory (like the pak-gadget)
  
  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }

  oled_clear_display(slave);
  oled_set_xy(slave, 0, 0);
  sprintf(line, "Reading file");
  oled_display_string(slave, line);
  
  oled_set_xy(slave, 0, 8);
  sprintf(line, "%s", filename);
  oled_display_string(slave, line);

  loop_delay(3000000);
  
  // Read the file from the SD card into the pak memory
  FIL fil;
  FRESULT fr = f_open(&fil, filename, FA_READ);
  
  if (FR_OK != fr)
    {
      sprintf(line, "Failed to open:");
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      oled_display_string(slave, line);
      
      oled_set_xy(slave, 0, 7);
      sprintf(line, "%s", filename);
      oled_display_string(slave, line);

      loop_delay(3000000);
      unmount_sd();
      return;
    }

  char opk_header[6];
  char buf[1];
  int pak_i = 0;
  int br = 0;
  bool done = false;
  char name[80];
  char extension[20];
  bool modify_header = false;
  int numBytes = 0;
  
  // If the extension is .opk then we drop the first 6 bytes
  if( sscanf(filename, "%[^.].%s", name, extension) == 2 )
    {
      if( strcmp(extension, "opk") == 0 )
	{
	  // Read header to get pack size
	  f_read(&fil, opk_header, sizeof(opk_header), &br);

	  numBytes = opk_header[3]*256*256+opk_header[4]*256+opk_header[5];
	  printf("\nOPK file data block length = 0x%06X (%d) bytes", numBytes, numBytes);
	}
    }

  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 0); // take SLOT_SPGM_PIN low - select & program - need SLOT_SPGM_PIN low for CE_N low if OE_N high
      program_low = true;
    }

  resetAddrCounter(); // reset address counters, after SLOT_SPGM_PIN low

  // Write the data

  for (addr = 0; addr <= numBytes; addr++)
    {
      f_read(&fil, buf, sizeof buf, &br);
      
      if( br == 0 )
	{
	  // File is too short, exit
	  printf("\nFile too short.");
	  break;
	}
      
      done_w = writePakByte(buf[0], false);
      if (done_w == false)
	{
	  printf("\n(Ard) Write byte failed!");
	  break;
	}
      nextAddress();
    }
  
  f_close(&fil);
  
  oled_set_xy(slave, 0, 16);
  sprintf(line, "%d bytes read", numBytes);
  oled_display_string(slave, line);
  
  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 1); // take SLOT_SPGM_PIN high
      program_low = false;
    }
  
  if (done_w == true)
    {
      printf("\n(Ard) Write done ok");
    }
  
  unmount_sd();
  
  return done_w;
}


////////////////////////////////////////////////////////////////////////////////
//
// Reads a datapack and writes the contents to an OPK file on the SD card
//
////////////////////////////////////////////////////////////////////////////////

bool read_opk_file(I2C_SLAVE_DESC *slave, char *filename)
{
  bool done_w = false;
  word addr = 0;
  char line[200];
  
  // Mount SD card
  // Files are in PAK directory (like the pak-gadget)

  printf("\nReading datapack to SD card file...");
  
  mount_sd();
  
  if( cd_to_pak_dir(slave) )
    {
      unmount_sd();
      return;
    }

  oled_clear_display(slave);
  oled_set_xy(slave, 0, 0);
  oled_printf(&oled0, "Writing file");
  
  oled_set_xy(slave, 0, 8);
  oled_printf(&oled0, "%s", filename);

  loop_delay(3000000);
  
  // Write the data to the file
  FIL fil;
  FRESULT fr = f_open(&fil, filename, FA_CREATE_NEW | FA_WRITE);
  
  if (FR_OK != fr)
    {
      printf("\nFailed to open file '%s'", filename);
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      oled_printf(&oled0, "Failed to open:");
      
      oled_set_xy(slave, 0, 7);
      oled_printf(&oled0, "%s", filename);

      loop_delay(3000000);
      unmount_sd();
      return;
    }

  printf("\nOpened file '%s'", filename);
  
  char opk_header[6];
  char buf[1];
  int pak_i = 0;
  int br = 0;
  bool done = false;
  char name[80];
  char extension[20];
  bool modify_header = false;
  int numBytes = 0;

  // Write an OPK file header.
  
  // If the extension is .opk then we drop the first 6 bytes
  numBytes = 32*1024;

  opk_header[0] = 'O';
  opk_header[1] = 'P';
  opk_header[2] = 'K';
  opk_header[3] = (numBytes >> 16) & 0xff;
  opk_header[4] = (numBytes >>  8) & 0xff;
  opk_header[5] = (numBytes >>  0) & 0xff;
  
  f_write(&fil, opk_header, sizeof(opk_header), &br);

  printf("\nOPK file header = %d %d %d %d %d %d",
	 opk_header[0],
	 opk_header[1],
	 opk_header[2],
	 opk_header[3],
	 opk_header[4],
	 opk_header[5]);
	 
  printf("\nDatapack length = 0x%06X (%d) bytes", numBytes, numBytes);


  resetAddrCounter(); // reset address counters, after SLOT_SPGM_PIN low

  // Write the data

  for (addr = 0; addr <= numBytes; addr++)
    {
      // Read a byte at a time
      buf[0] = readByte();
      f_write(&fil, buf, sizeof buf, &br);
      
      if( br == 0 )
	{
	  // File is too short, exit
	  printf("\nwrite error");
	  break;
	}
      nextAddress();
    }
  
  f_close(&fil);
  
  oled_set_xy(slave, 0, 16);
  oled_printf(&oled0, "%d bytes read", addr);
  loop_delay(3000000);
  
  unmount_sd();
  
  return done_w;
}


//------------------------------------------------------------------------------------------------------

bool writePakSerial(word numBytes)
{
  // write PC serial data to pack
    bool done_w = false;

#if 0
  word addr = 0;
  
  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 0); // take SLOT_SPGM_PIN low - select & program - need SLOT_SPGM_PIN low for CE_N low if OE_N high
      program_low = true;
    }
  
  resetAddrCounter(); // reset address counters, after SLOT_SPGM_PIN low
  
  for (addr = 0; addr <= numBytes; addr++)
    {
      unsigned long t = millis();

      while (!(Serial.available() > 0))
	{ // if no data from PC, loop until there is, or timeout
	  if (millis()-t > 1000)
	    {
	      // if timeout
	      printf("\n(Ard) Timeout!");
	      return false;
	    }
	}

    byte datw = Serial.read(); // read byte value from serial
    
    Serial.write(datw); // write data back to PC to verify and control data flow
    done_w = writePakByte(datw, /* output */ false); // write value to current memory address, no output because PC needs to verify data
    
    if (done_w == false)
      {
	printf("\n(Ard) Write byte failed!");
	break;
      }
    nextAddress();
    }
  
  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 1); // take SLOT_SPGM_PIN high
      program_low = false;
    }
  
  if (done_w == true)
    {
      printf("\n(Ard) Write done ok");
    }

#endif
  return done_w;
}

//------------------------------------------------------------------------------------------------------

void eraseBytes(word addr, word numBytes)
{
  // erase numBytes, starting at addr - ony for rampaks

  setAddress(addr);

  bool done_ok = false;
  byte addr_low = addr & 0xFF;

  printf("(Ard) Erasing:");

  for (word i = 0; i <= numBytes; i++)
    {
      // loop through numBytes to write
      done_ok = writePakByte(0xFF, false /* no output */); // write 0xFF
      
      if (done_ok == false)
	{
	  printf("\n(Ard) Erase failed!");
	  break; // break out of for loop
	}
      
      if (addr_low == 0xFF)
	{
	  // if end of page reached, go to next page, addr_low will wrap around to zero
	  printf("."); // "." printed for each end of page
	}
      
      nextAddress(); 
      addr_low++;  
    }
  
  if (done_ok == true)
    {
      printf("\n");
      printf("\n(Ard) Erased ok");
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// write record to main
//
////////////////////////////////////////////////////////////////////////////////

void WriteMainRec(bool output)
{
  word endAddr = read_dir();

  printf("\nPack size (from dir) is: %08X", endAddr);

  if (datapak_mode)
    {
      gpio_put(SLOT_SPGM_PIN, 0); // take SLOT_SPGM_PIN low - select & program - need SLOT_SPGM_PIN low for CE_N low if OE_N high
      program_low = true;
    }

  if (readAddr(endAddr, /* output */ true) == 0xFF)
    {
      // move to start address & read it, if value is 0xFF write record

      char main[] = "--The quick brown fox jumps over the lazy dog.";     // Main record text with leading "--" for length & identifier bytes
      byte len_main = sizeof(main)-1;                                     // not including 0 at end
      
      main[0] = len_main-2;      // record text length identifier byte
      main[1] = 0x90;            // MAIN file identifier byte

      bool done_ok = false;
      int8_t cycles = 0;         // can be -ve if write failed
      
      for (byte i = 0; i < len_main; i++)
	{
	  // index starts from 0, so do while i < len_str, not i <= len_str
	  char chr = main[i];
	  
	  cycles = writePakByte(chr, /* output */ true);             // write i'th char of main record, cycles is no. of write cycles for EPROM
	  
	  if (cycles > 0)
	    {
	      done_ok = true;
	    }
	  
	  if (output)
	    {
	      printf("(Ard) %04x: %02d %02x %c", i, cycles, chr, chr);
	    }    

	  // if (((endAddr & 0xFF) == 0xFF) & (paged_addr == true)) nextPage(); // if end of page reached, and paged_addr mode is true, go to next page - moved to nextAddress()
	  nextAddress(); // next address
	  endAddr++; // increment address pointer

	  if (done_ok == false)
	    {
	    break;
	    }
	}
      
      if (datapak_mode)
	{
	  gpio_put(SLOT_SPGM_PIN, 1); // take SLOT_SPGM_PIN high
	  program_low = false;
	}
      
      if (done_ok == true)
	{
	  printf("\n(Ard) add record done successfully");
	}
      
      else
	{
	  printf("\n(Ard) add record failed!");
	}
    }
  else
    {
      printf("\n(Ard) no 0xFF byte to add record!");
    }
}

//------------------------------------------------------------------------------------------------------
// pack sizing and id bytes - code originally from Matt Callow, but modified. https://github.com/mattcallow/psion_pak_reader
//------------------------------------------------------------------------------------------------------

byte read_next_byte()
{
  // only used by Matt's code
  nextAddress();
  byte data = readByte();

  return data;
}

void incr_addr(word bytes) { // only used by Matt's code
  for (word i=0;i<bytes;i++) { // increase address while i < bytes
    nextAddress();
  }
}


void print_pak_id()
{
  // the first 2 bytes on a pack are the id and size bytes. id gives info about the pack

  ArdDataPinsToInput();              // ensure Arduino data pins are set to input
  packOutputAndSelect();             // Enable pack data bus output, then select it
  resetAddrCounter();

  byte id = readByte();
  byte sz = read_next_byte();
  byte pack_size = sz * 8;

  packDeselectAndInput();            // deselect pack, then set pack data bus to input
  printf("\n");
  printf("Id Flags: %02X", id);
  
  // print bit flag value using conditional operator: (condition) ? true : false

  printf("\n0: ");    printf((id & 0x01)? "invalid"             : "valid"); 
  printf("\n1: ");    printf((id & 0x02)? "datapak"             : "rampak");
  printf("\n2: ");    printf((id & 0x04)? "paged"               : "linear");
  printf("\n3: ");    printf((id & 0x08)? "not write protected" : "write protected");
  printf("\n4: ");    printf((id & 0x10)? "non-bootable"        : "bootable");
  printf("\n5: ");    printf((id & 0x20)? "copyable"            : "copy protected");
  printf("\n6: ");    printf((id & 0x40)? "standard"            : "flashpak or debug RAM pak");
  printf("\n7: ");    printf((id & 0x80)? "MK1"                 : "MK2");

  printf("\nSize: "); printf("%d", pack_size); printf(" kB");
}

void button_pak_id(struct MENU_ELEMENT *e)
{
  // the first 2 bytes on a pack are the id and size bytes. id gives info about the pack

  ArdDataPinsToInput();              // ensure Arduino data pins are set to input
  packOutputAndSelect();             // Enable pack data bus output, then select it
  resetAddrCounter();

  byte id = readByte();
  byte sz = read_next_byte();
  byte pack_size = sz * 8;

  packDeselectAndInput();            // deselect pack, then set pack data bus to input

  oled_clear_display(&oled0);
  
  // print bit flag value using conditional operator: (condition) ? true : false

  oled_set_xy(&oled0, 0, 0);
  oled_printf(&oled0, "0: ");    oled_printf(&oled0, (id & 0x01)? "invalid"             : "valid");
  oled_set_xy(&oled0, 64, 0);
  oled_printf(&oled0, "1: ");    oled_printf(&oled0, (id & 0x02)? "datapak"             : "rampak");
  oled_set_xy(&oled0, 0,  8);
  oled_printf(&oled0, "2: ");    oled_printf(&oled0, (id & 0x04)? "paged"               : "linear");
  oled_set_xy(&oled0, 0, 16);
  oled_printf(&oled0, "3: ");    oled_printf(&oled0, (id & 0x08)? "not write protected" : "write protected");
  oled_set_xy(&oled0, 0, 24);
  oled_printf(&oled0, "4: ");    oled_printf(&oled0, (id & 0x10)? "non-bootable"        : "bootable");
  oled_set_xy(&oled0, 0, 32);
  oled_printf(&oled0, "5: ");    oled_printf(&oled0, (id & 0x20)? "copyable"            : "copy protected");
  oled_set_xy(&oled0, 0, 40);
  oled_printf(&oled0, "6: ");    oled_printf(&oled0, (id & 0x40)? "standard"            : "flash/dbg RAM");
  oled_set_xy(&oled0, 0, 48);
  oled_printf(&oled0, "7: ");    oled_printf(&oled0, (id & 0x80)? "MK1"                 : "MK2");
}

void button_pak_hdr(struct MENU_ELEMENT *e)
{
  // the first 2 bytes on a pack are the id and size bytes. id gives info about the pack

  ArdDataPinsToInput();              // ensure Arduino data pins are set to input
  packOutputAndSelect();             // Enable pack data bus output, then select it
  resetAddrCounter();

  byte id = readByte();
  byte sz = read_next_byte();
  byte pack_size = sz * 8;

  packDeselectAndInput();            // deselect pack, then set pack data bus to input

  oled_clear_display(&oled0);

  oled_set_xy(&oled0, 0, 0);
  oled_printf(&oled0, "Id Flags: %02X", id);
  
  oled_set_xy(&oled0, 0, 8);
  oled_printf(&oled0, "Size: "); oled_printf(&oled0, "%d", pack_size); printf(" kB");
}

word read_dir(void)
{
  // read filenames and size pack
  ArdDataPinsToInput(); // ensure Arduino data pins are set to input
  packOutputAndSelect(); // Enable pack data bus output, then select it
  resetAddrCounter();
  
  uint8_t id = 0; // datafile id
  
  printf("\n");
  
  printf("\nADDR     TYPE           NAME        ID        Del? SIZE");
  
  incr_addr(9); // move past header to 10th byte
  
  while(current_address < max_eprom_size)
    {
      printf("\n0x%06X ", current_address+1);
      
      char short_record[10] = "         ";    // 9 spaces + terminator
      uint8_t rec_len = read_next_byte();
      
      if (rec_len == 0xff)
	{
	  printf("\nEnd of pack");
	  break;
	}
      
      word jump = rec_len; // for jump, reduces when bytes read
      word rec_size = rec_len; // for printing
      uint8_t rec_type = read_next_byte();
      
      if (rec_type == 0x80)
	{
	  jump = (read_next_byte()<<8) + read_next_byte();
	  
	  printf("Long record, length = 0x%08X", jump);
	} 
      else
	{
	  if (rec_len > 9)
	    {
	      rec_len = 9; // read first 8 chars of short record for printing
	    }
	  
	  for (uint8_t i=0;i<=rec_len-1;i++)
	    {
	      short_record[i] = read_next_byte();
	      jump--;
	    }
	  
	  printf("0x%04X", rec_type); // print rec type (4 chars)
	  
	  switch (rec_type & 0x7f)
	    {
	      // print type (9 chars)
	      
	    case 0x01:
	      printf(" [Data]  ");
	      break;
	    case 0x02:
	      printf(" [Diary] ");
	      break;
	    case 0x03:
	      printf(" [OPL]   ");
	      break;
	    case 0x04:
	      printf(" [Comms] ");
	      
	      break;
	    case 0x05:
	      printf(" [Sheet] ");
	      break;
	    case 0x06:
	      printf(" [Pager] ");
	      break;
	    case 0x07:
	      printf(" [Notes] ");
	      break;
	    case 0x10 ... 0x7E :
	      printf(" [Rec]   ");// datafile record
	      break;
	    default:
	      printf(" [misc]  ");// unknown type
	    }

	  for(int i=0; i<strlen(short_record); i++)
	    {
	      if( short_record[i] == 9 )
		{
		  short_record[i] = ' ';
		}
	    }
	  
          printf("%-10s", short_record);

	  // isn't 1 <= 7?
	  
          if ( ((rec_type & 0x7f) == 1) ||
	       ((rec_type & 0x7f) <= 7))
	    {
	      // filename, with id in last byte
	      id = short_record[8];
	      printf("  0x%06X ", id); // id (6 chars + 1 space)
	    }
          else
	    {
	      printf("           ");
	    }
	  
          printf(" %-3s  ", (rec_type < 0x80) ? "Yes" : "No"); // deleted y/n? (6 chars)
          printf("0x%06X ", rec_size); // length (6 chars)
	}
      
      incr_addr(jump);
    }
  
  packDeselectAndInput(); // deselect pack, then set pack data bus to input
  
  return current_address;
}

bool blank_check()
{
  ArdDataPinsToInput(); // ensure Arduino data pins are set to input
  packOutputAndSelect(); // Enable pack data bus output, then select it
  resetAddrCounter();
  
  printf("\nBlank Check in 1k chunks '.'-blank 'x'-not blank\n");

  bool blank = (readByte() == 0xff); // is 1st byte blank?
  bool blank_1k = blank; // is this 1k blank?

  //for (word i=1;(blank && (i <= max_eprom_size));i++) { // read bytes while blank or up to max_eprom_size
  
  for (int i=1;i <= max_eprom_size;i++)
    {
      // read bytes up to max_eprom_size 

      if (read_next_byte() != 0xff)
	{
	  // blank true if byte is 0xFF
	  blank = false;
	  blank_1k = false;
	}
      
      if ((i % 1024) == 0)
	{
	  // every 1024 bytes print a dot (addr div 1024)
	  // dot if blank, x if not, using conditional operator: (condition) ? true : false
	  
	  printf(blank_1k ? "." : "x"); 
	  blank_1k = true; // reset blank for next 1k chunk
	}
    }
  
  printf("\nIs pack blank? : ");
  printf(blank ? "Yes" : "No");
  printf("\n");
  packDeselectAndInput();               // deselect pack, then set pack data bus to input

  return blank;
}


void button_blank(struct MENU_ELEMENT *e)
{
  ArdDataPinsToInput(); // ensure Arduino data pins are set to input
  packOutputAndSelect(); // Enable pack data bus output, then select it
  resetAddrCounter();

  oled_clear_display(&oled0);
  oled_set_xy(&oled0, 0, 0);
  oled_printf(&oled0, "Blank Check");

  bool blank = (readByte() == 0xff); // is 1st byte blank?
  bool blank_1k = blank; // is this 1k blank?

  //for (word i=1;(blank && (i <= max_eprom_size));i++) { // read bytes while blank or up to max_eprom_size
  
  for (int i=1;i <= max_eprom_size;i++)
    {
      // read bytes up to max_eprom_size 

      if (read_next_byte() != 0xff)
	{
	  // blank true if byte is 0xFF
	  blank = false;
	  blank_1k = false;
	}
      
      if ((i % 1024) == 0)
	{
	  // every 1024 bytes print a dot (addr div 1024)
	  // dot if blank, x if not, using conditional operator: (condition) ? true : false
	  
	  printf(blank_1k ? "." : "x"); 
	  blank_1k = true; // reset blank for next 1k chunk
	}
    }
  
  oled_set_xy(&oled0, 0, 8);
  oled_printf(&oled0, "%s", blank ? "Blank" : "Not blank");

  packDeselectAndInput();               // deselect pack, then set pack data bus to input
}

////////////////////////////////////////////////////////////////////////////////
//
// Serial over USB main loop 
//
////////////////////////////////////////////////////////////////////////////////

void wait_serial_key(void)
{
  int ch;
  do
    {
      ch = getchar_timeout_us(100);
    }
  while (ch == PICO_ERROR_TIMEOUT);
}

void take_high(int gpio, char *gpio_name)
{
#if 0
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_OUT);
#endif
  
  printf("\nGPIO %s high (%d)", gpio_name, gpio);
  gpio_put(gpio, 1);

  // Wait for key
  wait_serial_key();

  printf("\nGPIO %s low\n", gpio_name);
  gpio_put(gpio, 0);
}


void serial_loop()
{
  int  key;
  
  if( (key = getchar_timeout_us(100)) != PICO_ERROR_TIMEOUT)
    {
      printf("\nkey %d", key);
      
      //char buf[15];
      //sprintf(buf, "(Ard) In: 0x%02x", byte(key)); // print input character
      //printf(buf);
      
      switch (key)
	{
	case '-':
#if 1	  
	  ArdDataPinsToOutput();
#endif
	  gpio_set_dir(SLOT_SD0_PIN, GPIO_OUT);
#if 0
	  gpio_set_dir(LS_DIR_PIN, GPIO_OUT);
	  gpio_put(LS_DIR_PIN, 1);
#endif	  

	  gpio_put(SLOT_SS_PIN, 0);
	  gpio_put(SLOT_SOE_PIN, 0);
	  gpio_put(SLOT_SPGM_PIN, 0);
	  gpio_put(SLOT_SMR_PIN, 0);
	  gpio_put(SLOT_SCLK_PIN, 0);
	  gpio_put(SLOT_SD0_PIN, 0);
	  gpio_put(SLOT_SD1_PIN, 0);
	  gpio_put(SLOT_SD2_PIN, 0);
	  gpio_put(SLOT_SD3_PIN, 0);
	  gpio_put(SLOT_SD4_PIN, 0);
	  gpio_put(SLOT_SD5_PIN, 0);
	  gpio_put(SLOT_SD6_PIN, 0);
	  gpio_put(SLOT_SD7_PIN, 0);
	  
	  take_high(SLOT_SS_PIN, "SS");
	  take_high(SLOT_SOE_PIN, "SOE");
	  take_high(SLOT_SMR_PIN, "SMR");
	  take_high(SLOT_SPGM_PIN, "SPGM");
	  take_high(SLOT_SCLK_PIN, "SCLK");
	  take_high(SLOT_SD0_PIN, "SD0");
	  take_high(SLOT_SD1_PIN, "SD1");
	  take_high(SLOT_SD2_PIN, "SD2");
	  take_high(SLOT_SD3_PIN, "SD3");
	  take_high(SLOT_SD4_PIN, "SD4");
	  take_high(SLOT_SD5_PIN, "SD5");
	  take_high(SLOT_SD6_PIN, "SD6");
	  take_high(SLOT_SD7_PIN, "SD7");
	  ArdDataPinsToInput();
	  break;
	  
	  // Debug test option
	case '=':
#if 1	  
	  ArdDataPinsToInput();          // ensure Arduino data pins are set to input
	  packOutputAndSelect();         // Enable pack data bus output then select it
	  resetAddrCounter();            // reset counters

	  gpio_put(VPP_ON_PIN, 0);
	    
	  gpio_put(SLOT_SMR_PIN, 1);           // reset address counter - asynchronous, doesn't require SLOT_SS_PIN or OE_N
	  loop_delay(100000);
	  
	  gpio_put(SLOT_SMR_PIN, 0);

	  loop_delay(100000);
	  gpio_put(SLOT_SOE_PIN, 0);
	  gpio_put(SLOT_SPGM_PIN, 1);
	  gpio_put(SLOT_SS_PIN, 0);
	  delayShort();
	  
	  for(int j=0; j<10; j++)
	    {
	      //byte dat = readByte(); // read Datapak byte at current address
#if 0
	      nextAddress();
#else
	      gpio_put(SLOT_SCLK_PIN, 1);
	      gpio_put(SLOT_SCLK_PIN, 0);

#endif

	      for(int i=0; i<8; i++)
		{
		  printf("\ndata line %d = %d", i, gpio_get(data_gpio[i]));
		}
	      
	      //printf("\nData byte:%02X\n", dat);
	    }
	  printf("\n");


#endif
	  break;
	  
	case 'e' : { // erase bytes
	  if (!datapak_mode)
	    {
	      // 512 bytes for first 2 pages
	      printf("\n(Ard) Erase 512 bytes:");
	      
	      // starting from 0, erase bytes
	      eraseBytes(0,512); 
	    }
	  else
	    {
	      printf("\n(Ard) Can't erase a Datapak! Use UV lamp, or a Rampak");
	    }
	  break;
	}
	  
	case 'w' :
	  {
	    // This needs converting to C from Arduino library code.
	    // write pack from PC data
	    bool write_ok = false;
	    
	    printf("\n(Ard) Write Serial data to pack");
	    
	    // Check for "XXWrite" from PC to indicate following data is write data
	    char str[200] = "XXWrite";
	    
	    if( 0 /* Wait for XXWrite */)
	      {
		// waits for "XXWrite" to signal start of data, or until timeout
		
		byte numBytes[2] = {0};                             // byte array to store pack image size
		//		byte bytes_read = Serial.readBytes(numBytes,2);     // read 2 bytes for pack size, will need to be 3 if pack > 64kB !

		if (/*bytes_read == 2*/ 0)
		  {
		    word numBytesTotal = (numBytes[0] << 8) + numBytes[1];   // shift 1st byte 8 bits left for high byte and add 2nd byte as low byte

		    write_ok = writePakSerial(numBytesTotal);                // write numBytes from file

		    char buf[30];
		    sprintf(buf, "(Ard) Pack size to write was: %04x bytes", numBytesTotal);
		    printf(buf); 
		  }
		else
		  {
		    printf("(Ard) Wrong no. of size bytes sent!");
		  }
	      }
	    else
	      {
		printf("(Ard) No XXWrite to begin data");
	      }
	  
	    if (write_ok == false)
	      {
		printf("(Ard) Write failed!");
	      }
	    break;
	  } 
	
	case 'r' :
	  {
	    // read pack and send to PC
	  
	    // 0 - no output, 1 - print data, 2 - dump data to serial
	    word endAddr = readAll(1); 
	    char buf[30];

	    // end address is same as size as address starts from 0, size starts from 1
	    printf("\n(Ard) Size of pack is: 0x%04x bytes\n", endAddr); 
	    break;
	  }
	  
	case '0':
	  { // read page 0
	    printf("(Ard) Page 0:");
	    printPageContents(0);              // print zero page - first 256 bytes of datapak
	    break;
	  }
      
	case '1':
	  { // read page 1
	    printf("(Ard) Page 1:");
	    printPageContents(1);                // print page 1 - second 256 bytes of datapak
	    break;
	  }
	  
	case '2':
	  { // read page 2
	    printf("(Ard) Page 2:");
	    printPageContents(2); // print page 2 - third 256 bytes of datapak
	    break;
	  }
	  
	case '3':
	  { // read page 3
	    printf("(Ard) Page 3:");
	    printPageContents(3); // print page 3 - fourth 256 bytes of datapak
	    break;
	  }
	  
	case 't':
	  { // add test record to main
	    printf("(Ard) add record to Main");
	    WriteMainRec(true /*true for output */); // add a record to main
	    break;
	  }
	  
	case 'm':
	  {// toggle between datapak and rampak modes
	    datapak_mode = 1-datapak_mode; // toggle datapak mode
	    printPackMode();
	    break;
	  }
	  
	case 'l':
	  {// toggle between paged and linear addressing modes
	    paged_addr = 1-paged_addr; // toggle addressing mode
	    printAddrMode();
	    break;
	  }
	  
	case 'i':
	  {// read pack id byte and print flag values
	    print_pak_id();
	    break;
	  }
	  
	case 'd':
	  {
	    // read dir and size pack

	    word pack_size = read_dir();

	    printf("\nPack size is: %08X", pack_size);
	    break;
	  }
	  
	case 'b':
	  {// check if pack is blank
	    blank_check();
	    break;
	  }

	case 'h':
	case '?':
	  {
	    // add test record to main
	    printCommands();
	    break;
	  }

	case 'z':
	  auto_size(false);
	  break;
	  
	case 'x':
	  {
	    // x to exit
	    
	    // exit - set all control lines to input - pack will set default line states
	    gpio_set_dir(SLOT_SS_PIN,  GPIO_IN); // deselect first
	    gpio_set_dir(SLOT_SOE_PIN,  GPIO_IN);
	    gpio_set_dir(SLOT_SPGM_PIN, GPIO_IN);
	    gpio_set_dir(SLOT_SCLK_PIN, GPIO_IN);
	    gpio_set_dir(SLOT_SMR_PIN, GPIO_IN);
	    
	    ArdDataPinsToInput();
	    
	    printf("\n(Ard) Please Remove Rampak/Datapak");
	    printf("XXExit"); // send exit command to PC
	    
	    do
	      {
		// tight_loop?
		loop_delay(1000);
	      }
	    while (true);
	    
	    break;
	  }
	  
	default:
	  {
	    printf("\n(Ard) Command not recognised!");
	    break;
	  }
	}    
    }
}

////////////////////////////////////////////////////////////////////////////////

// read the value on the data bus

BYTE get_data_bus(void)
{
  int data = 0;

  // As we are reading the bus, the direction line should be high
  gpio_put(LS_DIR_PIN, 0);
  
#if DIRECT_GPIO
  // Direct register access, as it's faster
  data = sio_hw->gpio_in;
  data >>= 0;
  data &= 0xff;
  
#else
  int i;
  
  for(i=0; i<8; i++)
    {
      if( gpio_get(data_gpio[i]) )
	{
	  data |= (1<<i);
	}
    }
#endif
  return((BYTE)data);
  
}

// Set the data bus to point to the Psion, i.e. outputs from us
void set_bus_outputs(void)
{
  return;
  // Drive level shifters to be driving Psion

  gpio_put(LS_DIR_PIN, 1);

#if DIRECT_GPIO
#if 0
  sio_hw->gpio_oe |= 0x000000FF;
#else
  sio_hw->gpio_oe_set = 0x000000FF;
#endif
#else
  int i;
  
  
  for(i=0; i<8; i++)
    {
      gpio_set_dir(data_gpio[i], GPIO_OUT);
    }
#endif
}

// Set data bus to drive us
void set_bus_inputs(void)
{
  return;
#if DIRECT_GPIO
  
  // Direct register access to make things faster

  // Get current output states
#if 0
  sio_hw->gpio_oe &= 0xffffff00;
#else
  sio_hw->gpio_oe_clr = 0x000000FF;
#endif
#else
  int i;
  
  for(i=0; i<8; i++)
    {
      gpio_set_dir(data_gpio[i], GPIO_IN);
    }
#endif
  // Drive level shifters to be driving Pico
  
  gpio_put(LS_DIR_PIN, 0);

}

// Set up the data bus GPIO lines

void set_data_bus(BYTE data)
{
  return;
#if DIRECT_GPIO
  int states;
  int dat = data & 0xff;
  //dat = ss_address;
  
  // Direct register access to make things faster
#if 0
  // Get current output states
  states = sio_hw->gpio_out;

  // Our data has its LSB at GPIO8
  states &= 0xFFFFFF00;
  sio_hw->gpio_out = states | (dat <<0);
#else
  sio_hw->gpio_set = (  dat  << 0);
  sio_hw->gpio_clr = ((dat ^ 0xFF) << 0);
    
#endif
  
#else
  int i;

  // Ensure the level shifters are towards the Psion, and we are set as ouputs
  for(i=0; i<8; i++)
    {
      if( data & (1 << i) )
	{
	  gpio_put(data_gpio[i], 1);
	}
      else
	{
	  gpio_put(data_gpio[i], 0);
	}
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
// Poll and handle the address counter on the second core
//

void handle_address(void)
{
  int last_ss;
  int last_sclk;
  int last_soe;
  int last_smr;
  int last_spgm;
  
  last_ss     = gpio_get(SLOT_SS_PIN);
  last_sclk   = gpio_get(SLOT_SCLK_PIN);
  last_soe    = gpio_get(SLOT_SOE_PIN);
  last_smr    = gpio_get(SLOT_SMR_PIN);
  last_spgm   = gpio_get(SLOT_SPGM_PIN);

  int ss;
  int sclk;
  int soe;
  int smr;
  int spgm;

  TRACE1('I');
  TRACE1('N');
  TRACE1('I');
  TRACE1('T');

  while(1)
    {
      // Read GPIO states
      ss     = gpio_get(SLOT_SS_PIN);
      sclk   = gpio_get(SLOT_SCLK_PIN);
      soe    = gpio_get(SLOT_SOE_PIN);
      smr    = gpio_get(SLOT_SMR_PIN);
      spgm   = gpio_get(SLOT_SPGM_PIN);
      
      //----------------------------------------------------------------------
      // SCLK handling
      // The lower address bit is the CLK line
      // Falling edge
      if( (last_sclk == 1) && (sclk == 0))
	{
	  // Only increment if the SMR line is low
	  if( smr == 0 )
	    {
	      TRACE1('c');
	      ss_address+=2;
	      ss_address &= (~1);
	      
	      // Wrap address
#if 0	      
	      ss_address &= PAK_MEMORY_SIZE - 1;
#else
	      // Wrap address according to pack size and whether we are paged
	      if( pak_memory[0] & PACK_ID_PAGED )
		{
		  // Paged packs have address counter that wraps at 0x100
		  ss_address &= 0xFF;
		}
	      else
		{
		  ss_address &= PAK_MEMORY_SIZE - 1;
		}
#endif	      
	      TRACE1(ss_address & 0xFF);
	      TRACE1((ss_address >> 8) & 0xFF);
	    }
	  else
	    {
	      // Trace 'clock when SMR high', this is seen in traces
	      TRACE1('z');
	    }
	}
      
      // Rising edge
      if( (last_sclk == 0) && (sclk == 1))
	{
	  if( smr == 0 )
	    {
	      ss_address |= 1;
	      TRACE1('C');
	      TRACE1(ss_address & 0xFF);
	      TRACE1((ss_address >> 8) & 0xFF);
	    }
	  else
	    {
	      TRACE1('Z');
	    }
	}

      // We now have to present data if we are selected
      if( ss == 0 )
	{
	  // We are selected, look at SOE to see if we should drive the data bus or not
	  if ( soe )
	    {
	      int data;
	      
	      // High so don't drive the data bus, this is a write
	      // Capture data on bus
	      set_bus_inputs();
	      
	      // We don't write here as SS hasn't gone high and OE hasn't gone high
	      data = get_data_bus();

#if !READ_ONLY		  
	      if( smr == 0 )
		{
		  // write to ram
		  WRITE_TRAP;
		  pak_memory[PAK_ADDRESS] = data;
		}
#endif
	    }
	  else
	    {
	      // Low, so this is a read
	      // Is it a read of the pak ID?
	      if( smr && spgm )
		{
		  TRACE1('i');
		  
		  // ID byte
		  set_bus_outputs();
		  set_data_bus(1);
		}
	      else
		{
		  // Read of pak memory
		  set_bus_outputs();
		  set_data_bus(pak_memory[PAK_ADDRESS]);
		}
	    }
	  
	}

      //--------------------------------------------------------------------------------
      // Page Counter

      if( (last_spgm == 1) && (spgm == 0) )
	{
	  TRACE1('P');
	  ss_page += (1 << 8);
	  ss_page &= 0x00007F00;
	}
      
      //----------------------------------------------------------------------
      // SMR handling
      
      if( (last_smr == 0) && (smr == 1) )
	{
	  TRACE1('R');
	  TRACE1('E');
	  TRACE1('S');
	  
	  ss_address=0;
	  ss_page = 0;
	}

      last_ss     = ss;
      last_sclk   = sclk;
      last_soe    = soe;
      last_smr    = smr;
      last_spgm   = spgm;

    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Main Loop
//
// Two items are serviced here, the menu for the OLED and the serial USB
// interface
//
////////////////////////////////////////////////////////////////////////////////


int main()
{
  char line[80];

  // Turn VPP off immediately
  gpio_init(VPP_ON_PIN);
  gpio_put(VPP_ON_PIN, 0);
  gpio_set_dir(VPP_ON_PIN, GPIO_OUT);

  // Set up default file name, default is angling pak
  strcpy(current_file, "angler.opk");
  
#if FF_FIRST_BYTES
  for(int k=0; k< 100; k++)
    {
      pak_memory[k] = 0xff;
    }
#endif

  stdio_init_all();
  loop_delay(2000000);

  printf("\n\nPsion Organiser Datapack Tool\n\n");
  printf("\nInitialising...");
  printf("\nSetting GPIOs...");
  
  // Select the SD card
  gpio_init(22);
  gpio_put(22, 0);
  gpio_set_dir(22, GPIO_OUT);
  gpio_put(22, 0);
  
  // Set up directions for the control lines
  gpio_init(SLOT_SS_PIN);
  gpio_init(SLOT_SCLK_PIN);
  gpio_init(SLOT_SMR_PIN);
  gpio_init(SLOT_SOE_PIN);
  gpio_init(SLOT_SPGM_PIN);

  for(int i=0; i<8; i++)
    {
      gpio_init(data_gpio[i]);
      gpio_set_dir(data_gpio[i], GPIO_IN);
    }
  
  gpio_set_dir(SLOT_SS_PIN, GPIO_OUT);
  gpio_set_dir(SLOT_SCLK_PIN, GPIO_OUT);
  gpio_set_dir(SLOT_SMR_PIN, GPIO_OUT);
  gpio_set_dir(SLOT_SOE_PIN, GPIO_OUT);
  gpio_set_dir(SLOT_SPGM_PIN, GPIO_OUT);

  // Drive data bus towards us
  gpio_init(LS_DIR_PIN);
  gpio_put(LS_DIR_PIN, 0);

  // LS_DIR is an output
  gpio_set_dir(LS_DIR_PIN, GPIO_OUT);

#if 0
  gpio_set_irq_enabled_with_callback(SLOT_SS_PIN,   GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(SLOT_SCLK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(SLOT_SMR_PIN,  GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(SLOT_SOE_PIN,  GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(SLOT_SPGM_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
#endif

  printf("\nInitialising SD card driver...");

#define SD_CARD 1
  
#if SD_CARD  
  // Initialise SD card driver
  sd_init_driver();
#endif

  // Set up button gpios
#if 1
  printf("\nSetting up buttons...");
  
  for(int i=0; i<NUM_BUTTONS; i++)
    {
      gpio_init(but_pins[i]);
      gpio_set_dir(but_pins[i], GPIO_IN);

#if PICOPAK      
      // Set pull ups for buttons
      gpio_set_pulls(but_pins[i], 1, 0);
#endif
    }
#endif
  
  printf("\nSetting up OLED...");

#define OLED_ON 1
  
#if OLED_ON  
  // Set up OLED display
  i2c_init(&i2c_bus_0);
    
  oled_setup(&oled0);

  // Set up the OLED menu
  
  current_menu = &(home_menu[0]);
  last_menu = &(home_menu[0]);
  the_home_menu = last_menu;

  to_home_menu(NULL);

  init_buttons();
#endif
  
  int count = 0;

#if SD_CARD  
  // Read config file into memory buffer
  // That will tell us which file to load
  process_config_file(&oled0);
#endif
  



