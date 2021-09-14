#include <Kernel/Hardware/Serial.hpp>
#include <Kernel/Core/Kernel.hpp>

namespace System
{
    namespace HAL
    {
        const char* PortStrings[5] = 
        {
            "Disabled",
            "COM1",
            "COM2",
            "COM3",
            "COM4",
        };

        const char* SerialController::SerialPortToString(SERIALPORT port)
        {
            byte i = 0;
            switch (port)
            {
                case SERIALPORT_DISABLED: { i = 0; break; }
                case SERIALPORT_COM1:     { i = 1; break; }
                case SERIALPORT_COM2:     { i = 2; break; }
                case SERIALPORT_COM3:     { i = 3; break; }
                case SERIALPORT_COM4:     { i = 4; break; }
                default:                   { i = 0; break; }
            }
            return (char*)PortStrings[i];
        }

        // set active serial port
        void SerialController::SetPort(SERIALPORT port)
        {
            // set port variable
            CurrentPort = port;
            
            // send port data to serial controller
            Ports::Write8((uint)CurrentPort + 1, 0x00);    // disable all interrupts
            Ports::Write8((uint)CurrentPort + 3, 0x80);    // set baud rate divisor
            Ports::Write8((uint)CurrentPort + 0, 0x03);    // set divisor to 3 (lo byte) 38400 baud
            Ports::Write8((uint)CurrentPort + 1, 0x00);    // hi byte
            Ports::Write8((uint)CurrentPort + 3, 0x03);    // 8 bits, no parity, one stop bit
            Ports::Write8((uint)CurrentPort + 2, 0xC7);    // enable fifo, clear them, with 14-byte threshold
            Ports::Write8((uint)CurrentPort + 4, 0x0B);    // irqs enabled, rts/dsr set
            Ports::Write8((uint)CurrentPort + 4, 0x1E);    // set in loopback mode, test the serial chip
            Ports::Write8((uint)CurrentPort + 0, 0xAE);    // test serial chip (send byte 0xAE and check if serial returns same byte)
        
            // check if serial is faulty
            if(Ports::Read8((uint)CurrentPort) != 0xAE) { /* FAILURE */ return; }
            
            // ff serial is not faulty set it in normal operation mode
            Ports::Write8((uint)CurrentPort + 4, 0x0F);
        }

        // get active serial port
        SERIALPORT SerialController::GetPort() { return CurrentPort; }

        // read byte from serial controller
        char SerialController::Read()
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return 0; }
            while (HasRecieved() == 0);
            return Ports::Read8((ushort)CurrentPort);
        }

        void SerialController::WriteChar(char c)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            while (CanSend() == 0);
            Ports::Write8((ushort)CurrentPort, c);
        }

        void SerialController::WriteChar(char c, byte color)
        {
            WriteChar(27);
            WriteChar('[');
            switch (color)
            {
                case 0x0: { WriteChar('3'); WriteChar('4'); break; }
                case 0x1: { WriteChar('3'); WriteChar('4'); break; }
                case 0x2: { WriteChar('3'); WriteChar('2'); break; }
                case 0x3: { WriteChar('3'); WriteChar('6'); break; }
                case 0x4: { WriteChar('3'); WriteChar('1'); break; }
                case 0x5: { WriteChar('3'); WriteChar('5'); break; }
                case 0x6: { WriteChar('3'); WriteChar('3'); break; }
                case 0x7: { WriteChar('3'); WriteChar('7'); break; }
                case 0x8: { WriteChar('3'); WriteChar('7'); break; }
                case 0x9: { WriteChar('3'); WriteChar('4'); break; }
                case 0xA: { WriteChar('3'); WriteChar('2'); break; }
                case 0xB: { WriteChar('3'); WriteChar('6'); break; }
                case 0xC: { WriteChar('3'); WriteChar('1'); break; }
                case 0xD: { WriteChar('3'); WriteChar('5'); break; }
                case 0xE: { WriteChar('3'); WriteChar('3'); break; }
                case 0xF: { WriteChar('3'); WriteChar('7'); break; }
                default:  { WriteChar('0'); break; }
            }
            WriteChar('m');
            WriteChar(c);
            WriteChar(27);
            WriteChar('[');
            WriteChar('0');
            WriteChar('m');
        }

        void SerialController::Write(char* text)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            int i = 0;
            while (text[i] != 0) { WriteChar(text[i]); i++; }
        }

        void SerialController::Write(char* text, byte color)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            SetColor(color);
            Write(text);
            SetColor(0xFF);
        }

        void SerialController::WriteLine(char* text)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            Write(text);
            WriteChar('\n');
        }

        void SerialController::WriteLine(char* text, byte color)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            Write(text, color);
            WriteChar('\n');
        }

        void SerialController::SetColor(byte color)
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return; }
            switch (color)
            {
                case 0x0: { Write("\033[34");  break; }
                case 0x1: { Write("\033[34m"); break; }
                case 0x2: { Write("\033[32m"); break; }
                case 0x3: { Write("\033[36m"); break; }
                case 0x4: { Write("\033[31m"); break; }
                case 0x5: { Write("\033[35m"); break; }
                case 0x6: { Write("\033[33m"); break; }
                case 0x7: { Write("\033[37m"); break; }
                case 0x8: { Write("\033[37m"); break; }
                case 0x9: { Write("\033[34m"); break; }
                case 0xA: { Write("\033[32m"); break; }
                case 0xB: { Write("\033[36m"); break; }
                case 0xC: { Write("\033[31m"); break; }
                case 0xD: { Write("\033[35m"); break; }
                case 0xE: { Write("\033[33m"); break; }
                case 0xF: { Write("\033[37m"); break; }
                default:  { Write("\033[0m");  break; }
            }
        }

        byte SerialController::HasRecieved()
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return 0; }
            return Ports::Read8((uint)CurrentPort + 5) & 1;
        }

        byte SerialController::CanSend()
        {
            if (CurrentPort == SERIALPORT_DISABLED) { return 0; }
            return Ports::Read8((uint)CurrentPort + 5) & 0x20;
        }
    }
}