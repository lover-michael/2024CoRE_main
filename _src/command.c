#include "command.h"

void MakeSendData(uint8_t message_id, uint8_t button, uint16_t power, uint16_t movedir, uint8_t hijou, uint8_t* senddata)
{
    uint8_t payloadSize = sizeof(senddata);
    memset(senddata, 0, payloadSize);

    *senddata = message_id;

    switch (message_id)
    {
        case STOP:
        {
            if(hijou == ALERT)
                *(senddata + 6) |= hijou;
            return;
        }
            break;
        case MOVE:
        {
            *(senddata + 1) = payloadSize;
            *(senddata + 2) = button;
            *(senddata + 3) = power >> 8;
            *(senddata + 4) = power & 0xFF; 
            *(senddata + 5) = movedir >> 1;
            *(senddata + 6) = hijou;
        }
            break;
        case HELLO:
        {
            return;
        }
            break;
        default:
            break;
    }
}

void MakeByte(uint8_t* senddata, uint16_t arg_1, ...)
{
    va_list ap;
    uint8_t count = 0;
    
    va_start(ap, arg_1);

    while(arg_1 != NULL)
    {
        *senddata |= va_arg();
    }


    va_end(ap);
}