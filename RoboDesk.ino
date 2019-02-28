#include "pins.h"
#include "LogicData.h"
#include "buttons.h"
#include <EEPROM.h>


// 7 pin din socket

//1: RxD
//2: HS3
//3: HS1 up
//4: HS4
//5: HS2 down
//6: TxD
//7: +5V
//shell: Ground

//#define DISABLE_LATCHING

#define LOWEST 65
#define HIGHEST 130

//Initial values for when we haven't saved a preset before
unsigned int HEIGHT_HIGH = HIGHEST;
unsigned int HEIGHT_LOW  = LOWEST;

unsigned long last_signal = 0;
unsigned long last_latch = 0;
unsigned int last_direction = -1;

LogicData ld(INTF_TX);

uint8_t height;
uint8_t target;

//-- Pass through mode sends input signal straight to output
inline void passThrough(uint8_t out, uint8_t in) {
  digitalWrite(out, digitalRead(in));
}

// Pass module TX straight through to interface RX
void dataPassThrough() {
  if (!ld.is_active()) passThrough(INTF_TX, MOD_TX);
}

unsigned gather_test = 0;
//-- Buffered mode parses input words and sends them to output separately
void dataGather() {
  digitalWrite(STATUS_LED, digitalRead(MOD_TX));
  ld.PinChange(HIGH == digitalRead(MOD_TX));
  gather_test++;
//  passThrough(5, MOD_TX);
}



uint32_t test_display_stream[] = {
    0x40600495,
    0x40600594,
    0x40600455,
    0x40600554,
    0x406004d4,
    0x406005d5,
    0x40600435,
    0x40600534,
    0x406004b4,
    0x406005b5,
    0x40600474,
    0x40600575,
    0x406004f5,
    0x406005f4,
    0x4060040c,
    0x4060050d,
    0x4060048d,
    0x4060058c,
    0x4060044d,
    0x4060054c,
    0x406004cc,
    0x406005cd,
    0x4060042d,
    0x4060052c,
    0x406004ac,
    0x406005ad,
    0x4060046c,
    0x4060056d,
    0x40601dfe,
    0x406004ed,
    0x40649dfb,
    0x406005ec,
    0x4060041d,
    0x4060051c,
    0x4060049c
};
uint32_t test_display_word[] = {
    // display word?
    0x40601dfe,
    0x40600515,
    0x40649dfb
};

uint32_t test_display_off[] = {
    // display OFF
    0x40601dfe,
    0x40600515,
    0x40649dfb,
    0x406e1400
};
uint32_t test_display_set[] = {
    // Enters "SET" mode; display commands have no effect until undone; Display: "S -"
    0x40641400,

    // Set 3 chosen; Display: "S 3"
    0x406c1d80,

    // "S 3" idle
    0x40601dfe,
    0x406c1d80,
    0x40649dfb,

    // ----------------------------------------
    // Display "120"
    0x40681400,
    0x4060043c,
    0x40649dfb,
    // ----------------------------------------
    // Display "120"
    0x40601dfe,
    0x4060043c,
    0x40649dfb,
    // ----------------------------------------
    // Display "120"
    0x40601dfe,
    0x4060043c,
    0x40649dfb,
    // ----------------------------------------
    // Display off
    0x406e1400
};

void setup() {
  
  pinMode(MOD_TX, INPUT);
//  digitalWrite(MOD_TX, HIGH); // turn on pullups (Needed for attiny85)

  pinMode(MOD_HS1, INPUT);
  pinMode(MOD_HS2, INPUT);
  pinMode(MOD_HS3, INPUT);
  pinMode(MOD_HS4, INPUT);

  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  dataGather();
  attachInterrupt(digitalPinToInterrupt(MOD_TX), dataGather, CHANGE);

  ld.Begin();
  delay(1000);

  //unsigned size = sizeof(test_display_stream) / sizeof(test_display_stream[0]);
  //ld.Send(test_display_stream, size);

  Serial.begin(9600);
  Serial.println("Robodesk v1.0  build: " __DATE__ " " __TIME__);
  readPresets(); 
  buzz(440, 100);
  buzz(440, 100);
}

