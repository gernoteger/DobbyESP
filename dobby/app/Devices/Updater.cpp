/*
 * Switch.cpp
 *
 *  Created on: 18.10.2015
 *      Author: gernot
 */
#include "user_config.h"

#include <SmingCore/SmingCore.h>

#undef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG

#include "logging.h"
#include "Updater.h"
#include "update.h"

#include "Node.h"

namespace dobby {



Updater::Updater() : Device("updater") {

}

Updater::~Updater() {
}

/*
 * start: setup  IOs
 */
void Updater::start() {
	Device::start();
}

void Updater::load(JsonObject& object) {
//	  "updater": {
//		"app": "http://192.168.1.100:8081/rom0.bin",
//		"files": "http://192.168.1.100:8081/spiff_rom.bin"
//	  },
	appUpdateUrl=object["app"];
	filesUpdateUrl=object["files"];
}

/**
 *
 */
void Updater::publishInfo(){
	String info=Node::node().info();
	publish("info",info,true); // retain: is a  state
}


/**
 * commands
 *
 * ota -> update defaults
 * updateApp <app>
 * updateAll app files
 * updateFiles files
 *
 * testing:
 * 	dobby-a69c7f/updater/do/update
 *  http://192.168.1.100:8081/rom0.bin http://192.168.1.100:8081/spiff_rom.bin
 *
 * @param command
 * @param message
 */
void Updater::handleCommand(String command, String message) {
	LOG_DEBUG(" Updater::handleCommand");
	Device::handleCommand(command,message); // mainly debugging

	if(command=="update"){ //TODO: extract files from message
		Vector<String> urls=Vector<String>();

		splitString(message,' ',urls);

		if(urls.size()<1){
			invalidCommand(command,message,"url(s) needed");
		}else if(urls.size()==1){
			update_app(false,urls[0],"");
		}else{
			update_app(true,urls[0],urls[1]);
		}
	}else if(command=="info"){ //TODO: gehört in node
		infoTimer.initializeMs(500, TimerDelegate(&Updater::publishInfo, this)).start();


	}else{
		invalidCommand(command,message,"command unknown");
	}

}

String Updater::usage(){
	return "update appUrl [filesUrl]\r\nupdateFiles filesUrl";
}

} /* namespace dobby */

