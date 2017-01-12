# API Definition of Dasudian IoT DataHub C SDK

## General Info
These are the C APIs which use protocol MQTT to tranfer massively real-time data to Dasudian IoT Cloud

Most APIs are asynchronous.

You can publish the infomation collecting from a device to the cloud. Also you can subscribe to
a topic, receive a message and handle the message.

To use the SDK:

1.  Create a client object

2.  Set or not set the options for connecting to a cloud instance

3.  Subsrcibe to topics depending on whether the client needs to receive messages

4.  Repeatedly publishing messages or handling messages

5.  Disconnect the client object

## create client of datahub

```
/*
 * description:This function creates a client which is ready to connect to cloud. You can
 *          use default options or set your own's.
 * parameter:
 *      client: a valid client object returned by this function after being successfully created.
 *              Note:Can not be NULL.
 *      instance_id: a valid id for connecting to cloud of dasudian, provided by Dasudian
 *              Note:Can not be NULL.
 *      instance_key: a secure key for connecting to cloud of dasudian, provided by Dasudian
 *              Note:Can not be NULL
 *      client_name: name of a device/client, usually describe the device/client
 *              Note:Can not be NULL
 *      client_id: id of device/client, uniquely identifying a device/client
 *              Note:Can not be NULL
 *      options: options of MQTT. See structure datahub_options for details. If you don't want
 *              to set them, just pass NULL. If you want to set some options, please use
 *              DATAHUB_OPTIONS_INITIALIZER to initialize first.
 *              Note:Can be NULL
 *return value:
 *      DE_OK is returned when success,otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_create(datahub_client *client,
        char *instance_id, char *instance_key, char *client_name, char *client_id,
        datahub_options *options);
```

## connect to cloud

```
/*
 * description:
 *      let client connect to cloud of Dasudian.
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success, otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_connect(datahub_client *client);
```

## get the status of client

```
/*
 * description:
 *      get the status of client
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DATAHUB_TRUE means connected and DATAHUB_FALSE means disconnected
 */
extern int datahub_isconnected(datahub_client *client);
```

## send messages (asynchronous)

```
/*
 * description:
 *      publish messages asynchronously
 * parameter:
 *      client: returned by datahub_create()
 *          Note:can not be NULL
 *      topic: name of topic the message belongs to or regards
 *          Note:can not be NULL
 *      msg: define a message, use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specfy your own data and length of data.
 *          Note: can not be NULL
 *      dt: a token representing a message being delivered, returned by this function.
 *          If you do not care whether this message arrives cloud, set it as NULL.
 *          Otherwise, this token should be defined and used in delivered callback.See
 *          option msg_delivered_cb for details.
 *          Note:can be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_publish(datahub_client *client, char *topic,
        datahub_message *msg, datahub_delivery_token *dt);
```
## send messages (synchronous)

```
/*
 * description:
 *      send messages synchronously
 *      Note: program will block untill message is delivered
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 *      topic: name of topic the message belongs to or regards
 *          Note: can not be NULL
 *      msg: define a message, use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specfy your own data and length of data.
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_sendrequest(datahub_client *client, char *topic, datahub_message *msg);
```

## subscribe to a topic (synchronous)

```
/*
 * description:
 *      subscribe to a topic(synchronously only)
 * parameter:
 *      client: returned by datahub_create()
 *          Note:can not be NULL
 *      topic: name of topic you want to subscribe to
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success, otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_subscribe(datahub_client *client, char *topic);
```

## unsubscribe a topic(synchronous)

```
/*
 * description:
 *      unsubscribe a topic(synchronously only)
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 *      topic: name of topic you want to unsubscribe
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_unsubscribe(datahub_client *client, char *topic);
```

## close session(synchronous)

```
/*
 * description:
 *      close session
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_disconnect(datahub_client *client);
```

## free memory of message

```
/*
 * description:
 *      free the memory the message occupies
 *      only useful in callback of receiving a message
 * parameter:
 *     msg: describing a message
 *     Note: can not be NULL
 * return value:
 *      none
 */
extern void datahub_message_free(datahub_message *msg);
```
