/*
 * 此文件是大数点C SDK的头文件. C SDK提供了一个简单的基于MQTT协议与服务器交互的方法.
 * 请享受编程的乐趣
 */
#ifndef DATAHUB_SDK_C_H
#define DATAHUB_SDK_C_H

#include <stdlib.h>
#include <string.h>

/* 真值 */
#define DATAHUB_TRUE 1
/* 假值 */
#define DATAHUB_FALSE 0

#define DEFAULT_SERVER_URL      "tcp://try.iotdatahub.net:1883"
#define DEFAULT_DEBUG_OPT       DATAHUB_FALSE
#define DEFAULT_CLEANSESSION    DATAHUB_FALSE
#define DEFAULT_CONTEXT         NULL

/* 选项的初始化宏 */
#define DATAHUB_OPTIONS_INITIALIZER {\
DEFAULT_SERVER_URL,\
DEFAULT_DEBUG_OPT,\
DEFAULT_CLEANSESSION,\
DEFAULT_CONTEXT,\
}

/* 消息的服务质量 */
enum qos_s {
    QoS0 = 0,
    QoS1 = 1,
    QoS2 = 2,
};

/* 消息的初始化宏,只包含字符串结尾符'\0' */
#define DATAHUB_MESSAGE_INITIALIZER {\
1,\
""\
}

#define DATAHUB_DT_INITIALIZER {0}

/* 客户端的类型 */
typedef void* datahub_client;

/*
 * 描述: 消息的结构体类型
 */
typedef struct datahub_message_s {
    /* 消息长度，必须大于0 */
    unsigned int payload_len;
    /* 发送消息的起始地址 */
    void *payload;
} datahub_message;

enum datahub_error_code_s {
    /*
     * 返回码: 成功
     */
    ERROR_NONE = 0,
    /*
     * 返回码: 某些参数不合法
     */
    ERROR_ILLEGAL_PARAMETERS = -1,
    /*
     * 返回码: 客户端未连接服务器
     */
    ERROR_DISCONNECTED = -2,
    /*
     * 返回码: MQTT服务器不支持当前使用的协议版本号,请联系开发人员
     */
    ERROR_UNACCEPT_PROTOCOL_VERSION = -3,
    /*
     * 返回码: client_id不可用,可能使用了不支持的字符或者client_id的长度太大
     */
    ERROR_IDENTIFIER_REJECTED = -4,
    /*
     * 返回码: 服务器不可用
     */
    ERROR_SERVER_UNAVAILABLE = -5,
    /*
     * 返回码: instance_id 或者instance_key不正确,请检查或者联系客服人员
     */
    ERROR_BAD_USERNAME_OR_PASSWD = -6,
    /*
     * 返回码: 未被授权
     */
    ERROR_UNAUTHORIZED = -7,
    /*
     * 返回码: 验证服务器不可用
     */
    ERROR_AUTHORIZED_SERVER_UNAVAILABLE = -8,
    /*
     * 返回码: 操作失败
     */
    ERROR_OPERATION_FAILURE = -9,
    /*
     * 返回码: 消息过长
     */
    ERROR_MESSAGE_TOO_BIG = -10,
    /*
     * 返回码: 网络不可用
     */
    ERROR_NETWORK_UNREACHABLE = -11,
    /*
     * 返回码: 同步超时
     */
    ERROR_TIMEOUT = -12,
    /*
     * 返回码: 内存申请失败
     */
    ERROR_MEMORY_ALLOCATE = -100,
};

/*
 * 描述： 接收到消息后的回调函数
 * 参数：
 *      context: 即datahub_options中的'context'
 *      topic: 本次消息所属的主题,需要调用datahub_callback_free()手动释放内存
 *      msg: 存放消息的结构体,需要调用datahub_callback_free()手动释放内存
 * 例子:
 *      void message_received_callback(void *context, char *topic, datahub_message *msg)
 *      {
 *          //做一些操作
 *          //...
 *
 *          //释放topic和msg占用的内存
 *          datahub_callback_free(topic, msg);
 *      }
 */
