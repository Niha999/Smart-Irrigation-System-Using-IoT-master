#include<SoftwareSerial.h>
int srdata;
int prdata;
int pump_status;
SoftwareSerial esp8266(3, 4);
#define SSID "Pradyumna"   // Wifi username 
#define PASS "PradyumnaDanigond"   // Wifi password

//Function To Send AT Commands To ESP8266


String sendAT(String command, const int timeout)
{
  String response = "";
  esp8266.print(command);
  long int  time = millis();
  while (( time + timeout ) > millis())
  {

    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;

    }

  }
  Serial.print(response);
  return response;

}

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);  // 115200   esp8266
  sendAT("AT+RST\r\n", 2000);
  sendAT("AT\r\n", 1000);
  sendAT("AT+CWMODE=1\r\n", 1000);
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n", 2000);
  while (!esp8266.find("OK"))
  {

  }
  sendAT ("AT+CIFSR\r\n", 1000);
  sendAT("AT+CIPMUX=0\r\n", 1000);
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  srdata = analogRead(A0);
  prdata = map(srdata, 0, 1023, 100, 0);
  Serial.print("Sensor Data: ");
  Serial.println(prdata);
  String sensor_value = String(prdata);
  if (prdata < 0)
  {
    digitalWrite(8, LOW);
    pump_status = 100;
  }
  else
  {
    digitalWrite(8, HIGH);
    pump_status = 0;
  }
  String pump = String(pump_status);
  updateTS(sensor_value, pump);
  delay(2000);
}

void updateTS (String T, String P)
{
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 1000);
 // api.thingspeak.com
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=29QITJH2SS8LZ0P8&field1="+T+"&field2="+P+"\r\n";
  //New API Write Key. Make sure to update it with Jyoti's API Write Key
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n", 2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n", 2000);
  Serial.println("");
  delay(15000);
}
//integrate the ppunp testing progrsm with this oonne. Also make sure to try this program individually. Might work this way too.

