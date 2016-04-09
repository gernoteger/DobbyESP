/*
 * Configurable.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */


#include "Configurable.h"

void ICACHE_FLASH_ATTR dobby::Configurable::loadFromParent(JsonObject& parent) {
	JsonObject& item = parent[getTypeName()];
	load(item);
}

void ICACHE_FLASH_ATTR dobby::Configurable::saveToParent(JsonObject& parent) {
	JsonObject& item = parent.createNestedObject(getTypeName());
	save(item);
}
