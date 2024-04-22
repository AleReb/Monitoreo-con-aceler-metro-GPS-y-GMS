
/**************************************************************
 *
 * This sketch connects to a website and downloads a page.
 * It can be used to perform HTTP/RESTful API calls.
 *
 * For this example, you need to install ArduinoHttpClient library:
 *   https://github.com/arduino-libraries/ArduinoHttpClient
 *   or from http://librarymanager/all#ArduinoHttpClient
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 * For more HTTP API examples, see ArduinoHttpClient library
 *
 **************************************************************/

//////////////////////
String  test =""; // cadena para mandar datos
//////////////////////////////////////////
#include "MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);

////////////////////////////////////
#include <TinyGPS.h>
TinyGPS gps;
// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);
#define ss Serial2
 

///////////
// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266

// Increase RX buffer if needed
#define TINY_GSM_RX_BUFFER 512

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// Uncomment this if you want to see all AT commands
//#define DUMP_AT_COMMANDS

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial3 //en el mega el serial3 es 14 15


// Your GPRS credentials
// Leave empty, if missing user or pass

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "wap.tmovil.cl";
const char user[] = "wap";
const char pass[] = "wap";
// Server details
const char server[] = "api.pushingbox.com"; //servicio 1000 publicaciones  por dia
//const char server[] = "script.google.com";
//char resource[]=""; 
const int  port = 80;
//acelerometro
String value1= ""; 
String value2= ""; 
String value3= "";  
//giroscopio
String value4= "";
String value5= "";
String value6= "";
//magentrometro
String value7= "";
String value8= "";
String value9= "";
//temperatura
String value10= "";
String value11= "";
String value12= "";
//gps
String latitud= "";
String longitud= "";
//////////////////////////////
boolean conectado = false;
/////////////////////////////
#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
HttpClient http(client, server, port);
///////////////////////////////////////////////////////

void setup() {

  ss.begin(9600);
  IMU.begin();
  delay(10);
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(57600);
  delay(3000);

  SerialMon.println(F("Initializing modem..."));
  modem.restart();

  String modemInfo = modem.getModemInfo();
  SerialMon.print(F("Modem: "));
  SerialMon.println(modemInfo);

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}

void loop() {

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
     while (ss.available())
     {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
     }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    value11 += String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6); //lat
    value12 += String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);//long
  }
  
  gps.stats(&chars, &sentences, &failed);
  
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  
  String tag("mi_disposivo");
  
  IMU.readSensor();
  
  Serial.print(IMU.getAccelX_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(),6);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(),6);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(),6);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(),6);
  Serial.print("\t");
  Serial.print(IMU.getTemperature_C(),3);
  Serial.println("\t");
  Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  //Serial.println("\t");
  Serial.print("Lat y long :   ");
  Serial.println(value11);
  Serial.print("\t");
  Serial.print(value12);
  
  

  
  value1 += String(IMU.getAccelX_mss(),6);
  value2 += String(IMU.getAccelY_mss(),6);
  value3 += String(IMU.getAccelZ_mss(),6);
  value4 += String(IMU.getGyroX_rads(),6);
  value5 += String(IMU.getGyroY_rads(),6);
  value6 += String(IMU.getGyroZ_rads(),6);
  value7 += String(IMU.getMagX_uT(),6);
  value8 += String(IMU.getMagY_uT(),6);
  value9 += String(IMU.getMagZ_uT(),6);
  value10 += String(IMU.getTemperature_C(),3);
// value11 += String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6); //lat
// value12 += String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;//long
 
test = "/pushingbox?devid=vCC8791B4F809B06&tag2="+tag+"&value="+value1+"&value2="+value2+"&value3="+value3+"&value4="+value4+"&value5="+value5+"&value6="+value6+"&value7="+value7+"&value8="+value8+"&value9="+value9+"&value10="+value10+"&value11="+value11+"&value12="+value12;
 //test= "/macros/s/AKfycbxq7QJP2IIyyy96rX-ePgYXMe-k0TkywBNfXCxuHzvcYxHhn-OL/exec?tag=test&value=-1";
if(conectado == false){
    SerialMon.print(F("Waiting for network..."));
    if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);   
    return;
     }
 SerialMon.println(" OK");
  
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
   }
      
  }
 
  conectado = true;
  SerialMon.println("conectado");
  

  SerialMon.print(F("Performing HTTP GET request... "));
  int err = http.get(test);//se puede usar un string
  if (err != 0) {
    SerialMon.println(F("failed to connect"));
    conectado = false;
    delay(10000);
    return;
  }

  int status = http.responseStatusCode();
  SerialMon.println(status);
  SerialMon.println(status);
  if(status>200)
  {
    SerialMon.println("desconectando y reintentando....");
    delay(10000);
    http.stop();
  SerialMon.println(F("Server disconnected"));
  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));
  conectado = false;
  //status=200;
    }
  
  if (!status) {
    delay(10000);
    return;
  }

  while (http.headerAvailable()) {
    String headerName = http.readHeaderName();
    String headerValue = http.readHeaderValue();
    //SerialMon.println(headerName + " : " + headerValue);
  }

  int length = http.contentLength();
  if (length >= 0) {
    SerialMon.print(F("Content length is: "));
    SerialMon.println(length);
  }
  if (http.isResponseChunked()) {
    SerialMon.println(F("The response is chunked"));
  }
 
  String body = http.responseBody();
  SerialMon.println(F("Response:"));
  SerialMon.println(body);

  SerialMon.print(F("Body length is: "));
  SerialMon.println(body.length());
///////////////////////////////////////////////////////
  // Shutdown

 // http.stop();
 //SerialMon.println(F("Server disconnected"));
 // modem.gprsDisconnect();
 // SerialMon.println(F("GPRS disconnected"));
   

value1= ""; 
value2= ""; 
value3= "";  
//giroscopio
value4= "";
value5= "";
value6= "";
//magentrometro
value7= "";
value8= "";
value9= "";
value10="";
//for (unsigned long start = millis(); millis() - start < 5000;)
  //{
  value11= "";
  value12= "";
 // }
//value11= "";
//value12= "";

}
