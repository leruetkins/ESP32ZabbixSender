#include "ESP32ZabbixSender.h"

ESP32ZabbixSender::ESP32ZabbixSender() {}

void ESP32ZabbixSender::Init(IPAddress ZabbixServerAddr, uint16_t ZabbixServerPort, String ZabbixItemHostName) {
	zAddr = ZabbixServerAddr;
	zPort = ZabbixServerPort;
	zItemHost = ZabbixItemHostName;
}

int ESP32ZabbixSender::Send(void) { // Send items
	int retValue = EXIT_FAILURE;
	int packetLen = createZabbixPacket();
	if (zClient.connect(zAddr, zPort)) {
		zClient.write(&zabbixPacket[0], packetLen);
		for (int i = 0; i < ZABBIXTIMEOUT / 10; i++) {
			if (zClient.available()) {
#ifndef SILENT
				Serial.print("result = ");
				while (zClient.available()) {
					Serial.print(zClient.readString());
				}
				Serial.println();
#endif
				retValue = EXIT_SUCCESS;
				break;
			}
			delay(10);
		}
	}
	
	if (retValue != EXIT_SUCCESS) {
#ifndef SILENT
		Serial.println("No result");
#endif
	}
	return retValue;
	zClient.stop();
}

int ESP32ZabbixSender::SendString(void) { // Send items
	int retValue = EXIT_FAILURE;
	int packetLen = createZabbixPacketString();
	if (zClient.connect(zAddr, zPort)) {
		zClient.write(&zabbixPacket[0], packetLen);
		for (int i = 0; i < ZABBIXTIMEOUT / 10; i++) {
			if (zClient.available()) {
#ifndef SILENT
				Serial.print("result = ");
				while (zClient.available()) {
					Serial.print(zClient.readString());
				}
				Serial.println();
#endif
				retValue = EXIT_SUCCESS;
				break;
			}
			delay(10);
		}
	}
	
	if (retValue != EXIT_SUCCESS) {
#ifndef SILENT
		Serial.println("No result");
#endif
	}
	return retValue;
	zClient.stop();
}

void ESP32ZabbixSender::ClearItem(void) { // Clear item list
	zabbixItemSize = 0;
}

void ESP32ZabbixSender::ClearItemString(void) { // Clear item list
	zabbixItemSizeString = 0;
}

void ESP32ZabbixSender::AddItem(String key, float value) {
	zabbixItemList[zabbixItemSize].key = key;
	zabbixItemList[zabbixItemSize].val = value;
	zabbixItemSize++;
}

void ESP32ZabbixSender::AddItemString(String key, String value) {
	zabbixItemListString[zabbixItemSizeString].key = key;
	zabbixItemListString[zabbixItemSizeString].val = value;
	zabbixItemSizeString++;
}

int ESP32ZabbixSender::createZabbixPacket(void) { // [private] create ZabbixPacket
	int packetLen = 0;
	char s[16];
	String Json = "{\"request\":\"sender data\",\"data\":[";
	for (int i = 0; i < zabbixItemSize; i++) {
		if (i > 0) {
			Json += ",";
		}
		Json += "{\"host\":\"" + zItemHost + "\",\"key\":\"" + zabbixItemList[i].key + "\",\"value\":\"" + zabbixItemList[i].val + "\"}";
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