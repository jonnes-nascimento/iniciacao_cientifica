#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "MQTTClient.h"
#include "DHT22.h"

#define DTH22_GPIO_PIN                  25 // Na placa Wemos D1 R32, pino digital IO25
#define CONFIG_WIFI_SSID                "nome_da_rede"
#define CONFIG_WIFI_PASSWORD            "senha"
#define MQTT_BROKER                     "iot.jonnes.eng.br"
#define MQTT_PORT                       1883
#define MQTT_CLIENT_THREAD_NAME         "mqtt_client_thread"
#define MQTT_CLIENT_THREAD_STACK_WORDS  8192
#define MQTT_CLIENT_THREAD_PRIO         8

static const char *TAG = "MQTT";
static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

static void wifi_init(void) {
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_LOGI(TAG, "start the WIFI SSID:[%s] password:[%s]", CONFIG_WIFI_SSID, "******");
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

static void mqtt_client_thread(void* pvParameters) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80] = {0};
    int rc = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    printf("mqtt client thread starts\n");

    // Wait for the callback to set the CONNECTED_BIT in the event group.
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    char* address = MQTT_BROKER;

    if ((rc = NetworkConnect(&network, address, MQTT_PORT)) != 0) {
        printf("Return code from network connect is %d\n", rc);
    }

#if defined(MQTT_TASK) 
    if ((rc = MQTTStartTask(&client)) != pdPASS) {
        printf("Return code from start tasks is %d\n", rc);
    } else {
        printf("Use MQTTStartTask\n");
    }
#endif

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "Jonnes Weather Station";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Return code from MQTT connect is %d\n", rc);
    } else {
        printf("MQTT Connected\n");
    }

    setDHTgpio(DTH22_GPIO_PIN);

    while (1) {

        int ret = readDHT();
		
		errorHandler(ret);

		//printf( "Hum %.1f\n", getHumidity() );
		//printf( "Tmp %.1f\n", getTemperature() );

        MQTTMessage message;
        char payload[30];

        message.qos = QOS2;
        message.retained = 0;

        // publica a temperatura lida pelo DHT22
        message.payload = payload;
        sprintf(payload, "%.1f", getTemperature());

        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, "temperatura", &message)) != 0) {
            printf("Return code from MQTT publish is %d\n", rc);
        } else {
            printf("MQTT publish topic \"temperatura\", message number is %.1f\n", getTemperature());
        }

        // publica a umidade lida pelo DHT22
        message.payload = payload;
        sprintf(payload, "%.1f", getHumidity());

        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, "umidade", &message)) != 0) {
            printf("Return code from MQTT publish is %d\n", rc);
        } else {
            printf("MQTT publish topic \"umidade\", message number is %.1f\n", getHumidity());
        }

        vTaskDelay(10000 / portTICK_RATE_MS);  //send every 10 seconds
    }

    printf("mqtt_client_thread going to be deleted\n");
    vTaskDelete(NULL);

    return;
}

void app_main() {

    ESP_ERROR_CHECK( nvs_flash_init() );

    wifi_init();
    
    xTaskCreate(&mqtt_client_thread,
                MQTT_CLIENT_THREAD_NAME,
                MQTT_CLIENT_THREAD_STACK_WORDS,
                NULL,
                MQTT_CLIENT_THREAD_PRIO,
                NULL);

}
