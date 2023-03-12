// menu setup & control code

// Number of buttons used for the menu system. The 'exit polling' button is not in this
// list, it is coded as a GPIO line as we don't want to waste time processing the menu
// buttons in the fast polling loop

#define NUM_BUTTONS  3

// Menu Buttons (switches) for datapak_tool PCB 
const int SW0_PIN		= 28;
const int SW1_PIN		= 27;
const int SW2_PIN		= 26;

const int but_pins[NUM_BUTTONS] = {SW0_PIN, SW1_PIN, SW2_PIN}; // why curly braces here?

const int LS_DIR_PIN    = 13;
const int VPP_ON_PIN    = 17;

# define SD_CARD 0

// memory store for pack image
typedef void (*FPTR)();
typedef void (*CMD_FPTR)(char *cmd);


#if PICOPAK
  #define MENU_MAX  3
#else
  #define MENU_MAX  6 // datapak_tool
#endif

int menu_offset = 0;
int menuloop_done = 0;

typedef struct _BUTTON
{
  int  count;
  bool pressed;
  bool last_pressed;   // For edge detection
  FPTR event_fn;
} BUTTON;

BUTTON buttons[NUM_BUTTONS];
enum ELEMENT_TYPE
  {
   BUTTON_ELEMENT = 10,
   SUB_MENU,
   MENU_END,
  };

struct MENU_ELEMENT
{
  enum ELEMENT_TYPE type;
  char *text;
  void *submenu;
  void (*function)(struct MENU_ELEMENT *e);
};

void button_display(struct MENU_ELEMENT *e);
void button_list(struct MENU_ELEMENT *e);
void button_pak_id(struct MENU_ELEMENT *e);
void button_pak_hdr(struct MENU_ELEMENT *e);
void button_blank(struct MENU_ELEMENT *e);

// file handling stuff

void button_select_file(struct MENU_ELEMENT *e);

//void but_ev_file_up();
//void but_ev_file_down();
//void but_ev_file_select();

#define MAX_LISTFILES 200
#define MAX_NAME 20

struct MENU_ELEMENT listfiles[MAX_LISTFILES+1];
int num_listfiles;
char names[MAX_LISTFILES][MAX_NAME];
char current_file[MAX_NAME+1];


////////////////////////////////////////////////////////////////////////////////
//
// Menu system
//
////////////////////////////////////////////////////////////////////////////////


void but_ev_up();
void but_ev_down();
void but_ev_select();
void draw_menu(I2C_SLAVE_DESC *slave, struct MENU_ELEMENT *e, bool clear);

struct MENU_ELEMENT *current_menu;
int file_menu_size = 0;

struct MENU_ELEMENT *last_menu;
struct MENU_ELEMENT *the_home_menu;
int menu_selection = 0;
//unsigned int menu_size = 0;

// returns the number of elements in a menu
int menu_size(struct MENU_ELEMENT *menu)
{
  int result = 0;
  
  while( menu->type != MENU_END )
    {
      result++;
      menu++;
    }
    
  return(result);
}

// file handling stuff


