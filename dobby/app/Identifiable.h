/*
 * Identifiable.h
 *
 *  Created on: 07.11.2015
 *      Author: gernot
 */

#ifndef APP_IDENTIFIABLE_H_
#define APP_IDENTIFIABLE_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

namespace dobby {


/**
 * something that can be identified
 */
class Identifiable {
public:
	Identifiable();
	~Identifiable();

	/**
	 * return id for Object.
	 * @return id;
	 */
	String id(){ return _id;}
	void setId(String id);

private:
	/**
	 * id for this one
	 */
	String _id=""; //TODO: need a good default?
};

} /* namespace dobby */

#endif /* APP_IDENTIFIABLE_H_ */