typedef void MESSAGE_RECEIVED(void *context, char *topic, datahub_message *msg);

/*
 * 描述： 当客户端的状态发生改变(连接上服务器或者从服务器断开)的时候，SDK会通知用户
 * 参数：
 *      context: 即datahub_options中的'context'
 *      isconnected: 连接状态， DATAHUB_FALSE 表示从服务器断开，DATAHUB_TRUE 表示连接上服务器
 */
typedef void CONNECTION_STATUS_CHANGED(void *context, int isconnected);

typedef struct datahub_options_s {
    /*
     * 描述: 设置使用哪种协议(ssl, 普通的tcp)连接服务器, 以及设置服务器的地址和端口号.
     * 值:
     *     "协议：//服务器地址：端口号". 协议支持普通的tcp协议和加密的ssl协议; 服务器地址和端口号
     *      由大数点提供. 默认值为DEFAULT_SERVER_URL
     */
    char *server_url;

    /*
     * 描述：开启调试选项
     * 值：
     *      DATAHUB_TRUE表示开启调试选项;DATAHUB_FALSE表示关闭调试选项,默认值为DATAHUB_FALSE
     */
    int debug;

    /*
     * 描述: 是否保存客户端的会话信息.
     *     cleansession为DATAHUB_FALSE, 当客户端断线或下线后, 保存客户端订阅的topic和发送给客户端的所有消息.
     *     cleansession为DATAHUB_TRUE, 当客户端断线或下线后, 不保留客户端订阅的topic和发送给客户端的任何消息.
     * 值: 
     *     默认为DATAHUB_FALSE
     */

    int cleansession;

    /*
     * 描述:
     *      传递给回调函数message_received()和connection_status_changed()的参数, 对应回调函数的第一个参数
     *      context
     * 值:
     *      默认值为NULL
     */
    void *context;

    /*
     * 描述： 接收到消息后的回调函数
     * 值:
     *      NULL或者函数指针, 默认值为DEFAULT_CALLBACK
     */
    MESSAGE_RECEIVED *message_received;

    /*
     * 描述： 当客户端的状态发生改变(连接上服务器或者从服务器断开)的时候，SDK会通知用户
     * 值:
     *      NULL或者函数指针, 默认值为DEFAULT_CALLBACK
     */
    CONNECTION_STATUS_CHANGED *connection_status_changed;
}datahub_options;

/*
 * 描述: 该函数创建一个客户端实例，该实例可用于连接大数点MQTT服务器
 * 参数:
 *    client: 如果函数成功调用，则会返回一个客户端实例
 *          注意: 不能为空
 *    instance_id: 用于连接大数点服务器的唯一标识，由大数点提供
 *          注意: 不能为空
 *    instance_key: 用于连接大数点服务器的密码，由大数点提供
 *          注意: 不能为空
 *    client_name: 设备的名字
 *          注意: 不能为空
 *    client_id: 设备的id
 *          注意: 不能为空；一个客户可以与服务器建立多条连接，每条连接由instance_id和
 *          client_id唯一确定
 *    options: MQTT的选项。具体包含的选项可以查看datahub_options结构体.如果不想设置选项，
 *          请传递NULL。如果你想设置某些选项，先使用DATAHUB_OPTIONS_INITIALIZER初始化
 *          注意:可以为空
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。
 *      其他错误码请查看开发文档API.md
 */
extern int datahub_create(datahub_client *client,
        char *instance_id, char *instance_key, char *client_name, char *client_id,
        datahub_options *options);

/*
 * 描述: 发送消息(异步)
 *  注意：异步操作不阻塞线程，但不能保证消息发送成功，适用于对时间敏感，对消息成功与否
 *   不敏感的应用
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 消息对应的topic。如果消息发送前有另一个客户端已经订阅该topic，则
 *          另一个客户端就会收到消息。
 *          注意: 不能为空
 *    msg: 发送的消息,使用前请使用DATAHUB_MESSAGE_INITIALIZER初始化.注意：消息
 *         的长度必须小于512K，否则会发生错误
 *          注意: 不能为空
 *    QoS: 消息的服务质量
 *          0   消息可能到达，也可能不到达
 *          1   消息一定会到达，但可能会重复，当然，前提是返回ERROR_NONE
 *          2   消息一定会到达，且只到达一次，当然，前提是返回ERROR_NONE
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。
 *      其他错误码请查看开发文档API.md
 *
 */

