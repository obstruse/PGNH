
#define BUTTON_GAP 16
static byte  buttonSize = 60;
static byte  grooveSize = 36;
static int screenWidth = 320; //(LCD_TYPE == ITDB24E_8 || LCD_TYPE == TFT01_24_8) ? 320 : 220;
static int screenHeight = 240; //(LCD_TYPE == ITDB24E_8 || LCD_TYPE == TFT01_24_8) ? 240 : 176;
static int centreYPosition = 112; //(LCD_TYPE == ITDB24E_8 || LCD_TYPE == TFT01_24_8) ? 112 : 80;


#define BUTTON_SET_HOME 1
#define BUTTON_DRAW_FROM_SD 2
#define BUTTON_MORE_RIGHT 3 // UNUSED
#define BUTTON_PAUSE_RUNNING 4
#define BUTTON_RESUME_RUNNING 5

#define BUTTON_RESET 6
#define BUTTON_PEN_UP 7
#define BUTTON_PEN_DOWN 8
#define BUTTON_INC_SPEED 9
#define BUTTON_DEC_SPEED 10

#define BUTTON_NEXT_FILE 11
#define BUTTON_PREV_FILE 12
#define BUTTON_MAIN_MENU 13
#define BUTTON_OK 14
#define BUTTON_CANCEL_FILE 15

#define BUTTON_DRAW_THIS_FILE 16
#define BUTTON_INC_ACCEL 18
#define BUTTON_DEC_ACCEL 19
#define BUTTON_DONE 20
#define BUTTON_MOVE_INC_A 21

#define BUTTON_MOVE_DEC_A 22
#define BUTTON_MOVE_INC_B 23
#define BUTTON_MOVE_DEC_B 24
#define BUTTON_INC_PENSIZE 25
#define BUTTON_DEC_PENSIZE 26

#define BUTTON_INC_PENSIZE_INC 27
#define BUTTON_DEC_PENSIZE_INC 28
#define BUTTON_ADJUST_SPEED_MENU 29
#define BUTTON_ADJUST_PENSIZE_MENU 30
#define BUTTON_ADJUST_POSITION_MENU 31

#define BUTTON_POWER_ON 32
#define BUTTON_POWER_OFF 33
#define BUTTON_STOP_FILE 34
#define BUTTON_SETTINGS_MENU 35
#define BUTTON_CALIBRATE 36

#define BUTTON_TOGGLE_ECHO 37
#define BUTTON_RESET_SD 38
#define BUTTON_SETTINGS_MENU_2 39
#define BUTTON_INC_PENLIFT_UP 40
#define BUTTON_DEC_PENLIFT_UP 41

#define BUTTON_INC_PENLIFT_DOWN 42
#define BUTTON_DEC_PENLIFT_DOWN 43
#define BUTTON_ADJUST_PENLIFT 44
#define BUTTON_PENLIFT_SAVE_TO_EEPROM 45

// an array of buttons specifications, indexed by button id.
#define NUM_OF_BUTTONS 46
#define BUTTONS_PER_MENU 6
#define NUM_OF_MENUS 10

static ButtonSpec buttons[NUM_OF_BUTTONS];
int buttonCoords[12][2];
typedef byte Menus[NUM_OF_MENUS][BUTTONS_PER_MENU];

#define MENU_INITIAL 1
#define MENU_RUNNING 2 // UNUSED
#define MENU_CHOOSE_FILE 3
#define MENU_ADJUST_PENSIZE 4
#define MENU_ADJUST_POSITION 5
#define MENU_ADJUST_SPEED 6
#define MENU_SETTINGS 7
#define MENU_SETTINGS_2 8
#define MENU_ADJUST_PENLIFT 9

char currentMenu = 0;

static Menus menus = {
  {0,0,0,
    0,0,0}, // the empty first element 0
    
  // MENU_INITIAL 1
  {BUTTON_POWER_ON, BUTTON_DRAW_FROM_SD, BUTTON_CALIBRATE,
     BUTTON_PAUSE_RUNNING, BUTTON_PEN_DOWN, BUTTON_SETTINGS_MENU},
     
  // MENU_RUNNING (NOT USED) 2
  {BUTTON_MAIN_MENU, BUTTON_ADJUST_SPEED_MENU, BUTTON_ADJUST_PENSIZE_MENU,
     BUTTON_PAUSE_RUNNING, 0, 0},
     
  // MENU_CHOOSE_FILE 3
  {0, BUTTON_NEXT_FILE, 0,
     BUTTON_DONE, BUTTON_PREV_FILE, BUTTON_DRAW_THIS_FILE},
     
  // MENU_ADJUST_PENSIZE 4
  {0, BUTTON_INC_PENSIZE_INC, BUTTON_INC_PENSIZE,
     BUTTON_DONE, BUTTON_DEC_PENSIZE_INC, BUTTON_DEC_PENSIZE},

  // MENU_ADJUST_POSITION 5
  {0, BUTTON_MOVE_INC_A, BUTTON_MOVE_INC_B,
     BUTTON_DONE, BUTTON_MOVE_DEC_A, BUTTON_MOVE_DEC_B},
     
  // MENU_ADJUST_SPEED 6
  {0, BUTTON_INC_SPEED, BUTTON_INC_ACCEL,
     BUTTON_DONE, BUTTON_DEC_SPEED, BUTTON_DEC_ACCEL},

  // MENU_SETTINGS 7
  {BUTTON_ADJUST_POSITION_MENU, BUTTON_ADJUST_SPEED_MENU, BUTTON_ADJUST_PENSIZE_MENU,
     BUTTON_DONE, BUTTON_TOGGLE_ECHO, BUTTON_SETTINGS_MENU_2},
     
  // MENU_SETTINGS_2 8
  {BUTTON_ADJUST_PENLIFT, 0, 0,
     BUTTON_DONE, 0, 0},
     
  // MENU_ADJUST_PENLIFT 9
  {BUTTON_PENLIFT_SAVE_TO_EEPROM, BUTTON_INC_PENLIFT_DOWN, BUTTON_INC_PENLIFT_UP,
     BUTTON_DONE, BUTTON_DEC_PENLIFT_DOWN, BUTTON_DEC_PENLIFT_UP}
};

