#include <SD.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define CS_PIN D8


const char* ssid = "alpha";  // Enter SSID here
const char* password = "password";  //Enter Password here
ESP8266WebServer server(80);

String TemperatureOUT, TemperatureIN;
String HumidityOUT, HumidityIN;
String TextReceived;
String WaterTemp, DewPoint;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  WiFi.begin(ssid, password);
   while(!SD.begin(CS_PIN)) {
    Serial.println("Memory Not working");
    //programa encerrrado
//    return;
    delay(1000);
  }
   Serial.println("Memory init success");
  server.begin();
  
  Serial.println("HTTP server started");
  
}

void loop() {
  // put your main code here, to run repeatedly:
    server.handleClient();

}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(String Temperatureinstat,String Humidityinstat, String Temperatureoutstat,String Humidityoutstat, String Waterinstat, String DewPointstat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"3;url=http://192.168.43.2/\"/>";

  ptr +="<title>Vitamin Air Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 250px;} h1 {color: #008080;margin: 100px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #ff6767;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Vitamin Air Humidity & Temperature</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr += Temperatureinstat;
  ptr +="&#8451;</p>";
  ptr +="<p>Humidity: ";
  ptr += Humidityinstat;
  ptr +="%</p>";

  ptr +="<p>Dew Point or Vaporize Starting ";
  ptr += DewPointstat;
  ptr += "&#8451;</p>";
  ptr +="<p>Water Temperature ";
  ptr += Waterinstat;
  ptr += "&#8451;</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void handle_OnConnect() {
  TextReceived = Serial.readString();
  TemperatureOUT = TextReceived.substring(TextReceived.indexOf("outt")+5,TextReceived.indexOf("outt")+11);// Gets the values of the temperature
  HumidityOUT = TextReceived.substring(TextReceived.indexOf("outh")+5,TextReceived.indexOf("outh")+11);// Gets the values of the humidity 
  TemperatureIN = TextReceived.substring(TextReceived.indexOf("int")+4,TextReceived.indexOf("int")+9);
  HumidityIN = TextReceived.substring(TextReceived.indexOf("inh")+4,TextReceived.indexOf("inh")+9);
  WaterTemp = TextReceived.substring(TextReceived.indexOf("wint")+5,TextReceived.indexOf("wint")+9);
  
  DewPoint =  TextReceived.substring(TextReceived.indexOf("dp")+3,TextReceived.indexOf("dp")+9);
  server.send(200, "text/html", SendHTML(TemperatureIN,HumidityIN,TemperatureOUT,HumidityOUT,WaterTemp, DewPoint )); 
  
  Serial.print("temperature: ");
  Serial.println(TemperatureOUT);
  Serial.print("humidity: ");
  Serial.println(HumidityOUT);
  File dataFile = SD.open("LOG.txt", FILE_WRITE);
   if (dataFile) {
    Serial.println("O arquivo foi aberto com sucesso.");
      //formatação no arquivo: linha a linha >> UMIDADE | TEMPERATURA
      dataFile.print(HumidityOUT);
      dataFile.print(" | ");
      dataFile.println(TemperatureOUT);
      dataFile.print(" | ");
      dataFile.print(HumidityIN);
      dataFile.print(" | ");
      dataFile.println(TemperatureIN);
      dataFile.print(" | ");
      dataFile.print(WaterTemp);
      dataFile.print(" | ");
      dataFile.println(DewPoint);
      
      //fecha o arquivo após usá-lo
      dataFile.close();
     Serial.println("data writed");
  }
  else {
    Serial.println("Falha ao abrir o arquivo LOG.txt");
  }
//  delay(1000);
}