/*void button_list(struct MENU_ELEMENT *e)
{
  int file_n = 0;
  num_listfiles = 0;
  int i;
  
  char cwdbuf[FF_LFN_BUF] = {0};
  FRESULT fr;
  char const *p_dir;

  //DEBUG_STOP;

  mount_sd();
  
  if( cd_to_pak_dir(&oled0) )
    {
      unmount_sd();
      return;
    }

  f_chdrive("0:");
  fr = f_getcwd(cwdbuf, sizeof cwdbuf);

  if (FR_OK != fr)
    {
      printf("f_getcwd error: %s (%d)\n", FRESULT_str(fr), fr);
      unmount_sd();
      return;    f_chdrive("0:");
    }
  
  p_dir = cwdbuf;

  printf("\nDirectory Listing: %s\n", p_dir);
  
  DIR dj;      // Directory object 
  FILINFO fno; // File information 

  memset(&dj, 0, sizeof dj);
  memset(&fno, 0, sizeof fno);

  fr = f_findfirst(&dj, &fno, p_dir, "*");

  if (FR_OK != fr)
    {
      printf("f_findfirst error: %s (%d)\n", FRESULT_str(fr), fr);
      unmount_sd();
      return;
    }
  
  while( (fr == FR_OK) && fno.fname[0] && (num_listfiles < MAX_LISTFILES) )
    { 
      if (fno.fattrib & AM_DIR)
	{
	  // Directory, we ignore these
	}
      else
	{
	  char extension[40];
	  char name[80];
	   
	  // If the file has an extension of .opk then display it
	  // otherwise ignore.
	  extension[0] = '\0';

	  printf("\n%s", fno.fname);
	  
	  if( sscanf(fno.fname, "%[^.].%s", name, extension) )
	    {
	      if( strcmp(extension, "opk") == 0 )
		{
		  // Create a new menu element
		  // we also don't want to display anything before the offset
		  if( file_n >= file_offset )
		    {
		      // It is an opk file so display it
		      strncpy(&(names[num_listfiles][0]), fno.fname, MAX_NAME);
		       
		      //	display.println(&(names[nu);
		      listfiles[num_listfiles].text = &(names[num_listfiles][0]);
		      listfiles[num_listfiles].type = BUTTON_ELEMENT;
		      listfiles[num_listfiles].submenu = NULL;
		      listfiles[num_listfiles].function = button_select_file;
		       
		      num_listfiles++;
		    }
		  
		  // Next file
		  file_n++;
		}
	    }
	}
       
      fr = f_findnext(&dj, &fno); // Search for next item 
    }
  f_closedir(&dj);

  printf("\n");
  
  // terminate the menu
  listfiles[num_listfiles].text = "";
  listfiles[num_listfiles].type = MENU_END;
  listfiles[num_listfiles].submenu = NULL;
  listfiles[num_listfiles].function = button_select_file;


#if 1// We know how big the menu is now
  if( num_listfiles != 0 )
    {
      file_menu_size = num_listfiles;
    }
#endif

  // Button actions modified
  buttons[0].event_fn = but_ev_up;
  buttons[1].event_fn = but_ev_down;
  buttons[2].event_fn = but_ev_file_select;

  // Set up menu of file names
  current_menu = &(listfiles[0]);
  draw_menu(&oled0, current_menu, false);

  unmount_sd();
}*/




////////////////////////////////////////////////////////////////////////////////
//
// Menu system
//
////////////////////////////////////////////////////////////////////////////////


// The switch menu/OLED display system
void to_back_menu(struct MENU_ELEMENT *e)
{ printf("back menu setup\n");
  menu_selection = 0;
  menu_offset = 0;
  current_menu = last_menu;
  draw_menu(&oled0, current_menu, true);
}

void to_home_menu(struct MENU_ELEMENT *e)
{ printf("home menu setup\n");
  menu_selection = 0;
  menu_offset = 0;
  current_menu = the_home_menu;
  draw_menu(&oled0, current_menu, true);
}



//--------------------------------------------------------------------------------

// Move up in a menu
void but_ev_up()
{ printf("but_ev_up\n");
  // Move up one place
  menu_selection--;

  // Keep within bounds
  if( menu_selection <= 0 )
    {
      menu_selection = 0;
    }

  // Now move offset up if it is greter than selection
  if( menu_offset > menu_selection )
    {
      menu_offset = menu_selection;
    }
  
  draw_menu(&oled0, current_menu, false);
}

// Move down in a menu
void but_ev_down()
{
	printf("but_ev_down\n");
  // Move cursor down one entry
  menu_selection++;

  // Are we off the end of the menu?
  if( menu_selection == menu_size(current_menu) )
    {
      // At last element of menu
      menu_selection--;
    }

  // Do we need to pull the top element down?
  if( (menu_selection - menu_offset) >= MENU_MAX )
    {
      menu_offset = menu_selection - MENU_MAX+1;
      if( menu_offset < 0 )
	{
	  menu_offset = 0;
	}
    }
  
  draw_menu(&oled0, current_menu, false);
  //  button_list(NULL);
}

