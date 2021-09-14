#pragma once
#include <Kernel/Lib/Types.hpp>

extc
{
    // kernel code selector
    #define KERNEL_CS 0x08

    typedef struct
    {
        ushort LowOffset;
        ushort Selector;
        byte   AlwaysZero;
        byte   Flags;
        ushort HighOffset;
    } attr_pack IDTGate;

    typedef struct
    {
        ushort Limit;
        uint   Base;
    } attr_pack IDTRegister;

    void IDTSetGate(int n, uint handler);
    void IDTSet();
}