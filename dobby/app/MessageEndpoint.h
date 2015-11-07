/*
 * MessageEndpoint.h
 *
 *  Created on: 07.11.2015
 *      Author: gernot
 */

#ifndef APP_MESSAGEENDPOINT_H_
#define APP_MESSAGEENDPOINT_H_

#include "Identifiable.h"

namespace dobby {

/**
 * An source and sink for mqtt messages.
 */
class MessageEndpoint: public virtual Identifiable {

public:
	MessageEndpoint();
	virtual ~MessageEndpoint();
};

} /* namespace dobby */

#endif /* APP_MESSAGEENDPOINT_H_ */
