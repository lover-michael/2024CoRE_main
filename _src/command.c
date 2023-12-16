#include "command.h"



void MakeSendData(uint8_t message_id, uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata)
{
    uint8_t payloadSize = sizeof(senddata);
    memset(senddata, 0, payloadSize);
    printf("%x %x %x %x %x %x %x\n",*senddata, *(senddata + 1), *(senddata + 2), *(senddata + 3), *(senddata + 4), *(senddata + 5), *(senddata + 6));

    *senddata = message_id;

    switch (message_id)
    {
        case STOP:
        {
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
            *(senddata + 6) = MS_TERMINAL;
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
