/*
 * commands.h
 *
 *  Created on: 25.09.2015
 *      Author: gernot
 */

#ifndef INCLUDE_COMMANDS_H_
#define INCLUDE_COMMANDS_H_



void checkHeap();
void startDebug();

void applicationCommand(String commandLine  ,CommandOutput* commandOutput);

void appheapCommand(String commandLine  ,CommandOutput* commandOutput);


#endif /* INCLUDE_COMMANDS_H_ */
