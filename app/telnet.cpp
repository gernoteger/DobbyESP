/**
 * telnet server: start & register commands
 */

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>
#include "Services/CommandProcessing/CommandProcessingIncludes.h"
#include <SmingCore/Debug.h>


#include <user_interface.h>




#include "telnet.h"
#include "commands.h"

void tcpServerClientConnected (TcpClient* client)
{
	debugf("Application onClientCallback : %s\r\n",client->getRemoteIp().toString().c_str());
}

bool tcpServerClientReceive (TcpClient& client, char *data, int size)
{
	debugf("Application DataCallback : %s, %d bytes \r\n",client.getRemoteIp().toString().c_str(),size );
	debugf("Data : %s", data);
	client.sendString("sendString data\r\n", false);
	client.writeString("writeString data\r\n",0 );
	if (strcmp(data,"close") == 0)
	{
		debugf("Closing client");
		client.close();
	};
	return true;
}



void tcpServerClientComplete(TcpClient& client, bool succesfull)
{
	debugf("Application CompleteCallback : %s \r\n",client.getRemoteIp().toString().c_str() );
}

TcpServer tcpServer(tcpServerClientConnected, tcpServerClientReceive, tcpServerClientComplete);
TelnetServer telnetServer;


void startTelnetServer()
{
	tcpServer.listen(8023);

	Serial.println("\r\n=== TCP SERVER Port 8023 STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");

	telnetServer.listen(23);

	Serial.println("\r\n=== Telnet SERVER Port 23 STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");

	commandHandler.registerCommand(CommandDelegate("application","This command is defined by the application\r\n","testGroup", applicationCommand));
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");
	startTelnetServer();
}

void connectFail()
{
	debugf("I'm NOT CONNECTED!");
	WifiStation.waitConnection(connectOk, 10, connectFail);
}


