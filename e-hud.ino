/*Final iteration V3.0 code for the E HUD in ABWI HUD systems designed and developed by Anindya Malik.
Some of the major features like the bluetooth interfacing is there in the command but will not be connected to.
As the bluetooth requires seperate conversion so it is excluded in and not connected in the hardware,
further development in the software will allow the interfacing of the bluetooth easily.*/


#include <Wire.h> //This library is required to allow I2c devices to be interfaced
#include <Adafruit_GFX.h>//This library is needed for any additional draw function in the ssd1306 oled display.
#include <Adafruit_SSD1306.h>//This library is absoloutly essential to  drive the ssd1306 oled display. There are also alternatives 
#include <RTClib.h>//Library for the RTC module.
#include <SoftwareSerial.h>//This library is required for the bluetooth module.

RTC_DS3231 rtc; //Declearing an object named rtc for the Ds3231 RTC module

#define OLED_RESET 4 //Setting the default reset pin to 4 which will share the arduino reset pin.
Adafruit_SSD1306 display(OLED_RESET);

#define BUTTON1 2
#define BUTTON2 3 
// Define constants for two buttons connected to pins 2 and 3
// These buttons will be used to trigger different functions in the program

SoftwareSerial bluetooth(6, 7); // RX, TX 
// Initialize a SoftwareSerial object to communicate with a Bluetooth module

void setup() {
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT_PULLUP);
  // Set the pinMode for the two buttons to INPUT and INPUT_PULLUP respectively
  // This configures the Arduino to read the button states and detect when they are pressed

  Serial.begin(9600); // Begin serial communication with the computer at a baud rate of 9600
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize an Adafruit_SSD1306 object to control an OLED display connected to the Arduino
  display.clearDisplay(); // Clear the OLED display
  display.setTextColor(WHITE); //Set the display text color to white
  
  
  if (!rtc.begin()) { 
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // Check if the RTC module is present and print an error message if not
  // The RTC is used to keep track of time and date, and must be present for the program to function properly
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, resetting time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // Check if the RTC module lost power and reset the time if necessary
  // This ensures that the RTC has the correct time when the program starts
  
  bluetooth.begin(9600); // Begin serial communication with the Bluetooth module at a baud rate of 9600
   display.setTextSize(1);
  String message="Starting up....."; // Show a startup message on the OLED display
  for(int i=0;  i< message.length(); i++){
  display.write(message[i]);
  display.display();
  delay(10);
  }
  display.setCursor(0,10);
  String m2="Initialising Modules"; 
  for(int j=0; j<m2.length(); j++){
    display.write(m2[j]);
    display.display();
    delay(10);
  }
 display.setCursor(0,20);
 display.print("WELCOME");
 display.display();
 
  delay(2000);
  
  display.clearDisplay();
  display.display();
}

void loop() {
  DateTime now = rtc.now(); // Read the current date and time from the RTC module
  // This information is used to show the date and time on the OLED display
  
  if (digitalRead(BUTTON1) == HIGH) { // Check if button 1 is pressed and show the temperature reading on the OLED display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Temperature: ");
    display.setCursor(15,12);
    display.setTextSize(2);
    display.print(rtc.getTemperature());
    display.print(" C");
    display.display();
    delay(1000);
  }
  
   else if (digitalRead(BUTTON2) == LOW) { // Check if button 2 is pressed and read a message from the Bluetooth module
    String message = "";
    while (bluetooth.available()) {
      char c = bluetooth.read();
      Serial.println(bluetooth.read());
      message += c;
      delay(10);
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Message: ");
    display.setCursor(15,12);
    display.setTextSize(2);
    display.print(message);
    display.display();
    delay(1000);
  } 


  else {   // If neither button is pressed, show the current date and time on the OLED display
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(now.day(), DEC);
    display.print('/');
    display.print(now.month(), DEC);
    display.print('/');
    display.print(now.year(), DEC);
    display.setCursor(0, 16);
    display.print(now.hour(), DEC);
    display.print(':');
    display.print(now.minute(), DEC);
    display.print(':');
    display.print(now.second(), DEC);
    display.display();
    delay(1000);
  }
}
