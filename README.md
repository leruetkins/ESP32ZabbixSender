# ESP32ZabbixSender
Library to realize the zabbix-sender on ESP32-Arduino

# How to install this library to Arduino IDE
## Use git command
(Windows and default arduino setting)

    cd %USERPROFILE%\Documents\Arduino\libraries
    git clone https://github.com/leruetkins/ESP32ZabbixSender.git
## Use ZIP import function of the Arduino IDE
Google it

# Usage
See `sample_ESP32ZabbixSender/sample_ESP32ZabbixSender.ino`

    ESP32ZabbixSender zSender;
    zSender.Init(IPAddress(192, 168, 35, 14), 10051, "IOTBOARD_00"); // Init zabbix server address, port, and hostname of item
    zSender.ClearItem(); // clear item list
    zSender.AddItem("air.temp", 29.62); // add item such as air temperture
    zSender.AddItem("air.hum", 70.60); // add item such as air humidity
    if (zSender.Send() == EXIT_FAILURE){ // send packet
        // error handling
    }

# Based on:  
https://github.com/zaphodus/ESP8266ZabbixSender

# Improvements done:
This library now sends Strings
```cpp
void ESP32ZabbixSender::AddItemString(String key, String value) {
	zabbixItemListString[zabbixItemSizeString].key = key;
	zabbixItemListString[zabbixItemSizeString].val = value;
	zabbixItemSizeString++;
}```

```cpp
int ESP32ZabbixSender::createZabbixPacketString(void) { // [private] create ZabbixPacket
	int packetLen = 0;
	char s[16];
	String Json = "{\"request\":\"sender data\",\"data\":[";
	for (int i = 0; i < zabbixItemSizeString; i++) {
		if (i > 0) {
			Json += ",";
		}
		Json += "{\"host\":\"" + zItemHost + "\",\"key\":\"" + zabbixItemListString[i].key + "\",\"value\":\"" + zabbixItemListString[i].val + "\"}";
	}
	Json += "]}";

	for (int i = 0; i < ZABBIXMAXLEN; i++) {
		zabbixPacket[i] = 0;
	}
	zabbixPacket[0] = 'Z';
	zabbixPacket[1] = 'B';
	zabbixPacket[2] = 'X';
	zabbixPacket[3] = 'D';
	zabbixPacket[4] = 0x01;
	uint16_t JsonLen = Json.length();
	uint16_t remLen = JsonLen;
	for (int i = 0; i < 8; i++) {
		zabbixPacket[5 + i] = (remLen % 256);
		remLen = (uint16_t)remLen / 256;
	}
	Json.getBytes(&(zabbixPacket[13]), ZABBIXMAXLEN - 12);
	packetLen = 13 + JsonLen;
#ifndef SILENT
	Serial.print("request = ");
	for (int i = 0; i < packetLen; i++) {
		Serial.print((char)(zabbixPacket[i]));
	}
	Serial.println();
#endif
	return packetLen;
}
```

# Bug fixing:
This library had a function that when the client was available it sends the package, and when it doesn't it freezes. That is fixed.