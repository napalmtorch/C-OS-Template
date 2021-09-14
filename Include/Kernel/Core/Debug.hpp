#pragma once
#include <Kernel/Lib/Types.hpp>
#include <Kernel/Lib/String.hpp>
#include <Kernel/Graphics/Color.hpp>

// debug message type
extc typedef enum : byte
{
    D_MSG_INFO          = 0x00,
    D_MSG_OK            = 0x01,
    D_MSG_WARNING       = 0x02,
    D_MSG_ERROR         = 0x03,
    D_MSG_PANIC         = 0x04,
} D_MSG;

// debug mode type
extc typedef enum : byte
{
    D_MODE_DISABLED     = 0x00,
    D_MODE_SERIAL       = 0x01,
    D_MODE_CONSOLE      = 0x02,
    D_MODE_BOTH         = 0x03,
} D_MODE;

namespace System
{
    class DebugHandler
    {
        public:
            D_MODE Mode;
            D_MODE TempMode;

        public:
            DebugHandler();
            void SetMode(D_MODE mode);
            void PrintMessageTag(D_MSG type, D_MODE mode);

        private:
            void PrintFormatted(char* msg, va_list args);

        public:
            void NewLine();
            void NewLine(D_MODE mode);
            void WriteChar(char c);
            void WriteChar(char c, D_MODE mode);
            void WriteChar(char c, COL4 fg);
            void WriteChar(char c, COL4 fg, D_MODE mode);
            void Write(char* msg);
            void Write(char* msg, D_MODE mode);
            void Write(char* msg, COL4 fg);
            void Write(char* msg, COL4 fg, D_MODE mode);
            void Print(char* msg, ...);
            void WriteLine(char* msg);
            void WriteLine(char* msg, D_MODE mode);
            void PrintLine(char* msg, ...);

        public:
            void ThrowMessage(char* msg, D_MSG type, ...);
            void ThrowInfo(char* msg, ...);
            void ThrowOK(char* msg, ...);
            void ThrowWarning(char* msg, ...);
            void ThrowError(char* msg, ...);
            void ThrowPanic(char* msg, ...);
    };
}