void button_exit(struct MENU_ELEMENT *e)
{
  menuloop_done = 1;
}

void button_send(struct MENU_ELEMENT *e)
{
  //send_databytes(true);
  draw_menu(&oled0, current_menu, true);
}

void button_auto_size(struct MENU_ELEMENT *e)
{
  //auto_size(true); // run datapack autosizing
  draw_menu(&oled0, current_menu, true);
}

// Clear flag indicates whether we redraw the menu text and clear the screen. Or not.
//
// Menus are displayed starting at menu_offset and will only put
// MENU_MAX lines on the screen
// This allows either size of display to be used

void draw_menu(I2C_SLAVE_DESC *slave, struct MENU_ELEMENT *e, bool clear)
{
  int i = 0;
  char etext[50];

  e = current_menu;

  //DEBUG_STOP
  
  // Clear screen
  if(clear,1)
    {
      oled_clear_display(slave);
      //display.clearDisplay();
    }

  int entry_number = -1;
  
  while( e->type != MENU_END )
    {
      entry_number++;
      
      // Skip the first menu_offset entries
      if( entry_number < menu_offset )
	{
	  e++;
	  continue;
	}

      // Don't ever display more than MENU_MAX elements
      if( (entry_number - menu_offset) > MENU_MAX )
	{
	  break;
	}

      if( entry_number == menu_selection )
	{
	  sprintf(etext, ">%-19s", e->text);
	}
      else
	{
	  sprintf(etext, " %-19s", e->text);
	}
      
      switch(e->type)
	{
	case BUTTON_ELEMENT:
	  oled_set_xy(slave, 0, i*8);
	  //display.printChar(curs);
	  if( clear,1 )
	    {
	      oled_display_string(slave, etext);
		  printf("%s\n", etext);
	    }
	  break;

	case SUB_MENU:
	  oled_set_xy(slave, 0, i*8);
	  oled_display_string(slave, etext);
	  printf("%s\n", etext);
	  break;
	}

      e++;
      i++;
    }
  
  //menu_size = i;

  // Blank the other entries
  //make sure menu_selection isn't outside the menu
  if( menu_selection >= menu_size(current_menu) )
    {
      menu_selection = menu_size(current_menu)-1;
    }

  for(; i<MENU_MAX; i++)
    {
      oled_set_xy(slave, 0, i*8);
      oled_display_string(slave, "               ");
      //      display.setCursor(0, i*8);
      //display.println("               ");
    }

#if 0  
  for(i=menu_offset; i<menu_offset+MENU_MAX-1;i++)
    {
      if( i == menu_selection )
	{
	  curs = ">";	  
	}
      else
	{
	  curs = " ";
	}

      oled_set_xy(slave, 0, i*8);
      oled_display_string(slave, curs);

      //display.setCursor(0, i*8);
      //display.print(curs);
    }
#endif 

#if 1
  char line[40];
  oled_set_xy(slave, 0, 8);
  sprintf(line, "\nsel: %d, offset: %d, size: %d, file_menu_size: %d\n", menu_selection, menu_offset, menu_size(current_menu), file_menu_size);
  //oled_display_string(slave, line);
  printf(line);
#endif
}

