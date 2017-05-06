LIB_DIR=../lib
CFLAGS=-L ${LIB_DIR} \
	   -ldasudian-datahub-sdk \
	   -lpaho-mqtt3cs \
	   -lssl \
	   -lcrypto \
	   -Wl,-rpath=${LIB_DIR} \
	   -lpthread \

HEADER_DIR=../include
CHFLAGS=-I${HEADER_DIR}

VPATH=..

DEBUG=

test_targets=\
			test_datahub_demo_send_syn \
			test_datahub_demo_send_asyn \
			test_datahub_demo_subscribe \
			test_datahub_demo_unsubscribe \
			test_datahub_demo_ssl_connect \

test_demo:${test_targets}
	./test_datahub_demo.sh ./test_datahub_demo_send_syn
	./test_datahub_demo.sh ./test_datahub_demo_send_asyn
	./test_datahub_demo.sh ./test_datahub_demo_subscribe
	./test_datahub_demo.sh ./test_datahub_demo_unsubscribe
	./test_datahub_demo.sh ./test_datahub_demo_ssl_connect
	touch test_demo

test_%:%.c
	gcc $^ -o $@ ${CFLAGS} ${CHFLAGS} ${DEBUG}

clean:
	rm -f *.o
	rm -f test_demo ${test_targets}