void buzz(long frequency, long length) {
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(BUZZER,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(BUZZER,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}

// Record last time the display changed
void check_display() {
  static uint32_t prev = 0;
  uint32_t msg = ld.ReadTrace();
  if (msg) {
    char buf[80];
    uint32_t now = millis();
    sprintf(buf, "%6lums %s: %s", now - prev, ld.MsgType(msg), ld.Decode(msg));
    Serial.println(buf);
    prev=now;
  }

  // Reset idle-activity timer if display number changes or if any other display activity occurs (i.e. display-ON)
  if (ld.IsNumber(msg)) {
    static uint8_t prev_number;
    auto display_num = ld.GetNumber(msg);
    height = display_num;
    
    if (display_num == prev_number) {
      return;
    }
    prev_number = display_num;
  }
  if (msg)
    last_signal = millis();

}

void latch(unsigned latch_pins, unsigned long max_latch_time = 15000) {
  set_latch(latch_pins, max_latch_time);

  // Restart idle timer when we get an interesting button
  last_latch = last_signal = millis();
}

// Time out stale latches
void hold_latch() {
  digitalWrite(STATUS_LED, is_latched());

  if (!is_latched()) return;

  if(height != target) {
    return;
  } else {
    display_buttons(get_latched(), "Hit Target");
    break_latch();
    return;
  }

  unsigned long delta = millis() - last_signal;

  unsigned long activity_timeout = (last_signal - last_latch < 2000) ? 2500 : 1500 ;

  // Let go after 500ms with no signals
  if (delta > activity_timeout) {
    display_buttons(get_latched(), "Idle");
    break_latch();
    return;
  }
}

// Check the buttons
void check_actions() {
  // quick-read buttons
  auto buttons = read_buttons();

  if (buttons && is_latched() && buttons != get_latched()) {
    //TODO: This doesn't trigger correctly all the time
    break_latch();

    display_buttons(buttons, "Interrupted");

    if(buttons == UP || buttons == DOWN) {
      // Drain all button events from handset interface so the interruption doesn't become a command
      clear_buttons();

      return;
    }
  }

  // read and display button changes
  static unsigned prev = NONE;
  buttons = read_buttons_debounce();


  // Ignore unchanged state
  if (buttons == prev) return;
  prev = buttons;

  // If we're latched here, then buttons is same as latched or else buttons is NONE
  switch (buttons) {
    case UP:
      target = HEIGHT_HIGH;
      Serial.print("Moving to height: ");
      Serial.println(target);
      last_direction = UP;
      if (is_latched()) {
        latch(buttons, 42000);
      } else {
        latch(buttons, 500);
      }
      break;

    case DOWN:
      //If it's latched, go till target
      target = HEIGHT_LOW;
      Serial.print("Moving to height: ");
      Serial.println(target);      
      last_direction = DOWN;
      if (is_latched()) {
        latch(buttons, 42000);
      } else {
        latch(buttons, 500);
      }
      break;

    case MEM1:
    case MEM2:
    case MEM3:
      latch(buttons);
      break;

    case SET:
      writePresets();
      break;
    // Ignore all other buttons (NONE)
    default:
      return;
  }
}

void writePresets() {
  if (last_direction == UP) {  
    HEIGHT_HIGH = height;    
    EEPROM.write(0, height);
    Serial.print("Wrote high target: ");
    Serial.println(height);
    buzz(440, 300);
    buzz(493, 300);
    buzz(523, 300);
  }
  else if (last_direction == DOWN) {
    HEIGHT_LOW = height;
    EEPROM.write(100, height);
    Serial.print("Wrote low target: ");
    Serial.println(height);
    buzz(523, 300);
    buzz(493, 300);
    buzz(440, 300);
  }  
}

void readPresets() {
  HEIGHT_HIGH = EEPROM.read(0);
  HEIGHT_LOW = EEPROM.read(100);

  if (HEIGHT_HIGH > HIGHEST) HEIGHT_HIGH = HIGHEST;
  if (HEIGHT_LOW < LOWEST) HEIGHT_LOW = LOWEST;
  Serial.print("Read presets - high: ");
  Serial.print(HEIGHT_HIGH);
  Serial.print(", low: ");
  Serial.println(HEIGHT_LOW);
}


#ifdef DEBUG_QUEUE
void debug_queue() {
  static size_t prevQ = 0;
  index_t h, t;
  size_t Q = ld.QueueSize(h, t);
  if (Q != prevQ) {
    Serial.print("Q: t=");
    Serial.print(t);
    Serial.print(" h=");
    Serial.print(h);
    Serial.print(" size=");
    Serial.print(Q);
    Serial.print(" [");
    micros_t lead;
    size_t i=0;
    while (ld.q.peek(i++, &lead)) {
      Serial.print(lead);
      Serial.print(", ");
    }
    Serial.println("]");
    prevQ = Q;
  }
}
#endif

void demo() {
  pinMode(MOD_HS1, OUTPUT);
  pinMode(MOD_HS2, OUTPUT);

  digitalWrite(MOD_HS1, HIGH);
  delay(4000);
  digitalWrite(MOD_HS1, LOW);

  digitalWrite(MOD_HS2, HIGH);
  delay(4000);
  digitalWrite(MOD_HS2, LOW);

}

void loop() {
  // Monitor panel buttons for our commands and take over when we see one

  check_actions();
  check_display();
  hold_latch();


  //demo();

}
