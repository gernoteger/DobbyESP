/**
 *
 * @file commands.cpp
 * All commands are concentrated here
 *
 * TODO: doc test:

 @msc
 arcgradient = 8;
 a [label="Client"],b [label="Server"];
 a-xb [label="get accel"];
 a=>b [label="get accel"];
 a<=b [label="ack"];
 a<=b [label="accel data"];
 @endmsc

 */

/*! \brief Key scan routine which implements the state machine.
 @dot
 digraph example_api_graph {
 node [shape=box];
 DBNC_KEY_IDLE    [fillcolor=lightblue,style=filled,label="DBNC_KEY_IDLE" ];
 DBNC_KEY_PRESSED [fillcolor=lightblue,style=filled,label="DBNC_KEY_PRESSED" ];
 DBNC_KEY_RELEASE [fillcolor=lightblue,style=filled,label="DBNC_KEY_WAIT_RELEASE"];
 DBNC_KEY_IDLE -> DBNC_KEY_PRESSED -> DBNC_KEY_RELEASE ;
 DBNC_KEY_PRESSED -> DBNC_KEY_PRESSED ;
 DBNC_KEY_IDLE -> DBNC_KEY_IDLE ;
 }
 @enddot
 */

/* includes */
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>
#include "Services/CommandProcessing/CommandProcessingIncludes.h"
#include <SmingCore/Debug.h>

#include <user_interface.h>

#include "buildinfo.h"
#include "commands.h"
#include "update.h"
#include "MessageHandler.h"

//TODO: move this out!!
void networkScanCompleted(bool succeeded, BssList list);

Timer memoryTimer;	///< Timer for checkHeap()
int savedHeap = 0;

/**
 * outputs the heap size if changed since last invocation
 *
 */

void checkHeap() {
	int currentHeap = system_get_free_heap_size();
	if (currentHeap != savedHeap) {
		Debug.printf("Heap change, current = %d\r\n", currentHeap);
		savedHeap = currentHeap;
	}
}

/**
 * start debugging functions
 */

void startDebug() {
	Debug.setDebug(Serial);
	Debug.initCommand();
	Debug.start();
	Debug.printf("This is the debug output\r\n");
	//telnetServer.setDebug(true);/* is default but here to show possibility */
	commandHandler.registerCommand(
			CommandDelegate("appheap",
					"Usage appheap on/off/now for heapdisplay\r\n", "testGroup",
					appheapCommand));
	//memoryTimer.initializeMs(250,checkHeap).start();

}

/**
 * @defgroup serial Commands from Serial example
 * @{
 */

/**
 * connect to wifi
 * @param commandLine
 * @param commandOutput
 */
void connectCommand(String commandLine, CommandOutput* commandOutput) {
	WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiStation.enable(true);
}

/**
 * switch roms
 * @param commandLine
 * @param commandOutput
 */
void switchCommand(String commandLine, CommandOutput* commandOutput) {
	update_switch_roms(commandOutput);
}

void restartCommand(String commandLine, CommandOutput* commandOutput) {
	System.restart();
}

/**
 * show system info
 * @param commandLine
 * @param commandOutput
 */
void infoCommand(String commandLine, CommandOutput* commandOutput) {
    //Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());

	commandOutput->println("Version: " BUILD_VERSION );
	commandOutput->println("git ref: " BUILD_GITREF );
	commandOutput->println("built at: " BUILD_TIME );
    commandOutput->printf("\r\nSDK: v%s\r\n", system_get_sdk_version());

	commandOutput->printf("Free Heap: %d\r\n", system_get_free_heap_size());
	commandOutput->printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
	commandOutput->printf("System Chip ID: %x\r\n", system_get_chip_id());
	commandOutput->printf("SPI Flash ID: %x\r\n", spi_flash_get_id());

	commandOutput->printf("ip: %s mac: %s\r\n", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str());

    //Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    update_print_config(commandOutput);
}

/**
 * list files
 * @param commandLine
 * @param commandOutput
 */
void lsCommand(String commandLine, CommandOutput* commandOutput) {
	Vector<String> files = fileList();
	Serial.printf("filecount %d\r\n", files.count());
	for (unsigned int i = 0; i < files.count(); i++) {
		Serial.println(files[i]);
	}
}

