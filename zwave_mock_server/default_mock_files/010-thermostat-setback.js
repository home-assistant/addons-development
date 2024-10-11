/// This mock simulates a thermostat that only supports the Thermostat Setback CC

// @ts-check
const { CommandClasses } = require("@zwave-js/core");

/** @type {import("zwave-js/Testing").MockServerOptions["config"]} */
module.exports.default = {
	nodes: [
		{
			id: 2,
			capabilities: {
				commandClasses: [
					CommandClasses.Version,
					CommandClasses["Thermostat Setback"],
				],
			},
		},
	],
};
