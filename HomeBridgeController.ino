#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 201 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString; 

//////////////////////

void setup(){

  pinMode(5, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  server.begin();
  //enable serial data print 
  Serial.begin(9600); 
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //print to serial monitor for debuging 

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>Simple Arduino button</H1>");
          
          client.println("<a href=\"/?on\">ON</a>"); 
          client.println("<a href=\"/?off\">OFF</a>"); 

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();

          ///////////////////// control arduino pin
          if(readString.indexOf("on") != -1)//checks for on
          {
            Serial.println(readString.indexOf("on"));
            
            digitalWrite(5, HIGH);    // set pin 5 high
            Serial.println("Led On");

          }
          if(readString.indexOf("off") != 1)//checks for off
          {
            Serial.println(readString.indexOf("off"));
            digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 
