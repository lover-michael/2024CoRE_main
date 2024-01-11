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
    bool flagHijou = true;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    // SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            MakeSendData(HELLO, 0, 0, 0, senddata);
            // SerialWrite(_handle, senddata, 2);
            for(int i = 0;i < 10000;i++)
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
            MakeSendData(MessID_H, cPac.button, cPac.stick_value[0], cPac.stick_angle[0], senddata);
            // SerialWrite(_handle, senddata, 5);
            hijou++;
            if(hijou > 50)
                break;
        }
        else
        {    
            if(cPac.button == PS)
            {
                flagHijou = !flagHijou;
                
                if(flagHijou == true){ MessID_H = FINE; }
                else{ MessID_H = ALERT; }

                memset(&cPac, 0, sizeof(cPac));
                MakeSendData(MessID_H, cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata);
                // SerialWrite(_handle, senddata, 5);
            }
            
            if(MessID_H != ALERT)
            {
                MessID_H = ARM;
                MakeSendData(MessID_H, cPac.button, cPac.stick_value[0], cPac.stick_angle[0], senddata);
                // SerialWrite(_handle, senddata, 5);

                MessID_H = MOVE;
                MakeSendData(MessID_H, cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata);
                // SerialWrite(_handle, senddata, 5);                
            }
        }
    }

    ControllerClose(_handle_con);
    // SerialClose(_handle);

    return 0;
}

            // printf("%x %x %x %d %d %d %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5]);
            // printf("%x %x %d %d\n", MessID_H, cPac.button, cPac.stick_value, cPac.stick_angle);

            // else if(cPac.button == SANKAKU_B && judge == 1)
            // {
            //     MessID_H = STOP;
            //     memset(&cPac, 0, sizeof(cPac));
            // }
            // else if(cPac.button == PS && MessID_H == STOP)
            // {
            //     break;
            // }
            // else if(judge ==  1 && cPac.stick_value[0] > 100)
            // {
            //     MessID_H = MOVE;
            //     judge = 1;
            // }
