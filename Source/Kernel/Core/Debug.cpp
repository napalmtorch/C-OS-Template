#include <Kernel/Core/Debug.hpp>
#include <Kernel/Core/Kernel.hpp>

namespace System
{
    DebugHandler::DebugHandler() { Mode = D_MODE_BOTH; }

    void DebugHandler::SetMode(D_MODE mode) { Mode = mode; }

    void DebugHandler::PrintMessageTag(D_MSG type, D_MODE mode)
    {
        Write("[", mode);
        switch (type)
        {
            case D_MSG_INFO:    { Write("  >>  ", COL4_CYAN, mode); break; }
            case D_MSG_OK:      { Write("  OK  ", COL4_GREEN, mode); break; }
            case D_MSG_WARNING: { Write("  ??  ", COL4_YELLOW, mode); break; }
            case D_MSG_ERROR:   { Write("  !!  ", COL4_RED, mode); break; }
            case D_MSG_PANIC:   { Write("  !!  ", COL4_RED, mode); break; }
            default:            { Write("  >>  ", COL4_CYAN, mode); break; }
        }
        Print("] ", mode);
    }

    void DebugHandler::NewLine() { NewLine(Mode); }

    void DebugHandler::NewLine(D_MODE mode) 
    { 
        if (mode == D_MODE_BOTH || mode == D_MODE_SERIAL)  { Serial.WriteChar('\n'); }
        if (mode == D_MODE_BOTH || mode == D_MODE_CONSOLE) {  }
    }

    void DebugHandler::WriteChar(char c) { WriteChar(c, Mode); }

    void DebugHandler::WriteChar(char c, D_MODE mode)
    {
        if (mode == D_MODE_BOTH || mode == D_MODE_SERIAL)  { Serial.WriteChar(c); }
        if (mode == D_MODE_BOTH || mode == D_MODE_CONSOLE) {  }
    }

    void DebugHandler::WriteChar(char c, COL4 fg) { WriteChar(c, fg, Mode); }

    void DebugHandler::WriteChar(char c, COL4 fg, D_MODE mode)
    {
        if (mode == D_MODE_BOTH || mode == D_MODE_SERIAL)  { Serial.WriteChar(c, (byte)fg); }
        if (mode == D_MODE_BOTH || mode == D_MODE_CONSOLE) {  }
    }

    void DebugHandler::Write(char* msg) { Write(msg, Mode); }

    void DebugHandler::Write(char* msg, D_MODE mode)
    {
        if (mode == D_MODE_BOTH || mode == D_MODE_SERIAL)  { Serial.Write(msg); }
        if (mode == D_MODE_BOTH || mode == D_MODE_CONSOLE) {  }
    }

    void DebugHandler::Write(char* msg, COL4 fg) { Write(msg, fg, Mode); }

    void DebugHandler::Write(char* msg, COL4 fg, D_MODE mode)
    {
        if (mode == D_MODE_BOTH || mode == D_MODE_SERIAL)  { Serial.Write(msg, (byte)fg); }
        if (mode == D_MODE_BOTH || mode == D_MODE_CONSOLE) {  }
    }

    void DebugHandler::Print(char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
    }

    void DebugHandler::WriteLine(char* msg)
    {
        Write(msg);
        NewLine();
    }

    void DebugHandler::WriteLine(char* msg, D_MODE mode)
    {
        Write(msg, mode);
        NewLine(mode);
    }

    void DebugHandler::PrintLine(char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);

