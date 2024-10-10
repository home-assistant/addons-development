// @ts-check
const { NotificationCCReport } = require("@zwave-js/cc/NotificationCC");
const { CommandClasses } = require("@zwave-js/core");
const { ccCaps, createMockZWaveRequestFrame } = require("@zwave-js/testing");

let simulationTimer;
process.on("SIGINT", () => {
	if (simulationTimer) clearInterval(simulationTimer);
});

/** @type {import("zwave-js/Testing").MockServerOptions["config"]} */
module.exports.default = {
	nodes: [
		{
			id: 2,
			capabilities: {
				commandClasses: [
					CommandClasses.Version,
					ccCaps({
						ccId: CommandClasses.Notification,
						isSupported: true,
						version: 11,
						notificationTypesAndEvents: {
							// Access Control - Door state
							[0x06]: [0x16, 0x17],
						},
					}),
				],
			},
			behaviors: [
				{
					// Small hack - start the state simulation timer when the node
					// receives a command from the controller
					async onControllerFrame(controller, self, _frame) {
						if (!simulationTimer) {
							// Then send notifications regularly
							simulationTimer = setInterval(() => {
								// 75% of reports are open, 25% are closed
								const isOpen = Math.random() < 0.75;
								// 50% of reports include the tilt parameter
								const supportsTilt = Math.random() < 0.5;
								// 50% of those are tilted, 50% are not
								const isTilted = Math.random() < 0.5;

								const cc = new NotificationCCReport(self.host, {
									nodeId: controller.host.ownNodeId,
									notificationType: 0x06, // Access Control
									notificationEvent: isOpen ? 0x16 : 0x17,
									eventParameters: isOpen && supportsTilt
										? Buffer.from([
											isTilted ? 0x01 : 0x00,
										])
										: undefined,
								});

								self.sendToController(
									createMockZWaveRequestFrame(cc, {
										ackRequested: false,
									}),
								);
								// Tune this value according to how often you want to get notifications
							}, 5000).unref();
						}
						return false;
					},
				},
			],
		},
	],
};