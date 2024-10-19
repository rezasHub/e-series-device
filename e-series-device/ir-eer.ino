
#include <EEPROM.h>

#include <SoftwareSerial.h>

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

SoftwareSerial mySerial(10, 11); // RX, TX D2, D3

// define the GPIO connected with Relays and switches
#define RelayPin1 4  //D4
#define RelayPin2 5  //D5
#define RelayPin3 6  //D6
#define RelayPin4 7  //D7




char bt_data; // variable for storing bluetooth data
int rf_data;



void relayOnOff(int relay){
 switch(relay){
      case 1:
            digitalWrite(RelayPin1, !digitalRead(RelayPin1)); // change state for relay-1
            EEPROM.update(0,digitalRead(RelayPin1));
            delay(100);
      break;
      case 2:
            digitalWrite(RelayPin2, !digitalRead(RelayPin2)); // change state for relay-2
            EEPROM.update(1,digitalRead(RelayPin2));
            delay(100);
      break;
      case 3:
            digitalWrite(RelayPin3, !digitalRead(RelayPin3)); // change state for relay-3
            EEPROM.update(2,digitalRead(RelayPin3));
            delay(100);
      break;
      case 4:
            digitalWrite(RelayPin4, !digitalRead(RelayPin4)); // change state for relay-4
            EEPROM.update(3,digitalRead(RelayPin4));
            delay(100);
      break;
      default : break;      
      }  
}

void eepromState()
{
  digitalWrite(RelayPin1, EEPROM.read(0)); delay(200);
  digitalWrite(RelayPin2, EEPROM.read(1)); delay(200);
  digitalWrite(RelayPin3, EEPROM.read(2)); delay(200);
  digitalWrite(RelayPin4, EEPROM.read(3)); delay(200);
}  

void bluetooth_control()
{
  if(mySerial.available()) {
    bt_data = mySerial.read();
    Serial.println(bt_data);
    switch(bt_data)
        {
          case 'A': digitalWrite(RelayPin1, LOW);  EEPROM.update(0,LOW); break; // if 'A' received Turn on Relay1
          case 'a': digitalWrite(RelayPin1, HIGH); EEPROM.update(0,HIGH); break; // if 'a' received Turn off Relay1
          case 'B': digitalWrite(RelayPin2, LOW);  EEPROM.update(1,LOW); break; // if 'B' received Turn on Relay2
          case 'b': digitalWrite(RelayPin2, HIGH); EEPROM.update(1,HIGH); break; // if 'b' received Turn off Relay2
          case 'C': digitalWrite(RelayPin3, LOW);  EEPROM.update(2,LOW); break; // if 'C' received Turn on Relay3
          case 'c': digitalWrite(RelayPin3, HIGH); EEPROM.update(2,HIGH); break; // if 'c' received Turn off Relay3
          case 'D': digitalWrite(RelayPin4, LOW);  EEPROM.update(3,LOW); break; // if 'D' received Turn on Relay4
          case 'd': digitalWrite(RelayPin4, HIGH); EEPROM.update(3,HIGH); break; // if 'd' received Turn off Relay4
          case 'Z': all_Switch_ON(); break;  // if 'Z' received Turn on all Relays
          case 'z': all_Switch_OFF(); break; // if 'z' received Turn off all Relays
          default : break;
        }
        delay(20);
  }
}




void all_Switch_ON(){
  digitalWrite(RelayPin1, LOW); EEPROM.update(0,LOW); delay(100);
  digitalWrite(RelayPin2, LOW); EEPROM.update(1,LOW); delay(100);
  digitalWrite(RelayPin3, LOW); EEPROM.update(2,LOW); delay(100);
  digitalWrite(RelayPin4, LOW); EEPROM.update(3,LOW); delay(100);
}

void all_Switch_OFF(){
  digitalWrite(RelayPin1, HIGH); EEPROM.update(0,HIGH); delay(100);
  digitalWrite(RelayPin2, HIGH); EEPROM.update(1,HIGH); delay(100);
  digitalWrite(RelayPin3, HIGH); EEPROM.update(2,HIGH); delay(100);
  digitalWrite(RelayPin4, HIGH); EEPROM.update(3,HIGH); delay(100);
}

void half_Switch_ON(){
  digitalWrite(RelayPin1, LOW); EEPROM.update(0,LOW); delay(100);
  digitalWrite(RelayPin2, LOW); EEPROM.update(1,LOW); delay(100);
 
}

void half_Switch_OFF(){
 digitalWrite(RelayPin3, LOW); EEPROM.update(2,LOW); delay(100);
  digitalWrite(RelayPin4, LOW); EEPROM.update(3,LOW); delay(100);
 
}
void rf_sw(){
   if (mySwitch.available()) {
      
    rf_data = mySwitch.getReceivedValue();
        Serial.println(rf_data);
   switch(rf_data){

          case 8120 : half_Switch_ON(); break; // if 'C' received Turn on Relay3
          case 8116 : half_Switch_OFF(); break; // if 'c' received Turn off Relay3
          case 8114 : all_Switch_OFF(); break; // if 'D' received Turn on Relay4
          case 8113 : delay(30000); all_Switch_OFF(); break; // if 'd' received Turn off Relay4
        
          default : break;}
  mySwitch.resetAvailable(); }
}

void rf_wall(){
   if (mySwitch.available()) {
      
    rf_data = mySwitch.getReceivedValue();
        Serial.println(rf_data);
   switch(rf_data){

          case XXXXXXX : half_Switch_ON(); break; // if 'C' received Turn on Relay3
          case XXXXXXX : half_Switch_OFF(); break; // if 'c' received Turn off Relay3
          case XXXXXXX : all_Switch_OFF(); break; // if 'D' received Turn on Relay4
          case XXXXXXX : delay(30000); all_Switch_OFF(); break; // if 'd' received Turn off Relay4
        
          default : break;}
  mySwitch.resetAvailable(); }
}


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);


  mySwitch.enableReceive(0); 
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);



  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);



  
  delay(500);
  eepromState();
}

void loop() {


  bluetooth_control();
  rf_sw();

}
