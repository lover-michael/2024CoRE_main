#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/ttyUSB0";
    
    uint8_t senddata[100];
    uint8_t hijou = 0;
    uint8_t numM = 0;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    // while(1)
    // {
    //     ControllerRead(_handle_con, &cPac);
    //     printf("button -> %d  ", cPac.button);
    //     printf("Lstick -> %d  ", cPac.stick_value[1]);
    //     printf("Rstick -> %d  ", cPac.stick_value[0]);
    //     printf("LTvakue -> %d  ", cPac.trigger_value[1]);
    //     printf("RTvakue -> %d\n", cPac.trigger_value[0]);
    // }

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            numM = MakeDataCobs(cPac.button, 0, 0, senddata, cPac.trigger_value);
            count = SerialWrite(_handle, senddata, 3);
            printf("%ld\n", count);
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
        
        if(count > 0)
        {
            memset(senddata, 0, 12);

            if(cPac.button < 30)
            {
                numM = MakeDataCobs(cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata, cPac.trigger_value);
                printf("%x %x %x\n",senddata[0], senddata[1], senddata[2]);
                if(numM != 0xff){
                    count = SerialWrite(_handle, senddata, numM);
                    if(count < 0){ 
                        hijou++;
                        continue;
                    }
                }
                // msleep(10);            
            }

            // ControllerRead(_handle_con, &cPac);

            senddata[7] = 0xAA;
            numM = MakeDataCobs(cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata, cPac.trigger_value);
            printf("%d %x %d %d %d %d %d %d %d %d %d %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6], senddata[7], senddata[8], senddata[9], senddata[10], senddata[11]);
            count = SerialWrite(_handle, senddata, numM);
            // msleep(10);

            if(count < 0){ 
                hijou++;
                continue;
            }

            else
            {//放蕩浅海用の通信
                // ControllerRead(_handle_con, &cPac);

                senddata[7] = 0xAB;
                numM = MakeDataCobs(cPac.button, cPac.stick_value[0], cPac.stick_angle[0], senddata, cPac.trigger_value);
                printf("%x %x %d %d %x %x %x %x %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6], senddata[7], senddata[8]);
                count = SerialWrite(_handle, senddata, numM);
                // msleep(10);
                if(count < 0){ 
                    hijou++; 
                    continue; 
                }
            }
        }
        else
        {
            hijou++;
            if(hijou > 30)
                break;
        }
        msleep(10);
    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}
