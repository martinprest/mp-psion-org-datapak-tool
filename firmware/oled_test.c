
// test of oled and menu code

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

//#include <stdarg.h>  // can't see any use for this yet
//#include <ctype.h>

#include "oled.c"
#include "menus.c"
#include "sd_card_fs.c"

#define PICOPAK 0
#define OLED_ON 1
#define SD_CARD 1 // currently crashes if SD_CARD 1
#define USE_INTERRUPTS 0

void main()
 {
      char line[80];
	
	  stdio_init_all();
	  loop_delay(2000000);
	  
	  // Set up OLED display
	  i2c_init(&i2c_bus_0);
		
	  oled_setup(&oled0);
	  
	  // print info to serial
	  printf("\n\nPsion Organiser Datapack Tool\n\n");
	  printf("\nInitialising...\n");
	  
	  
	  /*
	  oled_clear_display(&oled0);
	  oled_set_xy(&oled0, 0,0);
	  oled_display_string(&oled0, "OLED test");
	  //oled_display_string(&oled0, "HELLO");
	  */
	  
	  // datapack tool main loop

  while(1)
    {

      //stdio_flush();
	  
#if OLED_ON
      // Overall loop, which contains the polling loop and the menu loop
      oled_clear_display(&oled0);
      oled_set_xy(&oled0, 0,0);
      oled_display_string(&oled0, "Datapak Gadget Test");
#endif
      
#if USE_INTERRUPTS
	  oled_set_xy(&oled0, 0,14);
	  oled_display_string(&oled0, "Interrupts");
#endif

loop_delay(2000000);
      
#if SD_CARD
	  // Initialise SD card driver
	  sd_init_driver();

      // Mount and unmount the SD card to set the sd_ok_flag up
      mount_sd();
      unmount_sd();
      
      oled_set_xy(&oled0, 0,21);
      if( sd_ok_flag )
	{
	  oled_display_string(&oled0, "SD card OK");
	  printf("\nSD card OK");
	}
      else
	{
	  oled_display_string(&oled0, "SD card NOT OK");
	  printf("\nSD card NOT OK");
	}

      loop_delay(1000000);
#endif

#if SD_CARD 
	  sprintf(line, "%s", current_file); // current_file defined in menus.c
	  oled_set_xy(&oled0, 0, 14);
	  oled_display_string(&oled0, line);
#endif
      
#if 0
      // Indicate we are now in menu
      oled_clear_display(&oled0);
      oled_set_xy(&oled0, 0,0);
      oled_display_string(&oled0, "Datapak Gadget Menu");
	  loop_delay(1000000);
#endif

#if OLED_ON
	  // Set up the OLED menu
	  
	  printf("\nMENU\n");
  
	  current_menu = &(home_menu[0]);
	  last_menu = &(home_menu[0]);
	  the_home_menu = last_menu;

	  to_home_menu(NULL);

	  init_buttons();

#endif
      
      // Menu loop
      menuloop_done = 0;
      //stdio_flush();
      
      while(!menuloop_done)
	{
	  stdio_flush();
	  
#if OLED_ON
	  // Run menu
	  update_buttons();
#endif
	  
	  // Run serial interface for datapak commands
	  // serial_loop();
	  loop_delay(100000);
	}
    }
};

