#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <MQTTAsync.h>

#include "config.h"

#define CERT_PATH PROJECT_SOURCE_DIR"/material/tls/"

#define ADDRESS     "ssl://link.router7.com:8883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "gateway/test0"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTAsync_token deliveredtoken;
int disc_finished = 0;
int subscribed = 0;
int finished = 0;

int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}
void onDisconnect(void* context, MQTTAsync_successData* response)
{
        printf("Successful disconnection\n");
        disc_finished = 1;
}
void onSubscribe(void* context, MQTTAsync_successData* response)
{
        printf("Subscribe succeeded\n");
        subscribed = 1;
}
void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Subscribe failed, rc %d\n", response ? response->code : 0);
        finished = 1;
}

void onConnect(void* context, MQTTAsync_successData* response)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
        int rc;
        printf("Successful connection\n");
        printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
        opts.onSuccess = onSubscribe;
        opts.onFailure = onSubscribeFailure;
        opts.context = client;
        deliveredtoken = 0;
        if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start subscribe, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;
    printf("\nConnection failure\n");
    printf("Reconnecting\n");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.automaticReconnect = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
        finished = 1;
    }
}

void connlost(void *context, char *cause)
{
        printf("\nConnection lost\n");
        printf("     cause: %s\n", cause);
}

static void traceCallback(enum MQTTASYNC_TRACE_LEVELS level, char* message)
{
    printf("[ %d ]: %s\n", level, message);
    return;
}


int main(int argc, char* argv[])
{
        MQTTAsync client;
        MQTTAsync_SSLOptions sslopts = MQTTAsync_SSLOptions_initializer;
        MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
        MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
        int rc;
        int ch;
        MQTTAsync_setTraceLevel(MQTTASYNC_TRACE_PROTOCOL);
        MQTTAsync_setTraceCallback(traceCallback);

        MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        MQTTAsync_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        conn_opts.username = "admin";
        conn_opts.password = "admin";
        conn_opts.automaticReconnect = 1;
        conn_opts.minRetryInterval = 10;
        conn_opts.maxRetryInterval = 10;
        conn_opts.onSuccess = onConnect;
        conn_opts.onFailure = onConnectFailure;
        conn_opts.context = client;

        /* https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/struct_m_q_t_t_client___s_s_l_options.html */
        conn_opts.ssl = &sslopts;
        conn_opts.ssl->trustStore = CERT_PATH"ca.crt";
        conn_opts.ssl->keyStore = CERT_PATH"client.crt";
        conn_opts.ssl->privateKey = CERT_PATH"client.key";
        conn_opts.ssl->enableServerCertAuth = 1; /* verify server cert sign from CA */
        conn_opts.ssl->verify = 0;  /* verify server cert CN(Conmon Name) */

        if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start connect, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
        while   (!subscribed)
                #if defined(WIN32) || defined(WIN64)
                        Sleep(100);
                #else
                        usleep(10000L);
                #endif
        if (finished)
                goto exit;
        do
        {
                ch = getchar();
        } while (ch!='Q' && ch != 'q');
        disc_opts.onSuccess = onDisconnect;
        if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start disconnect, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
        while   (!disc_finished)
                #if defined(WIN32) || defined(WIN64)
                        Sleep(100);
                #else
                        usleep(10000L);
                #endif
exit:
        MQTTAsync_destroy(&client);
        return rc;
}
