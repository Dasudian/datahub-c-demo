/*
 * subscribe a topic
 */

#include "datahub_sdk_c.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* instance id, gived by dasudian */
#define INSTANCE_ID    "dsd_9FmYSNiqpFmi69Bui0_A"
/* instance key, gived by dasudian */
#define INSTANCE_KEY   "238f173d6cc0608a"
/* name of device */
#define USER_NAME      "sensor1"
/* id of device */
#define CLIENT_ID      "1"

static char *topic = "topic";

static char *send_message = {
    "hello world"
};

static int g_exit = 0;

/* called when receiving a message */
void msg_recv_cb(void *context, char *topic_name, datahub_message *msg)
{
    /* print message */
    fprintf(stdout, "%s\n%s\n%d\n", topic_name, (char *)msg->payload, msg->payload_len);

    /* free memory of topic */
    free(topic_name);
    /* free memory of message */
    datahub_message_free(msg);

    /* main thread can exit */
    g_exit = 1;
}

int main()
{
    datahub_client client;
    int ret;
    datahub_message msg = DATAHUB_MESSAGE_INITIALIZER ;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;

    /* set callback when receiving a message */
    options.msg_received_cb = msg_recv_cb;

    /* create a client object */
    ret = datahub_create(&client,
            INSTANCE_ID, INSTANCE_KEY, USER_NAME, CLIENT_ID,
            &options);
    if (DE_OK != ret) {
        fprintf(stdout, "create client failed\n");
        return 1;
    }

    /* client connects to server */
    ret = datahub_connect(&client);
    if (DE_OK != ret) {
        fprintf(stdout, "client connects to server failed\n");
        return 1;
    }

    /* subscribe a topic */
    ret = datahub_subscribe(&client, topic);
    if (DE_OK != ret) {
        fprintf(stdout, "subscribed topic [ %s ] failed\n", topic);
        return 1;
    }

    /* message to send */
    msg.payload = send_message;
    /* len of message */
    msg.payload_len = strlen(send_message) + 1;//including '\0'

    ret = datahub_publish(&client, topic, &msg, NULL);
    if (DE_OK != ret) {
        fprintf(stdout, "asynchronously send a message failed\n");
        return 1;
    }

    /* wait g_exit */
    while(0 == g_exit) {};

    /* disconnect this session and free memory */
    datahub_disconnect(&client);

    return 0;
}
