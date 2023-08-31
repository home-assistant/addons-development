/*******************************************************************************
 * Copyright (c) 2023, Nabu Casa, Inc, Stefan Agner
 * based on src/samples/MQTTAsync_publish_time.c
 * Copyright (c) 2012, 2022 IBM Corp., Ian Craggs
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sched.h>
#include <inttypes.h>
#include <pthread.h>

#include "MQTTAsync.h"

#define CLIENTID        "mqtt-rt-test"
#define QOS             0
#define TIMEOUT         10000L
#define SAMPLE_PERIOD   100L    // in ms

#define TOPIC_BASE "homeassistant/sensor/rt-test"
#define TOPIC_DISCOVERY TOPIC_BASE "/config"
#define TOPIC_STATE TOPIC_BASE "/state"
#define TOPIC_SUBSCRIBE TOPIC_BASE "/response"
#define DISCOVERY_PAYLOAD "{ \"name\": \"RT Test\", \"state_topic\": \"" TOPIC_STATE "\" }"

volatile int finished = 0;
volatile int connected = 0;
volatile int subscribed = 0;

volatile int samples = 0;
volatile int64_t overallTime = 0;
volatile int64_t maxTime = 0;
volatile int64_t minTime = ~0LL;
volatile int64_t sentTime;

int64_t getTime(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ((int64_t) ts.tv_sec * 1000000) + ((int64_t) ts.tv_nsec / 1000);
}

void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
	exit(1);
}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Disconnect failed\n");
	finished = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
	printf("Successful disconnection\n");
	finished = 1;
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message send failed token %d error code %d\n", response->token, response->code);
	opts.onSuccess = onDisconnect;
	opts.onFailure = onDisconnectFailure;
	opts.context = client;
	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void onSendSuccess(void* context, MQTTAsync_successData* response)
{
	int64_t sentSuccess = getTime();

	// This gets called when a message is acknowledged successfully.
	printf("%" PRId64 ": Message sent: %d, took (%" PRId64 "us)\n",
		   sentSuccess, response->token, sentSuccess - sentTime);
}


void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response ? response->code : 0);
	finished = 1;
}


void onConnect(void* context, MQTTAsync_successData* response)
{
	printf("Successful connection\n");
	connected = 1;
}

int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{
	int64_t arrivalTime = getTime();
	int64_t payloadTime;

	/* not expecting any messages */
	printf("%" PRId64 ": Message arrived: %.*s, took (%" PRId64 "us)\n",
		   arrivalTime, m->payloadlen, (const char *)m->payload, arrivalTime - sentTime);
	payloadTime = strtoll(m->payload, m->payload + m->payloadlen, 10);

	if (payloadTime == sentTime) {
		uint64_t timediff = arrivalTime - sentTime;
		overallTime += timediff;
		if (maxTime < timediff)
			maxTime = timediff;
		if (minTime > timediff)
			minTime = timediff;
		samples++;
	} else
		printf("NO MATCH!!\n");

	return 1;
}

void deliveryComplete(void* context, MQTTAsync_token token)
{
	//printf("%" PRId64 ": Message delivered: %d\n", getTime(), token);
}

int sendMessage(MQTTAsync client, const char *destinationName, void *buf, int n, int retained)
{
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;

	int rc;

	pub_opts.onSuccess = onSendSuccess;
	pub_opts.onFailure = onSendFailure;
	pub_opts.context = client;

	pubmsg.payload = buf;
	pubmsg.payloadlen = n;
	pubmsg.qos = QOS;
	pubmsg.retained = retained;

	if ((rc = MQTTAsync_sendMessage(client, destinationName, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	return rc;
}

void onSubscribeSuccess(void* context, MQTTAsync_successData* response)
{
	printf("Subscribe succeeded\n");
	subscribed = 1;
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response->code);
	finished = 1;
}

int subscribeTopic(MQTTAsync client, const char *topic)
{
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc;

	printf("Successful connection\n");

	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n",
           topic, CLIENTID, QOS);
	opts.onSuccess = onSubscribeSuccess;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;
	if ((rc = MQTTAsync_subscribe(client, topic, QOS, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
		finished = 1;
	}

	return rc;
}

void intHandler(int dummy) {
	finished = 1;
}

int main(int argc, char* argv[])
{
	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	struct sched_param sp = { .sched_priority = 90 };
	const char *serverURI;
	const char *username;
	const char *password;

	int rc;

	if (argc < 4) {
		printf("Usage: %s SERVER_URI USERNAME PASSWORD\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	serverURI = argv[1];
	username = argv[2];
	password = argv[3];

	printf("Connecting to %s", serverURI);
	if ((rc = MQTTAsync_create(&client, serverURI, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to create client object, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	if ((rc = MQTTAsync_setCallbacks(client, NULL, connlost, messageArrived, deliveryComplete)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to set callback, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	conn_opts.username = username;
	conn_opts.password = password;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	while (!connected) {
		usleep(100000L);
	}

	subscribeTopic(client, TOPIC_SUBSCRIBE);

	while (!subscribed) {
		usleep(100000L);
	}

	sendMessage(client, TOPIC_DISCOVERY, DISCOVERY_PAYLOAD, strlen(DISCOVERY_PAYLOAD), 1);

	/* Lock memory */
	if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
		printf("WARNING: mlockall failed.\n");
	}

	/* We are all that matters :) */
	rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
	if (rc != 0) {
		printf("WARNING: sched_setscheduler failed, err=%d.\n", rc);
	}

	signal(SIGINT, intHandler);

	while (!finished) {
		int64_t t = getTime();

		char buf[256];
		int n = snprintf(buf, sizeof(buf), "%" PRId64 "", t);

		printf("%" PRId64 ": Sending message %s...\n", getTime(), buf);
		sendMessage(client, TOPIC_STATE, buf, n, 0);
		sentTime = t;

		usleep(SAMPLE_PERIOD * 1000);
	}
	printf("\n\n");
	if (samples) {
		printf("Samples:   %7d\n", samples);
		printf("Average:   %7" PRId64 " us\n", overallTime / samples);
		printf("Max time   %7" PRId64 " us\n", maxTime);
		printf("Min time   %7" PRId64 " us\n", minTime);
	}

	MQTTAsync_destroy(&client);
 	return rc;
}