/*
void old_draw_menu(I2C_SLAVE_DESC *slave, struct MENU_ELEMENT *e, bool clear)
{
  int i = 0;
  char *curs = " ";
  char etext[20];

  //DEBUG_STOP
  
  // Clear screen
  if(clear,1)
    {
      oled_clear_display(slave);
      //display.clearDisplay();
    }
  
  while( e->type != MENU_END )
    {
      sprintf(etext, " %-19s", e->text);
      
      switch(e->type)
	{
	case BUTTON_ELEMENT:
	  oled_set_xy(slave, 0, i*8);
	  //display.printChar(curs);
	  if( clear,1 )
	    {
	      oled_display_string(slave, etext);
	    }
	  break;

	case SUB_MENU:
	  oled_set_xy(slave, 0, i*8);
	  //	  display.setCursor(0, i*8);
	  //display.printChar(curs);
	  if ( clear,1 )
	    {
	      oled_display_string(slave, etext);
	      //	      display.println(etext);
	    }
	  break;
	}
      e++;
      i++;
    }
  
  //menu_size = i;

  // Blank the other entries
  //make sure menu_selection isn't outside the menu
  if( menu_selection >= menu_size(current_menu) )
    {
      menu_selection = menu_size(current_menu)-1;
    }

  for(; i<MAX_LISTFILES; i++)
    {
      oled_set_xy(slave, 0, i*8);
      oled_display_string(slave, "               ");
      //      display.setCursor(0, i*8);
      //display.println("               ");
    }

  for(i=0;i<menu_size(current_menu);i++)
    {
      if( i == menu_selection )
	{
	  curs = ">";	  
	}
      else
	{
	  curs = " ";
	}

      oled_set_xy(slave, 0, i*8);
      oled_display_string(slave, curs);

      //display.setCursor(0, i*8);
      //display.print(curs);
    }
}*/

// Null button event function
void but_ev_null()
{  printf("but_ev_null\n");
	/*
	oled_clear_display(&oled0);
	oled_set_xy(&oled0, 0,0);
	oled_display_string(&oled0, "Button test");
	*/
}

void old_but_ev_up()
{
  if( menu_selection == 0 )
    {
      menu_selection = menu_size(current_menu) - 1;
    }
  else
    {
      menu_selection = (menu_selection - 1) % menu_size(current_menu);
    }
  
  draw_menu(&oled0, current_menu, false);
}

void old_but_ev_down()
{

  //DEBUG_STOP

  menu_selection = (menu_selection + 1) % menu_size(current_menu);

  draw_menu(&oled0, current_menu, false);
}

void but_ev_select()
{printf("but_ev_select\n");
  struct MENU_ELEMENT *e;
  int i = 0;
  
  // Do what the current selection says to do
  for(e=current_menu, i=0; (e->type != MENU_END); i++, e++)
    {
      if( i == menu_selection )
		{
		  switch(e->type)
			{
			case SUB_MENU:
			  current_menu = (struct MENU_ELEMENT *)e->submenu;
			  menu_offset = 0;
			  menu_selection = 0;
			  
			  draw_menu(&oled0, current_menu, true);
			  break;
			  
			default:
			  // Do action
			  (e->function)(e);
			  draw_menu(&oled0, current_menu, true);
			  break;
			}
		}
    }
}

void init_buttons()
{
	  // Set up buttons
	  
	#if 1
	printf("\nSetting up buttons...");

	// Set up button gpios
	for(int i=0; i<NUM_BUTTONS; i++)
	{
	  gpio_init(but_pins[i]);
	  gpio_set_dir(but_pins[i], GPIO_IN);

	#if PICOPAK      
	  // Set pull ups for buttons
	  gpio_set_pulls(but_pins[i], 1, 0);
	#endif

	}
	printf("\n");
	#endif
		
  for(int i=0; i<NUM_BUTTONS; i++)
    {
      buttons[i].count = 0;
      buttons[i].pressed = false;
      buttons[i].last_pressed = false;
      buttons[i].event_fn = but_ev_null;
    }

  buttons[0].event_fn = but_ev_up;
  buttons[1].event_fn = but_ev_down;
  buttons[2].event_fn = but_ev_select;
  printf("Buttons initialised\n");
}

