#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/ttyUSB0";
    
    uint8_t senddata[100];
    uint8_t MessID_H = 0;
    uint8_t hijou = 0;
    uint8_t judge = 0;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    // SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            MakeSendData(HELLO, 0, 0, 0,senddata);
            // SerialWrite(_handle, senddata, 2);
            // printf("%x %x %x %x %x %x %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6]);
            for(int i = 0;i < 1000000;i++)
            {
                for(int j = 0;j < 1000;)
                {
                    j++;
                }
            }
        }
    }while(cPac.button != PS);

    memset(&cPac, 0, sizeof(cPac));

    while(1)
    {
        count = ControllerRead(_handle_con, &cPac);

        if(count < 0)
        {
            MessID_H = ALERT;
            MakeSendData(MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle, senddata);
            // SerialWrite(_handle, senddata, sizeof(senddata));
            hijou++;
            if(hijou > 20)
                break;
        }
        else
        {    
            if(cPac.button == START && judge == 1)
            {
                MessID_H = ALERT;
                judge = 0;
            }
            else if (cPac.button == SELECT && judge == 0)
            {
                MessID_H = FINE;
                judge = 1;
            }
            else if(cPac.button == SANKAKU_B && judge == 1)
            {
                MessID_H = STOP;
                memset(&cPac, 0, sizeof(cPac));
            }
            else if(cPac.button == PS && MessID_H == STOP)
            {
                break;
            }
            else if(judge ==  1 && cPac.stick_value > 100)
            {
                MessID_H = MOVE;
                judge = 1;
            }
            
            MakeSendData(MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle, senddata);
            // SerialWrite(_handle, senddata, sizeof(senddata));
            // printf("%x %x %x %d %d %d %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5]);
            printf("%x %x %d %d\n", MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle);
        }
    }

    ControllerClose(_handle_con);
    // SerialClose(_handle);

    return 0;
}