#include <SoftwareSerial.h>
#include <dht.h>
SoftwareSerial mySerial(2, 3); // RX, TX
int ledPin = 5;
dht DHT;

#define DHT11_PIN 7

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  sendCommand("AT");
  sendCommand("AT+ROLE0");
  sendCommand("AT+NAMECupOGreen");
}

void sendCommand(const char * command){
  Serial.print("Command send :");
  Serial.println(command);
  mySerial.println(command);
  //wait some time
  delay(100);
  
  char reply[100];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("Reply end");
}

void light(char val){
  analogWrite(ledPin, val);
}

void readSerial(){
  char reply[50];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  if(strlen(reply) > 0){
    Serial.print(reply);
    Serial.println("We have just read some data");
    light(reply[0]);
    writeToBLE(reply[0]);
  }
}

void writeToBLE(char value) {
  Serial.print("Writing hex :");
  Serial.println(value, HEX);
  mySerial.write(value);
}

void send_data(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(1000);
}


void loop() {
  readSerial();
  int brightness=analogRead(A0);
  Serial.print("Brightness =")
  Serial.println(brightness);
  send_data();
}
