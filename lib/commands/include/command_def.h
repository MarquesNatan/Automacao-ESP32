/******************************************************************************/
#ifndef COMMAND_DEF_H
#define COMMAND_DEF_H
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
#define COMMAND_SOF                 '<'
#define COMMAND_EOF                 '>'
#define COMMAND_END_PARAM           ':'

#define COMMAND_MAX_LENGTH          15
#define COMMAND_MAX_PARAMS          3

#define COMM_TURN_ON_OUTPUT         'L' /* <O.x> */
#define COMM_TURN_OFF_OUTPUT        'D' /* <D.x> */
#define COMM_DIMMER_ADJUST          'A' /* <D.O.V> */ /* O -> output V -> value */
#define COMM_TOGGLE_OUTPUT          'T' /* <D.x> */

#define COMM_RESP_OK                'O' /* <O> */
#define COMM_RESP_ERROR             'E' /* <E> */
#define COMM_RESP_FAIL              'F' /* <F> */
#define COMM_RESP_CRC_ERROR         'C' /* <C> */
/******************************************************************************/
typedef enum
{
    COMMAND_RECEIVED_OK         = 0x00,
    COMMANDL_RECEIVE_FAIL       = 0x01,
    COMMAND_CRC_FAIL            = 0x02,
    COMMAND_EXECUTE_WAIT        = 0x03,
    COMMAND_EXECUTE_OK          = 0x04, 
    COMMAND_EXECUTE_FAIL        = 0x05,
    COMMAND_EMPTY               = 0x06,
    COMMAND_UNKNOWN             = 0x07,
    COMMAND_SOF_FAIL            = 0x08,
    COMMAND_EOF_FAIL            = 0x09,
    COMMAND_TOPIC_FAIL          = 0x10,
    COMMAND_PARAMS_FAIL         = 0x11,
    COMMAND_VALIDATION_WAIT     = 0x12
}COMMAND_STATUS; 
/******************************************************************************/
typedef struct 
{
    COMMAND_STATUS status;
    uint8_t data[9];
    uint8_t length;
}command_packet_t;
/******************************************************************************/
#endif /* COMMAND_DEF_H */
/******************************************************************************/