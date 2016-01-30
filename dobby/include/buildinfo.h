/*
 * buildinfo.h
 *
 *	documents defines generated in build
 *  Created on: 26.09.2015
 *      Author: gernot
 */

#ifndef INCLUDE_BUILDINFO_H_
#define INCLUDE_BUILDINFO_H_

/**
 * avoid errors in eclipse...
 */

#ifndef BUILD_VERSION
#define BUILD_VERSION "##NO BUILD_VERSION"
#endif


#ifndef _BUILD_GITREF
#define _BUILD_GITREF "##NO GIT REF"
#endif

#ifndef _BUILD_TIME
#define _BUILD_TIME "##NO BUILD TIME"
#endif

#ifndef WIFI_SSID
#warning  "##NO WIFI SSID, set local environment variables"
#define WIFI_SSID "WIFI SSID_FOR_COMPILE"
#endif

#ifndef WIFI_PWD
#warning "##NO WIFI_PWD, set local environment variables"
#define WIFI_PWD "WIFI_PWD_FOR_COMPILE"
#endif

#endif /* INCLUDE_BUILDINFO_H_ */
