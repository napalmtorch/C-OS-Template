#include <Kernel/Hardware/Management/MemoryManager.hpp>
#include <Kernel/Core/Kernel.hpp>

namespace System
{
    namespace HAL
    {
        void MemoryManager::Initialize() { Initialize(false); }

        void MemoryManager::Initialize(bool dirty)
        {
            // enable messages by default
            MemoryMapReady = false;
            MessagesEnabled = true;

            // set address for memory map copy
            Heap.MMapAddress = (uint)Kernel.GetAddressEnd() + MM_ALIGN;

            // read memory map from multiboot
            ReadMemoryMap();

            // set mode
            DirtyMode = dirty;

            // dirty mode
            if (dirty)
            {
                // table is unused in this mode
                Heap.TableAddress  = 0;
                Heap.TablePosition = 0;
                Heap.TableCount    = 0;
                Heap.TableSize     = 0;
                Heap.MaxEntries    = 0;

                // data properties
                Heap.DataAddress = Heap.MMapAddress + Heap.MMapSize + MM_ALIGN;
                Heap.DataAddress &= 0xFFFFF000;
                Heap.DataAddress += 0x1000;
                Heap.DataSize    = Heap.DataMaxAddress - Heap.DataAddress;
                Heap.DataUsed    = 0;
                Heap.DataPosition = Heap.DataAddress;
            }
            else
            // default mode
            {
                // align table address
                uint addr = Heap.MMapAddress + Heap.MMapSize + MM_ALIGN;

                // reset table
                Heap.MaxEntries    = 65536;
                Heap.TableAddress  = addr;
                Heap.TableSize     = Heap.MaxEntries * sizeof(HeapEntry);
                Heap.TablePosition = 0;
                Heap.TableCount    = 0;

                // clear table
                Memory::Set((void*)Heap.TableAddress, 0, Heap.TableSize);

                addr = Heap.TableAddress + Heap.TableSize + MM_ALIGN;

                // reset data
                Heap.DataAddress  = addr;
                Heap.DataSize     = Heap.DataMaxAddress - Heap.DataAddress;
                Heap.DataUsed     = 0;
                Heap.DataPosition = 0;

                // clear data
                Memory::Set((void*)Heap.DataAddress, 0, Heap.DataSize);

                // create primary entry
                (void)CreateEntry(Heap.DataAddress, Heap.DataSize, ResourceType::Unused);
            }        
        }

        void MemoryManager::ReadMemoryMap()
        {
            // check if memory map has already been read
            if (!MemoryMapReady)
            {
                // get memory map properties
                Heap.MMapSize  = Kernel.Multiboot.MemoryMapLength;
                Heap.MMapCount = 0;
                int pos = 0;

                // loop through memory map entries
                for (uint i = 0; i < Kernel.Multiboot.MemoryMapLength; i += sizeof(MemoryMapEntry))
                {
                    // get entry
                    MemoryMapEntry* entry = (MemoryMapEntry*)(Kernel.Multiboot.MemoryMapAddress + i);

                    // located valid entry
                    if (entry->Type > 0) 
                    { 
                        // add entry to our copy
                        Memory::Copy((void*)(Heap.MMapAddress + pos), entry, sizeof(MemoryMapEntry));

                        // found available block
                        if (entry->Type == 0x01 && i > 0)
                        {
                            // get max address of data region
                            Heap.DataMaxAddress = entry->AddressLow + entry->LengthLow;

                            // calculate kernel size
                            KernelSize = KernelEnd - entry->AddressLow;
                        }

                        // increment count
                        Heap.MMapCount++;
                        pos += sizeof(MemoryMapEntry);     
                    }
                }

                // validate that memory map has been read
                MemoryMapReady = true;
            }
        }
        
        void MemoryManager::ToggleMessages(bool state)
        {
            // set messages enabled flag
            MessagesEnabled = state;
        }
        
