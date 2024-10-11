/// This mock simulates a device with just a Multilevel Switch

// @ts-check
const { CommandClasses } = require("@zwave-js/core");
const { ccCaps } = require("@zwave-js/testing");
const { ColorComponent } = require("zwave-js");

/** @type {import("zwave-js/Testing").MockServerOptions["config"]} */
module.exports.default = {
	nodes: [
		{
			id: 6,
			capabilities: {
				commandClasses: [
					CommandClasses.Version,
					ccCaps({
						ccId: CommandClasses["Multilevel Switch"],
						isSupported: true,
						version: 4,
					}),
				],
			},
		},
	],
};
