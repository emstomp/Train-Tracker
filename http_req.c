#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "config_data.h"

#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10

#define MIN(a,b) \
   ({ typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a < _b ? _a : _b; })

static EventGroupHandle_t wifi_event_group;

static const char *TAG = "Wifi station";

static int reconnect_num = 0;

static char URL[128];

static int res_size_num = 0;
static char *res_ptr = NULL;
static char *copy_ptr = NULL;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, 
                                int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Conecting...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (reconnect_num < MAX_FAILURES) {
            ESP_LOGI(TAG, "Reconnecting...");
            esp_wifi_connect();
            reconnect_num++;
        } else {
            xEventGroupSetBits(wifi_event_group, WIFI_FAILURE);
        }
    }
}

static void ip_event_handler(void *ip_handler_arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "IP set");
        reconnect_num = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_SUCCESS);
    }
}

static esp_err_t wifi_connection()
{
    wifi_event_group = xEventGroupCreate();
    int status = WIFI_FAILURE;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    
    esp_event_handler_instance_t wifi_handler_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, 
                                                        ESP_EVENT_ANY_ID, 
                                                        wifi_event_handler, 
                                                        NULL,
                                                        &wifi_handler_event_instance));

    esp_event_handler_instance_t ip_handler_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, 
                                                        IP_EVENT_STA_GOT_IP, 
                                                        ip_event_handler, 
                                                        NULL,
                                                        &ip_handler_event_instance));

    wifi_config_t wifi_set_config = {
        .sta = {
            .ssid = SSID,
            .password = SSID_PASS
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_set_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wifi station initialization complete");

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                            WIFI_SUCCESS | WIFI_FAILURE,
                                            pdFALSE,
                                            pdFALSE,
                                            portMAX_DELAY);

    if (bits & WIFI_SUCCESS) {
        ESP_LOGI(TAG, "Connected");
        status = WIFI_SUCCESS;
    } else if (bits & WIFI_FAILURE) {
        ESP_LOGI(TAG, "Failed to connect");
        status = WIFI_FAILURE;
    } else {
        ESP_LOGE(TAG, "Unexpected event");
        status = WIFI_FAILURE;
    }
    
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ip_handler_event_instance));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler_event_instance));
    vEventGroupDelete(wifi_event_group);

    return status;
}

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    static int output_len = 0;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len: %d", evt->data_len);
        int copy_len = 0;
        copy_len = MIN(evt->data_len, (res_size_num - output_len));
        if (copy_len) {
            memcpy(copy_ptr, evt->data, copy_len);
            copy_ptr += copy_len;
            output_len += copy_len;
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "get request finished");
        output_len = 0;
        copy_ptr = res_ptr;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "http event disconnected");
        output_len = 0;
        copy_ptr = res_ptr;
        break;
    default:
        break;
    }
    return ESP_OK;
}

int http_rest_get(char response[], int response_size, char rt[], int rt_size)
{
    if (rt_size > 4) {
        ESP_LOGI(TAG, "size of rt is too large");
        return 1;
    } else if (rt == NULL) {
        ESP_LOGI(TAG, "invalid rt");
        return 1;
    }

    strcpy(URL, "http://lapi.transitchicago.com/api/1.0/ttpositions.aspx?key=");
    strcat(URL, API_KEY);
    strcat(URL, "&rt=");
    strncat(URL, rt, rt_size);
    strcat(URL, "&outputType=JSON");

    res_size_num = response_size;
    res_ptr = response;
    copy_ptr = response;
    int req_status = 1;

    esp_http_client_config_t config_get = {
        .url = URL,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_get_handler
    };

    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        req_status = 0;
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        req_status = 1;
    }

    esp_http_client_cleanup(client);
    return req_status;
}

int wifi_begin()
{
    esp_err_t status = WIFI_FAILURE;

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    status = wifi_connection();
    if (status != WIFI_SUCCESS) {
        ESP_LOGI(TAG, "Failed to connect to AP");
        return 1;
    }
    return 0;
}