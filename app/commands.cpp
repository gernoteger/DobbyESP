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

#include <c_types.h>
#include <user_interface.h>

#include "buildinfo.h"
#include "commands.h"
#include "update.h"
#include "MessageHandler.h"
#include "Version.h"
#include "buildinfo.h"

#include "ADC.h"
#include "TemperatureController.h" //TODO: handle with AppController??

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


class CommandHelper{
public:
	CommandHelper(String commandLine,String usageText,CommandOutput* commandOutput)
		:usageText(usageText),commandOutput(commandOutput){
		int numToken = splitString(commandLine, ' ', commandToken);
	}

/**
 * return index of argument #arg in parameter list
 * return -1 if arf# is not existing (=assume default)
 * return -2 if existing but not in list
 *
 * @param arg
 * @param opt0
 * @param opt1
 * @param opt2
 * @return
 */
	int argumentIs(int arg,String opt0,String opt1="",String opt2="",String opt3="",String opt4="",String opt5="" ){
		if(commandToken.size()<arg+1) return -1;
		//if()

		String argvalue=commandToken[arg];

		if(argvalue==opt0) return 0;
		if(argvalue==opt1) return 1;
		if(argvalue==opt2) return 2;
		if(argvalue==opt3) return 3;
		if(argvalue==opt4) return 4;
		if(argvalue==opt5) return 5;

		return -2;
	}

	void usage(){
		commandOutput->print("Usage: ");
		commandOutput->println(usageText);
	}
private:
	Vector<String> commandToken;
	String usageText;
	CommandOutput* commandOutput;
};

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

	commandOutput->println("Version: "+Version::version() );
	commandOutput->println("git ref: "+Version::gitref() );
	commandOutput->println("built at: "+Version::buildtime() );
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

TemperatureController tc=TemperatureController(1000);
void heaterControllerCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"heater on/off/set",commandOutput);
	switch(cmd.argumentIs(1,"on","off")){
	case 0:
		tc.start();
		tc.setTargetReading(700,100);
		break;
	case 1:
		tc.stop();
		break;
	case 2: // set temperature to 3rd argument
		tc.setTargetReading(700,100);
		break;
	default: cmd.usage();
	}
}

/**
 * read adc (once??)
 * @param commandLine
 * @param commandOutput
 */
void adcCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"adc tout/vdd33",commandOutput);

	ADC_SOURCE source;
	switch(cmd.argumentIs(1,"tout","vdd33")){
	case 0:
	case -1:
		source = ADC_TOUT;
		break;

	case 1:
		source = ADC_VDD33;
		break;

	default:
		cmd.usage();
		return;
	}

	ADC adc = ADC(source); //ADC_TOUT,ADC_VDD33
	uint16 reading = adc.read();

	commandOutput->printf("adc=%u\r\n", reading);


}

/**
 * command that launches ota
 * TODO: not yet implemented!
 * @param commandLine
 * @param commandOutput
 */
void otaCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"ota all/rom=default/files",commandOutput);
	switch(cmd.argumentIs(1,"all","rom","files")){
	case 0:
		commandOutput->println("updating all");
		update_app(commandOutput,true);
		break;

	case 1:
	case -1:
		commandOutput->println("updating rom");
		update_app(commandOutput,false);
		break;
	case 2:
		commandOutput->println("updating files");
		update_files(commandOutput);
		break;
	default:
		cmd.usage();
		return;
	}

}

void appheapCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"appheap on/off/now",commandOutput);
	switch(cmd.argumentIs(1,"on","off","now")){
	case 0:
		savedHeap = 0;
		memoryTimer.initializeMs(250, checkHeap).start();
		break;

	case 1:
		commandOutput->printf("Timer heap display stopped \r\n");
		savedHeap = 0;
		break;

	case 2:
	case -1:
		commandOutput->printf("Heap current free = %d\r\n",system_get_free_heap_size());
		break;

	default:
		cmd.usage();
		return;
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

	//sensor tests
	commandHandler.registerCommand(CommandDelegate("adc", "read adc", "sensors",adcCommand));
	commandHandler.registerCommand(CommandDelegate("heater", "heater on/off", "sensors",heaterControllerCommand));



	// mqtt tests
	commandHandler.registerCommand(CommandDelegate("mqtt-send", "send test message", "mqtt",mqttTest1Command));
	commandHandler.registerCommand(CommandDelegate("mqtt-status", "mqtt status message", "mqtt",mqttStatusCommand));


}
