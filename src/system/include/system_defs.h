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
/******************************************************************************/
#endif /* SYSTEM_DEFS_H */
/******************************************************************************/