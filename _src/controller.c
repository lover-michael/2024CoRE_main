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

    while((count = read(_table->ps3_serial, (&EVENT), sizeof(EVENT))) != 0);
    switch (EVENT.type)
    {
        case JS_EVENT_BUTTON:
        {
            // printf("Button %u %s\n", EVENT.number, EVENT.value ? "pressed" : "released");      
            _cntPkt->button = EVENT.number;
        }
        case JS_EVENT_AXIS:
        {
            if(EVENT.number == 0 || 1)
            {
                if(EVENT.number % 2 == 0)
                    x_axis = EVENT.value;
                else
                    y_axis = EVENT.value;
            }
            _cntPkt->stick_value = sqrt(x_axis*x_axis + y_axis*y_axis);
            int ang = 180 * (atan2(y_axis, x_axis) / M_PI);
            if(_cntPkt->stick_angle < 0)
                _cntPkt->stick_angle = (uint16_t)(ang - 180);
            else
                _cntPkt->stick_angle = ang;
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