#include <Kernel/Hardware/Ports.hpp>

extc uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

extc uint16_t inw(uint16_t port)
{
    uint16_t result;
    asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

extc uint32_t inl(uint16_t port)
{
    uint32_t rv;
    asm volatile("inl %%dx, %%eax" : "=a" (rv) : "dN" (port));
    return rv;
}

extc void insw(uint16_t port, uint8_t* data, uint32_t size)
{
    asm volatile("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}

extc void outb(uint16_t port, uint8_t data)
{
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

extc void outw(uint16_t port, uint16_t data)
{
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port)); 
}

extc void outl(uint16_t port, uint32_t data)
{
    asm volatile("outl %%eax, %%dx" : : "dN" (port), "a" (data));
}

extc void outsw(uint16_t port, uint8_t* data, uint32_t size)
{
    asm volatile("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}

namespace System
{
    namespace HAL
    {
        namespace Ports
        {
            byte Read8(ushort port) { return inb(port); }
            ushort Read16(ushort port) { return inw(port); }
            uint Read32(ushort port) { return inl(port); }
            void ReadString(ushort port, byte* data, uint size) { insw(port, data, size); }

            void Write8(ushort port, byte data) { outb(port, data); }
            void Write16(ushort port, ushort data) { outw(port, data); }
            void Write32(ushort port, uint data) { outl(port, data); }
            void WriteString(ushort port, byte* data, uint size) { outsw(port, data, size); }
        }
    }
}