        void MemoryManager::PrintHeapTable(bool debug)
        {
            // determine debug mode
            D_MODE mode = D_MODE_CONSOLE;
            if (debug) { mode = D_MODE_SERIAL; }

            // print header message
            Debug.PrintLine("%o%a-------- %aHEAP TABLE%a ---------------------------------------%a", mode, COL4_DARK_GRAY, COL4_GREEN, COL4_DARK_GRAY, COL4_WHITE);

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* e = GetEntry(i);

                // validate entry
                if (IsAddressValid(e->Address) && e->Size > 0)
                {
                    // print info
                    COL4 tc = COL4_BLACK;
                    switch ((ResourceType)e->Type)
                    {
                        case ResourceType::Unused:       { tc = COL4_GREEN; break; }
                        case ResourceType::Default:      { tc = COL4_WHITE; break; }
                        case ResourceType::String:       { tc = COL4_RED; break; }
                        case ResourceType::Bitmap:       { tc = COL4_CYAN; break; }
                        default: { tc = COL4_BLACK; break; }
                    }

                    char temp[32];
                    uint size = e->Size;
                    if (size < 1024) { String::FromDecimal(size, temp); String::Append(temp, " BYTES"); }
                    else if (size >= 1024 && size < 1024 * 1024) { String::FromDecimal(size / 1024, temp); String::Append(temp, " KB"); }
                    else if (size >= 1024 * 1024 && size < 1024 * 1024 * 1024) { String::FromDecimal(size / 1024 / 1024, temp); String::Append(temp, " MB"); }
                    else if (size >= 1024 * 1024 * 1024) { String::FromDecimal(size / 1024 / 1024 / 1024, temp); String::Append(temp, " GB"); }

                    
                    Debug.Print("%o[%a%4x%a] ", mode, COL4_YELLOW, i, COL4_WHITE);
                    Debug.PrintLine("%o%aADDR%a = 0x%8x  %aTYPE%a = %a0x%2x%a  %aSIZE%a = %s", mode,
                                COL4_BLUE, COL4_WHITE, e->Address, COL4_BLUE, COL4_WHITE, tc, (uint)e->Type, COL4_WHITE, COL4_BLUE, COL4_WHITE, temp);
                }
            }
        }
        
        
        void MemoryManager::PrintMemoryMap(bool debug)
        {
            
        }
        
        void MemoryManager::PrintMemoryMapEntry(MemoryMapEntry* entry, bool debug)
        {
            
        }
        
        void MemoryManager::PrintAllocate(uint addr, uint size, ResourceType type, void* thread, void** reference)
        {
            char temp[16];
            Debug.Write("[");
            Debug.Write("MALLOC", COL4_CYAN);
            Debug.Write("] ");
            Debug.Write("ADDR", COL4_GREEN);
            Debug.Write(" = ");
            Debug.Write(String::FromHex(addr, temp, true, 4));
            Debug.Write("  TYPE", COL4_GREEN);
            Debug.Write(" = ");
            Debug.Write(String::FromHex((uint)type, temp, true, 1));
            Debug.Write("  SIZE", COL4_GREEN);
            Debug.Write(" = ");
            Debug.WriteLine(String::FromDecimal(size, temp));
        }
        
        void MemoryManager::PrintFree(HeapEntry* entry)
        {
            char temp[16];
            Debug.Write("[");
            Debug.Write(" FREE ", COL4_YELLOW);
            Debug.Write("] ");
            Debug.Write("ADDR", COL4_GREEN);
            Debug.Write(" = ");
            Debug.Write(String::FromHex(entry->Address, temp, true, 4));
            Debug.Write("  TYPE", COL4_GREEN);
            Debug.Write(" = ");
            Debug.Write(String::FromHex((uint)entry->Type, temp, true, 1));
            Debug.Write("  SIZE", COL4_GREEN);
            Debug.Write(" = ");
            Debug.WriteLine(String::FromDecimal(entry->Size, temp));
        }
        
        void* MemoryManager::Allocate(uint size) { return Allocate(size, ResourceType::Default); }
        
