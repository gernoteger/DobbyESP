/*
 * CommandLine.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_COMMANDLINE_H_
#define APP_COMMANDLINE_H_

#define APP_COMMANDLINE_THERMOSTAT 0

#if APP_COMMANDLINE_THERMOSTAT
#include "Devices/Thermostat.h"
#endif

namespace dobby {

class CommandLine {
public:
	CommandLine();
	~CommandLine();

	///@name The Commands
	///@{
	void registerCommands();

	/**
	 * start debugging to Serial..
	 */
	static void startDebug();


	void checkHeap();

	/**
	 * command for app
	 * @param commandLine
	 * @param commandOutput
	 */
	void applicationCommand(String commandLine  ,CommandOutput* commandOutput);

	void appheapCommand(String commandLine  ,CommandOutput* commandOutput);
	void otaCommand(String commandLine  ,CommandOutput* commandOutput);

	void connectCommand(String commandLine, CommandOutput* commandOutput);

	void switchCommand(String commandLine, CommandOutput* commandOutput);
	void restartCommand(String commandLine, CommandOutput* commandOutput);
	void infoCommand(String commandLine, CommandOutput* commandOutput);
	void lsCommand(String commandLine, CommandOutput* commandOutput);
	void catCommand(String commandLine, CommandOutput* commandOutput);
	void scanCommand(String commandLine, CommandOutput* commandOutput);

	///@name Sensors
	///@{
#if APP_COMMANDLINE_THERMOSTAT
	void heaterControllerCommand(String commandLine, CommandOutput* commandOutput);
#endif

	void adcCommand(String commandLine, CommandOutput* commandOutput);
	void gpioControllerCommand(String commandLine, CommandOutput* commandOutput);
	///@}

	void mqttTest1Command(String commandLine, CommandOutput* commandOutput);
	void mqttStatusCommand(String commandLine, CommandOutput* commandOutput);
	void mqttSubscribeCommand(String commandLine, CommandOutput* commandOutput);
	void mqttConnectCommand(String commandLine, CommandOutput* commandOutput);

	void saveSettingsCommand(String commandLine, CommandOutput* commandOutput);

	///@}

private:
	Timer memoryTimer;	///< Timer for checkHeap()
	int savedHeap=-1;

#if APP_COMMANDLINE_THERMOSTAT
	Thermostat tc=Thermostat("testThermostat",1000);
#endif
};

} /* namespace dobby */

#endif /* APP_COMMANDLINE_H_ */
