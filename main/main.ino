#include <Wire.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int red_led = 12;
int blue_led = 11;
int button = 2;

volatile int button_state;
volatile int last_button_state = HIGH; // Initialize to HIGH for pull-up
volatile unsigned long last_debounce_time = 0;
volatile unsigned long debounce_delay = 50; // Set the debounce delay in milliseconds
volatile int interrupt_count = 0;

void button_interrupt() {
  // Get the current time
  unsigned long current_time = millis();

  // Check if enough time has passed since the last debounce
  if (current_time - last_debounce_time > debounce_delay) {
    // Update the last debounce time
    last_debounce_time = current_time;

    // Read the button state
    button_state = digitalRead(button);

    // Check if the button state has changed
    if (button_state != last_button_state) {
      // Update the last button state
      last_button_state = button_state;

      // If the button is pressed (LOW), increment the interrupt count
      if (button_state == LOW) {
        digitalWrite(blue_led, HIGH);
        interrupt_count++;
      } else {
        digitalWrite(blue_led, LOW);
      }
    }
  }
}

//==========================================================================
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize the U8G2 library and set up the display
  u8g2.begin();

  // Set up pin modes
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  // Attach the button interrupt handler
  attachInterrupt(0, button_interrupt, CHANGE);
}

//==========================================================================

void oled() {
  // Display the interrupt count on the OLED screen
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_fewture_tf);
    u8g2.setCursor(10, 10);
    u8g2.print("interrupt");
    u8g2.setCursor(63, 40);
    u8g2.print(interrupt_count);

  } while (u8g2.nextPage());
}

//==========================================================================
void loop() {
  // Update and display the OLED screen
  oled();

  // Blink the red LED
  digitalWrite(red_led, HIGH);
  delay(250);
  digitalWrite(red_led, LOW);
  delay(250);
}
