#include "serial.h"

typedef struct{
    int serial;
} Serialtable;

typedef struct termios TIO;

SERIAL_HANDLE SerialOpen(const char *const dev, uint64_t baudrate)
{
    Serialtable *_table = (Serialtable *)malloc(sizeof(Serialtable));
    memset(_table, 0, sizeof(Serialtable));

    _table->serial = open(dev, O_RDWR); // デバイスオープン
    if (_table->serial == -1)
    {
        perror("Open Error");
        free(_table);

        return NULL;
    }

    TIO _tio = {}; // シリアル通信設定


    /*ボーレートの設定(入力 = 出力)*/
    _tio.c_cflag += CREAD;  // 受信有効
    _tio.c_cflag += CLOCAL; // ローカルライン
    _tio.c_cflag += CS8;    // データビット:8bit

    cfsetispeed(&_tio, baudrate);
    cfsetospeed(&_tio, baudrate);

    cfmakeraw(&_tio); // RAWモード

    tcsetattr(_table->serial, TCSANOW, &_tio); // デバイス設定      

    ioctl(_table->serial, TCSETS, &_tio); // ポート設定の有効化

    // u_long val = 1;
    // ioctl(_table->serial, FIONBIO, &val); // 非同期化

    return _table;
}

void SerialClose(SERIAL_HANDLE handle)
{
    Serialtable *_table = handle;

    close(_table->serial);
}

ssize_t SerialWrite(SERIAL_HANDLE handle, unsigned char *buff, uint8_t size)
{
    Serialtable *_table = handle;

    return write(_table->serial, buff, size);
}

ssize_t SerialRead(SERIAL_HANDLE handle, unsigned char *buff, uint8_t size)
{
    Serialtable *_table = handle;

    return read(_table->serial, buff, size);
}