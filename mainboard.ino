/*Final update ver 4.97 
By Anindya Malik
NOTE that more stability of gps is required, hence some changes in the code will be done in the future.
NOTE the gps 3d fix gets lost every 4sec time delay of GNASS and GALLELLIO sats.
More about the problem can be found in the documentation section in the main webpage.*/
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <DHT.h>

// Set the pins for the GPS module
#define GPS_RX 2
#define GPS_TX 3

// Set the pins for the OLED display
#define OLED_SDA A4
#define OLED_SCL A5
#define OLED_RST 4

// Set the pins for the RTC module
#define RTC_SDA A4
#define RTC_SCL A5

// Set the pins for the DHT11 module
#define DHT_PIN 5

// Define the crash threshold value
#define CRASH_THRESHOLD 25 //adjust to your needs

// Initialize the GPS module using SoftwareSerial
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// Create a TinyGPS object
TinyGPSPlus gps;

// Initialize the OLED display
Adafruit_SSD1306 display(OLED_RST);

// Initialize the RTC module
RTC_DS1307 rtc;

// Initialize the DHT11 module
DHT dht(DHT_PIN, DHT11);


// Define the button pins
const int BUTTON1=6;
const int BUTTON2=7;
const int trigg_inp=4;

int buttonstate1=0;
int buttonstate2=0;

// Initialize the button states


void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Start GPS serial communication
  gpsSerial.begin(9600);

  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();

  // Initialize the RTC module
  Wire.begin();
  rtc.begin();

  // Initialize the DHT11 module
  dht.begin();
   
   // Set button pins to input
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(trigg_inp,INPUT_PULLUP);

   if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, resetting time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
    
  display.clearDisplay();
  display.setCursor(10,10);
  String message="ABWI HUD  ";
  for(int i=0;  i< message.length(); i++){
  display.setTextSize(2);    
  display.write(message[i]);
  display.display();
  
  }
  
  display.clearDisplay();
  display.setCursor(0,0);
  String m2="by Anindya Malik";
  for(int j=0;j<m2.length();j++){
    display.setTextSize(2);
    display.write(m2[j]);
    display.display();
  
}  

delay(2000);

display.clearDisplay();
display.setCursor(0,0);
display.setTextSize(1);
String m3="INTIALIZING GPS FIX.........";
for(int k=0;k<m3.length();k++){
  display.write(m3[k]);
  display.display();
  delay(10);
}
  delay(600);

  display.clearDisplay();
  display.display();  
}

void loop() {

  // Update the GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      break;
    }
  }
    


buttonstate1=digitalRead(BUTTON1);
buttonstate2=digitalRead(BUTTON2);
int trigger=digitalRead(trigg_inp);
Serial.print("trigger=");
Serial.print(trigger);
Serial.print("button1=");
Serial.print(buttonstate1);
Serial.print("Button2 ");
Serial.println(buttonstate2);
  // Update the button states
  
  
if(gps.speed.kmph()<0.02){  //Checking for the gps fix. Here the speed is converted and measured for relaiability and fast fix.
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Fixing Satellites");
  display.display();
}


else{    
  
  if (buttonstate1==LOW) {     // Check if button 1 is pressed
    // Display the time from the RTC module
    DateTime now = rtc.now();
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

  // Check if button 2 is pressed
 else if (buttonstate2==LOW) {
    // Display the temperature from the DHT11 module
    float temperature = dht.readTemperature();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Temperature:");
    display.setCursor(10,10);
    display.setTextSize(2);    
    display.print(temperature);
    display.print(" C");
    display.display();
  }

  else if(trigger==LOW) //to detect the crash if any changes nee to be done the variables should be changed at the slave arduino.
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("CRASH DETECTED");
    display.display();
    delay(5000);
    
  }

 
  else{                    // Display the GPS speed  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("GPS Speed");
  display.setCursor(90,0);
  display.print("SAT:");  
  display.print(gps.satellites.value());
  display.setCursor(14,12);
  display.setTextSize(2);  
  display.print(gps.speed.kmph());
  Serial.print("gps speed= ");
  Serial.println(gps.speed.kmph());
  display.print(" km/h");
  display.display();
  }
}
}

