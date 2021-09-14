#include <Boot/Entry.hpp>
#include <Kernel/Core/Kernel.hpp>

extc
{
    void KernelEntry()
    {
        // kernel boot sequence
        System::Kernel.BootStage1();
        System::Kernel.BootStage2();
        
        // validate that the system has successfully booted
        if (System::Kernel.HasFinishedBooting())
        {
            // kernel main loop
            while (true) { System::Kernel.Run(); }
        }
        else { /* SYSTEM UNABLE TO BOOT */ }

        // anything after this point is unexpected behavior - halt the system
        asm volatile("cli");
        asm volatile("hlt");
    }
}