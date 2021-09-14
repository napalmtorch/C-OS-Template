#pragma once
#include <Kernel/Lib/Types.hpp>
#include <Kernel/Lib/Memory.hpp>
#include <Kernel/Lib/Math.hpp>
#include <Kernel/Core/Debug.hpp>
#include <Kernel/Hardware/PIT.hpp>
#include <Kernel/Hardware/Ports.hpp>
#include <Kernel/Hardware/Serial.hpp>
#include <Kernel/Hardware/Multiboot.hpp>
#include <Kernel/Hardware/Management/MemoryManager.hpp>
#include <Kernel/Hardware/Interrupts/IDT.hpp>
#include <Kernel/Hardware/Interrupts/ISR.hpp>

extc
{
    extern uint KernelStart;
    extern uint KernelEnd;
    extern uint KernelSize;
}

namespace System
{
    class KernelBase
    {
        public:
            HAL::MultibootHeader Multiboot;
            DebugHandler Debug;

        private:
            bool FinishedBooting;

        public:
            void BootStage1();
            void BootStage2();
            void Run();

        public:
            bool HasFinishedBooting();

        private:
            void FetchMultiboot();

        public:
            uint GetAddressStart();
            uint GetAddressEnd();
            uint GetSize();
    };

    extern KernelBase Kernel;
    extern HAL::InterruptRequestManager InterruptMgr;
    extern HAL::SerialController Serial;
    extern HAL::MemoryManager MemoryMgr;
    extern HAL::PITController PIT;
}