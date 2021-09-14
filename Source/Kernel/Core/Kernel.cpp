#include <Kernel/Core/Kernel.hpp>

extc
{
    uint KernelSize = 0;
}

// kernel object instances
namespace System
{
    // kernel base
    KernelBase Kernel;

    // interrupt request manager
    HAL::InterruptRequestManager InterruptMgr;

    // serial port controller
    HAL::SerialController Serial;

    // memory manager
    HAL::MemoryManager MemoryMgr;

    // programmable interval timer controller
    HAL::PITController PIT;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------

namespace System
{   
    
    // initialize core kernel functionality
    void KernelBase::BootStage1()
    {
        // initialize interrupt request manager
        InterruptMgr.Initialize();

        // set debugging mode
        Debug.SetMode(D_MODE_SERIAL);

        // initialize serial controller
        Serial.SetPort(SERIALPORT_COM1);
        Debug.ThrowOK("Initialized serial controller connection on COM1");

        // initialize interrupt service routines
        InterruptMgr.Initialize();
        Debug.ThrowOK("Initialized interrupt service routines");

        // fetch multiboot header from memory and store a copy
        FetchMultiboot();
        Debug.ThrowOK("Located multiboot header");

        // initialize memory manager
        MemoryMgr.Initialize(false);
        Debug.ThrowOK("Initialized memory manager");

        // enable interrupts
        InterruptMgr.EnableInterrupts();
    }
    
    // initialize optional hardware and prepare user
    void KernelBase::BootStage2()
    {
        // set boot flag
        Debug.ThrowOK("Finished booting");
        FinishedBooting = true;
    }

    // kernel main loop
    void KernelBase::Run()
    {
        
    }

    void KernelBase::FetchMultiboot()
    {
        // copy data to struct
        HAL::MultibootHeader* header = (HAL::MultibootHeader*)0x10000;
        Memory::Copy(&Multiboot, header, sizeof(HAL::MultibootHeader));
    }

    uint KernelBase::GetAddressStart() { return (uint)&KernelStart; }

    uint KernelBase::GetAddressEnd() { return (uint)&KernelEnd; }

    uint KernelBase::GetSize() { return KernelSize; }

    // return whether system has completed boot sequence
    bool KernelBase::HasFinishedBooting() { return FinishedBooting; }
}