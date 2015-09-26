#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>
#include "Services/CommandProcessing/CommandProcessingIncludes.h"
#include <SmingCore/Debug.h>


#include <user_interface.h>



#include "commands.h"

Timer memoryTimer;
int savedHeap = 0;

void checkHeap()
{
	int currentHeap = system_get_free_heap_size();
	if (currentHeap != savedHeap)
	{
		Debug.printf("Heap change, current = %d\r\n", currentHeap);
		savedHeap = currentHeap;
	}
}

/**
 * start debugging functions
 */

void startDebug()
{
	Debug.setDebug(Serial);
	Debug.initCommand();
	Debug.start();
	Debug.printf("This is the debug output\r\n");
	//telnetServer.setDebug(true);/* is default but here to show possibility */
	commandHandler.registerCommand(CommandDelegate("appheap","Usage appheap on/off/now for heapdisplay\r\n","testGroup", appheapCommand));
	memoryTimer.initializeMs(250,checkHeap).start();

}

void applicationCommand(String commandLine  ,CommandOutput* commandOutput)
{
	commandOutput->printf("Hello from Telnet Example application\r\nYou entered : '");
	commandOutput->printf(commandLine.c_str());
	commandOutput->printf("'\r\n");
	commandOutput->printf("Tokenized commandLine is : \r\n");

	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);
	for (int i=0;i<numToken;i++)
	{
		commandOutput->printf("%d : %s\r\n",i,commandToken.at(i).c_str());
	}
}

void appheapCommand(String commandLine  ,CommandOutput* commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);
	if (numToken != 2)
	{
		commandOutput->printf("Usage appheap on/off/now\r\n");
	}
	else if (commandToken[1] == "on")
	{
		commandOutput->printf("Timer heap display started \r\n");
		savedHeap = 0;
		memoryTimer.initializeMs(250,checkHeap).start();
	}
	else if (commandToken[1] == "off")
	{
		commandOutput->printf("Timer heap display stopped \r\n");
		savedHeap = 0;
		memoryTimer.stop();
	}
	else if (commandToken[1] == "now")
	{
		commandOutput->printf("Heap current free = %d\r\n", system_get_free_heap_size());
	}
	else
	{
		commandOutput->printf("Usage appheap on/off/now\r\n");
	}
}
