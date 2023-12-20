#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include "libCamera\OV7670.h"
#include "libCamera\BMP.h"

#define SSID  "Beeline_MF"  // "ZyXEL_KEENETIC_MF"
#define PASS  "$@ndr0nix"   // "zyxel-592-yqz"

// OV7670 pins connection
const int SIOD = 21; //SDA
const int SIOC = 22; //SCL
const int VSYNC = 34;
const int HREF = 35;
const int XCLK = 32;
const int PCLK = 33;
const int D0 = 27;
const int D1 = 17;
const int D2 = 16;
const int D3 = 15;
const int D4 = 14;
const int D5 = 13;
const int D6 = 12;
const int D7 = 4;

OV7670 *camera;

WiFiMulti wifiMulti;

WiFiServer server(80);

unsigned char bmpHeader[BMP::headerSize];

void serve()
{
  WiFiClient client = server.available();
  if (client) 
  {
    String currentLine = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        if (c == '\n') 
        {
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();                   //CHANGED 50% and auto margin
            client.print(
              "<style>body{margin: 0}\nimg{height: 50%; width: auto; margin-left: auto; margin-right: auto}</style>"
              "<img id='a' src='/camera' onload='this.style.display=\"initial\"; var b = document.getElementById(\"b\"); b.style.display=\"none\"; b.src=\"camera?\"+Date.now(); '>"
              "<img id='b' style='display: none' src='/camera' onload='this.style.display=\"initial\"; var a = document.getElementById(\"a\"); a.style.display=\"none\"; a.src=\"camera?\"+Date.now(); '>");
            client.println();
            break;
          } 
          else 
          {
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {
          currentLine += c;
        }
        
        if(currentLine.endsWith("GET /camera"))
        {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:image/bmp");
            client.println();
            
            for(int i = 0; i < BMP::headerSize; i++)
              client.write(bmpHeader[i]);
            for(int i = 0; i < camera->xres * camera->yres * 2; i++)
              client.write(camera->frame[i]);
        }
      }
    }
    client.stop();
  }  
}

void setup() 
{
  Serial.begin(115200);

  wifiMulti.addAP(SSID, PASS);
  Serial.println("Connecting Wifi...");

  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  }
  
  camera = new OV7670(OV7670::Mode::QQQVGA_RGB565, 
                      SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, 
                      D0, D1, D2, D3, D4, D5, D6, D7); //CHANGED
  BMP::construct16BitHeader(bmpHeader, camera->xres, camera->yres);
  
  server.begin();
}

void loop()
{
  camera->oneFrame();
  serve();
}
