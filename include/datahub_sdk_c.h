/**
 * @mainpage IoT Datahub C SDK
 *
 * @brief SDK提供了一个简单的基于MQTT协议与服务器交互的方法<br>
 *
 * SDK基于MQTT协议,传输实时的消息到大数点IoT云服务器<br>
 * 你可以收集设备上的数据发送到云上;也可以订阅某个topic,来接收云服务器推送的
 * 消息<br>
 * <br>
 * 如何使用SDK:<br>
 * <b>1</b>: 创建一个客户端实例<br>
 * <b>2</b>: 如果想接收消息,那么就订阅某个topic<br>
 * <b>3</b>: 或者发送消息到服务器<br>
 * <b>4</b>: 退出时,销毁该客户端<br>
 * <br>
 * 功能:<br>
 * <b>1</b>: 当连接丢失时,SDK会尝试自动重连。如果连接失败,下一次重连将会在1s,2s,4s,8s,
 * 16s,1s,2s,4s,8s,16s,1s,...后再次尝试,最大重连间隔为16秒<br>
 * <b>2</b>: SDK为线程安全<br>
 * <b>3</b>: 一个进程只能创建一个客户端, 如果需要多个客户端, 需要创建多个进程<br>
 */

#ifndef DATAHUB_SDK_C_H
#define DATAHUB_SDK_C_H

#include <stdlib.h>
#include <string.h>

/**
 *  真值
 */
#define DATAHUB_TRUE 1
/**
 *  假值
 */
#define DATAHUB_FALSE 0

#define DEFAULT_SERVER_URL      "tcp://try.iotdatahub.net:1883"
#define DEFAULT_DEBUG_OPT       DATAHUB_FALSE
#define DEFAULT_CLEANSESSION    DATAHUB_FALSE
#define DEFAULT_CONTEXT         NULL

/**
 * 选项的初始化宏
 */
#define DATAHUB_OPTIONS_INITIALIZER {\
DEFAULT_SERVER_URL,\
DEFAULT_DEBUG_OPT,\
DEFAULT_CLEANSESSION,\
DEFAULT_CONTEXT,\
}

/**
 * 消息的服务质量
 */
enum qos_s {
    /**
     * 客户端最多发送一次, 服务器最多接收一次
     */
    QoS0 = 0,
    /**
     * 客户端至少发送一次, 服务器可能接收多次
     */
    QoS1 = 1,
    /**
     * 客户端至少发送一次, 但保证服务器只接收到一次消息
     */
    QoS2 = 2,
};

/**
 * 消息的初始化宏,只包含字符串结尾符'\0'
 */
#define DATAHUB_MESSAGE_INITIALIZER {1, ""}

/**
 * 客户端的类型
 */
typedef void* datahub_client;

/**
 * @brief 消息的结构体类型
 */
typedef struct datahub_message_s {
    /**
     * 消息长度,必须大于0
     */
    unsigned int payload_len;
    /**
     * 发送消息的起始地址
     */
    void *payload;
} datahub_message;

/**
 * 错误码
 */
enum datahub_error_code_s {
    /**
     * 成功
     */
    ERROR_NONE = 0,
    /**
     * 某些参数不合法
     */
    ERROR_ILLEGAL_PARAMETERS = -1,
    /**
     * 客户端未连接服务器
     */
    ERROR_DISCONNECTED = -2,
    /**
     * MQTT服务器不支持当前使用的协议版本号,请联系开发人员
     */
    ERROR_UNACCEPT_PROTOCOL_VERSION = -3,
    /**
     * client_id不可用,可能使用了不支持的字符或者client_id的长度太大
     */
    ERROR_IDENTIFIER_REJECTED = -4,
    /**
     *  服务器不可用
     */
    ERROR_SERVER_UNAVAILABLE = -5,
    /**
     * instance_id 或者instance_key不正确,请检查或者联系客服人员
     */
    ERROR_BAD_USERNAME_OR_PASSWD = -6,
    /**
     * 未被授权
     */
    ERROR_UNAUTHORIZED = -7,
    /**
     * 验证服务器不可用
     */
    ERROR_AUTHORIZED_SERVER_UNAVAILABLE = -8,
    /**
     * 操作失败
     */
    ERROR_OPERATION_FAILURE = -9,
    /**
     * 消息过长
     */
    ERROR_MESSAGE_TOO_BIG = -10,
    /**
     * 网络不可用
     */
    ERROR_NETWORK_UNREACHABLE = -11,
    /**
     * 同步超时
     */
    ERROR_TIMEOUT = -12,
    /**
     * 内存申请失败
     */
    ERROR_MEMORY_ALLOCATE = -100,
    /**
     * 不合法的JSON字符串
     */
    ERROR_MESSAGE_INVALID_JSON = -101,
    /**
     * 不合法的设备类型字符;设备类型不能包含竖线"|", 也不能以下划线"_"开头
     */
    ERROR_INVALID_CLIENT_TYPE = -102
};

