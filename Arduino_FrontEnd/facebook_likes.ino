/*
 Facebook LiveLike FrontEnd
 
 This sketch connects to Server using an Ethernet shield. It parses the XML
 returned, and looks for <fan_count>4</fan_count>
 
 You can use the Arduino Ethernet shield, or the Adafruit Ethernet shield, 
 either one will work, as long as it's got a Wiznet Ethernet module on board.
 
 This example uses the DHCP routines in the Ethernet library which is part of the 
 Arduino core from version 1.0 beta 1
 
 This example uses the String library, which is part of the Arduino core from
 version 0019.  

 This example uses a Display Shield to display de results of the call to facebook.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * LCD Display shield attached to pins 8, 9, 4, 5, 6, 7
 
 created in 20 Feb 2014
 by Neel Kadia
 
 Ethernet part based on 'Twitter Client' by Tom Igoe (public domain, http://arduino.cc/en/Tutorial/TwitterClient).
 
 This code is in the public domain.
 
 */ 

//ARDUINO 1.0+ ONLY
//ARDUINO 1.0+ ONLY

#include <Ethernet.h>
#include <SPI.h>
#include <LiquidCrystal.h>
////////////////////////////////////////////////////////////////////////
//CONFIGURE
////////////////////////////////////////////////////////////////////////
byte server[] = { 192,168,1,10 }; //ip Address of the server you will connect to
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//The location to go to on the server
//make sure to keep HTTP/1.0 at the end, this is telling it what type of file it is
String location = "/getdata HTTP/1.0";


// if need to change the MAC address (Very Rare)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
////////////////////////////////////////////////////////////////////////

EthernetClient client;

char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?

void setup(){
  Ethernet.begin(mac);
  Serial.begin(9600);
}

void loop(){
  lcd.begin(16, 2);  
  String pageValue = connectAndRead(); //connect to the server and read the output

  Serial.println(pageValue); //print out the findings.
  lcd.setCursor(0,0);
  lcd.print(pageValue);
  delay(2000); //wait 5 seconds before connecting again
}

String connectAndRead(){
  //connect to the server

  Serial.println("connecting...");

  //port 80 is typical of a www page
  if (client.connect(server, 8080)) {
    Serial.println("connected");
    client.print("GET ");
    client.println(location);
    client.println();

    //Connected - Read the page
    return readPage(); //go and read the output

  }else{
    return "connection failed";
  }

}

String readPage(){
  //read the page, and capture & return everything between '<' and '>'

  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory

  while(true){

    if (client.available()) {
      char c = client.read();

      if (c == '<' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }else if(startRead){

        if(c != '>'){ //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }else{
          //got what we need here! We can disconnect now
          startRead = false;
          client.stop();
          client.flush();
          Serial.println("disconnecting.");
          return inString;

        }

      }
    }

  }

}
