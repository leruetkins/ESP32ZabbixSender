#include <ESP32ZabbixSender.h>

ESP32ZabbixSender zSender;

/* WiFi settings */
String ssid = "-----------";
String pass = "--------------";

/* Zabbix server setting */
#define SERVERADDR --------- // Zabbix server Address
#define ZABBIXPORT 10051			// Zabbix erver Port
#define ZABBIXAGHOST "-------"  // Zabbix item's host name

boolean checkConnection();

void setup() {
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	WiFi.begin(ssid.c_str(), pass.c_str());
	while (!checkConnection()) {
	}
	zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}

void loop() {
	static int counter1 = 1;
	static int counter2 = 1;
	checkConnection();							  // Check wifi connection
	zSender.ClearItem();						  // Clear ZabbixSender's item list
	zSender.AddItem("counter1", (float)counter1); // Exmaple value of zabbix trapper item
	zSender.AddItem("counter2", (float)counter2); // Exmaple value of zabbix trapper item
	if (zSender.Send() == EXIT_SUCCESS) {		  // Send zabbix items
		Serial.println("ZABBIX SEND: OK");
	} else {
		Serial.println("ZABBIX SEND: NG");
	}
	counter1 += 1;
	counter2 *= 2;
	delay(1000); // wait 1sec
}

boolean checkConnection() {
	int count = 0;
	Serial.print("Waiting for Wi-Fi connection");
	while (count < 300) {
		if (WiFi.status() == WL_CONNECTED) {
			Serial.println();
			Serial.println("Connected!");
			return (true);
		}
		delay(500);
		Serial.print(".");
		count++;
	}
	Serial.println("Timed out.");
	return false;
}