void update_buttons()
{//printf("Update_buttons\n");
  for(int i=0; i<NUM_BUTTONS; i++)
    {
      if( gpio_get(but_pins[i]) == 0 ) // buttons are pull down
	  {
		  while (gpio_get(but_pins[i]) == 0) {} // wait until button not pressed
		  buttons[i].pressed = true;
		  printf("button pressed\n");
	  }
	  else 
	  {buttons[i].pressed = false;}
      
      // If button pressed then we treat that as a key event
      if( (buttons[i].last_pressed == false) && (buttons[i].pressed == true) )
		{printf("button event\n");
		  (buttons[i].event_fn)();
		}

      buttons[i].last_pressed = buttons[i].pressed;
    }
}

#if SD_CARD

//////////////////////////////////////////////////////////////////////////
//
// file handling menus
//
//////////////////////////////////////////////////////////////////////////

// Write an OPK file from SD card to the datapack
// Writes the previously selected file

/*

void button_write_file(struct MENU_ELEMENT *e)
{
  write_opk_file(&oled0, current_file);

  loop_delay(3000000);
  draw_menu(&oled0, current_menu, true);
}

// Compare an OPK file from SD card to the datapack
// Compares the previously selected file

void button_compare_file(struct MENU_ELEMENT *e)
{
  compare_opk_file(&oled0, current_file);

  loop_delay(3000000);
  draw_menu(&oled0, current_menu, true);
}

*/

