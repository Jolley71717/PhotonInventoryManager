// This #include statement was automatically added by the Particle IDE.
#include <SparkJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

// This #include statement was automatically added by the Particle IDE.
#include <Grove_LCD_RGB_Backlight.h>


#include <Wire.h>
#include "application.h"
#include "HttpClient.h"
#include "Grove_LCD_RGB_Backlight.h"

//for the LCD
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// Integers that contain which item in the array you're looking at
int itemSelected=0;
int quantitySelected = 0;

char intString[40];
char finalPath[200];

//Loop controls for decision making
bool start = false;
bool itemWasSelected = false;
bool quantityWasSelected = false;
bool confirmationCheck = false;

int itemLibrarysz = 3;

// For the buttonPressed counting
int buttonPressed = 0;

// For the http request section
HttpClient http;
http_request_t request;
http_response_t response;

http_header_t headers[] = {
// { "Content-Type", "multipart/form-data" },
    { "Accept" , "*/*"},
// { "Phant-Private-Key", "M5lERd2mRGs2PL0qX06guePjB6D3"},
  { NULL, NULL } // NOTE: Always terminate headers will NULL
};

//May eventually turn this into an object, or use set index [?][2] to the existing quantity in the json
String Items[3][3] = {
   {"1", "Hose", "0"} ,   /*  initializers for row indexed by 0 */
   {"2", "Extinguisher", "0"} ,   /*  initializers for row indexed by 1 */
   {"3", "Helmet", "0"}   /*  initializers for row indexed by 2 */
};

void setup() {
    // Sets up the IP address for the get request later
    request.ip= IPAddress(192,168,1,141);
    // request.ip= IPAddress(192,168,1,107);
    request.port = 8080;

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    // Print a message to the LCD.
    //lcd.print("hello, world!");

    delay(1000);

    //Serial.begin(9600);
      lcd.begin(16, 2);
      lcd.setRGB(colorR, colorG, colorB);

    //Set up the listening pins for button input
    // For the reading of the Start/Select
    pinMode(D3, INPUT);
    // For the reading of the Up Button
    pinMode(D4, INPUT);
    // For the reading of the Down Button
    pinMode(D5, INPUT);
    // For the reading of the Cancel Button
    pinMode(D6, INPUT);

    // output each array element's value
   for ( int i = 0; i < 3; i++ ){
      for ( int j = 1; j < 2; j++ ) {

         //cout << "a[" << i << "][" << j << "]: ";
         String item = Items[i][j];
         lcd.setCursor(0, 0);
         lcd.print(item);// had errors in the past with converting body to string
         delay(1000);
         // Clear the screen.
        lcd.clear();
      }
   }
}


// pushes the inventory changes for the selected item
// Will need to look something like void postPhantPushRequest(Integer ItemID, String Description, Integer Quantity)
void postPhantPushRequest() {
    String inputPath = "/input/81OKvjmdqwtpEgOA6EMGu1Zyb1g?private_key=yBZ3q8RADXcGD8aAZD0BtKpnrKx";
    String itemIDString = "&ItemID=" + Items[itemSelected][0];
  String descriptionString = "&Description=" + Items[itemSelected][1];
  String quantityString = "&Quantity=";

    sprintf(finalPath, inputPath + itemIDString
                            + descriptionString + quantityString + "%d", quantitySelected);

    request.path = finalPath;



//the old path
//   //String added_bit="&buttonPressed=YES";
//   String itemIDString = "&ItemID=" + itemID; //may need to do a ".toString"
//   String descriptionString = "&Description=" + description;
//   String quantityString = "&Quantity=" + quantity;
//   request.path = "/input/ELY86vemlMHADYgo9DQzC3MeY30?private_key=VNPXEmG7wgUmKv9EGKxVCng0lnA"+ itemIDString + descriptionString + quantityString;

   http.get(request, response, headers);
   lcd.clear();
   lcd.setCursor(0, 1);
   lcd.print(response.status);// had errors in the past with converting body to string
 }

 //retrieves json from the phant server to populate the inventory options
 void postPhantPullRequest() {

  request.path = "/output/ELY86vemlMHADYgo9DQzC3MeY30.json";

   http.get(request, response, headers);

   //response.status to get the code
    //check for the code that means that everything was good
   //response.body to get the json
 }

void adQuantity(){
  quantitySelected = quantitySelected + 1;
}

