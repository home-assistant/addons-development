/// This mock simulates a device with a Multilevel Switch, a Binary Switch, and a Color Switch command class
/// The resulting entities should be a colored light and a switch.

// @ts-check
const { CommandClasses } = require("@zwave-js/core");
const { ccCaps } = require("@zwave-js/testing");
const { ColorComponent } = require("zwave-js");

/** @type {import("zwave-js/Testing").MockServerOptions["config"]} */
module.exports.default = {
	nodes: [
		{
			id: 4,
			capabilities: {
				commandClasses: [
					CommandClasses.Version,
					ccCaps({
						ccId: CommandClasses["Multilevel Switch"],
						isSupported: true,
						version: 4,
					}),
					ccCaps({
						ccId: CommandClasses["Binary Switch"],
						isSupported: true,
						version: 2,
					}),
					ccCaps({
						ccId: CommandClasses["Color Switch"],
						isSupported: true,
						version: 3,
						colorComponents: {
							[ColorComponent.Red]: 0,
							[ColorComponent.Green]: 0,
							[ColorComponent.Blue]: 0,
						},
					}),
				],
			},
		},
	],
};