// Loads labels, display condition and next button into the buttons array
void button_setup_loadButtons()
{
  buttons[0] = {0, 0, 0, 0, 0};
  buttons[BUTTON_PAUSE_RUNNING] = (ButtonSpec){BUTTON_PAUSE_RUNNING, "pause motors", button_genericButtonAction, BUTTON_RESUME_RUNNING, NEVER_RETRIGGER};

  buttons[BUTTON_SET_HOME] = (ButtonSpec){BUTTON_SET_HOME, "set home", button_genericButtonAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_DRAW_FROM_SD] = (ButtonSpec){BUTTON_DRAW_FROM_SD, "draw from sd", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_PAUSE_RUNNING] = (ButtonSpec){BUTTON_PAUSE_RUNNING, "pause motors", button_genericButtonAction, BUTTON_RESUME_RUNNING, NEVER_RETRIGGER};
  buttons[BUTTON_RESUME_RUNNING] = (ButtonSpec){BUTTON_RESUME_RUNNING, "run motors", button_genericButtonAction, BUTTON_PAUSE_RUNNING, NEVER_RETRIGGER};

  buttons[BUTTON_RESET] = (ButtonSpec){BUTTON_RESET, "reset", button_genericButtonAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_PEN_UP] = (ButtonSpec){BUTTON_PEN_UP, "pen up", button_genericButtonAction, BUTTON_PEN_DOWN, LONG_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_PEN_DOWN] = (ButtonSpec){BUTTON_PEN_DOWN, "pen down", button_genericButtonAction, BUTTON_PEN_UP, LONG_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_INC_SPEED] = (ButtonSpec){BUTTON_INC_SPEED, "inc. speed", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_SPEED] = (ButtonSpec){BUTTON_DEC_SPEED, "dec. speed", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};

  buttons[BUTTON_NEXT_FILE] = (ButtonSpec){BUTTON_NEXT_FILE, "next file", button_genericButtonAction, 0, LONG_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_PREV_FILE] = (ButtonSpec){BUTTON_PREV_FILE, "prev file", button_genericButtonAction, 0, LONG_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_MAIN_MENU] = (ButtonSpec){BUTTON_MAIN_MENU, "main menu", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_OK] = (ButtonSpec){BUTTON_OK, "OK", button_genericButtonAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_CANCEL_FILE] = (ButtonSpec){BUTTON_CANCEL_FILE, "cancel drawing", button_genericButtonAction, 0, NEVER_RETRIGGER};

  buttons[BUTTON_DRAW_THIS_FILE] = (ButtonSpec){BUTTON_DRAW_THIS_FILE, "draw this file", button_genericButtonAction, BUTTON_STOP_FILE, NEVER_RETRIGGER};
  buttons[BUTTON_INC_ACCEL] = (ButtonSpec){BUTTON_INC_ACCEL, "inc. accel", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_ACCEL] = (ButtonSpec){BUTTON_DEC_ACCEL, "dec. accel", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DONE] = (ButtonSpec){BUTTON_DONE, "done", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_MOVE_INC_A] = (ButtonSpec){BUTTON_MOVE_INC_A, "inc. motor A", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};

  buttons[BUTTON_MOVE_DEC_A] = (ButtonSpec){BUTTON_MOVE_DEC_A, "dec. motor A", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_MOVE_INC_B] = (ButtonSpec){BUTTON_MOVE_INC_B, "inc. motor B", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_MOVE_DEC_B] = (ButtonSpec){BUTTON_MOVE_DEC_B, "dec. motor B", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_INC_PENSIZE] = (ButtonSpec){BUTTON_INC_PENSIZE, "inc. pentip width", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_PENSIZE] = (ButtonSpec){BUTTON_DEC_PENSIZE, "dec. pentip width", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};

  buttons[BUTTON_INC_PENSIZE_INC] = (ButtonSpec){BUTTON_INC_PENSIZE_INC, "inc. pentip incr", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_PENSIZE_INC] = (ButtonSpec){BUTTON_DEC_PENSIZE_INC, "dec. pentip incr", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_ADJUST_SPEED_MENU] = (ButtonSpec){BUTTON_ADJUST_SPEED_MENU, "adjust speed", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_ADJUST_PENSIZE_MENU] = (ButtonSpec){BUTTON_ADJUST_PENSIZE_MENU, "adjust pentip width", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_ADJUST_POSITION_MENU] = (ButtonSpec){BUTTON_ADJUST_POSITION_MENU, "adjust position", genericChangeMenuAction, 0, NEVER_RETRIGGER};

  buttons[BUTTON_POWER_ON] = (ButtonSpec){BUTTON_POWER_ON, "motors on", button_genericButtonAction, BUTTON_POWER_OFF, NEVER_RETRIGGER};
  buttons[BUTTON_POWER_OFF] = (ButtonSpec){BUTTON_POWER_OFF, "motors off", button_genericButtonAction, BUTTON_POWER_ON, NEVER_RETRIGGER};
  buttons[BUTTON_STOP_FILE] = (ButtonSpec){BUTTON_STOP_FILE, "stop file", button_genericButtonAction, BUTTON_DRAW_THIS_FILE, NEVER_RETRIGGER};
  buttons[BUTTON_SETTINGS_MENU] = (ButtonSpec){BUTTON_SETTINGS_MENU, "settings", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_CALIBRATE] = (ButtonSpec){BUTTON_CALIBRATE, "calibrate", button_genericButtonAction, 0, NEVER_RETRIGGER};

  buttons[BUTTON_TOGGLE_ECHO] = (ButtonSpec){BUTTON_TOGGLE_ECHO, "toggle echo", button_genericButtonAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_RESET_SD] = (ButtonSpec){BUTTON_RESET_SD, "reset SD", button_genericButtonAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_SETTINGS_MENU_2] = (ButtonSpec){BUTTON_SETTINGS_MENU_2, "more settings", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_INC_PENLIFT_UP] = (ButtonSpec){BUTTON_INC_PENLIFT_UP, "inc. penlift up", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_PENLIFT_UP] = (ButtonSpec){BUTTON_DEC_PENLIFT_UP, "dec. penlift up", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};

  buttons[BUTTON_INC_PENLIFT_DOWN] = (ButtonSpec){BUTTON_INC_PENLIFT_DOWN, "inc penlift down", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_DEC_PENLIFT_DOWN] = (ButtonSpec){BUTTON_DEC_PENLIFT_DOWN, "dec penlift down", button_genericButtonAction, 0, SHORT_TOUCH_RETRIGGER_DELAY};
  buttons[BUTTON_ADJUST_PENLIFT] = (ButtonSpec){BUTTON_ADJUST_PENLIFT, "adjust penlift", genericChangeMenuAction, 0, NEVER_RETRIGGER};
  buttons[BUTTON_PENLIFT_SAVE_TO_EEPROM] = (ButtonSpec){BUTTON_PENLIFT_SAVE_TO_EEPROM, "save to eeprom", button_genericButtonAction, 0, NEVER_RETRIGGER};
}

