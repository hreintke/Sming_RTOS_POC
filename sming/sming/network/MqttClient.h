/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _SMING_CORE_NETWORK_MqttClient_H_
#define _SMING_CORE_NETWORK_MqttClient_H_

#define MQTT_MAX_BUFFER_SIZE 1024

#include "TcpClient.h"
#include "../core/Delegate.h"
#include "../wiring/WString.h"
#include "../services/libemqtt/libemqtt.h"
#include "../core/DataSourceStream.h"
#include "../commandprocessing/CommandProcessingIncludes.h"

//typedef void (*MqttStringSubscriptionCallback)(String topic, String message);
typedef Delegate<void(String topic, String message)> MqttStringSubscriptionCallback;

class MqttClient;
class URL;

class MqttClient: protected TcpClient
{
public:
	MqttClient(String serverHost, int serverPort, MqttStringSubscriptionCallback callback = NULL);
	MqttClient(IPAddress serverIp, int serverPort, MqttStringSubscriptionCallback callback = NULL);
	virtual ~MqttClient();

	
        void setKeepAlive(int seconds);			//send to broker
 	void setPingRepeatTime(int seconds);            //used by client
 	
	// Sets Last Will and Testament
	bool setWill(String topic, String message, int QoS, bool retained = false);

	bool connect(String clientName);
	bool connect(String clientName, String username, String password);

	using TcpClient::setCompleteDelegate;

	__forceinline bool isProcessing()  { return TcpClient::isProcessing(); }
	__forceinline TcpClientState getConnectionState() { return TcpClient::getConnectionState(); }

	bool publish(String topic, String message, bool retained = false);
	bool publish(String topic, MemoryDataStream reqDataStream, bool retained = false);
	bool publishWithQoS(String topic, String message, int QoS, bool retained = false);


	bool subscribe(String topic);
	bool unsubscribe(String topic);

	void commandProcessing(bool reqEnabled, String reqCommandTopic, String reqCommandResponse , String reqCommandName = "");

protected:
	virtual err_t onReceive(pbuf *buf);
	virtual void onReadyToSendData(TcpConnectionEvent sourceEvent);
	void debugPrintResponseType(int type, int len);
	static int staticSendPacket(void* userInfo, const void* buf, unsigned int count);

private:
	String server;
	IPAddress serverIp;
	int port;
	mqtt_broker_handle_t broker;
	int waitingSize;
	uint8_t buffer[MQTT_MAX_BUFFER_SIZE + 1];
	uint8_t *current;
	int posHeader;
	MqttStringSubscriptionCallback callback;
	int keepAlive = 60;
        int PingRepeatTime = 20;

	String mqttCommandEnabled = "";
	String mqttCommandTopic = "";
	String mqttCommandResponse = "";
	String mqttCommandName = "";
};

#endif /* _SMING_CORE_NETWORK_MqttClient_H_ */
