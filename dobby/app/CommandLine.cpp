/*
 * CommandLine.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

//#include "espinc/c_types_compatible.h"

#include "user_config.h"

#include <SmingCore/SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>

#include "../SmingCore/Digital.h"
//#include "../Wiring/WiringFrameworkIncludes.h"

//#include <CommandDelegate.h>

#include "update.h"
#include "MessageConnection.h"
#include "Version.h"
#include "buildinfo.h"
#include "ADC.h"
#include "Devices/Thermostat.h" //TODO: handle with AppController??

#include "Logger.h"
#include "Node.h"

#include "CommandLine.h"

namespace dobby {



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

	String arg(int arg,String dflt=""){
		if(commandToken.size()<arg+1){
			return dflt;
		}
		return commandToken[arg];
	}

	int nargs(){
		return commandToken.size()-1;
	}

private:
	Vector<String> commandToken;
	String usageText;
	CommandOutput* commandOutput;
};


CommandLine::CommandLine() {
	Debug.println("CommandLine::CommandLine()");

}

CommandLine::~CommandLine() {
	// TODO Auto-generated destructor stub
}

/**
 * outputs the heap size if changed since last invocation
 *
 */

void CommandLine::checkHeap() {
	int currentHeap = system_get_free_heap_size();
	if (currentHeap != savedHeap) {
		Debug.printf("Heap change, current = %d\r\n", currentHeap);
		savedHeap = currentHeap;
	}
}

/**
 * start debugging functions
 */

