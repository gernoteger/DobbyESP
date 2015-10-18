# Structure of MQTT Messages
## Concepts

- **Node:** A network node with 1 radio and ip
- **Device:** A logical unit, e.g a shutter controller, a simple switch, a heater controller
-- **nodeId:** A unique (scope: installation)  id of the esp node; it is used as identifier across the system, e.g hostname, mqtt-prefix
-- **deviceId:** A unique (scope: node) id of the esp node

## Sending Commands

NOTE: I removed the device type, it is implicit in the id!

```
<nodeId>/<deviceId>/status		// tell device to send status information ###TODO: define json structure
<nodeId>/<deviceId>/<command>
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

## Special Devices that always exist
exist on all nodes
###node

Command | Details
--------|-------
`<nodeId>/ota`	| initiate ota update; #TODO: define scope (files, app)
`<nodeId>/reboot`	| initiate reboot

###network
TODO
###mqtt
TODO

## Device Types

### pushbutton
### switch

