#include "controller.h"

typedef struct{
    int ps3_serial;
}PS3_controllertable;

typedef struct{
    
}Pac_controller;

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

ssize_t ControllerRead(PS3_HANDLE handle)
{
    PS3_controllertable* _table = handle;
    uint8_t count = 0;
    static uint8_t i = 0;

    while(1)
    {
        read(_table->ps3_serial, (&EVENT), sizeof(EVENT));
        switch (EVENT.type)
        {
            case JS_EVENT_BUTTON:
            {
                printf("Button %u %s\n", EVENT.number, EVENT.value ? "pressed" : "released");      
            }
            break;
            case JS_EVENT_AXIS:
            {
                if(EVENT.number == 0)
                {
                    if(EVENT.number % 2 == 0)
                        printf("X-Axis %d", EVENT.value);
                    else
                        printf("Y-Axis %d", EVENT.value);
                }
            }
            break;
            default:
                break;
        }
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