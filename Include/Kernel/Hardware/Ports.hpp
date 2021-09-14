#pragma once
#include <Kernel/Lib/Types.hpp>

extc uint8_t  inb(uint16_t port);
extc uint16_t inw(uint16_t port);
extc uint32_t inl(uint16_t port);
extc void     insw(uint16_t port, uint8_t* data, uint32_t size);

extc void outb(uint16_t port, uint8_t data);
extc void outw(uint16_t port, uint16_t data);
extc void outl(uint16_t port, uint32_t data);
extc void outsw(uint16_t port, uint8_t* data, uint32_t size);

namespace System
{
    namespace HAL
    {
        namespace Ports
        {
            byte   Read8(ushort port);
            ushort Read16(ushort port);
            uint   Read32(ushort port);
            void   ReadString(ushort port, byte* data, uint size);      

            void Write8(ushort port, byte data);
            void Write16(ushort port, ushort data);
            void Write32(ushort port, uint data);
            void WriteString(ushort port, byte* data, uint size);
        }
    }
}