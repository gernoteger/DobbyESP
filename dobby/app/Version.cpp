/*
 * Version.cpp
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#include "Version.h"
#include "buildinfo.h"


namespace dobby {

Version::Version() {
	// TODO Auto-generated constructor stub

}

Version::~Version() {
	// TODO Auto-generated destructor stub
}

String Version::version() {
	return BUILD_VERSION;
}

String Version::gitref() {
	return _BUILD_GITREF;
}


String Version::buildtime() {
	return _BUILD_TIME;
}

String Version::buildinfo() {
	return version()+" "+buildtime();
}

}  // namespace dobby
