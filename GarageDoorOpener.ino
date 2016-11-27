/*
 Garage Door Opener
  
 Based on WebServer example

 This detects a specific url and toggles a relay on and off to pulse a garage door button
 
 To just load the webpage:
 http://your-IP-address
 To pulse the opener
 http://your-IP-address/?1
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Connect a relay board or a transistor to drive a relay to pin D2. If you don't use a relay board, you need to have a kickback diode across the relay coil 
 * example board: http://www.ebay.com/itm/181219706599?_trksid=p2059210.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT
*/

#include <WString.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

#define DOOR_BUTTON 2

String readString = String(100); //string for fetching data from address

void setup()
{
  Serial.begin(9600);

  digitalWrite(DOOR_BUTTON, LOW);
  pinMode(DOOR_BUTTON, OUTPUT);

  // start the Ethernet connection and the server:

  Serial.println("Initiaizing ethernet...");
  
  // this uses a fixed address
  Ethernet.begin(mac, ip);
  
  // get an address with DHCP
  //if (Ethernet.begin(mac) == 0)
  //  Serial.println("Failed to configure Ethernet using DHCP");
  
  // give the card a second to initialize 
  delay(1000);

  server.begin();

  Serial.print("Garage Door Opener Control Ready at IP address ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // command received (one character)  '1' - activate garage door button
  char cmd = 0;          // 1 - pulse button
  boolean done = false;  // set to indicate that response is complete
   
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    
    Serial.println("new client");
    readString = "";
  
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //int i = c;
        //Serial.print("(");
        //Serial.print(i);
        //Serial.print(")");

        // store character received in receive string
        if (readString.length() < 100) {
          readString += (c);
        }
        
        // check for end of line
        if (c == '\n') {
            //Serial.print("Receved line: ");
            //Serial.print(readString);  
          
            // process line if its the "GET" request
            // a request looks like "GET /?1" or "GET /?2"
            if (readString.indexOf("GET") != -1) {
                if (readString.indexOf("?1") != -1)
                   cmd = '1';
                // check for other commands here. ie turn on light, etc.
                //if (readString.indexOf("?2") != -1)
                //   cmd = '2';   
            }              
 
           // if a blank line was received (just cr lf, length of 2), then its the end of the request
           if (readString.length() == 2) {
             if (cmd == '1'){
                Serial.println("Activate Button");
                digitalWrite(DOOR_BUTTON, HIGH);
                delay(1000);
                digitalWrite(DOOR_BUTTON, LOW);
             }
             // add other commands here
        
             // send web page back to client 
             Serial.println("sending web page");
             SendWebPage(client); 
             Serial.println("web page sent");
             
             cmd = 0;
             
             // break out and disconnect. This will tell the browser the request is complete without having to specify content-length
             break;
             
           }  // end of request reached

           // start line over            
           readString = "";
        }  // end of line reached
      }  // end data is available from client
    }  // end cient is connected
    // give the web browser time to receive the data
    Serial.println("delay before disconnect");
    delay(100);
    // close the connection:
    client.stop();
    Serial.println("client disonnected"); 
  }  // end client has been created
}

// send web page
void SendWebPage(EthernetClient client)
{
   client.println("HTTP/1.1 200 OK"); 
   client.println("Content-Type: text/html"); 
   // to specify the length, wooul have to construct the entire string and then get its length
   //client.println("Content-Length: 1234"); 
   client.println("Connnection: close"); 
   client.println(); 
   
   client.println("<!DOCTYPE HTML>");
   client.println("<html>");
   client.println("<head>");
   client.println("<title>Garage Door Control</title>");

   client.println("<style type='text/css'>");
   client.println(".label {font-size: 40px; text-align:center;}");
   client.println("button {width: 200px; height: 100px; font-size: 40px; -webkit-appearance: none; background-color:white; }");
   client.println("</style>");
   
   client.println("<script type='text/javascript'>");

   client.println("function OnButtonClicked(parm) { window.location.href=\"X?\" + parm; }");

   client.println("</script>");
 
   client.println("</head>");

   client.println("<body style=\"background-color:orange\">");

   client.println("<div class=\"label\">");
   
   client.println("Garage Door Control<br/><br/>");
   
   // future idea: could read a limit switch on the garage door here and tell the user if the door is currently open or closed
   /*
   if (digitalRead(DOOR_OPEN_INPUT) == HIGH)
     client.println("Door is Open"); 
   else  
     client.println("Door is Closed"); 
   client.println("<br>");
   */

   // door open / close button
   client.println("<button onclick=\"OnButtonClicked('1');\">Activate</button><br/>");
   
   // add more buttons here
   // button separator
   //client.println("<div style=\"height:20px\"></div>");
   //client.println("<button onclick=\"OnButtonClicked('2');\">Off</button>");

   client.println("</div>");

   client.println("</body>");
   client.println("</html>");

   client.println("");
}
