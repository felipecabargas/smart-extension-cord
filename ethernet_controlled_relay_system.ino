#include <SPI.h>
#include <Ethernet.h>

#define RELAY1  6
#define RELAY2  7
#define RELAY3  8
#define RELAY4  9

//internet stuff
byte mac[] = {
  0xE8, 0x03, 0x9A, 0x2E, 0x4C, 0x2D
};
IPAddress ip(192, 168, 1, 178);
EthernetServer server(80);
String readString;

void setup()
{
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
}

void loop()
{
  EthernetClient client = server.available();
  if (client){
    Serial.println("new client");

    while (client.connected()){
      if (client.available()){
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100){

          readString += c;

          Serial.write(c);
          if (c == '\n') {
            Serial.println(readString); //print to serial monitor for debuging
            client.println("HTTP/1.1 200 OK"); //send new page
            client.println("Content-Type: text/html");
            client.println();
            client.println("<HTML>");
            client.println("<HEAD>");
            client.println("<link href='http://cabargas.com/css/foundation.min.css' rel='stylesheet' />");
            client.println("<link href='http://cabargas.com/css/styles.css' rel='stylesheet' />");
            client.println("<title>AFCM Automagico v1.0</title>");
            client.println("<center>");
            client.println("</HEAD>");
            client.println("<BODY>");
            client.println("<p class='title font-effect-anaglyph' style='font-family: Helvetica;'>Automagico</p>");
            client.println("<hr />");
            client.println("<center>");

            client.println("<p class='links font-effect-anaglyph'><span><a class='sites' href=\"/?lighton\"\">Relay 1 ON</a></span>");
            client.println("&nbsp;&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;&nbsp;");
            client.println("<a href=\"/?lightoff\"\">Relay 1 OFF</a></p>");
            client.println("<footer>");
            client.println("<p class='font-effect-anaglyph'>Made with &hearts; by Unicorns &amp; now in 3D!</p>");
            client.println("</footer>");
            client.println("</BODY>");
            client.println("</HTML>");

            delay(1);
            client.stop();

            if (readString.indexOf("?lighton") > 0){
              digitalWrite(RELAY1, LOW);          // Turns ON Relays 1
              Serial.println("Relay 1 ON");
            }
            else {
              if (readString.indexOf("?lightoff") > 0){
                digitalWrite(RELAY1, HIGH);       // Turns ON Relays 1
                Serial.println("Relay 2 OFF");
              }
            }

            readString = "";

            delay(1);
            client.stop();
            Serial.println("client disonnected");

          }
        }
      }
    }
  }
}
