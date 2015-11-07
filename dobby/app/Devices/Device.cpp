/*
 * Device.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include "user_config.h"

#include <SmingCore/SmingCore.h>
#include <Debug.h>

#include "Node.h"

#include "Device.h"

namespace dobby {

Device::Device(String _id) {
	setId(_id);
}

Device::~Device() {
	// TODO Auto-generated destructor stub
}



void Device::start() {
	Debug.println("Device::start():"+id());
}

void Device::stop() {
	Debug.println("Device::stop():"+id());
}

} /* namespace dobby */

