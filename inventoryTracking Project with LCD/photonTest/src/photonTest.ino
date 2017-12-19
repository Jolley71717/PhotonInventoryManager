
#include <Wire.h>
#include "application.h"
#include "HttpClient.h"

#include "Grove_LCD_RGB_Backlight.h"

//for the LCD
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// For the buttonPressed counting
int buttonPressed = 0;

// For the http request section
HttpClient http;
http_request_t request;
http_response_t response;

http_header_t headers[] = {
    { "Accept" , "*/*"},
  { NULL, NULL } // NOTE: Always terminate headers will NULL
};

void setup() {
    // For the LED light
    pinMode(D6, OUTPUT);
    // For the reading of the buttonPressed
    pinMode(D7, INPUT);

    // Sets up the IP address for the get request later
    request.ip= IPAddress(192,168,1,107);
    request.port = 8080;

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    // Print a message to the LCD.
    lcd.print("hello, world!");

    delay(1000);

    // Turns on the LED to say it's okay to press the button
    digitalWrite(D6,HIGH);
}

void getRequest() {

  String added_bit="&buttonPressed=YES";
  // path includes the url to input data into the phant server
  // the path is /input/[public-phant-key]?private_key=[private-phant-key]
  request.path = "/input/OA6BXgb8XmFxJGpq0p3XHrxQA2aL?private_key=M5lERd2mRGs2PL0qX06guePjB6D3"+ added_bit;

  //executes the request
   http.get(request, response, headers);
 }

void addButtonPress(){
  buttonPressed = buttonPressed + 1;
}


void loop() {
  while (digitalRead(D7) == HIGH) {
    if (digitalRead(D6) == HIGH) {

      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      // print the number of times the button was pressed
      addButtonPress();
      lcd.print(buttonPressed);

      delay(100);

      digitalWrite(D6, LOW);
      delay(100);
      digitalWrite(D6, HIGH);
      delay(100);
      digitalWrite(D6, LOW);
      delay(100);
      digitalWrite(D6, HIGH);
      delay(100);
      digitalWrite(D6, LOW);
      delay(100);
      digitalWrite(D6, HIGH);
      delay(100);
      digitalWrite(D6, LOW);
      delay(100);
      digitalWrite(D6, HIGH);
      delay(100);
      digitalWrite(D6, LOW);


      getRequest();
    }
    delay(100);
  }
  if(digitalRead(D6) == LOW){
        digitalWrite(D6, HIGH);
  }

}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