/**
 * @}
 */

/**
 * Scan networks
 *
 * @ingroup serial
 * @param commandLine
 * @param commandOutput
 */
void scanCommand(String commandLine, CommandOutput* commandOutput) {
	WifiStation.startScan(networkScanCompleted);
}

/**
 * some command for the application
 * @param commandLine
 * @param commandOutput
 */
void applicationCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->printf(
			"Hello from Telnet Example application\r\nYou entered : '");
	commandOutput->printf(commandLine.c_str());
	commandOutput->printf("'\r\n");
	commandOutput->printf("Tokenized commandLine is : \r\n");

	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ', commandToken);
	for (int i = 0; i < numToken; i++) {
		commandOutput->printf("%d : %s\r\n", i, commandToken.at(i).c_str());
	}
}

/**
 * command that launches ota
 * TODO: not yet implemented!
 * @param commandLine
 * @param commandOutput
 */
void otaCommand(String commandLine, CommandOutput* commandOutput) {
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ', commandToken);
	if (numToken != 2) {
		commandOutput->printf("Usage ota all/rom/files\r\n");
	} else if (commandToken[1] == "all") {
		commandOutput->println("updating all");
		update_app(commandOutput,true);
	} else if (commandToken[1] == "rom") {
		commandOutput->println("updating rom");
		update_app(commandOutput,false);
	} else if (commandToken[1] == "files") {
		commandOutput->println("updating files");
		update_files(commandOutput);
	} else {
		commandOutput->println("Usage ota on/off/now");
	}
}

void appheapCommand(String commandLine, CommandOutput* commandOutput) {
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ', commandToken);
	if (numToken != 2) {
		commandOutput->printf("Usage appheap on/off/now\r\n");
	} else if (commandToken[1] == "on") {
		commandOutput->printf("Timer heap display started \r\n");
		savedHeap = 0;
		memoryTimer.initializeMs(250, checkHeap).start();
	} else if (commandToken[1] == "off") {
		commandOutput->printf("Timer heap display stopped \r\n");
		savedHeap = 0;
		memoryTimer.stop();
	} else if (commandToken[1] == "now") {
		commandOutput->printf("Heap current free = %d\r\n",
				system_get_free_heap_size());
	} else {
		commandOutput->printf("Usage appheap on/off/now\r\n");
	}
}

/**
 * reference to messag handler
 * @defgroup msgHandlerTest
 *
 * @{
 */
extern MessageHandler messageHandler;

void mqttTest1Command(String commandLine, CommandOutput* commandOutput) {
	messageHandler.sendTestMessage1();
}

void mqttStatusCommand(String commandLine, CommandOutput* commandOutput) {
	messageHandler.printStatus(commandOutput);
}

/**
 * (re-)connect mqtt
 * @param commandLine
 * @param commandOutput
 */
void mqttConnectCommand(String commandLine, CommandOutput* commandOutput) {
	messageHandler.start();
}


/**
 * @}
 */

/**
 * register all commands defined here in this procedure..
 */
void registerCommands() {
	commandHandler.registerCommand(
			CommandDelegate("application",
					"This command is defined by the application\r\n",
					"testGroup", applicationCommand));
	commandHandler.registerCommand(CommandDelegate("ota", "update over the air", "update", otaCommand));
	commandHandler.registerCommand(CommandDelegate("switch", "switch roms", "update", switchCommand));
	commandHandler.registerCommand(CommandDelegate("restart", "switch roms", "update", restartCommand));

	commandHandler.registerCommand(CommandDelegate("ls", "update all", "files", lsCommand));
	commandHandler.registerCommand(CommandDelegate("scan", "update all", "net", scanCommand));
	commandHandler.registerCommand(CommandDelegate("connect", "update all", "net", connectCommand));
	commandHandler.registerCommand(CommandDelegate("info", "update all", "serial", infoCommand));

	// mqtt tests
	commandHandler.registerCommand(CommandDelegate("mqtt-send", "send test message", "mqtt",mqttTest1Command));
	commandHandler.registerCommand(CommandDelegate("mqtt-status", "mqtt status message", "mqtt",mqttStatusCommand));
	commandHandler.registerCommand(CommandDelegate("mqtt-connect", "mqtt connect", "mqtt",mqttConnectCommand));
}
