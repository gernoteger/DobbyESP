/**
 * @file commands.h
 * @brief commands for dobby
 *
 * @mainpage DobbyESP-Description
 *
 * @section Goals Design Goals
 * This library is designed to be...
 * @li Maximally compliant with the intended operation of the chip
 * @li Easy for beginners to use
 * @li Consumed with a public interface that's similiar to other Arduino standard libraries
 *
 * @section News News
*/

/*
 * commands.h
 *
 *  Created on: 25.09.2015
 *      Author: gernot
 */

#ifndef INCLUDE_COMMANDS_H_
#define INCLUDE_COMMANDS_H_


namespace dobby {

void registerCommands();


void checkHeap();
void startDebug();

/**
 * command for app
 * @param commandLine
 * @param commandOutput
 */
void applicationCommand(String commandLine  ,CommandOutput* commandOutput);

void appheapCommand(String commandLine  ,CommandOutput* commandOutput);
void otaCommand(String commandLine  ,CommandOutput* commandOutput);

}  // namespace dobby

#endif /* INCLUDE_COMMANDS_H_ */

