/*
 * Configurable.cpp
 *
 *  Created on: 04.10.2015
 *      Author: gernot
 */


#include "Configurable.h"

void dobby::Configurable::loadFromParent(JsonObject& parent) {
	JsonObject& item = parent[typeName()];
	load(item);
}

void dobby::Configurable::saveToParent(JsonObject& parent) {
	JsonObject& item = parent.createNestedObject(typeName());
	save(item);
}