void rmQuantity(){
  quantitySelected = quantitySelected - 1;
}

 void loop() {
  // not used in this example
  lcd.setCursor(0,0);
  lcd.print("Push To Begin");
  // need to add in about a second of load time so that it doesn't re read the same button push
  if(digitalRead(D3) == HIGH)
  {
    start = true;

    lcd.clear();
      lcd.setCursor(0, 0);
      //delay so you don't get a second select read
      lcd.print("Loading.");
      delay(180);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loading..");
      delay(180);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loading...");
      delay(180);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loading....");
      delay(180);
  }

  while(start){
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Item:");
      lcd.setCursor(0,1);
      lcd.print(Items[itemSelected][1]);
      // Up button was pressed
      if(digitalRead(D4) == HIGH){
          // itemSelected is a 0 based index and
          // itemLibrarysz is a one based size variable
          if(itemSelected < (itemLibrarysz - 1)){
            itemSelected ++;
            delay(100);
          }
      }
      //Down Button was pressed
      else if (digitalRead(D5) == HIGH){
          // Don't let them reduce below zero
          if(itemSelected > 0){
            itemSelected --;
            delay(100);
          }
      }
      // Select was pressed
      else if (digitalRead(D3) == HIGH){
          // Enter the quantity loop here
          itemWasSelected = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          //delay so you don't get a second select read
          lcd.print("Loading.");
          delay(180);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Loading..");
          delay(180);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Loading...");
          delay(180);


          while(itemWasSelected){
              delay(100);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Item:" + Items[itemSelected][1]);
            lcd.setCursor(0,1);
            sprintf(intString, "Quantity:%d", quantitySelected);
            lcd.print(intString);

                if(digitalRead(D4) == HIGH){
                    // itemSelected is a 0 based index and
                    // itemLibrarysz is a one based size variable
                    if(quantitySelected < 99)
                        adQuantity();
                        delay(100);
                }
                  // Down Button was pressed
                else if (digitalRead(D5) == HIGH){
                    // Don't let them reduce below zero
                    if(quantitySelected > 0)
                        rmQuantity();
                        delay(100);
                }
                // Select button was pressed
                // Enter confirmation loop
                else if (digitalRead(D3) == HIGH){
                    lcd.clear();
                      lcd.setCursor(0, 0);
                      //delay so you don't get a second select read
                      lcd.print("Loading.");
                      delay(180);
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Loading..");
                      delay(180);
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Loading...");
                      delay(180);
                    quantityWasSelected = true;
                    lcd.clear();
                    lcd.setCursor(0,0);
                    sprintf(intString, "I:" + Items[itemSelected][1] + " Qty:%d", quantitySelected);
                    lcd.print(intString);
                    lcd.setCursor(0,1);
                    lcd.print("Yes Or No?");
                    while(quantityWasSelected){
                        delay(100);
                        if (digitalRead(D3) == HIGH){
                            //submit to phant eventually, but for now just use particle publish

                            sprintf(intString, "ItemID:" + Items[itemSelected][0] + "Item:"
                            + Items[itemSelected][1] + " Quantity:%d", quantitySelected);

                            delay(1000);
                            lcd.clear();
                            bool success;
                            // success = Particle.publish("Inventory", intString,60,PRIVATE);
                            success = Particle.publish("Inventory","got To Here", 60, PRIVATE);
                            if (success) {
                            // get here if event publish did not work
                                lcd.print("success");
                            }
                            else{
                                lcd.print("failure");
                            }
                            lcd.setCursor(0,0);
                            delay(300);
                            lcd.clear();

                            postPhantPushRequest();

                            quantityWasSelected = false;
                            itemWasSelected = false;
                            quantitySelected = 0;
                            itemSelected = 0;
                            start = false;
                            delay(2000);
                            lcd.clear();
                        }
                        else if (digitalRead(D6) == HIGH)
                        {
                            // Reset the quantity because you are leaving the quantity loop
                            quantityWasSelected = false;
                            lcd.clear();
                        }
                    }

                }
                else if (digitalRead(D6) == HIGH)
                {
                    // Reset the quantity because you are leaving the quantity loop
                    itemWasSelected = false;
                    quantitySelected = 0;
                    lcd.clear();
                }

          }
      }
      // Cancel button was pressed
      else if (digitalRead(D6) == HIGH)
      {
          // Reset all the items because you are leaving the loop
        start = false;
        itemSelected = 0;
        quantitySelected =0;
        lcd.clear();
      }
  }
  delay(100);
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
