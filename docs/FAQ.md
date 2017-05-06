# Linux可能遇到的问题

默认使用gcc编译器

Q:
```
main.c:(.text+0x49): undefined reference to `datahub_create'
main.c:(.text+0x86): undefined reference to `datahub_connect'
main.c:(.text+0xf1): undefined reference to `datahub_sendrequest'
main.c:(.text+0x124): undefined reference to `datahub_disconnect'
main.c:(.text+0x137): undefined reference to `datahub_disconnect'
```

A: 需要添加链接选项 -ldasudian-datahub-sdk

Q:
```
/usr/bin/ld: cannot find -ldasudian-datahub-sdk
```

A: 需要指明SDK库所在的路径: -L lib

Q:
```
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_subscribe'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_disconnect'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_setCallbacks'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_publishMessage'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_isConnected'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_connect'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_unsubscribe'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_destroy'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_free'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_waitForCompletion'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_create'
lib//libdatahub-sdk.so: undefined reference to `MQTTClient_freeMessage'
```

A: 需要链接库paho-mqtt3cs: -lpaho-mqtt3cs

Q:
```
./main: error while loading shared libraries: libdasudian-datahub-sdk.so: cannot open shared object file: No such file or directory
```

A: 需要指明运行时SDK库所在的路径: -Wl,-rpath=lib
