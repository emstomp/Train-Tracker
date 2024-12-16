#ifndef HTTP_REQ_H
#define HTTP_REQ_H

int wifi_begin();
int http_rest_get(char response[], int response_size, char rt[], int rt_size);

#endif // HTTP_REQ_H