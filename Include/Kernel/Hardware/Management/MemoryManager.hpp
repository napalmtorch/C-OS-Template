#pragma once
#include <Kernel/Lib/Types.hpp>
#include <Kernel/Lib/Memory.hpp>
#include <Kernel/Core/Debug.hpp>

namespace System
{
    // enumerator for managing various resource types
    enum class ResourceType : uint8_t
    {
        Unused          = 0x00,
        Default         = 0x01,
        String          = 0x02,
        Bitmap          = 0x03,
    };

    namespace HAL
    {
        // structure for managing memory map entries
        typedef struct
        {
            uint Size;
            uint AddressLow;
            uint AddressHigh;
            uint LengthLow;
            uint LengthHigh;
            uint Type;
        } attr_pack MemoryMapEntry;

        // structure for managing heap entries
        typedef struct
        {
            uint           Address;         // physical address of data in memory
            uint           Size;            // physical size of data in memory
            ResourceType   Type;            // type of memory allocation
        } attr_pack HeapEntry;

        // structure for managing various properties of the heap table
        typedef struct
        {
            uint TableAddress;      // starting address of the entry table
            uint TableSize;         // size in bytes of the entry table
            uint TablePosition;     // current position of the entry table
            uint TableCount;        // current amount of entries in the entry table
            uint MaxEntries;        // maximum amount of entries in the entry table
            uint DataAddress;       // starting address of physical data
            uint DataSize;          // maximum usable size in bytes of physical data
            uint DataUsed;          // currently used size in bytes of physical data
            uint DataMaxAddress;    // maximum address usable determined by memory map
            uint DataPosition;      // acts like a watermark
            uint MMapAddress;       // address of our copy of the memory map
            uint MMapSize;          // size in bytes of our copy of the memory map
            uint MMapCount;         // current amount of entries in the memory map
        } attr_pack MemoryHeap;

        #define MM_ALIGN 0x10000

        // class for managing memory heaps
        class MemoryManager
        {
            private:
                MemoryHeap Heap;

            private:
                bool MessagesEnabled;
                bool DirtyMode;
                bool MemoryMapReady;
                DebugHandler Debug;

            public:
                void Initialize();
                void Initialize(bool dirty);
                void ToggleMessages(bool state);

            public:
                void PrintHeapTable(bool debug);
                void PrintMemoryMap(bool debug);
                void PrintMemoryMapEntry(MemoryMapEntry* entry, bool debug);

            private:
                void ReadMemoryMap();
                void PrintAllocate(uint addr, uint size, ResourceType type, void* thread, void** reference);
                void PrintFree(HeapEntry* entry);

            public:
                void* Allocate(uint size);
                void* Allocate(uint size, ResourceType type);
                void* AllocateSilent(uint size);
                void* AllocateSilent(uint size, ResourceType type);
                void  Free(void* ptr);   
                void  FreeSilent(void* ptr);   
                void  FreeArray(void** ptr, uint len);
                void  FreeArraySilent(void** ptr, uint len);
                HeapEntry* GetEntryByPointer(void* ptr);
                
            public:
                HeapEntry* GetEntry(int index);
                HeapEntry* GetFreeEntry(uint size);
                HeapEntry* GetUnusedEntry(uint size);
                HeapEntry* GetNearest(HeapEntry* entry);
                HeapEntry* CreateEntry(uint addr, uint size, ResourceType type);      
                bool       DeleteEntry(HeapEntry* entry);
                bool       IsAddressValid(uint addr);
                void       MergeFreeEntries();

            public:
                uint GetSize(uint* ptr);
                uint GetHeapCount();
                uint GetMaxHeapCount();
                uint GetRAMPhysical();
                uint GetRAMReserved();
                uint GetRAMUsed();
                uint GetRAMFree();
        };
    }
}