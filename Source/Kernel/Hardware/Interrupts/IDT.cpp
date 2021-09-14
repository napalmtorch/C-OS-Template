#include <Kernel/Hardware/Interrupts/IDT.hpp>

extc
{
    #define IDT_ENTRIES 256
    IDTGate IDT[IDT_ENTRIES];
    IDTRegister IDTReg;

    void IDTSetGate(int n, uint handler)
    {
        IDT[n].LowOffset  = LowBits16(handler);
        IDT[n].Selector   = KERNEL_CS;
        IDT[n].AlwaysZero = 0;
        IDT[n].Flags      = 0x8E;
        IDT[n].HighOffset = HighBits16(handler);
    }

    void IDTSet()
    {
        IDTReg.Base = (uint)&IDT;
        IDTReg.Limit = IDT_ENTRIES * sizeof(IDTGate) - 1;
        asm volatile("lidtl (%0)" : : "r" (&IDTReg));
    }
}