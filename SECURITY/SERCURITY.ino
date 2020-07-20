#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Keypad.h>

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

int failedAttempts;
String secret;
String password;

void setup()
{
  pinMode(13, OUTPUT); //LED Red
  pinMode(12, OUTPUT); //LED Green
//  lcd.begin(16, 2);
lcd.init();
lcd.backlight();
  password.reserve(4); // reduce the number of reallocations
  pinMode(A0, OUTPUT); //Contrast High and LCD Enabled
  failedAttempts = 0; // No of failed attempts
  secret = "1234"; //Password
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
}
void loop()
{
  char key = keypad.getKey();
  
  if(key){
    if(password.length() == 0) {
      lcd.clear();
    }
    password += key;
    lcd.print("*");
    if(password.length() == secret.length()) {
      if (secret.equals(password)) {
        lcd.clear();
        lcd.print("Access Granted");
        digitalWrite(12, HIGH);
        tone(12, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(12);     // Stop sound...
        failedAttempts = 0;
        delay(1000); // Time given to go in
      } else {
        lcd.clear();
        lcd.print("Access Denied");
        digitalWrite(13, HIGH);
        failedAttempts++;
        delay(500); //Time to read message
        delay(500*failedAttempts); //Waiting period before next try
      }
      lcd.clear();
      lcd.print("Enter Password");
      password = "";
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
    }
  }
}
