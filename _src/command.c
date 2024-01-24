#include "command.h"

void MakeDataCobs(uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata, uint8_t dataSize)
{
    uint8_t count = 0, mark = 0;
    uint8_t* cobs = (uint8_t*)malloc(dataSize);
    memset(senddata, 0, sizeof(senddata));
    
    *senddata = ReturnMessage(button);
    if(*senddata != ALERT)
    {
        *(senddata + 1) = power >> 7;
        *(senddata + 2) = ((power << 1) & 0xFE) | ((movedir >> 8) & 0x01);
        *(senddata + 3) = movedir;
    }
    

    for(uint8_t i = 0;i < (dataSize - 1);i++)
    {
        if(*(senddata + i) != 0x00)
        {
            count++;
            *(cobs + (i + 1)) = *(senddata + i);
        }
        else
        {
            count++;
            *(cobs + mark) = count;
            mark = i + 1;
            count = 0;
            *(cobs + mark) = count;
        }
    }
    *(cobs + mark) = count;
    *(cobs + (dataSize + 1)) = 0x00;

    memcpy(senddata, cobs, sizeof(cobs));
    free(cobs);

    return;
}

uint8_t ReturnMessage(uint8_t num)
{
    uint8_t msg[] = { TURN, FINE, ALERT, HELLO, STOP, MOVE };
    static bool pathButton[] = {0, 0, 0, 0, 0, 0};

    if(num == SELECT)
        return msg[1];
    else if(num == START)
        return msg[2];
    else if(num == SANKAKU_B)
        return msg[4];
    else if(num == PS)
        return msg[3];
    else if(num == MOVE)
        return msg[5];
    else if(num == TURN)
        return msg[0];
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