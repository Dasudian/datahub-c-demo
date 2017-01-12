/*
 * unsubscribe a topic
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

int main()
{
    datahub_client client;
    int ret;

    /* create a client object */
    ret = datahub_create(&client,
            INSTANCE_ID, INSTANCE_KEY, USER_NAME, CLIENT_ID,
            NULL);
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

    /* unsubscribe a topic */
    ret = datahub_unsubscribe(&client, topic);
    if (DE_OK != ret) {
        fprintf(stdout, "unsubscribed topic [ %s ] failed\n", topic);
        return 1;
    }

    /* disconnect this session and free memory */
    datahub_disconnect(&client);

    return 0;
}
