#include <Kernel/Hardware/PIT.hpp>
#include <Kernel/Core/Kernel.hpp>

namespace System
{
    namespace HAL
    {
        // initialize pit controller
        void PITController::Initialize(uint freq, ISR callback)
        {
            // validate frequency
            if (freq == 0) { Disable(); return; }

            // register interrupt
            InterruptMgr.Register(IRQ0, callback);

            // set frequency value
            if (freq > 5000) { Frequency = 5000; }
            else { Frequency = freq; }

            // pack frequency
            uint f = 1193180 / Frequency;
            byte high = (byte)((f & 0xFF00) >> 8);
            byte low  = (byte)((f & 0x00FF));

            Milliseconds = 0;
            TotalMilliseconds = 0;
            MillisTick = 0;

            // send frequency data to pit
            Ports::Write8(0x43, 0x36);
            Ports::Write8(0x40, low);
            Ports::Write8(0x40, high);
        }

        // disable pit controller
        void PITController::Disable()
        {
            // unregister interrupt
            InterruptMgr.Unregister(IRQ0);
            
            // reset properties
            Frequency         = 0;
            Milliseconds      = 0;
            TotalMilliseconds = 0;
        }

        // calculate pit times
        void PITController::CalculateMilliseconds()
        {
            // increment millis tick
            MillisTick++;

            // millisecond has passed
            if (MillisTick >= Frequency / 1000)
            {
                Milliseconds++;
                TotalMilliseconds++;
                MillisTick = 0;
            }

            // reset current millisecond timer
            if (Milliseconds >= 1000) { Milliseconds = 0; MillisTick = 0; }
        }

        // get currently set pit frequency
        uint PITController::GetFrequency() { return Frequency; }

        // get current millisecond within pit second
        uint PITController::GetMilliseconds() { return Milliseconds; }

        // get total amount of passed milliseconds
        ulong PITController::GetTotalMilliseconds() { return TotalMilliseconds; }
    }
}