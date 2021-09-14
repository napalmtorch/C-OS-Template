#pragma once
#include <Kernel/Lib/Types.hpp>
#include <Kernel/Hardware/Ports.hpp>

typedef enum
{
    SERIALPORT_DISABLED         = 0x0000,
    SERIALPORT_COM1             = 0x03F8,
    SERIALPORT_COM2             = 0x02F8,
    SERIALPORT_COM3             = 0x03E8,
    SERIALPORT_COM4             = 0x02E8,
} SERIALPORT;

namespace System
{
    namespace HAL
    {
        class SerialController
        {
            private:
                SERIALPORT CurrentPort;

            public:
                void SetPort(SERIALPORT port);
                SERIALPORT GetPort();

            public:
                static const char* SerialPortToString(SERIALPORT port);

            public:
                char Read();
                void WriteChar(char c);
                void WriteChar(char c, byte color);
                void Write(char* text);
                void Write(char* text, byte color);
                void WriteLine(char* text);
                void WriteLine(char* text, byte color);
                void SetColor(byte color);
                byte HasRecieved();
                byte CanSend();
        };
    }
}