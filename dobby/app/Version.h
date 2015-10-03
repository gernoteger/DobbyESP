/*
 * Version.h
 *
 *  Created on: 30.09.2015
 *      Author: gernot
 */

#ifndef APP_VERSION_H_
#define APP_VERSION_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <user_interface.h>


/**
 * @brief various version information about source base.
 *
 * Provides Version Information about this software. must be re-built each time we build
 */
class Version {
public:
	Version();
	virtual ~Version();

	/**
	 * get manually defined semantic version as defined in @http://semver.org/
	 * compatibility here means the API's provided to the outside, e.g. MQTT commands
	 * @return Version String as MAJOR.MINOR.PATCH and an optional "-SNAPSHOT"
	 */
	static String version();

	/**
	 * the (long) git head reference when gbuilt
	 * @return
	 */
	static String gitref();

	/**
	 * The build time as String (+%Y-%m-%dT%H:%M:%S), intended for human audience
	 * @return
	 */
	static String buildtime();

	/**
	 * a compsite string of version and time
	 * @return
	 */
	static String buildinfo();
};

#endif /* APP_VERSION_H_ */
