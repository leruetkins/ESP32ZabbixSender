#ifndef _ESP32ZabbixSender_H_
#define _ESP32ZabbixSender_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EthernetENC.h>
#define ZABBIXMAXLEN 300 // max 256byte packet
#define ZABBIXMAXITEM 50 // max 50 items
#define ZABBIXTIMEOUT 1000 // ms

class ESP32ZabbixSender {
public:
	ESP32ZabbixSender(void);
	void Init(IPAddress ZabbixServerAddr, uint16_t ZabbixServerPort, String ZabbixItemHostName);
	int Send(void);
	int SendString(void);
	void ClearItem(void);
	void ClearItemString(void);
	void AddItem(String key, float value);
	void AddItemString(String key, String value);

private:
	int createZabbixPacket(void);
	int createZabbixPacketString(void);
	struct zabbixCell {
		String key;
		float val;
	};
	struct zabbixCellString {
		String key;
		String val;
	};
	EthernetClient zClient;
	IPAddress zAddr;
	uint16_t zPort;
	String zItemHost;
	uint8_t zabbixPacket[ZABBIXMAXLEN];
	zabbixCell zabbixItemList[ZABBIXMAXITEM];
	zabbixCellString zabbixItemListString[ZABBIXMAXITEM];
	int zabbixItemSize;
	int zabbixItemSizeString;
};

#endif
