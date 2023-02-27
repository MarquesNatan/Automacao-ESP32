/******************************************************************************/
#ifndef MAIN_APP_H
#define MAIN_APP_H
/******************************************************************************/
void main_app(void *params);

void mqttHandleTask(void *params);
void commandHandleTask(void *params);
void switchManagerTask(void *params);
void rtcHandleTask(void *params);
/******************************************************************************/
#endif /* MAIN_APP_H*/
/******************************************************************************/