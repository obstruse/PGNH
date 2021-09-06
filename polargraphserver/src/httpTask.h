TaskHandle_t httpHandle = NULL;
int httpCore = 0;

#ifndef HTTP_H
#define HTTP_H
#include <WebServer.h>

WebServer server ( 80 );

//struct {
//  uint8_t red;
//  uint8_t green;
//  uint8_t blue;  
//} HTTP;
#endif

extern int commsReadCore, commsCommandCore;
extern int implLcdCore;
extern TaskHandle_t commsReadHandle, commsCommandHandle;
extern TaskHandle_t implLcdHandle;

extern boolean commandBuffered;
extern volatile int bufferPosition;
extern char nextCommand[];
extern char currentCommandRaw[];

extern int commsWhere;

//--------------------------------------------
void handleRoot() {

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
        
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.sendHeader("Content-Type","text/html",true);
  server.sendHeader("Cache-Control","no-cache");
  server.send(200);
   
/*-------------------------------------------*/
  sprintf ( temp,
"<html>\
  <head>\
    <title>PolarGraph</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>PolarGraph</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    ",
    hr, min % 60, sec % 60
  );
  server.sendContent ( temp );

//--------------------------------------------
  sprintf ( temp,
"\
<table border='1' cellpadding='5'>\
<tr><th colspan=2>Core</th><tr>\
<tr><th>HTTP</th><td>%d</td></tr>\
<tr><th>WIFI</th><td>%d</td></tr>\
<tr><th>COMMS Read</th><td>%d</td></tr>\
<tr><th>COMMS Command</th><td>%d</td></tr>\
<tr><th>IMPL LCD</th><td>%d</td></tr>\
</table>\
<br>",
   httpCore,
   wifiCore, 
   commsReadCore,
   commsCommandCore,
   implLcdCore
  );
  server.sendContent ( temp );

//--------------------------------------------
  sprintf ( temp,
"\
<table border='1' cellpadding='5'>\
<tr><th colspan=2>Stack</th><tr>\
<tr><th>HTTP</th><td>%d</td></tr>\
<tr><th>WIFI</th><td>%d</td></tr>\
<tr><th>COMMS Read</th><td>%d</td></tr>\
<tr><th>COMMS Command</th><td>%d</td></tr>\
<tr><th>IMPL LCD</th><td>%d</td></tr>\
</table>\
<br>",
    uxTaskGetStackHighWaterMark(httpHandle),
    uxTaskGetStackHighWaterMark(wifiHandle),
    uxTaskGetStackHighWaterMark(commsReadHandle),
    uxTaskGetStackHighWaterMark(commsCommandHandle),
    uxTaskGetStackHighWaterMark(implLcdHandle)
  );
  server.sendContent ( temp );

//--------------------------------------------
  sprintf ( temp,
"\
<table border='1' cellpadding='5'>\
<tr><th colspan=2>Command</th><tr>\
<tr><th>commandBuffered</th><td>%s</td></tr>\
<tr><th>bufferPosition</th><td>%d</td></tr>\
<tr><th>nextCommand</th><td>%s</td></tr>\
<tr><th>currentCommand</th><td>%s</td></tr>\
</table>\
<br>",
  String(commandBuffered).c_str(),
  bufferPosition,
  nextCommand, currentCommandRaw
  );

  server.sendContent ( temp );

//--------------------------------------------
  sprintf ( temp,
"<p>%s %s %s</p>\
<p>commsWhere: %d</p>\
</body></html>",
  fileName, __DATE__, __TIME__,
  commsWhere
  );
  server.sendContent ( temp );

  server.sendContent (" ");


}


//--------------------------------------------
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

//--------------------------------------------
// task
//--------------------------------------------
void http(void * pvParameters) {
  (void) pvParameters;
  
  for (;;) {
    server.handleClient();
    httpCore = xPortGetCoreID();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

//--------------------------------------------
//--------------------------------------------
void httpTaskCreate() {
  Serial.println("in http...");
  Serial.print("Created task: Executing on core ");
  Serial.println(xPortGetCoreID());
  
  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println("http started");
  
#ifndef HTTP_TASK
#define HTTP_TASK
  xTaskCreate( http, "HTTP", 5000, NULL, 1, &httpHandle );
#endif
}
