/******************************************************************************/
#ifndef SYSTEM_DEFS_H
#define SYSTEM_DEFS_H 
/******************************************************************************/
#define DEBUG                           0x00
#define RELEASE                         0x01
/******************************************************************************/
#define SYSTEM_BUILD_TYPE               DEBUG

#if defined SYSTEM_BUILD_TYPE && SYSTEM_BUILD_TYPE == DEBUG
    #define WIFI_DEBUG                  true
    #define MQTT_DEBUG                  true
#else
    #define WIFI_DEBUG                  false
    #define MQTT_DEBUG                  false
#endif

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

#define BOARD_ID                "7db8cbb3-47f8-48a7-8c5a-d0aa81fad54b"
/******************************************************************************/
#endif /* SYSTEM_DEFS_H */
/******************************************************************************/