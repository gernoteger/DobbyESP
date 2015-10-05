/*
 * CommandLine.h
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#ifndef APP_COMMANDLINE_H_
#define APP_COMMANDLINE_H_

#include "Devices/Thermostat.h"

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
	void scanCommand(String commandLine, CommandOutput* commandOutput);
	void heaterControllerCommand(String commandLine, CommandOutput* commandOutput);
	void adcCommand(String commandLine, CommandOutput* commandOutput);
	void mqttTest1Command(String commandLine, CommandOutput* commandOutput);
	void mqttStatusCommand(String commandLine, CommandOutput* commandOutput);
	void mqttConnectCommand(String commandLine, CommandOutput* commandOutput);

	void saveSettingsCommand(String commandLine, CommandOutput* commandOutput);

	///@}

private:
	Timer memoryTimer;	///< Timer for checkHeap()
	int savedHeap=-1;
	Thermostat tc=Thermostat(1000);
};

} /* namespace dobby */

#endif /* APP_COMMANDLINE_H_ */
