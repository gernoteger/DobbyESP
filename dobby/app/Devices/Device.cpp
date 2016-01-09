/*
 * Device.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */

#include "user_config.h"

#include <SmingCore/SmingCore.h>

#include "Node.h"

#include "Device.h"
#include "logging.h"

namespace dobby {

Device::Device(String _id) {
	setId(_id);
}

Device::~Device() {
	// TODO Auto-generated destructor stub
}



void Device::start() {
	LOG_INFO("Device::start():"+id());
}

void Device::stop() {
	LOG_INFO("Device::stop():"+id());
}

} /* namespace dobby */

