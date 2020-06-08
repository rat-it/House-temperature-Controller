#include <SPI.h>
#include <LoRa.h>
#include <dht.h>

dht DHT;
float temperature,humidity;

void loraActivateFn(){
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Emergency Hub : ");
  LoRa.setSPIFrequency(4E6);
  LoRa.setPins(10, 9, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (!LoRa.begin(433E6))
    {     Serial.println("Trying...");    }
  }
  LoRa.setTxPower(20);
}


void lupfn(){
  
  int packetSize = LoRa.parsePacket();
  char arr[60];
  char emergency;
  int humid, temperatur;
  if (packetSize){
    int i=0;
    while (LoRa.available()) { //Loop to store all incoming data in array
      arr[i]=(char)LoRa.read();
      i++;
    }
    
    String str = String(arr); //Converted to string
    //Using databases we can fetch the data and classify according to requirements
    if(str.charAt(0)=='a' && str.charAt(11)=='a'){ //Ideal packet checking from node A
      Serial.println("Full Packet Recevied");
      emergency = str.charAt(0);
      int temp = 1;
        while(str.charAt(temp)!='a'){ //Separation packet wise
          String tempStrH;
          char tempArr[5];
          //34.24a
          //56.00
          //Get Humidity From String
          for(int stringFiller = 0; stringFiller<5;stringFiller++){
            tempArr[stringFiller]=str.charAt(temp++);
          }
          
          tempStrH = tempArr; //Convert tempArr into string
          humid=tempStrH.toInt(); //String to int
          
          
          String tempStrT;
          char tempArr1[5];
          
          //Get Temperature From String
          for(int stringFiller = 0; stringFiller<5;stringFiller++){
            tempArr1[stringFiller]=str.charAt(temp++);
          }
          tempStrT = tempArr1; //Convert tempArr into string
          temperatur=tempStrT.toInt(); //String to int
      
        }
        Serial.print("Emergeny at node: ");
        Serial.println(emergency);
        Serial.print("Humidity there: ");
        Serial.println(humid);
        Serial.print("Temperature there: ");
        Serial.println(temperatur);
    }
  }
}

void setup() {
  loraActivateFn(); //Self defining fn
  }

void loop() {
  lupfn();
}
