/// This mock simulates a thermostat without overlap between its thermostat modes and thermostat setpoints
/// It MUST be possible to change all setpoints

// @ts-check
const { CommandClasses } = require("@zwave-js/core");
const { ccCaps } = require("@zwave-js/testing");
const { ThermostatMode, ThermostatSetpointType } = require("zwave-js");

/** @type {import("zwave-js/Testing").MockServerOptions["config"]} */
module.exports.default = {
	nodes: [
		{
			id: 11,
			capabilities: {
				basicDeviceClass: 4, // Routing Slave
				genericDeviceClass: 8, // Thermostat
				specificDeviceClass: 6, // General Thermostat V2

				commandClasses: [
					{
						ccId: CommandClasses.Version,
						version: 3,
					},
					{
						ccId: CommandClasses["Manufacturer Specific"],
						version: 1,
					},
					ccCaps({
						ccId: CommandClasses["Thermostat Mode"],
						isSupported: true,
						isControlled: false,
						secure: true,
						version: 3,
						supportedModes: [
							ThermostatMode.Off,
							ThermostatMode.Heat,
							ThermostatMode["Auto"],
							ThermostatMode["Resume (on)"],
							ThermostatMode["Auto changeover"],
						],
					}),
					ccCaps({
						ccId: CommandClasses["Thermostat Setpoint"],
						isSupported: true,
						isControlled: false,
						secure: true,
						version: 3,
						setpoints: {
							[ThermostatSetpointType.Cooling]: {
								minValue: 10,
								maxValue: 36,
								scale: "°C",
							},
							[ThermostatSetpointType["Dry Air"]]: {
								minValue: -9.87,
								maxValue: 56.78,
								scale: "°C",
							},
							[ThermostatSetpointType["Full Power"]]: {
								minValue: 20,
								maxValue: 32.767,
								scale: "°C",
							},
						},
					}),
					CommandClasses["Z-Wave Plus Info"],
				],
			},
		},
	],
};