#pragma once
#include <Kernel/Lib/Types.hpp>
#include <Kernel/Hardware/Interrupts/ISR.hpp>

namespace System
{
    namespace HAL
    {
        class PITController
        {
            private:
                uint Frequency;
                uint Milliseconds;
                ulong TotalMilliseconds;
                uint MillisTick;

            public:
                void Initialize(uint freq, ISR callback);
                void Disable();
                void CalculateMilliseconds();

            public:
                uint GetFrequency();
                uint GetMilliseconds();
                ulong GetTotalMilliseconds();
        };
    }
}