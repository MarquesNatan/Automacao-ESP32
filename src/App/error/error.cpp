/******************************************************************************/
#include "include/error.h"
/******************************************************************************/
ptr_heartbeat SetErrorAlert(ERROR_ALERT_MODE mode)
{
    ptr_heartbeat ptr = NULL;
    ptr = errorModeList[mode];

    if(ptr != NULL)
    {
        xQueueSendToBack(xQueueChangeMode, &ptr, 0);
    }

    return NULL;
}
/******************************************************************************/