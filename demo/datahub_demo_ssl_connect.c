/*
 * =====================================================================================
 *
 *       Filename:  test_datahub_connect.c
 *
 *    Description:  test API datahub_connect()
 *
 *        Version:  1.0
 *        Created:  12/28/2016 07:04:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  eden(pengquan.wang@dasudian.com)
 *   Organization:  www.dasudian.com
 *
 * =====================================================================================
 */

#include "datahub_sdk_c.h"
#include <stdio.h>

#define TEST_INSTANCE_ID    "dsd_9FmYSNiqpFmi69Bui0_A"
#define TEST_INSTANCE_KEY   "238f173d6cc0608a"
#define TEST_USER_NAME      "iot"
#define TEST_CLIENT_SSL_CON  "5"

static void tdc_ssl_connect();

int main()
{
   tdc_ssl_connect();
}


static void tdc_parse_return_code(int ret_code)
{
    if (DE_OK == ret_code) {
        fprintf(stdout, "OK\n");
    } else if (DE_SERVER_UNAVAIL == ret_code) {
        fprintf(stdout, "SERVER UNAVAIL\n");
    } else if (DE_BAD_UNAME_PASSWD == ret_code) {
        fprintf(stdout, "BAD UNAME PASSWD\n");
    } else if (DE_UNATHOR == ret_code) {
        fprintf(stdout, "UNATHOR\n");
    } else if (DE_AUTHO_SERVER_UNAVAIL == ret_code) {
        fprintf(stdout, "authorize server is unavailable\n");
    } else if (DE_CON_FAIL == ret_code) {
        fprintf(stdout, "connect failed\n");
    } else if (DE_CONNECT_OTHER_ERROR == ret_code) {
        fprintf(stdout, "OTHER\n");
    } else {
        fprintf(stdout, "undefined code [ %d ]\n", ret_code);
    }
}

static void tdc_ssl_connect()
{
    datahub_client client;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;
    int ret;

    options.server_url = "ssl://try.iotdatahub.net:8883";

//    options.server_url = "tcp://try.iotdatahub.net:1883";

    /*  create */
    ret = datahub_create(&client,
            TEST_INSTANCE_ID, TEST_INSTANCE_KEY, TEST_USER_NAME, TEST_CLIENT_SSL_CON,
            &options);
    if (DE_OK != ret) {
        fprintf(stderr, "datahub_create() wrong\n");
        return;
    }

    ret = datahub_connect(&client);
    tdc_parse_return_code(ret);
}
