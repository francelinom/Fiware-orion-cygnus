#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h> 
#include <ArduinoJson.h>

const char* wifiName     = "Bia";        
const char* wifiPass = "12345678"; 

const char* host= ".http://localhost:1026/v2/entities";

char idNode[80];
char nomeNode[12] = "Temperatura";
int cont = 2;

void setup() {

  Serial.begin(115200); 
  delay(10);
  
  Serial.print("Connecting to >>> ");
  Serial.print(wifiName); 
  
  WiFi.begin(wifiName, wifiPass); 
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  
  Serial.println("");
  Serial.println("Wi-Fi connected");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
  
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED){ 
    
//#####################################################################################################   
    //JSON
 
    sprintf(idNode, "urn:ngsi-ld:%s:%03i", nomeNode, cont++);

    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& root = JSONbuffer.createObject();

    root["id"] = idNode;
    root["type"] = "Temperatura";

    
    JsonObject& name = root.createNestedObject("name");
    name["type"] = "Text";
    name["value"] = "Temperatura";

    JsonObject& temp = root.createNestedObject("temp");
    temp["type"] = "Float";
    temp["value"] = "23.1";
    
    JsonObject& refSensor = root.createNestedObject("refSensor");
    refSensor["type"] = "Relationship";
    refSensor["value"] = "urn:ngsi-ld:Sensor:001";

    char JSONmessageBuffer[300];
    root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    
//#####################################################################################################
   
    HTTPClient http;
   
    Serial.print("Request Link:"); 
    Serial.println(host); 
   
    http.begin(host);   
    http.addHeader("Content-Type", "application/json"); 
    
    int httpCode = http.POST(JSONmessageBuffer); 
    String payload = http.getString(); 
    
    Serial.println(httpCode); 
    Serial.println(payload); 
    
    http.end(); 
    
  }else{
    
    Serial.println("Error in WiFi connection");
    
  }
 
  delay(30000);
 
}
