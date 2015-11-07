# Structure of MQTT Messages
## Concepts

- **Node:** A network node with 1 radio and ip
- **Device:** A logical unit, e.g a shutter controller, a simple switch, a heater controller
-- **nodeId:** A unique (scope: installation)  id of the esp node; it is used as identifier across the system, e.g hostname, mqtt-prefix
-- **deviceId:** A unique (scope: node) id of the esp node

## Sending Commands

NOTE: I removed the device type, it is implicit in the id!
NOTE: take care that commands can be filtered easily!

```
<nodeId>/<deviceId>/get-state		// tell device to send state information ###TODO: define json structure
<nodeId>/<deviceId>/do/<command>
```

Examples:

```
dobby-a78f3/shutter-1/move down
dobby-bathdoor/light/switch on
```

## Commands common to all Devices

Command | Details
--------|-------
`help`	| list all commands in human-readable form
`commands` | list all command topics understood by this device in human-readable form
`signal`	| list all signal topics sent by this device in human-readable form

## auto-registry messages

On Startup each node/device sends a "hello" topic with message: a usage text
"dobby-a78f3/hello" "a node"
"dobby-a78f3/light" "switch on|1|off|0"

## Special Devices that always exist
exist on all nodes

###node

Command | Details
--------|-------
`<nodeId>/ota`	| initiate ota update; #TODO: define scope (files, app)
`<nodeId>/reboot`	| initiate reboot

## Error Messages
whenever an invalid message is sent, 


## messages to devices

```
<nodeId>/<deviceId>/get-state		// tell device to send state information ###TODO: define json structure
<nodeId>/<deviceId>/do/<command>
```

## messages from devices
<nodeId>/<deviceId>/do/<command>


###network
TODO
###mqtt
TODO

## Device Types

### pushbutton
### switch