/**
 * 数据类型
 */
typedef enum datahub_data_type_s {
    /** 数据为JSON格式 */
    JSON = 0,
    /** 数据为文本/字符串 */
    TEXT = 1,
    /** 数据为二进制 */
    BINARY = 2,
    DATA_TYPE_END
}datahub_data_type;

/**
 * @brief 接收到消息后的回调函数
 *
 * 例子:<br>
@code
void message_received_callback(void *context, char *topic,
     datahub_message *msg)
{
    //做一些操作
    //...

    //释放topic和msg占用的内存
    datahub_callback_free(topic, msg);
}
 * @endcode
 * @param context 即datahub_options_s中的context
 * @param topic 本次消息所属的主题,需要调用datahub_callback_free()手动释放内存
 * @param msg 存放消息的结构体,需要调用datahub_callback_free()手动释放内存

 */
typedef void MESSAGE_RECEIVED(void *context, char *topic, datahub_message *msg);

/**
 * @brief 当客户端的状态发生改变(连接上服务器或者从服务器断开)的时候,SDK会通知用户
 *
 * @param context 即datahub_options_s中的context
 * @param isconnected 连接状态, DATAHUB_FALSE 表示从服务器断开,DATAHUB_TRUE 表示连接上服务器
 */
typedef void CONNECTION_STATUS_CHANGED(void *context, int isconnected);

/**
 * @brief 设置选项
 */
typedef struct datahub_options_s {
    /**
     * @brief 设置使用哪种协议(ssl, 普通的tcp)连接服务器, 以及设置服务器的地址和端口号.
     *
     * "协议：//服务器地址：端口号". 协议支持普通的tcp协议和加密的ssl协议; 服务器地址和端口号
     * 由大数点提供.<br>
     * 默认值为DEFAULT_SERVER_URL<br>
     */
    char *server_url;

    /**
     * @brief 开启调试选项
     *
     * DATAHUB_TRUE表示开启调试选项<br>
     * DATAHUB_FALSE表示关闭调试选项<br>
     * 默认值为DATAHUB_FALSE<br>
     */
    int debug;

    /**
     * @brief 是否保存客户端的会话信息.
     *
     * cleansession为DATAHUB_FALSE, 当客户端断线或下线后, 保存客户端订阅的topic
     * 和发送给客户端的所有消息.<br>
     * cleansession为DATAHUB_TRUE, 当客户端断线或下线后, 不保留客户端订阅的
     * topic和发送给客户端的任何消息.<br>
     * 默认值为DATAHUB_FALSE<br>
     */

    int cleansession;

    /**
     * @brief 传递给回调函数message_received()和connection_status_changed()的
     * 参数, 对应回调函数的第一个参数context
     *
     * 默认值为NULL
     */
    void *context;

    /**
     * @brief 接收到消息后的回调函数
     *
     * NULL或者函数指针, 默认值为DEFAULT_CALLBACK
     */
    MESSAGE_RECEIVED *message_received;

    /**
     * @brief 当客户端的状态发生改变(连接上服务器或者从服务器断开)的时候,SDK会通知用户
     *
     * NULL或者函数指针, 默认值为DEFAULT_CALLBACK
     */
    CONNECTION_STATUS_CHANGED *connection_status_changed;
}datahub_options;

/**
 * @brief 该函数创建一个客户端实例,该实例可用于连接大数点MQTT服务器.
 *
 * @param client 如果函数成功调用,则会返回一个客户端实例.<br>
 * 注意:不能为空<br>
 * @param instance_id 用于连接大数点服务器的唯一标识,由大数点提供.<br>
 * 注意:不能为空<br>
 * @param instance_key 用于连接大数点服务器的密码,由大数点提供.<br>
 * 注意:不能为空<br>
 * @param client_type 设备类型. 如传感器"sensor", 充电桩"charging_pile",
 * 车载电池"car_battery"<br>
 * 注意:可以为空<br>
 * @param client_id 设备的id, 用于服务器唯一标记一个设备<br>
 * 注意：不同的设备client id必须不同,如果两个设备有相同的id,服务器会关
 * 掉其中一个连接<br>
 * 可以使用设备的mac地址,或者第三方账号系统的id(比如qq号,微信号);如果没
 * 有自己的账号系统,则可以随机生成一个不会重复的id.或者自己指定设备的
 * client id,只要能保证不同客户端id不同即可.<br>
 * client_id不能为空<br>
 * @param options MQTT的选项.具体包含的选项可以查看datahub_options_s结构体.如果
 * 不想设置选项,请传递NULL.如果你想设置某些选项,先使用
 * DATAHUB_OPTIONS_INITIALIZER初始化.<br>
 * 注意:可以为空<br>
 * @return ERROR_NONE 表示成功,其他表示错误.<br>
 * 其他错误码请查看开发文档<br>
 */