void button_setup_generateButtonCoords()
{
  buttonSize = (320 - (BUTTON_GAP*4)) / 3;
  grooveSize = 240 - (BUTTON_GAP*2) - (buttonSize*2);

  buttonCoords[0][0] = BUTTON_GAP;
  buttonCoords[0][1] = BUTTON_GAP;
  buttonCoords[1][0] = BUTTON_GAP+buttonSize;
  buttonCoords[1][1] = BUTTON_GAP+buttonSize;

  buttonCoords[2][0] = BUTTON_GAP+buttonSize+BUTTON_GAP;
  buttonCoords[2][1] = BUTTON_GAP;
  buttonCoords[3][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize;
  buttonCoords[3][1] = BUTTON_GAP+buttonSize;

  buttonCoords[4][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize+BUTTON_GAP;
  buttonCoords[4][1] = BUTTON_GAP;
  buttonCoords[5][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize;
  buttonCoords[5][1] = BUTTON_GAP+buttonSize;

  buttonCoords[6][0] = BUTTON_GAP;
  buttonCoords[6][1] = BUTTON_GAP+buttonSize+grooveSize;
  buttonCoords[7][0] = BUTTON_GAP+buttonSize;
  buttonCoords[7][1] = BUTTON_GAP+buttonSize+buttonSize+grooveSize;

  buttonCoords[8][0] = BUTTON_GAP+buttonSize+BUTTON_GAP;
  buttonCoords[8][1] = BUTTON_GAP+buttonSize+grooveSize;
  buttonCoords[9][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize;
  buttonCoords[9][1] = BUTTON_GAP+buttonSize+buttonSize+grooveSize;

  buttonCoords[10][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize+BUTTON_GAP;
  buttonCoords[10][1] = BUTTON_GAP+buttonSize+grooveSize;
  buttonCoords[11][0] = BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize+BUTTON_GAP+buttonSize;
  buttonCoords[11][1] = BUTTON_GAP+buttonSize+buttonSize+grooveSize;
};