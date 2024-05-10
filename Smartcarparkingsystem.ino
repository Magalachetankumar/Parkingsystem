#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo gateServo; // Servo motor object to control the gate
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD object with I2C address 0x27

int irSensor1Pin = 3; // IR sensor pin 1
int irSensor2Pin = 5; // IR sensor pin 2
int slotsLeft = 5; // Maximum slots fixed at 5

bool gateOpen = false; // Flag to track gate status
unsigned long entryTime = 0; // Timestamp for car entry

void setup() {
  gateServo.attach(6); // Attach the servo to pin 6
  pinMode(irSensor1Pin, INPUT); // Set IR sensor pin 1 as input
  pinMode(irSensor2Pin, INPUT); // Set IR sensor pin 2 as input
  
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  
  lcd.setCursor(0, 0);
  lcd.print(" Smart Parking ");
  lcd.setCursor(0, 1);
  lcd.print("Slots Left: ");
  lcd.print(slotsLeft);
}

void loop() {
  if (digitalRead(irSensor1Pin) == LOW && !gateOpen) {
    openGate(); // Open the gate if a car enters and gate is closed
    entryTime = millis(); // Record entry time
    gateOpen = true; // Set gateOpen flag
  }

  if (gateOpen && millis() - entryTime >= 2000) {
    closeGate(); // Close the gate after 2 seconds if gate is open
    gateOpen = false; // Reset gateOpen flag
    entryTime = 0; // Reset entry time
    slotsLeft--; // Decrement available slots
    updateLCD(); // Update LCD display
    delay(1000); // Delay for stability
  }

  if (digitalRead(irSensor2Pin) == LOW && slotsLeft < 5) {
    slotsLeft++; // Increment available slots if a car exits
    updateLCD(); // Update LCD display
    delay(1000); // Delay for stability
  }
}

void openGate() {
  gateServo.write(0);
   // Open the gate (adjust angle as needed)
  delay(1000);
   // Delay for gate opening time
}

void closeGate() {
  gateServo.write(90); // Close the gate (adjust angle as needed)
  delay(1000); // Delay for gate closing time
}

void updateLCD() {
  lcd.setCursor(11, 1);
  lcd.print("    "); // Clear previous slot count
  lcd.setCursor(11, 1);
  lcd.print(slotsLeft);
}