        // new line
        NewLine(TempMode);
    }

    void DebugHandler::PrintFormatted(char* msg, va_list args)
    {
        COL4 color = COL4_WHITE;
        TempMode = Mode;

        // loop through string
        while (*msg != 0)
        {
            // found format specifier
            if (*msg == '%')
            {
                // increment another character
                msg++;
                // character specifier
                if (*msg == 'c')
                {
                    int c = va_arg(args, int);
                    WriteChar((char)c, color, TempMode);
                    msg++;
                }
                // decimal/integer specifier
                else if (*msg == 'd' || *msg == 'i')
                {
                    int dec = va_arg(args, int);
                    char temp[16];
                    Write(String::FromDecimal(dec, temp), color, TempMode);
                    msg++;
                }
                // string specifier
                else if (*msg == 's')
                {
                    char* str = va_arg(args, char*);
                    Write(str, color, TempMode);
                    msg++;
                }
                // unsigned decimal/integer specifier
                else if (*msg == 'u')
                {
                    uint dec = va_arg(args, uint);
                    char temp[16];
                    Write(String::FromDecimal((int)dec, temp), color, TempMode);
                    msg++;
                }
                // hex specifier
                else if (*msg == 'x')
                {
                    uint dec = va_arg(args, uint);
                    char temp[16];
                    Write(String::FromHex(dec, temp, false), color, TempMode);
                    msg++;
                }
                // 8-bit hex specifier
                else if (*msg == '2')
                {
                    msg++;
                    if (*msg != 'x' && *msg != 'X') { WriteChar(*msg, color, TempMode); msg++; continue; }
                    uint dec = va_arg(args, uint);
                    char temp[16];
                    Write(String::FromHex(dec, temp, false, 1), color, TempMode);
                    msg++;
                }
                // 16-bit hex specifier
                else if (*msg == '4')
                {   
                    msg++;
                    if (*msg != 'x' && *msg != 'X') { WriteChar(*msg, color, TempMode); msg++; continue; }
                    uint dec = va_arg(args, uint);
                    char temp[16];
                    Write(String::FromHex(dec, temp, false, 2), color, TempMode);
                    msg++;
                }
                // 32-bit hex specifier
                else if (*msg == '8')
                {
                    msg++;
                    if (*msg != 'x' && *msg != 'X') { WriteChar(*msg, color, TempMode); msg++; continue; }
                    uint dec = va_arg(args, uint);
                    char temp[16];
                    Write(String::FromHex(dec, temp, false, 4), color, TempMode);
                    msg++;
                }
                // color specifier
                else if (*msg == 'a')
                {
                    COL4 fg = (COL4)va_arg(args, uint);
                    color = fg;
                    msg++;
                }
                 // output type specifier
                else if (*msg == 'o') 
                { 
                    uint o = va_arg(args, uint);  
                    TempMode = (D_MODE)o;
                    msg++; 
                }
                // exponential float specifier
                else if (*msg == 'e') { (void)va_arg(args, float); msg++; }         // NOT YET IMPLEMENTED
                // float specifier
                else if (*msg == 'f') 
                { 
                    float dec = va_arg(args, float);
                    char temp[32];
                    Write(String::FromFloat(dec, temp, 4), color, TempMode);
                    msg++;
                } 
                // ignore
                else { WriteChar(*msg, color, TempMode); msg++; }
            }
            // otherwise print character
            else { WriteChar(*msg, color, TempMode); msg++; }
        }
    }

    void DebugHandler::ThrowMessage(char* msg, D_MSG type, ...)
    {
        PrintMessageTag(type, Mode);
        va_list args;
        va_start(args, type);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }

    void DebugHandler::ThrowInfo(char* msg, ...)
    {
        va_list args;
        PrintMessageTag(D_MSG_INFO, Mode);
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }

    void DebugHandler::ThrowOK(char* msg, ...)
    {
        PrintMessageTag(D_MSG_OK, Mode);
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }

    void DebugHandler::ThrowWarning(char* msg, ...)
    {
        PrintMessageTag(D_MSG_WARNING, Mode);
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }

    void DebugHandler::ThrowError(char* msg, ...)
    {
        PrintMessageTag(D_MSG_ERROR, Mode);
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }

    void DebugHandler::ThrowPanic(char* msg, ...)
    {
        PrintMessageTag(D_MSG_PANIC, Mode);
        va_list args;
        va_start(args, msg);
        PrintFormatted(msg, args);
        va_end(args);
        NewLine();
    }
}
