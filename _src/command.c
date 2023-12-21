#include "command.h"

void MakeSendData(uint8_t message_id, uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata)
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
            *(senddata + 1) = button;
            *(senddata + 2) = power >> 7;
            *(senddata + 3) = ((power << 1) & 0xFE) | ((movedir >> 8) & 0x01); 
            *(senddata + 4) = movedir;
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

// void MakeByte(uint8_t* senddata, uint16_t arg_1, ...)
// {
//     va_list ap;
//     uint8_t count = 0;
    
//     va_start(ap, arg_1);

//     while(arg_1 != NULL)
//     {
//         *senddata |= va_arg();
//     }


//     va_end(ap);
// }