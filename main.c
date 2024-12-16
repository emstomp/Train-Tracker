#include <stdio.h>
#include <stdint.h>
#include <cJSON.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_out.h"
#include "http_req.h"
#include "all_sta.c"

#define WS2812B_PIN 5
#define RED "red"
#define BLUE "blue"
#define BROWN "brn"
#define GREEN "g"
#define ORANGE "org"
#define PURPLE "p"
#define PINK "pink"
#define YELLOW "y"
#define MAX_RES_SIZE 15168
#define STATION_NUM 389
#define BLUE_COLOR_NUM 198

char response[MAX_RES_SIZE];
short sta_to_pixel[STATION_NUM];
short blue_line[BLUE_COLOR_NUM];

const short red[3] = {0, 15, 0};
const short blue[3] = {0, 0, 15};
const short brown[3] = {1, 15, 0};
const short green[3] = {15, 0, 0};
const short orange[3] = {3, 15, 0};
const short purple[3] = {0, 7, 15};
const short pink[3] = {0, 15, 3};
const short yellow[3] = {7, 15, 0};

void clear_color_arr(short arr[], int arr_size);
void set_color(int pixel_num, const short color[]);
void test_pixels();

void app_main(void)
{

    for (int i = 0; i < 10; i++) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    int status = 0;

    set_output(WS2812B_PIN);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    clear_color_arr(blue_line, BLUE_COLOR_NUM);
    pixel_arr_init(sta_to_pixel, STATION_NUM);
    update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);

    if(wifi_begin()) printf("Not connected to AP\n");

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (status == 0) {

        clear_color_arr(blue_line, BLUE_COLOR_NUM);

        if (http_rest_get(response, MAX_RES_SIZE, BLUE, 4)) {
            printf("Get request unsuccessful\n");
            break;
        }
        
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        cJSON *json = cJSON_Parse(response);
        if (json == NULL) {
            const char *err_ptr = cJSON_GetErrorPtr();
            if (err_ptr != NULL) {
                printf("Error in parsing json: %s\n", err_ptr);
            }
            cJSON_Delete(json);
            status = 1;
            break;
        }

        cJSON *ctatt = cJSON_GetObjectItem(json, "ctatt");
        cJSON *route = cJSON_GetObjectItem(ctatt, "route")->child;
        cJSON *train = cJSON_GetObjectItem(route, "train")->child;

        while (train != NULL) {
            char *stpID_str = cJSON_GetObjectItem(train, "nextStpId")->valuestring;
            int stpID = atoi(stpID_str);
            short pixelID = sta_to_pixel[(stpID - 30000)];
            if (pixelID == -1) {
                break;
            }
            set_color(pixelID, blue);
            train = train->next;
        }

        cJSON_Delete(json); 

        for (int i = 0; i < BLUE_COLOR_NUM; i++) {
            if (i % 3 == 0) printf("\n");
            printf("%d ", blue_line[i]);
        }

        update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }

    clear_color_arr(blue_line, BLUE_COLOR_NUM);
    update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
}

void clear_color_arr(short arr[], int arr_size)
{
    for (int i = 0; i <arr_size; i++) {
        arr[i] = 0;
    }
}

void set_color(int pixel_num, const short color[])
{
    int pixel_idx = pixel_num * 3;
    for (int i = 0; i < 3; i++) {
        blue_line[pixel_idx+i] = color[i];
    }
}

void test_pixels()
{
    for (int i = 0; i < 10; i++) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    set_output(WS2812B_PIN);
    clear_color_arr(blue_line, BLUE_COLOR_NUM);
    update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    while (1) {
        for (int train = 2; train < 198; train+=3) {
            blue_line[train] = 15;
            if ((train-3) > 0) blue_line[train-3] = 0;
            update_led(blue_line, BLUE_COLOR_NUM, WS2812B_PIN);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}
