enum {
  HS1 = 1,
  HS2 = 2
};

#define UP    HS1
#define DOWN  HS2
#define SET   (HS1 + HS2)
#define NONE  0

// Display anything that changed since last time
void display_buttons(unsigned buttons, const char * msg = "");

// Read the handset buttons
unsigned read_buttons();
unsigned read_buttons_debounce();
void clear_buttons();

// Control button io pins
void set_latch(unsigned latch_pins, unsigned long max_latch_time = 15000);
void break_latch();
bool is_latched();
unsigned get_latched();