int cd_to_pak_dir(I2C_SLAVE_DESC *slave)
{
  char line[40];
  FRESULT fr;
  char cwdbuf[FF_LFN_BUF] = {0};
  char const *p_dir;

  //DEBUG_STOP

  //f_chdrive("0:");


  // Are we in the pak directory already?
  fr = f_getcwd(cwdbuf, sizeof cwdbuf);
  if (FR_OK != fr)
    {
      oled_clear_display(slave);
      oled_set_xy(slave, 0, 0);
      sprintf(line, "cwd error");
      oled_display_string(slave, line);

      oled_set_xy(slave, 0, 8);
      sprintf(line, "cwd:'%s'", cwdbuf);
      oled_display_string(slave, line);
      
      return;
    }

// The button function puts up to the first 7 files on screen then sets up a button handler
// which will display subsequent pages.
// We use the menu structures to display the names and allow selection

// File selected
void button_select_file(struct MENU_ELEMENT *e)
{
}

// Store file name and exit menu
// File can be read later

void but_ev_file_select()
{
  char line[40];
  
  strcpy(current_file, listfiles[menu_selection].text);
  file_offset = 0;

  oled_clear_display(&oled0);
  oled_set_xy(&oled0, 0,0);
  sprintf(line, "Selected File:");
  oled_display_string(&oled0, line);

  oled_set_xy(&oled0, 0,14);
  sprintf(line, "%s", current_file);
  oled_display_string(&oled0, line);

  loop_delay(3000000);

  // As well as selecting the file, read in in to the buffer as well
  core_read(&oled0, current_file);

  menu_selection = 0;
  to_home_menu(NULL);
  
  buttons[0].event_fn = but_ev_up;
  buttons[1].event_fn = but_ev_down;
  buttons[2].event_fn = but_ev_select;
}

void button_to_home(struct MENU_ELEMENT *e)
{
  to_home_menu(NULL);
}


/*
// Get a list of the files on the SD card and display
// them so one can be selected.
void button_list_old(struct MENU_ELEMENT *e)
{
#if 0
  File dir;
  int file_n = 0;
  num_listfiles = 0;
  int i;

  dir = SD.open("/");

  // return to the first file in the directory
  dir.rewindDirectory();
  
  while (num_listfiles < MAX_LISTFILES) {

    File entry =  dir.openNextFile();

    if (! entry) {
      // no more files
      // terminate menu
      listfiles[num_listfiles].text = "";
      listfiles[num_listfiles].type = MENU_END;
      listfiles[num_listfiles].submenu = NULL;
      listfiles[num_listfiles].function = button_select_file;
      entry.close();
      break;
    }

    
    // We don't allow directories and don't count them
    if (entry.isDirectory())
      {
      }
    else
      {
#if DEBUG	
	printf("BList-file_n:");
	printf(file_n);
	printf(entry.name());
	printf("  num_listfiles:");
	printf("%d\n", num_listfiles);
#endif
	// Create a new menu element
	// we also don't want to display anything before the offset
	if( file_n >= file_offset )
	  {
	    strncpy(&(names[num_listfiles][0]), entry.name(), MAX_NAME);
	    //	display.println(&(names[nu);
	    listfiles[num_listfiles].text = &(names[num_listfiles][0]);
	    listfiles[num_listfiles].type = BUTTON_ELEMENT;
	    listfiles[num_listfiles].submenu = NULL;
	    listfiles[num_listfiles].function = button_select_file;
	    
	    num_listfiles++;
	  }
	// Next file
	file_n++;

      }
    entry.close();
    
  }

  dir.close();

  // terminate menu
  listfiles[num_listfiles].text = "";
  listfiles[num_listfiles].type = MENU_END;
  listfiles[num_listfiles].submenu = NULL;
  listfiles[num_listfiles].function = button_select_file;

  // We know how big the menu is now
#if 0
  if( num_listfiles != 0 )
    {
      menu_size = num_listfiles;
    }
#endif
  
  // Button actions modified
  buttons[0].event_fn = but_ev_up;
  buttons[1].event_fn = but_ev_down;
  buttons[2].event_fn = but_ev_file_select;

  // Set up menu of file names
  current_menu = &(listfiles[0]);
  draw_menu(&oled0, current_menu, false);
#endif
}


#define COLUMNS 5
#define PAGE_LENGTH 30

// Display the buffer

int display_offset = 0;

void but_page_up()
{
  if( display_offset > PAGE_LENGTH )
    {
      display_offset -= PAGE_LENGTH;
    }
  else
    {
      display_offset = 0;
    }
  button_display(NULL);
}

void but_page_down()
{
#if 0
  display_offset += PAGE_LENGTH;
  
  if( display_offset >= bytecount )
    {
      display_offset = bytecount-PAGE_LENGTH;
    }

  if( display_offset < 0 )
    {
      display_offset = 0;
    }
  
  button_display(NULL);
#endif
}

void but_page_exit()
{
  display_offset = 0;
  draw_menu(&oled0,current_menu, true);

  buttons[0].event_fn = but_ev_up;
  buttons[1].event_fn = but_ev_down;
  buttons[2].event_fn = but_ev_select;

}
*/

void button_display(struct MENU_ELEMENT *e)
{
#if 0
  int i;
  char ascii[17];
  char c[5];
  
  int ascii_i = 0;

  oled_clear_display(&oled0);
  
  //display.clearDisplay();
  
  for(i=0; (i<bytecount) && (i<PAGE_LENGTH); i++)
    {
      if( isprint(stored_bytes[i]) )
	{
	  sprintf(ascii, "%c", stored_bytes[i+display_offset]);
	}
      else
	{
	  sprintf(ascii, ".");
	}
      
      sprintf(c,     "%02X",  stored_bytes[i+display_offset]);

#if 0
      display.setCursor(6*15+(i%COLUMNS)*1*6, 8*(i/COLUMNS+1));
      display.println(ascii);
      display.setCursor(10*0+(i%COLUMNS)*2*8, 8*(i/COLUMNS+1));
      display.print(c);
#endif
    }

  // Drop into page up and down and exit buttoin handlers
  buttons[0].event_fn = but_page_up;
  buttons[1].event_fn = but_page_down;
  buttons[2].event_fn = but_page_exit;

  //display.display();
#endif
}


////////////////////////////////////////////////////////////////////////////////
//
//
// Read the datapack and write the contents to SD card
//
//
////////////////////////////////////////////////////////////////////////////////

void button_read(struct MENU_ELEMENT *e)
{
  // Find a filename that hasn't been used
  char filename[20];
  int file_index = 0;
  
  mount_sd();
  
  if( cd_to_pak_dir(&oled0) )
    {
      unmount_sd();
      return;
    }

  for(;;)
    {
      sprintf(filename, "PAK%04d.OPK", file_index);
      
      FF_FILE *fp = ff_fopen(filename, "r");
      
      if (fp == NULL)
	{
	  // We have a filename we can use
	  break;
	}
      else
	{
	  // Try the next index number
	  file_index++;
	  ff_fclose(fp);

	}
    }

  unmount_sd();
  
  // We have a filename now, use it
  read_opk_file(&oled0, filename);
  
  draw_menu(&oled0, current_menu, true);
}

#endif


////////////////////////////////////////////////////////////////////////////////
//
// Drives Vpp with square wave to test the switch circuit
//
////////////////////////////////////////////////////////////////////////////////

void button_cycle_vpp(struct MENU_ELEMENT *e)
{printf("cycle vpp\n");
  for(int i=0;i<20;i++)
    {
      gpio_put(VPP_ON_PIN, 0);
	  printf("VPP on %d\n",i);
      //sleep_ms(500);
      loop_delay(100000);
      
      gpio_put(VPP_ON_PIN, 1);
      //      sleep_ms(500);
      loop_delay(100000);
	  printf("VPP off\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Menu definitions
//
////////////////////////////////////////////////////////////////////////////////


// Prototypes
void button_write_file(struct MENU_ELEMENT *e);
void button_compare_file(struct MENU_ELEMENT *e);
void button_read(struct MENU_ELEMENT *e);

void button_display(struct MENU_ELEMENT *e);
void button_list(struct MENU_ELEMENT *e);
void button_pak_id(struct MENU_ELEMENT *e);
void button_pak_hdr(struct MENU_ELEMENT *e);
void button_blank(struct MENU_ELEMENT *e);

//void button_cycle_vpp(struct MENU_ELEMENT *e);


const struct MENU_ELEMENT test_menu[] =
  {
   {BUTTON_ELEMENT, "Cycle Vpp",                  NULL,     button_cycle_vpp},
   {BUTTON_ELEMENT, "Exit",                       NULL,     to_home_menu},
   {MENU_END,       "",                           NULL,     NULL},
  };

/*
const struct MENU_ELEMENT info_menu[] =
  {
   {BUTTON_ELEMENT, "Pack ID",                    NULL,     button_pak_id},
   {BUTTON_ELEMENT, "Pack Header",                NULL,     button_pak_hdr},
   {BUTTON_ELEMENT, "Blank Check",                NULL,     button_blank},
   {BUTTON_ELEMENT, "Auto Size",                  NULL,     button_auto_size},
   {BUTTON_ELEMENT, "Exit",                       NULL,     to_home_menu},
   {MENU_END,       "",                           NULL,     NULL},
  };


const struct MENU_ELEMENT home_menu[] =
  {
   {BUTTON_ELEMENT, "List",                       NULL,     button_list},
   {BUTTON_ELEMENT, "Write File to Pak",          NULL,     button_write_file},
   {BUTTON_ELEMENT, "Compare File",               NULL,     button_compare_file},
   {BUTTON_ELEMENT, "Display",                    NULL,     button_display},
   {SUB_MENU,       "Test",                       test_menu,     NULL},
   {SUB_MENU,       "Info",                       info_menu,     NULL},
   {BUTTON_ELEMENT, "Read Pak to File",           NULL,     button_read},
   {BUTTON_ELEMENT, "Exit",                       NULL,     button_exit},
   {MENU_END,       "",                           NULL,     NULL},
  };
  */
  
  const struct MENU_ELEMENT home_menu[] =
  {
   {BUTTON_ELEMENT, "Item1",                       NULL,     NULL},
   {BUTTON_ELEMENT, "Item2",          NULL,     NULL},
   {BUTTON_ELEMENT, "Item3",               NULL,     NULL},
   {SUB_MENU,       "Test",                       test_menu,     NULL},
   {MENU_END,       "",                           NULL,     NULL},
  };

  
