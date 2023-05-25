/******************************************************************************/
#ifndef SYSTEM_DEFS_H
#define SYSTEM_DEFS_H 
/******************************************************************************/
#define DEBUG                           0x00
#define RELEASE                         0x01
/******************************************************************************/
#define SYSTEM_BUILD_TYPE               RELEASE

#define WIFI_ENABLE                     true
#define MQTT_ENABLE                     true

#define TIMER_INDEX                     TIMER_INDEX_1
#define RTC_USE_ALARM                   1

#if defined SYSTEM_BUILD_TYPE && SYSTEM_BUILD_TYPE == DEBUG
    #if defined WIFI_ENABLE && WIFI_ENABLE == true
        #define WIFI_DEBUG                  true
    #endif /* WIFI_ENABLE */

    #if defined MQTT_ENABLE && MQTT_ENABLE == true
        #define MQTT_DEBUG                  true
    #endif /* MQTT_ENABLE */ 

    #define COMMAND_DEBUG               true
    #define RTC_DEBUG                   true
    #define ISR_DEBUG                   true
    #define DIMMER_DEBUG                true
    #define PIR_SENSOR_DEBUG            true
#else
    #if defined WIFI_ENABLE && WIFI_ENABLE == true
        #define WIFI_DEBUG                  true
    #endif /* WIFI_ENABLE */

    #if defined MQTT_ENABLE && MQTT_ENABLE == true
        #define MQTT_DEBUG                  true
    #endif /* MQTT_ENABLE */ 

    #define COMMAND_DEBUG               true
    #define RTC_DEBUG                   false
    #define ISR_DEBUG                   false
    #define DIMMER_DEBUG                false 
    #define PIR_SENSOR_DEBUG            false
#endif

#define MQTT_MAX_QUEUE_LENGTH           5

#define MQTT_PUBLIC_BROKER              "mqtt.eclipseprojects.io"
#define MQTT_PORT                       MQTT_PORT_DEFAULT

#define MQTT_SECURITY_LEVEL             UNENCRYPTED

#if defined MQTT_SECURITY_LEVEL && MQTT_SECURITY_LEVEL == UNENCRYPTED
    #define MQTT_PORT_DEFAULT                   1883
#else 
    #define MQTT_PORT_DEFAULT                   8883
#endif

#define MQTT_USER_REQUEST               false
#if defined MQTT_USER_REQUEST && MQTT_USER_REQUEST == true
    #define MQTT_USER_NAME              "mqtt_user_name"
    #define MQTT_USER_PASS              "mqtt_user_pass"
#endif

#define BOARD_ID                         "7db8cbb3-47f8-48a7-8c5a-d0aa81fad54b"

#define MQTT_TOPIC_COMMAND          "tccautomacao/comando/"
#define MQTT_TOPIC_READ             "tccautomacao/leitura/"


#define BOARD_DIGITAL_OUTPUT            5
#define BOARD_ANALOGIC_OUTPUT           1
#define BOARD_KNOW_COMMANDS             6
/******************************************************************************/
#endif /* SYSTEM_DEFS_H */
/******************************************************************************/