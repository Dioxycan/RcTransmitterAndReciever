/*
        DIY Arduino based RC Transmitter
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>







RF24 radio(6, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;

};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  
  // Initialize interface to the MPU6050

  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  
  // Define the radio communication
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

  
  // Set initial default values
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 127;

}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(A0), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A1), 0, 1023, 0, 255);


  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
