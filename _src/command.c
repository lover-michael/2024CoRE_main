#include "command.h"



void MakeSendMotarData(uint8_t message_id, uint16_t power, uint16_t movedir, uint8_t* senddata, CheckCB checkSum)
{
    uint8_t payloadSize = sizeof(senddata);
    memset(senddata, 0, payloadSize);

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
            *(senddata + 2) = power >> 8;
            *(senddata + 3) = power & 0xFF; 
            *(senddata + 4) = movedir >> 1;
            *(senddata + 5) = (movedir << 7) | MS_TERMINAL;
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

    *(senddata + 6) = checkSum(senddata, payloadSize);

}

uint8_t CheckSum(uint8_t* senddata, uint8_t sizeData)
{
     uint16_t sum = 0;

    for(uint8_t i = 0;i < sizeData;i++)
    {
        sum += (*(senddata + i));
    }

    
    
}