        void* MemoryManager::Allocate(uint size, ResourceType type)
        {
            // validate size
            if (size == 0) { return nullptr; }

            // allocate memory in dirty mode
            uint addr = 0;
            if (DirtyMode)
            {
                // get address
                addr = Heap.DataPosition;
                if (addr & 0xFFFFF000)
                {
                    addr &= 0xFFFFF000;
                    addr += 0x1000;
                }

                size &= 0xFFFFF000;
                size += 0x1000;
                
                // increment address
                Heap.DataPosition += size;
                
                // increment used memory counter
                Heap.DataUsed += size;

                // fill memory
                Memory::Set((void*)addr, 0, size);

                // print message and return physical address
                PrintAllocate(addr, size, type, nullptr, nullptr);
                return (void*)addr;
            }
            // allocate memory normally
            else
            {
                // get free entry
                HeapEntry* entry = GetFreeEntry(size + 2);

                // validate entry
                if (entry == nullptr) { goto out_of_ram; }

                // fill memory
                Memory::Set((void*)entry->Address, 0, entry->Size);

                // modify entry
                entry->Type = type;
                //if (TaskMgr.Properties.Flags.Running) { entry->Thread = TaskMgr.CurrentThread; }
                
                // set entry reference
                void* ptr = (void*)entry->Address;
                //entry->Reference = &ptr;

                // increment used memory counter
                Heap.DataUsed += entry->Size;

                // print message and return physical address
                PrintAllocate(entry->Address, size + 2, (ResourceType)entry->Type, 0, nullptr);
                return (void*)entry->Address;
            }

            // unable to allocate memory
            out_of_ram:
            Debug.ThrowError("System out of memory while attempting to allocate %d bytes", size + 2);
            asm volatile("cli");
            asm volatile("hlt");
            return nullptr;
        }
        
        void* MemoryManager::AllocateSilent(uint size) { return AllocateSilent(size, ResourceType::Default); }
        
        void* MemoryManager::AllocateSilent(uint size, ResourceType type)
        {
            // validate size
            if (size == 0) { return nullptr; }

            // allocate memory in dirty mode
            uint addr = 0;
            if (DirtyMode)
            {
                // get address
                addr = Heap.DataPosition;
                
                // increment address
                Heap.DataPosition += size + 2;
                
                // increment used memory counter
                Heap.DataUsed += size + 2;

                // fill memory
                Memory::Set((void*)addr, 0, size + 2);

                // return physical address
                return (void*)addr;
            }
            // allocate memory normally
            else
            {
                // get free entry
                HeapEntry* entry = GetFreeEntry(size + 2);

                // validate entry
                if (entry == nullptr) { goto out_of_ram; }

                // fill memory
                Memory::Set((void*)entry->Address, 0, entry->Size);

                // modify entry
                entry->Type = type;

                // increment used memory counter
                Heap.DataUsed += entry->Size;

                // return the address
                return (void*)entry->Address;
            }

            // unable to allocate memory
            out_of_ram:
            Debug.ThrowError("System out of memory while attempting to allocate %d bytes", size + 2);
            asm volatile("cli");
            asm volatile("hlt");
            return nullptr;
        }
        
