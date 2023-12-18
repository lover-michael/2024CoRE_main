#include "controller.h"

typedef struct{
    int ps3_serial;
}PS3_controllertable;

js_event EVENT;

PS3_HANDLE ConttrollerOpen(const char const* dev)
{
    PS3_controllertable* _table = (PS3_controllertable*)malloc(sizeof(PS3_controllertable));
    memset(_table, 0, sizeof(PS3_controllertable));

    _table->ps3_serial = open(dev, O_RDONLY);

    if(_table->ps3_serial < 0)
    {
        perror("Could not open joystick");
        _table = NULL;
    }

    return _table; 
}

ssize_t ControllerRead(PS3_HANDLE handle, controllerPac* _cntPkt)
{
    PS3_controllertable* _table = handle;
    uint8_t count = 0;
    int16_t x_axis, y_axis;

    while(read(_table->ps3_serial, (&EVENT), sizeof(EVENT)) < 7);
    switch (EVENT.type)
    {
        case JS_EVENT_BUTTON:
        {
            if((EVENT.value ? "pressed" : "released") == "pressed"){
                _cntPkt->button = EVENT.number;
                // printf("button:%d\n", _cntPkt->button);
            }
        }
        case JS_EVENT_AXIS:
        {
            if(EVENT.number == 0)
            {
                if(EVENT.number == 0)
                    x_axis = EVENT.value;
                else if(EVENT.number == 1)
                    y_axis = EVENT.value;

                _cntPkt->stick_value = sqrt(x_axis*x_axis + y_axis*y_axis);
                // uint16_t ang = 180 * atan2(y_axis, x_axis);
                _cntPkt->stick_angle = (float)180 * atan2(y_axis / 1000, x_axis / 1000);
                // if(_cntPkt->stick_angle < 0)
                //     _cntPkt->stick_angle = (uint8_t)ang - 180;
                // else
                //     _cntPkt->stick_angle = ang;
            }
            // printf("value : %d\nangle : %d\n", _cntPkt->stick_value, _cntPkt->stick_angle);
        }
        default:
            break;
    }

    return count;
}

void ControllerClose(PS3_HANDLE handle)
{
    PS3_controllertable* _table = handle;

    close(_table->ps3_serial);
    free(_table);
}

void GetButtonNum(PS3_HANDLE handle, uint8_t* buttonnum)
{
    PS3_controllertable* _table = handle;

    if(ioctl(_table->ps3_serial, JSIOCGBUTTONS, buttonnum) == -1)
    {
        perror("Failed!");
        return;
    }
}

void GetAxisNum(PS3_HANDLE handle, uint8_t* axisnum)
{
    PS3_controllertable* _table = handle;

    if(ioctl(_table->ps3_serial, JSIOCGAXES, axisnum) == -1)
    {
        perror("Failed!");
        return;
    }
}