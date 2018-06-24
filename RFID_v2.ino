/* Start of Code 
 This is code for an escape room project using the RC-522 RFID module and a 16x2 LCD with I2c backpack.

*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <MFRC522.h> // Include of the RC522 Library
#include <SPI.h> // Used for communication via SPI with the Module


#define SDAPIN 10 // RFID Module SDA Pin is connected to the UNO 10 Pin
#define RESETPIN 8 // RFID Module RST Pin is connected to the UNO 8 Pin

char code[5] = ("5742"); // This is the code you want for the padlock
byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number
byte GoodTagSerialNumber[5] = {0x94, 0xD3, 0xF3, 0x1E,}; // If the card is need to be change do here putting 0x  infront of the hex value 

MFRC522 nfc(SDAPIN, RESETPIN); // Init of the library using the UNO pins declared above
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.clear();

  SPI.begin();
  Serial.begin(115200);


  
  // Start to find an RFID Module
  Serial.println("Looking for RFID Reader");
  nfc.begin();
  byte version = nfc.getFirmwareVersion(); // Variable to store Firmware version of the Module
  
  // If can't find an RFID Module 
  if (! version) { 
    Serial.print("Didn't find RC522 board.");
    while(1); //Wait until a RFID Module is found
  }
  
  // If found, print the information about the RFID Module
  Serial.print("Found chip RC522 ");
  Serial.print("Firmware version: 0x");
  Serial.println(version, HEX);
  Serial.println();
  }

void loop() {
  
  lcd.setCursor(0,0);
  lcd.print("Please swipe");
  lcd.setCursor(0,1);
  lcd.print("a lobster card");
  
  
  String GoodTag="False"; // Variable used to confirm good Tag Detected
  
  // Check to see if a Tag was detected
  // If yes, then the variable FoundTag will contain "MI_OK"
  FoundTag = nfc.requestTag(MF1_REQIDL, TagData);
 
  if (FoundTag == MI_OK) {
    delay(200);
    
    // Get anti-collision value to properly read information from the Tag
    ReadTag = nfc.antiCollision(TagData);
    memcpy(TagSerialNumber, TagData, 4); // Write the Tag information in the TagSerialNumber variable
    
    Serial.println("Tag detected.");
    Serial.print("Serial Number: ");
  for (int i = 0; i < 4; i++) { // Loop to print serial number to serial monitor
    Serial.print(TagSerialNumber[i], HEX);
    Serial.print(", ");
  }

  
  
  // Check if detected Tag has the right Serial number we are looking for 
  for(int i=0; i < 4; i++){
    if (GoodTagSerialNumber[i] != TagSerialNumber[i]) {
    break; // if not equal, then break out of the "for" loop
  }
  if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
    GoodTag="TRUE";
    } 
  }
  if (GoodTag == "TRUE"){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your code is");
    lcd.setCursor(0,1);
    lcd.print(code);
    delay(3000);
    
  
  }
  else {
    Serial.println("TAG NOT ACCEPTED");
    Serial.println();

  
    }
  }
  lcd.clear(); 
}

/* End of Code */