extern int datahub_publish(datahub_client *client, char *topic,
        datahub_message *msg, int QoS);

/*
 * 描述: 发送消息(同步)
 *  注意：程序会阻塞
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 消息对应的topic。如果消息发送前有另一个客户端已经订阅该topic，则
 *          另一个客户端就会收到消息。
 *          注意: 不能为空
 *    msg: 发送的消息,使用前请使用DATAHUB_MESSAGE_INITIALIZER初始化.注意：消息
 *         的长度必须小于512K，否则会发生错误
 *          注意: 不能为空
 *    QoS: 消息的服务质量
 *          0   消息可能到达，也可能不到达
 *          1   消息一定会到达，但可能会重复，当然，前提是返回ERROR_NONE
 *          2   消息一定会到达，且只到达一次，当然，前提是返回ERROR_NONE
 *         注意：只能为0,1,2三者中的一个，其他为非法参数
 *    timeout: 函数阻塞的最大时间。
 *          注意：这是函数阻塞的最大时间，不是消息的超时时间
 * 返回值:
 *      ERROR_NONE 表示成功，消息一定发送出去。
 *      ERROR_TIMEOUT 表示阻塞等待时间的最大值已到，但是消息可能发送给服务器，也
 *          可能未发送。如果想确保消息一定发送出去，请根据消息大小和网络状况设置
 *          较大的阻塞等待时间.
 *      其他错误码请查看开发文档API.md
 *
 */

extern int datahub_sendrequest(datahub_client *client, char *topic, datahub_message *msg, int QoS, int timeout);

/*
 * 描述: 同步订阅某一个topic
 *  注意：程序会阻塞
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 订阅的topic
 *          注意: 不能为空
 *    QoS: 订阅消息的服务质量(发送消息的QoS和订阅消息的QoS共同决定服务器下发消息
 *          的QoS)
 *       0   消息可能到达，也可能不到达
 *       1   消息一定会到达，但可能会重复，当然，前提是返回ERROR_NONE
 *       2   消息一定会到达，且只到达一次，当然，前提是返回ERROR_NONE
 *    timeout: 函数阻塞的最大时间。
 *          注意：这是函数阻塞的最大时间
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。
 *      ERROR_TIMEOUT 表示阻塞等待时间的最大值已到，但是可能订阅成功，也
 *          可能订阅失败。如果想确保订阅一定成功，请根据设置较大的阻塞等
 *          待时间.
 *      其他错误码请查看开发文档API.md
 */
extern int datahub_subscribe(datahub_client *client, char *topic, int qos, int timeout);

/*
 * 描述: 同步取消订阅某一个topic
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 *    topic: 取消订阅的topic
 *          注意: 不能为空
 *    timeout: 函数阻塞的最大时间。
 *          注意：这是函数阻塞的最大时间
 * 返回值:
 *      ERROR_NONE 表示成功，其他表示错误。
 *      ERROR_TIMEOUT 表示阻塞等待时间的最大值已到，但是可能取消成功，也
 *          可能取消失败。如果想确保取消一定成功，请根据设置较大的阻塞等
 *          待时间.
 *      其他错误码请查看开发文档API.md
 */
extern int datahub_unsubscribe(datahub_client *client, char *topic, int timeout);

/*
 * 描述: 销毁客户端并断开连接
 * 参数:
 *    client: 由函数datahub_create()成功返回的客户端实例
 *          注意: 不能为空
 * 返回值:
 *    无返回值
 */
extern void  datahub_destroy(datahub_client *client);

/*
 * 描述：接收函数中，主题和消息占用的内存需要用户手动释放
 * 参数：
 *  topic: 返回的主题
 *  msg: 返回的消息
 * 返回值:
 *  无
 */
extern void datahub_callback_free(char *topic, datahub_message *msg);

#endif
