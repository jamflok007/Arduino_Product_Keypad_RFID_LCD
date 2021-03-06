#include <SPI.h>  
#include <MFRC522.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include <Keypad.h>
const byte rows = 4; //four rows
const byte columns = 4; //four columns

// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Initializing pins for keypad
byte row_pins[rows] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte column_pins[columns] = {3, 2, A1, A0}; //connect to the column pinouts of the keypad

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   

int *svid;
int card1[4];
//int led1 = A2;
//int led2 = A3;
#define led1 A2
#define led2 A3
int relay = 8;
int statu = 0;
int cnt =0;
byte test ;
//String tagUID = "29 B9 ED 23";
char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password
char password[4];
char key_pressed = 0; // Variable to store incoming keys
uint8_t j = 0;  // Variable used for counter
uint8_t RFIDMode = 0;

void setup() {
        
        pinMode(led1, OUTPUT);
        pinMode(led2, OUTPUT);
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        digitalWrite(relay, OUTPUT);
        lcd.begin();
        lcd.backlight();
    SPI.begin();        
    mfrc522.PCD_Init();   
     
        
}

void loop() {
      
if (RFIDMode == 0) {
      //j = 0;
      key_pressed = keypad_key.getKey(); // Storing keys

      lcd.clear();
      //lcd.print(" Door Lock ");
      lcd.setCursor(0,0);
      lcd.print("Enter Password : ");
      //lcd.setCursor(0,1);
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      
      if (key_pressed)
      {
        password[j++] = key_pressed; // Storing in password variable
        lcd.setCursor(0,1);
        lcd.print("*");
       
      }
      if (j == 4) // If 4 keys are completed
      {
        delay(200);
        if (!(strncmp(password, initial_password, 4))) // If password is matched
        {
          lcd.clear();
          lcd.print("Pass Accepted");
          
          analogWrite(led1, HIGH);
          analogWrite(led2, LOW);
          
          delay(3000);
          analogWrite(led1, LOW);
          analogWrite(led2, HIGH);
          
          lcd.clear();
          j = 0;
          RFIDMode = 1; // Make RFID = 1
          
        }
        else    // If password is not matched
        {
          lcd.clear();
          lcd.print("Wrong Password");

          analogWrite(led1, LOW);
          analogWrite(led2, HIGH);
          
          delay(3000);

          analogWrite(led1, HIGH);
          analogWrite(led2, LOW);
          lcd.clear();
          j = 0;
          RFIDMode = 0;
        }
      }
}
    if (RFIDMode == 1){  // Make RFID mode true
     
           
      lcd.setCursor(0, 0);
      lcd.print("   Door Lock");
      lcd.setCursor(0, 1);
      lcd.print(" Scan Your Tag ");

      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }

      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }

      //Reading from the card
      for (byte i = 0; i < mfrc522.uid.size; i++) {
            svid[i]= mfrc522.uid.uidByte[i];
    } 
           if(statu == 0)
           {
             lcd.clear();
             lcd.print("   Card UID:    ");
             lcd.setCursor(0,1);
             for (byte i = 0; i < mfrc522.uid.size; i++) {
               card1[i] = svid[i];
             lcd.print( card1[i], DEC);
             lcd.print( " ");
             statu =1;
            }
           delay(3000);
           lcd.clear();
           lcd.print(" Security Door ");
           lcd.setCursor(0,1);
           lcd.print("  For access  ");
           } 

           else{
            
           
             for (byte i = 0; i < mfrc522.uid.size; i++) {
               if(svid[i] == card1[i])
                cnt++;
             }
                            
            if(cnt == mfrc522.uid.size-1)
            {
              lcd.clear();
              lcd.print("     ACCESS     ");
              lcd.setCursor(0,1);
              lcd.print("    Welcome    ");
              digitalWrite(led1, HIGH);
              digitalWrite(led2, LOW);
              delay(2000);
              //RFIDMode = 2;
              j = 0;
              RFIDMode = 0;

               
             }
             else
             {
              lcd.clear();
              lcd.print("     ACCESS     ");
              lcd.setCursor(0,1);
              lcd.print("   Not correct   ");
              digitalWrite(led1, LOW);
              digitalWrite(led2, HIGH);
              delay(2000);
              j = 0;
              RFIDMode = 0;
             }
             
           }
           
           lcd.clear();
  cnt=0;
}
//          if (RFIDMode == 2){
//            digitalWrite(relay , LOW);
//            RFIDMode = 0;
//          }
//          else {
//            digitalWrite(relay , HIGH);
//            RFIDMode = 0;
//          }
}