void CommandLine::startDebug() {
	Debug.setDebug(Serial);
	Debug.initCommand();
	Debug.start();
	Debug.printf("This is the debug output\r\n");
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
void CommandLine::connectCommand(String commandLine, CommandOutput* commandOutput) {
	//##TODO: imlplement Node::node().
}

/**
 * switch roms
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::switchCommand(String commandLine, CommandOutput* commandOutput) {
	update_switch_roms(commandOutput);
}

void CommandLine::restartCommand(String commandLine, CommandOutput* commandOutput) {
	System.restart();
}

/**
 * show system info
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::infoCommand(String commandLine, CommandOutput* commandOutput) {
	//Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
	commandOutput->println("Node: "+Node::node().id());

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
void CommandLine::lsCommand(String commandLine, CommandOutput* commandOutput) {
	Vector<String> files = fileList();
	commandOutput->printf("filecount %d\r\n", files.count());
	for (unsigned int i = 0; i < files.count(); i++) {
		commandOutput->printf("%s: %u\r\n",files[i].c_str(),fileGetSize(files[i]));
	}
}

/**
 * list files
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::catCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"cat <filename>",commandOutput);

	if(cmd.nargs()!=1){
		cmd.usage();
		return;
	}
	Vector<String> files = fileList();
	String file=cmd.arg(1);
	Debug.printf("file=%s\r\n",file.c_str());
	for (unsigned int i = 0; i < files.count(); i++) {
		if(files[i]==file){
			commandOutput->printf("dumping file %s:\r\n", files[i].c_str());
			//TODO: can easily kill Memory!!!
			commandOutput->println(fileGetContent(files[i]));
			return;
		}
	}
	commandOutput->printf("file not found: '%s'\r\n", file.c_str());

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
void CommandLine::scanCommand(String commandLine, CommandOutput* commandOutput) {
	//TODO: impl..WifiStation.startScan(networkScanCompleted);
}

/**
 * some command for the application
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::applicationCommand(String commandLine, CommandOutput* commandOutput) {
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
 * gpio
 *
 * gpio	: read all, or
 * gpio <io> <value>/in : set to this value
 * gpio pullup on/off
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::gpioControllerCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"gpio [<io> <value>/in]",commandOutput);

	if(cmd.nargs()==0){
		// just list them all
		commandOutput->println("pin  dir val");
		commandOutput->println("------------");
		for(int i=1;i<17;i++){
			uint8_t val=digitalRead(i);
			bool isInput=isInputPin(i);
			String is=isInput?"in ":"out";

			commandOutput->printf("%3u: %s %u\r\n",i,is.c_str(),val);
		}
		commandOutput->println("------------");
	}else if(cmd.nargs()<2){
		cmd.usage();
	} else {
		// do sth with the pin
		int pin = cmd.arg(1).toInt();
		if (cmd.arg(2) == "pullup") {
			// set pullup
			switch (cmd.argumentIs(3, "on", "off")) {
			case 0:
				pinMode(pin, INPUT_PULLUP);
				break;
			case 1:
				pinMode(pin, INPUT);
				break;

			default:
				cmd.usage();
			}
		}else{
			// set value
			int val=cmd.arg(2).toInt();
			pinMode(pin, OUTPUT);
			digitalWrite(pin,val);
		}
	}
}

#if APP_COMMANDLINE_THERMOSTAT
void CommandLine::heaterControllerCommand(String commandLine, CommandOutput* commandOutput) {
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
#endif

/**
 * read adc (once??)
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::adcCommand(String commandLine, CommandOutput* commandOutput) {
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
void CommandLine::otaCommand(String commandLine, CommandOutput* commandOutput) {
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

void CommandLine::appheapCommand(String commandLine, CommandOutput* commandOutput) {
	CommandHelper cmd(commandLine,"appheap on/off/now",commandOutput);
	switch(cmd.argumentIs(1,"on","off","now")){
	case 0:
		savedHeap = 0;
		memoryTimer.initializeMs(250, TimerDelegate(&CommandLine::checkHeap,this)).start();
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

void CommandLine::mqttTest1Command(String commandLine, CommandOutput* commandOutput) {
	Node::node().mqttSendTestMessage();
}

void CommandLine::mqttStatusCommand(String commandLine, CommandOutput* commandOutput) {
	Node::node().mqttPrintStatus(commandOutput);
}

void CommandLine::mqttSubscribeCommand(String commandLine, CommandOutput* commandOutput) {
	Node::node().subscribeDevices();
}

/**
 * (re-)connect mqtt
 * @param commandLine
 * @param commandOutput
 */
void CommandLine::mqttConnectCommand(String commandLine, CommandOutput* commandOutput) {

	CommandHelper cmd(commandLine,"mqtt-connect [server] [port=1883]",commandOutput);
	if(cmd.nargs()>2){
		cmd.usage();
		return;
	}

	MessageConnection mqtt=Node::node().getMqttClient();
	if(cmd.nargs()>0){
		String server=cmd.arg(1,"192.168.1.1");
		String port=cmd.arg(1,"1883");
		mqtt.configure(server,port.toInt());
	}

	mqtt.start();
	mqtt.sendTestMessage1();
}


void CommandLine::saveSettingsCommand(String commandLine, CommandOutput* commandOutput) {
	Node::node().save();
}

/**
 * @}
 */


/**
 * register all commands defined here in this procedure..
 */
void CommandLine::registerCommands() {

	Logger::logheap("CommandLine::registerCommands() 0");
	commandHandler.registerCommand(
			CommandDelegate("application",
					"This command is defined by the application\r\n",
					"testGroup", commandFunctionDelegate(&CommandLine::applicationCommand,this)));
	commandHandler.registerCommand(CommandDelegate("ota", "update over the air", "update", commandFunctionDelegate(&CommandLine::otaCommand,this)));
	commandHandler.registerCommand(CommandDelegate("switch", "switch roms", "update", commandFunctionDelegate(&CommandLine::switchCommand,this)));
	commandHandler.registerCommand(CommandDelegate("restart", "switch roms", "update", commandFunctionDelegate(&CommandLine::restartCommand,this)));

	Logger::logheap("CommandLine::registerCommands() 1");

	commandHandler.registerCommand(CommandDelegate("ls", "list files", "files", commandFunctionDelegate(&CommandLine::lsCommand,this)));
	commandHandler.registerCommand(CommandDelegate("cat", "cat <file>", "files", commandFunctionDelegate(&CommandLine::catCommand,this)));

	Logger::logheap("CommandLine::registerCommands() 2");

	commandHandler.registerCommand(CommandDelegate("scan", "update all", "net", commandFunctionDelegate(&CommandLine::scanCommand,this)));
	commandHandler.registerCommand(CommandDelegate("connect", "update all", "net", commandFunctionDelegate(&CommandLine::connectCommand,this)));
	commandHandler.registerCommand(CommandDelegate("info", "update all", "serial", commandFunctionDelegate(&CommandLine::infoCommand,this)));

	commandHandler.registerCommand(CommandDelegate("save-settings", "save settings", "config", commandFunctionDelegate(&CommandLine::saveSettingsCommand,this)));

	Logger::logheap("CommandLine::registerCommands() 3");


	commandHandler.registerCommand(
			CommandDelegate("appheap",
					"Usage appheap on/off/now for heapdisplay\r\n", "testGroup",
					commandFunctionDelegate(&CommandLine::appheapCommand,this)));

	//sensor tests
	commandHandler.registerCommand(CommandDelegate("adc", "read adc", "sensors",commandFunctionDelegate(&CommandLine::adcCommand,this)));
#if APP_COMMANDLINE_THERMOSTAT
	commandHandler.registerCommand(CommandDelegate("heater", "heater on/off", "sensors",commandFunctionDelegate(&CommandLine::heaterControllerCommand,this)));
#endif

	commandHandler.registerCommand(CommandDelegate("gpio", "gpio [<io> <value>/in]", "sensors",commandFunctionDelegate(&CommandLine::gpioControllerCommand,this)));

	Logger::logheap("CommandLine::registerCommands() 4");

	// mqtt tests
	commandHandler.registerCommand(CommandDelegate("mqtt-send", "send test message", "mqtt",commandFunctionDelegate(&CommandLine::mqttTest1Command,this)));
	commandHandler.registerCommand(CommandDelegate("mqtt-status", "mqtt status message", "mqtt",commandFunctionDelegate(&CommandLine::mqttStatusCommand,this)));
	commandHandler.registerCommand(CommandDelegate("mqtt-connect", "mqtt-connect [server] [port=1883]", "mqtt",commandFunctionDelegate(&CommandLine::mqttConnectCommand,this)));
	commandHandler.registerCommand(CommandDelegate("mqtt-subscribe", "mqtt subscribe all devices", "mqtt",commandFunctionDelegate(&CommandLine::mqttSubscribeCommand,this)));

	Logger::logheap("CommandLine::registerCommands() done.");

}

} /* namespace dobby */
