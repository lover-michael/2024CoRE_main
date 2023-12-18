#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    // const char _dev_bord[] = "/dev/ttyUSB0";
    
    uint8_t senddata[100];
    uint8_t MessID_H = 0;
    uint8_t hijou = 0;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    // SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            MakeSendData(HELLO, 0, 0, 0, FINE,senddata);
            // SerialWrite(_handle, senddata, 2);
            printf("%x %x %x %x %x %x %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6]);
            for(int i = 0;i < 1000000;i++)
            {
                for(int j = 0;j < 1000;)
                {
                    j++;
                }
            }
        }
    }while(cPac.button != PS);

    while(1)
    {
        count = ControllerRead(_handle_con, &cPac);

        if(count < 0)
        {
            hijou = ALERT;
            MakeSendData(MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle, hijou, senddata);
        }
        else
        {    
            if(cPac.button == SELECT && hijou == FINE)
            {
                hijou = ALERT;
                MessID_H = STOP;
            }
            else if (cPac.button == START && hijou == ALERT)
            {
                hijou = FINE;
            }
            else
            {
                MessID_H = MOVE;

                if(cPac.stick_value == 0)
                    MessID_H = STOP;
            }
            
            MakeSendData(MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle, hijou, senddata);
            // SerialWrite(_handle, senddata, 7);
            // printf("%d %d %d %d %d %d %d\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6]);
            printf("%x %x %d %x %d\n", MessID_H, cPac.button, cPac.stick_value, hijou, cPac.stick_angle);
        }
    }

    ControllerClose(_handle_con);
    // SerialClose(_handle);

    return 0;
}