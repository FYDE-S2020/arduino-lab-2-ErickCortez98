/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/*VO - Button
 * V1 - Slider
 * V2 - Terminal
 */
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED_PIN 2

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "FH8W2r0mgivm64LfDVMoFlUHqXcYy-cK";
const int freq = 5000;     // 5KHz frequency is more than enough. Remember we used 100 before.
const int ledChannel = 0;  // You can have up to 16 pwm channels (0 to 15)
const int resolution = 10; // 10 bit resolution for 1023. Freq should be greater than resolution.

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "Ball-Park-West";
char pass[32] = "ballparkwest1600";
int val = 0;
int val2 = 1023; 
int active = 0;
int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )
BlynkTimer timer;

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_PIN, OUTPUT);
  // configure LED PWM functionality
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(LED_PIN, ledChannel);
  // Setup a function to be called every second
  timer.setInterval(10L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  ledcWrite(ledChannel, val);
}
BLYNK_WRITE(1)
{ 
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    val2 = param.asInt(); // assigning incoming value from pin V1 to a variable
    //LED changes brigthness only if button has made LED be on, otherwise LED doesn't turn on with changed brightness
    if(active == 1){
      val = val2;
     }else{
      val = 0;
      }

}
BLYNK_WRITE(V0)
{
    Serial.println("Inside Button Function");
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V0 is a button, pinValue will be a 0 or a 1.
    // Led turns on and off when pressing and releasing the button 
    if (pinValue == 1) {
       // turn LED off
       if(active == 0){
        val = val2;
        active = 1;
        }else{
          val = 0;
          active = 0;
        }
    }
}

// This function sends Arduino's up time every second to Virtual Pin 0.
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
// You can send any value at any time.
// Don't send more than 10 values a second or the Blynk server will block you!
    if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        Blynk.virtualWrite(V2, millis() / 1000); // Write the arduino uptime every second
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V2, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}
