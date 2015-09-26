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


TelnetServer telnetServer;


/**
 * start telnet server for remote operation.
 * The TelnetServer handles connections, and hands over everything to the global commandHandler.
 * The commands are registered elsewhere with the commandHandler.
 */
void startTelnetServer()
{
	telnetServer.listen(23);
	Serial.println("\r\n=== Telnet SERVER Port 23 STARTED ===");
	Serial.println(WifiStation.getIP());
	Serial.println("==============================\r\n");
}


/**
 * gracefully shut down telnet server
 */
void stopTelnetServer()
{
	telnetServer.close();
}
//// Will be called when WiFi station was connected to AP
//void connectOk()
//{
//	Serial.println("I'm CONNECTED");
//	startTelnetServer();
//}
//
//void connectFail()
//{
//	debugf("I'm NOT CONNECTED!");
//	WifiStation.waitConnection(connectOk, 10, connectFail);
//}
//

