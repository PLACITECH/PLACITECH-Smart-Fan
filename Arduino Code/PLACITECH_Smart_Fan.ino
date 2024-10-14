 /*  Author: PLACITECH
 *   
 *  YouTube: PLACITECH
 *  Instagram: @placitech_
 *  Tiktok: @placitech
 *   
 *  Use this code to create a Smart Fan that turns on
 *  whenever the temperature of your room gets too hot.
 *  This project was built using the Arduino Plug and Make Kit
 *  and the Modulinos that come with it!
 *  
 *  Have fun! :)
*/

//// LIBRARIES. Make sure to have them installed!
#include "Modulino.h"
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"


//// INSTANCES (Components we will control)

// Modulinos
ModulinoThermo thermo;
ModulinoBuzzer buzzer;
ModulinoPixels leds;

// Arduino UNO built-in LED Matrix
ArduinoLEDMatrix matrix;


//// PINS

// For this project, we'll only use one pin to control the relay that turns on/off the fan
#define relay 7


//// VARIABLES

int temperature; // Stores the current temperature (in degrees Celsius)

// The temperature threshold that triggers the fan (in degrees Celsius)
// Feel free to change this to what you find too hot!
int start_temp = 27; 

int freq = 1000; // Frequency of the buzzer sound (in Hertz)
int dur = 250; // Duration of the beep (in milliseconds)


//// FUNCTIONS

// Turn the LEDs red (used for alerting high temperature)
void leds_red(){
  for (int i = 0; i < 8; i++) {
    leds.set(i, RED, 25);
    leds.show();
  }
}

// Turn the LEDs green (used for signaling normal temperature)
void leds_green(){
  for (int i = 0; i < 8; i++) {
    leds.set(i, GREEN, 25);
    leds.show();
  }
}

// Get the room's temperature and show it on the Arduino UNO built-in LED Matrix
void get_temp(){
  temperature = thermo.getTemperature(); // Read the temperature from the sensor
  Serial.println("Temp = " + String(temperature) + "°C"); // Print it to the Serial Monitor

  // Display temperature starting from the 1st column and 1st row of the LED Matrix
  matrix.beginText(1, 1, 0xFFFFFF); 
  matrix.println(temperature);
  matrix.endText();
}



void setup(){
  // Start serial monitor
  Serial.begin(9600);

  // Set the relay pin as an output (to control the fan)
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);  // Ensure the fan starts off

  // Initialize all connected Modulinos and the built-in matrix
  Modulino.begin();
  thermo.begin();
  buzzer.begin();
  leds.begin();
  matrix.begin();

  // Set the font size for text displayed on the matrix
  matrix.textFont(Font_5x7);

  delay(100); // Small startup delay
}



void loop(){
  // Read temperature every 5 seconds until it reaches the threshold
  while (temperature < start_temp){
    delay(5000);
    get_temp();
  }
  
  // Alert the user that the threshold was reached using the Serial Monitor
  Serial.println();
  Serial.println("High temperature detected!");
  Serial.println("Turning on fan!");
  Serial.println();

  // Alert the user that threshold was reached using the Pixels (LEDs) and Buzzer Modulinos
  for (int i = 0; i < 3; i++){
    buzzer.tone(freq, dur); // Make a beep
    leds_red(); // Turn LEDs red
    delay(250);
    buzzer.tone(0, dur); // Stop the sound
    leds.clear(); // Turn off LEDs
    leds.show();
    delay(250);
  }

  // Turn on fan
  digitalWrite(relay, HIGH);

  // Continue monitoring temperature every 5 seconds
  // The fan will stay on until the temperature is 1 degree below the threshold
  // This avoids turning the fan on/off frequently due to minor fluctuations
  while (temperature >= start_temp - 1){
    delay(5000);
    get_temp();
  }


  // Alert the user that the temperature is OK using the Serial Monitor
  Serial.println();
  Serial.println("Temperature OK.");
  Serial.println("Turning off fan.");
  Serial.println();


  // Alert the user that the temperature is OK using the Pixels (LEDs) and Buzzer Modulinos
  for (int i = 0; i < 3; i++){
    buzzer.tone(freq, dur);
    leds_green();
    delay(250);
    buzzer.tone(0, dur);
    leds.clear();
    leds.show();
    delay(250);
  }

  // Turn off fan
  digitalWrite(relay, LOW);
}