extern int datahub_create(datahub_client *client,
        char *instance_id, char *instance_key, char *client_type, char *client_id,
        datahub_options *options);

/**
 * @brief 同步发送消息
 *
 * 注意：程序会阻塞<br>
 * @param client 由函数datahub_create()成功返回的客户端实例.<br>
 * 注意:不能为空.<br>
 * @param topic 消息对应的topic.如果消息发送前有另一个客户端已经订阅该topic,则
 * 另一个客户端就会收到消息.<br>
 * 注意:不能为空<br>
 * @param msg 发送的消息,使用前请使用DATAHUB_MESSAGE_INITIALIZER初始化.<br>
 * 注意：消息的长度必须小于512K,否则会发生错误.<br>
 * 注意:不能为空<br>
 * @param data_type 数据类型，只能为JSON或TEXT或BINARY
 * @param qos 消息的服务质量.<br>
 * <b>0</b>: 消息可能到达,也可能不到达.<br>
 * <b>1</b>: 消息一定会到达,但可能会重复,当然,前提是返回ERROR_NONE.<br>
 * <b>2</b>: 消息一定会到达,且只到达一次,当然,前提是返回ERROR_NONE.<br>
 * 注意：只能为0,1,2三者中的一个,其他为非法参数<br>
 * @param timeout 函数阻塞的最大时间.<br>
 * 注意：这是函数阻塞的最大时间,不是消息的超时时间<br>
 * @return
 * <b>ERROR_NONE</b>: 表示成功,消息一定发送出去.<br>
 * <b>ERROR_TIMEOUT</b>: 表示阻塞等待时间的最大值已到,但是消息可能发送给服务器,也
 * 可能未发送.如果想确保消息一定发送出去,请根据消息大小和网络状况设置较大的阻塞等
 * 待时间.<br>
 * 其他错误码请查看开发文档<br>
 */

extern int datahub_sendrequest(datahub_client *client, char *topic,
        datahub_message *msg, datahub_data_type data_type,
        int qos, int timeout);

/**
 * @brief 同步订阅某一个topic
 *
 * 注意：程序会阻塞<br>
 * @param client 由函数datahub_create()成功返回的客户端实例<br>
 * 注意:不能为空<br>
 * @param topic 订阅的topic<br>
 * 注意:不能为空<br>
 * @param qos 订阅消息的服务质量(发送消息的QoS和订阅消息的QoS共同决定服务器下发消息
 * 的QoS)<br>
 * <b>0</b>: 消息可能到达,也可能不到达<br>
 * <b>1</b>: 消息一定会到达,但可能会重复,当然,前提是返回ERROR_NONE<br>
 * <b>2</b>: 消息一定会到达,且只到达一次,当然,前提是返回ERROR_NONE<br>
 * @param timeout 函数阻塞的最大时间.<br>
 * 注意：这是函数阻塞的最大时间<br>
 * @return
 * <b>ERROR_NONE</b>: 表示成功,其他表示错误.<br>
 * <b>ERROR_TIMEOUT</b>: 表示阻塞等待时间的最大值已到,但是可能订阅成功,也
 * 可能订阅失败.如果想确保订阅一定成功,请根据消息大小和网络状况设置较大的
 * 阻塞等待时间.<br>
 * 其他错误码请查看开发文档<br>
 */
extern int datahub_subscribe(datahub_client *client, char *topic, int qos, int timeout);

/**
 * @brief 同步取消订阅某一个topic
 *
 * @param client 由函数datahub_create()成功返回的客户端实例<br>
 * 注意:不能为空<br>
 * @param topic 取消订阅的topic<br>
 * 注意:不能为空<br>
 * @param timeout 函数阻塞的最大时间.<br>
 * 注意：这是函数阻塞的最大时间<br>
 * @return
 * <b>ERROR_NONE</b>: 表示成功,其他表示错误.<br>
 * <b>ERROR_TIMEOUT</b>: 表示阻塞等待时间的最大值已到,但是可能取消成功,也
 * 可能取消失败.如果想确保取消一定成功,请根据消息大小和网络状况设置较大的
 * 阻塞等待时间.<br>
 * 其他错误码请查看开发文档<br>
 */
extern int datahub_unsubscribe(datahub_client *client, char *topic, int timeout);

/**
 * @brief 销毁客户端并断开连接
 *
 * @param client 由函数datahub_create()成功返回的客户端实例<br>
 * 注意:不能为空<br>
 * @return
 * 无<br>
 */
extern void  datahub_destroy(datahub_client *client);

/**
 * @brief 接收函数中,主题和消息占用的内存需要用户手动释放
 *
 * @param topic 返回的主题
 * @param msg 返回的消息
 * @return
 *  无
 */
extern void datahub_callback_free(char *topic, datahub_message *msg);

#endif
