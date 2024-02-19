#include "command.h"

#define M_PI 3.14159265358979323846
#define POWER(a, b, c) (int32_t)(a*cos((float)b * M_PI / 180.0) + c*sin((float)b * M_PI / 180.0))

uint8_t MakeDataCobs(uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata, uint8_t dataSize)
{
    uint8_t count = 0, mark = 0, dir = 0, pbutton = button, numReturn = 1;
    int16_t m_pow[4] = {0, 0, 0, 0};
    uint8_t* cobs = (uint8_t*)malloc(dataSize + 2);
    static bool flag_alert = false;
    
    if(*(senddata + 7) == 0xAA && button > 30) 
        button = MOVE;
    else if(*(senddata + 7) == 0xAB && button > 30)
        button = TURN;

    memset(senddata, 0, dataSize + 2);
    
    *senddata = ReturnMessage(button);
    if(*senddata == MOVE && flag_alert != true)
    {
        m_pow[0] = POWER(power, movedir, power);
        m_pow[1] = POWER(power, movedir, -power);
        m_pow[2] = POWER(-power, movedir, -power);
        m_pow[3] = POWER(-power, movedir, power);
    
        for(int i = 0;i < 4;i++)
        {
            if(m_pow[i] < 0)
            {
                dir += (1 << i);
                m_pow[i] = m_pow[i] * -1;
            }

            *(senddata + (1 + 2 * i)) = m_pow[i] >> 8;
            *(senddata + (2 + 2 * i)) = m_pow[i] & 0xFF;
        }
        *(senddata + 9) = dir;
    }
    else if(*senddata == TURN && flag_alert != true)
    {
        m_pow[0] = power*sin((float)movedir * M_PI / 180.0);
        m_pow[1] = power*cos((float)movedir * M_PI / 180.0);

        for(int i = 0;i < 2;i++)
        {
            if(m_pow[i] >= 0)
                dir += 0;
            else if(m_pow[i] < 0)
            {
                dir += (1 << i);
                m_pow[i] = m_pow[i] * -1;
            }

            if(m_pow[i] > 32000)
                m_pow[i] = 32000;

            *(senddata + (1 + 2 * i)) = m_pow[i] >> 8;
            *(senddata + (2 + 2 * i)) = m_pow[i] & 0xFF;
        }
        *(senddata + 5) = dir;

        if(pbutton == LEFT_T)
            *(senddata + 6) = 0xFF;
        
        *(senddata + 7) = ReturnMessage(pbutton);
    }
    else if(*senddata == ALERT)
    {
        flag_alert = true;
        numReturn = 0;
    }
    else if(*senddata == FINE)
        flag_alert = false;

    for(uint8_t i = 0;i < dataSize;i++)
    {
        if(*(senddata + i) != 0x00)
        {
            count++;
            *(cobs + (i + 1)) = *(senddata + i);
            if(i == (dataSize - 1) && mark == 0)
                count++;
        }
        else
        {
            count++;
            *(cobs + mark) = count;
            mark = i + 1;
            count = 0;
            if(i == (dataSize - 1))
                count++;
        }
    }
    *(cobs + mark) = count;
    *(cobs + (dataSize + 1)) = 0x00;

    memcpy(senddata, cobs, dataSize + 2);
    free(cobs);

    return numReturn;
}

uint8_t ReturnMessage(uint8_t num)
{
    uint8_t msg[] = { TURN, FINE, ALERT, HELLO, STOP, MOVE, SHOT_L, SHOT_S };
    static uint8_t pathB = 0;

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
    else if(num == LEFT_1)
        return msg[6];
    else if(num == RIGHT_1)
        return msg[7];
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