        void MemoryManager::Free(void* ptr)
        {
            // ignore free request if in dirty mode
            if (DirtyMode) { return; }

            // validate pointer
            if (!IsAddressValid((uint)ptr)) { return; }

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* temp = GetEntry(i);

                // check for matching entry
                if (temp->Address == (uint)ptr)
                {
                    // decrement used
                    Heap.DataUsed -= temp->Size;

                    // message
                    PrintFree(temp);

                    // clear
                    Memory::Set((void*)temp->Address, 0, temp->Size);

                    // set state
                    temp->Type = ResourceType::Unused;

                    // merge free entries and return success
                    MergeFreeEntries();
                    return;
                }
            }
        }
        
        void MemoryManager::FreeSilent(void* ptr)
        {
            // ignore free request if in dirty mode
            if (DirtyMode) { return; }

            // validate pointer
            if (!IsAddressValid((uint)ptr)) { return; }

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* temp = GetEntry(i);

                // check for matching entry
                if (temp->Address == (uint)ptr)
                {
                    // decrement used
                    Heap.DataUsed -= temp->Size;

                    // clear
                    Memory::Set((void*)temp->Address, 0, temp->Size);

                    // set state
                    temp->Type = ResourceType::Unused;

                    // merge free entries and return success
                    MergeFreeEntries();
                    return;
                }
            }
        }

        void MemoryManager::FreeArray(void** ptr, uint len)
        {
            if (ptr == nullptr) { return; }
            for (uint i = 0; i < len; i++)
            {
                if (ptr[i] != nullptr) { Free(ptr[i]); }
            }
            Free(ptr);
        }
        
        void MemoryManager::FreeArraySilent(void** ptr, uint len)
        {
            if (ptr == nullptr) { return; }
            for (uint i = 0; i < len; i++)
            {
                if (ptr[i] != nullptr) { FreeSilent(ptr[i]); }
            }
            FreeSilent(ptr);
        }
        
        HeapEntry* MemoryManager::GetEntryByPointer(void* ptr)
        {
            if (ptr == nullptr) { return nullptr; }

            // attempt to locate perfectly sized region
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                // check if entry is valid
                if (IsAddressValid(entry->Address) && entry->Address == (uint)ptr)
                {
                    return entry;
                }
            }
            return nullptr;
        }
        
        HeapEntry* MemoryManager::GetEntry(int index)
        {
            // validate index
            if ((uint)index >= Heap.MaxEntries) { return nullptr; }
            // return entry
            return (HeapEntry*)(Heap.TableAddress + (index * sizeof(HeapEntry)));
        }
        
        HeapEntry* MemoryManager::GetFreeEntry(uint size)
        {
            // check if size is valid
            if (size == 0) { return nullptr; }

            // attempt to locate perfectly sized region
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                // check if entry is valid
                if (IsAddressValid(entry->Address) && size == entry->Size && entry->Type == ResourceType::Unused)
                {
                    // set state to used and return entry
                    entry->Type = ResourceType::Default;
                    return entry;
                }
            }

            // borrow from mass entry
            HeapEntry* mass = GetEntry(0);

            // calculate new address
            uint addr = mass->Address;

            HeapEntry* entry = CreateEntry(addr, size, ResourceType::Default);

            mass->Address += size;
            mass->Size    -= size;
            mass->Type     = ResourceType::Unused;
            return entry;
        }
        
        HeapEntry* MemoryManager::GetUnusedEntry(uint size)
        {
            // validate size
            if (size == 0) { return nullptr; }

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                // validate entry is unused and return
                if (entry->Address == 0 && entry->Size == 0 && entry->Type == ResourceType::Unused) { return entry; }
            }

            // unable to locate unused entry
            return nullptr;
        }
        
        HeapEntry* MemoryManager::GetNearest(HeapEntry* entry)
        {
            // validate entry
            if (entry == nullptr) { return nullptr; }

            // validate address
            if (!IsAddressValid(entry->Address)) { return nullptr; }

            // loop through entries
            for (uint i = 1; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* temp = GetEntry(i);

                // return neighbouring entries
                if (temp != nullptr && temp != entry &&  temp->Address +  temp->Size == entry->Address && temp->Type == ResourceType::Unused) { return temp; }
                if (temp != nullptr && temp != entry && entry->Address - entry->Size == temp->Address  && temp->Type == ResourceType::Unused) { return temp; }
            }

            return nullptr;
        }
        
        HeapEntry* MemoryManager::CreateEntry(uint addr, uint size, ResourceType type)
        {
            // validate address and size
            if (!IsAddressValid(addr)) { return nullptr; }
            if (size == 0) { return nullptr; }

            // get entry offset
            HeapEntry* entry = GetUnusedEntry(size);

            // validate entry
            if (entry == nullptr) { return nullptr; }

            // set properties
            entry->Address = addr;
            entry->Size    = size;
            entry->Type    = type;

            // increment and return
            Heap.TablePosition++;
            Heap.TableCount++;
            return entry;
        }
        
        bool MemoryManager::DeleteEntry(HeapEntry* entry)
        {
            // validate entry
            if (entry == nullptr) { return false; }

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* temp = GetEntry(i);

                // validate entry
                if (temp->Address == entry->Address && temp->Size == entry->Size && temp->Type == entry->Type)
                {
                    // clear entry and return success
                    temp->Address = 0;
                    temp->Size    = 0;
                    temp->Type    = ResourceType::Unused;
                    Heap.TableCount--;
                    Heap.TablePosition--;
                    return true;
                }
            }

            // error
            return false;
        }
        
        bool MemoryManager::IsAddressValid(uint addr)
        {
            if (addr < Heap.DataAddress || addr >= Heap.DataMaxAddress) { return false; }
            return true;
        }
        
        void MemoryManager::MergeFreeEntries()
        {
            HeapEntry* mass = GetEntry(0);
            uint free_total = 0;
            uint free_addr = 0xFFFFFFFF;
            char temp[64];

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);
                
                // validate entry
                if (IsAddressValid(entry->Address) && entry->Type == ResourceType::Unused)
                {
                    // get nearest
                    HeapEntry* nearest = GetNearest(entry);

                    // validate nearest entry
                    if (nearest != nullptr && nearest != entry && nearest != mass && entry != mass)
                    {
                        if (entry->Address > nearest->Address) { entry->Address = nearest->Address; }
                        entry->Size += nearest->Size;
                        DeleteEntry(nearest);
                    }
                }
            }

            // attempt to free mass neighbour
            HeapEntry* nearest = GetNearest(mass);

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                if (entry->Address != mass->Address)
                {
                    if (entry->Address + entry->Size == mass->Address && entry->Type == ResourceType::Unused)
                    {
                        mass->Address = entry->Address;
                        mass->Size += entry->Size;
                        mass->Type = ResourceType::Unused;
                        DeleteEntry(entry);
                        break;
                    }
                }
            }
        }  

        uint MemoryManager::GetSize(uint* ptr)
        {
        uint addr = (uint)ptr;
            if (!IsAddressValid(addr)) { return 0; }

            // loop through entries
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                // found matching entry, return entry size
                if (entry->Address == addr && (byte)entry->Type > 0) { return entry->Size; }
            }
            // unable to find matching entry in table
            return 0;
        }
        
        uint MemoryManager::GetHeapCount()
        {
            // loop through entries
            uint count = 0;
            for (uint i = 0; i < Heap.MaxEntries; i++)
            {
                // get entry
                HeapEntry* entry = GetEntry(i);

                // entry is not null
                if (entry->Address > 0 && entry->Size > 0) { count++; }
            }

            // return count of heap entries
            return count;   
        }
        
        uint MemoryManager::GetMaxHeapCount() { return Heap.MaxEntries; }
        
        uint MemoryManager::GetRAMPhysical()
        {
            register ulong* mem;
            ulong mem_count, a;
            uint memkb;
            byte	irq1, irq2;

            irq1 = Ports::Read8(0x21);
            irq2 = Ports::Read8(0xA1);

            Ports::Write8(0x21, 0xFF);
            Ports::Write8(0xA1, 0xFF);

            mem_count = 0;
            memkb = 0;
            __asm__ __volatile__("wbinvd");

            do 
            {
                memkb++;
                mem_count += 1024 * 1024;
                mem = (ulong*)mem_count;

                a = *mem;
                *mem = 0x55AA55AA;

                asm("":::"memory");
                if (*mem != 0x55AA55AA) { mem_count = 0; }
                else 
                {
                    *mem = 0xAA55AA55;
                    asm("":::"memory");
                    if (*mem != 0xAA55AA55)
                        mem_count = 0;
                }

                asm("":::"memory");
                *mem = a;

            } while (memkb < 4096 && mem_count != 0);

            mem = (ulong*)0x413;

            Ports::Write8(0x21, irq1);
            Ports::Write8(0xA1, irq2);

            return memkb * 1024 * 1024;
        }
        
        uint MemoryManager::GetRAMReserved() { return Heap.DataSize; }
        
        uint MemoryManager::GetRAMUsed() { return Heap.DataUsed; }
        
        uint MemoryManager::GetRAMFree() { return Heap.DataSize - Heap.DataUsed; }
        
    }
}