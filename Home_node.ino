#include <SPI.h>
#include <LoRa.h>
#include <dht.h>
#define dht_apin A0

char did = 'a';
dht DHT;

//In order to showcase fucntioning of remote control we will be using LEDs
int D3 = 3;
int D4 = 4;
int D5 =5;

void loraActivateFn(){
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Thermostat");
  LoRa.setSPIFrequency(4E6);
  LoRa.setPins(10, 9, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (!LoRa.begin(433E6))//;
    {     Serial.println("Trying...");    }
  }
  LoRa.setTxPower(20);
}

void setup() {
  loraActivateFn(); //Lora activation function
  
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
}

void highled(){
          digitalWrite(D3, HIGH);
          digitalWrite(D4, HIGH);
          digitalWrite(D5, HIGH);
}

void lowled(){
          digitalWrite(D3, LOW);
          digitalWrite(D4, LOW);
          digitalWrite(D5, LOW);
}

void loop() {
    DHT.read11(dht_apin);
    int temp = DHT.temperature;
    int humid = DHT.humidity;
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humid);
    //Medium Temp
    if(temp >= 22  && temp <= 27 ){
          digitalWrite(D4, HIGH);
          digitalWrite(D3, HIGH);
          digitalWrite(D5, LOW);
    }
    //High Temp
    else if(temp > 27 && temp <= 30 ){ 
          digitalWrite(D3, HIGH); 
          digitalWrite(D4, HIGH);
          digitalWrite(D5, HIGH);
    }
    //Cool Temp
    else if(temp < 22 && temp > 7){
          digitalWrite(D3, HIGH);
          digitalWrite(D4, LOW);
          digitalWrite(D5, LOW); 
    }
    //emergency trigger
    else{
          //Send data to Emergency trigger(LoRa Device)
          highled();
          delay(800);
          LoRa.beginPacket();
          lowled();
          delay(800);
          Serial.println(DHT.humidity);
          highled();
          delay(800);
          LoRa.print(did);
          lowled();
          delay(800);
          LoRa.print(DHT.humidity);
          highled();
          delay(800);
          Serial.println(DHT.temperature);
          lowled();
          delay(800);
          LoRa.print(DHT.temperature);
          highled();
          delay(800);
          LoRa.print(did);
          lowled();
          delay(800);
          LoRa.endPacket();
          highled();
    }
    delay(10000);  
}
