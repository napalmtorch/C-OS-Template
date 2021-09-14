#include <Kernel/Lib/String.hpp>
#include <Kernel/Core/Kernel.hpp>

namespace String
{
    const char HexValues[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    size_t Length(char* text)
    {
        size_t len = 0;
        while (text[len] != 0) { len++; }
        return len;
    }

    char* ToUpper(char* text)
    {
        for (size_t i = 0; i < Length(text); i++)
        {
            if (text[i] >= 97 && text[i] <= 122) { text[i] = text[i] - 32; }
        }
        return text;
    }

    char* ToLower(char* text)
    {
        for (size_t i = 0; i < Length(text); i++)
        {
            if (text[i] >= 65 && text[i] <= 90) { text[i] = text[i] + 32; }
        }
        return text;
    }

    char* Append(char* src, char* add)
    {
        if (add == nullptr) { return src; }
        char* ptr = (char*)((uint)src + Length(src));
        while (*add != 0) { *ptr++ = *add++; }
        *ptr = 0;
        return src;
    } 

    char* Append(char* src, char* add, size_t len)
    {
        if (add == nullptr) { return src; }
        char* ptr = src + Length(src);
        while (*add != 0 && len--) { *ptr++ = *add++; }
        *ptr = 0;
        return src;
    }

    char* Append(char* src, char c)
    {
        size_t len = Length(src);
        src[len] = c;
        src[len + 1] = 0;
        return src;
    }

    char* InsertNew(char* src, int index, char c)
    {
        return nullptr;
    }

    char* InsertNew(char* src, int index, char* val)
    {
        int i = 0;
        char* output = src;
        for (uint i = 0; i < Length(val); i++)
        {
            output = InsertNew(output, i, val[i]);
        }
        System::MemoryMgr.Free(src);
        return output;
    }

    char* Insert(char* src, int index, size_t len, char c)
    {
        return nullptr;
    }

    char* Insert(char* src, int index, size_t len, char* val)
    {
        for (uint i = 0; i < Length(val); i++)
        {
            src = Insert(src, i, len, val[i]);
        }
        return src;
    }

    char* DeleteAt(char* src, int index, size_t len)
    {
        return nullptr;
    }

    char* DeleteAt(char* src, int index, size_t del_len, size_t len)
    {
        for (uint i = 0; i < del_len; i++)
        {
            src = DeleteAt(src, index, len);
        }
        return src;
    }

    char* Copy(char* dest, char* src)
    {
        if (dest == nullptr) { return nullptr; }
        char* ptr = dest;
        while (*src != 0) { *dest = *src; dest++; src++; }
        *dest = 0;
        return ptr;
    }

    char* Copy(char* dest, char* src, size_t len)
    {
        if (dest == nullptr) { return nullptr; }
        char* ptr = dest;
        while (*src && len--) { *dest = *src; dest++; src++; }
        *dest = 0;
        return ptr;
    }

    int Compare(char* str1, char* str2)
    {
        while (*str1)
        {
            if (*str1 != *str2) { break; }
            str1++; str2++;
        }
        return *(byte*)str1 - *(byte*)str2;
    }

    int Compare(char* str1, char* str2, size_t len)
    {
        while (len && *str1 && (*str1 == *str2))
        {
            ++str1;
            ++str2;
            --len;
        }
        if (len == 0) { return 0; }
        else { return (*(byte*)str1 = *(byte*)str2); }
        return 0;
    }

    bool Equals(char* str1, char* str2)
    {
        if (str1 == nullptr || str2 == nullptr) { return false; }
        if (Compare(str1, str2) == 0) { return true; }
        else { return false; }
    }

    char* Reverse(char* text)
    {
        int c = 0, i = 0, j = 0;
        for (i = 0, j = Length(text) - 1; i < j; i++, j--)
        {
            c = text[i];
            text[i] = text[j];
            text[j] = c;
        }
        return text;
    }

    char* FromDecimal(int num, char* text)
    {
        int i = 0, sign = 0;
        if ((sign = num) < 0) { num = -num; }
        i = 0;
        do { text[i++] = num % 10 + '0'; }
        while ((num /= 10) > 0);
        if (sign < 0) { text[i++] = '-'; }
        text[i] = 0;
        return Reverse(text);
    }

    char* FromHex(uint num, char* text, bool prefix)
    {
        if (num <= UINT8_MAX) { return FromHex(num, text, prefix, 1); }
        else if (num >= UINT8_MAX && num <= UINT16_MAX) { return FromHex(num, text, prefix, 2); }
        else if (num >= UINT16_MAX && num <= UINT32_MAX) { return FromHex(num, text, prefix, 4); }
        return text;
    }

    char* FromHex(uint num, char* text, bool prefix, byte bytes)
    {
        Clear(text);
        if (prefix) { Append(text, "0x"); }

        if (bytes == 1)
        {
            Append(text, HexValues[(byte)((num & 0xF0) >> 4)]);
            Append(text, HexValues[(byte)((num & 0x0F))]);
            return text;
        }
        else if (bytes == 2)
        {
            Append(text, HexValues[(byte)((num & 0xF000) >> 12)]);
            Append(text, HexValues[(byte)((num & 0x0F00) >> 8)]);
            Append(text, HexValues[(byte)((num & 0x00F0) >> 4)]);
            Append(text, HexValues[(byte)((num & 0x000F))]);
            return text;
        }
        else if (bytes == 4)
        {
            Append(text, HexValues[(byte)((num & 0xF0000000) >> 28)]);
            Append(text, HexValues[(byte)((num & 0x0F000000) >> 24)]);
            Append(text, HexValues[(byte)((num & 0x00F00000) >> 20)]);
            Append(text, HexValues[(byte)((num & 0x000F0000) >> 16)]);
            Append(text, HexValues[(byte)((num & 0x0000F000) >> 12)]);
            Append(text, HexValues[(byte)((num & 0x00000F00) >> 8)]);
            Append(text, HexValues[(byte)((num & 0x000000F0) >> 4)]);
            Append(text, HexValues[(byte)((num & 0x0000000F))]);
            return text;
        }
        else { return text; }
    }

    void srev(char* str, int len)
    {
        int i = 0, j = len - 1, temp;
        while (i < j) 
        {
            temp = str[i];
            str[i] = str[j];
            str[j] = temp;
            i++;
            j--;
        }
    }

    int intToStr(int x, char str[], int d)
    {
        int i = 0;
        while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }
        while (i < d) { str[i++] = '0'; }
        srev(str, i);
        str[i] = '\0';
        return i;
    }

    char* FromFloat(float num, char* text, int afterdot)
    {
        int ipart = (int)num;
        float fpart = num - (float)ipart;
        int i = intToStr(ipart, text, 0);
        if (afterdot != 0)
        {
            text[i] = '.';
            fpart = fpart * MathUtil::Pow(10.0f, afterdot);
            intToStr((int)fpart, text + i + 1, afterdot);
        }
        return text;
    }

    int ToDecimal(char* text)
    {
        for (size_t i = 0; i < Length(text); i++)
        { if (text[i] < 48 || text[i] > 57) { return 0; } }
        int res = 0;
        for (int i = 0; text[i] != 0; i++) { res = res * 10 + text[i] - '0'; }
        return res;
    }

    uint ToHex(char* text)
    {
        uint num = 0;
        for (size_t i = 0; i < Length(text); i++)
        {
            if (text[i] >= 48 && text[i] <= 57) { num = (num << 4) | (text[i] - 48); }
            else if (text[i] == 'A' || text[i] == 'a') { num = (num << 4) | 0x0A; }
            else if (text[i] == 'B' || text[i] == 'b') { num = (num << 4) | 0x0B; }
            else if (text[i] == 'C' || text[i] == 'c') { num = (num << 4) | 0x0C; }
            else if (text[i] == 'D' || text[i] == 'd') { num = (num << 4) | 0x0D; }
            else if (text[i] == 'E' || text[i] == 'e') { num = (num << 4) | 0x0E; }
            else if (text[i] == 'F' || text[i] == 'f') { num = (num << 4) | 0x0F; }
            else { return 0 ; }
        }
        return num;
    }

    char* Clear(char* text)
    {
        text[0] = 0;
        return text;
    }

    bool Contains(char* text, char* find)
    {
        if (StartAt(text, find) != nullptr) { return true; }
        else { return false; }
    }
    
    char* Delete(char* text)
    {
        text[Length(text) - 1] = 0;
        return text;
    }

    char* Delete(char* text, size_t len)
    {
        for (size_t i = 0; i < len; i++) { Delete(text); }
        return text;
    }

    char* StartAt(char* src, char* find)
    {
        const char * out = nullptr;
        const char * ptr;
        const char * acc;
        const char * p;
        size_t s = Length(find);
        
        for (ptr = src; *ptr != '\0'; ++ptr) 
        {
            size_t accept = 0;
            out = ptr;
            p = ptr;

            for (acc = find; (*acc != '\0') && (*p != '\0'); ++acc) 
            {
                if (*p == *acc) { accept++; p++;
                } else { break; }
            }

            if (accept == s) { return (char *)out; }
        }
        return nullptr;
    }
        
    int IndexOf(char* text, char c)
    {
       for (int i = 0; text[i] != 0; i++) { if (text[i] == c) { return i; } }
       return -1;
    }

    char* Replace(char* src, char* rep, char* with)
    {
        char *result; 
        char *ins;
        char *tmp;
        int len_rep;
        int len_with;
        int len_front;
        int count;

        // sanity checks and initialization
        if (!src || !rep)
            return nullptr;
        len_rep = Length(rep);
        if (len_rep == 0)
            return nullptr; // empty rep causes infinite loop during count
        if (!with)
            with = "";
        len_with = Length(with);

        // count the number of replacements needed
        ins = src;
        for (count = 0; (tmp = StartAt(ins, rep)); ++count) { ins = tmp + len_rep; }

        tmp = result = (char*)System::MemoryMgr.AllocateSilent(Length(src) + (len_with - len_rep) * count + 1, System::ResourceType::String);

        if (!result) { return nullptr; }

        // first time through the loop, all the variable are set correctly rom here on
        while (count--) 
        {
            ins = StartAt(src, rep);
            len_front = ins - src;
            tmp = Copy(tmp, src, len_front) + len_front;
            tmp = Copy(tmp, with) + len_with;
            src += len_front + len_rep; // move to next "end of rep"
        }

        Copy(tmp, src);
        return result;
    }
  
    char** Split(char* str, char delim, uint* arr_length)
    {
        if (str == nullptr) { return nullptr; }
        if (Length(str) == 0) { return nullptr; }

        int len = Length(str);
        uint num_delimeters = 0;

        for(int i = 0; i < len - 1; i++)
        {
            if(str[i] == delim) { num_delimeters++; }
        }

        uint arr_size = sizeof(char*) * (num_delimeters + 1);
        char** str_array = (char**)System::MemoryMgr.AllocateSilent(arr_size, System::ResourceType::String);
        int str_offset = 0;

        int start = 0;
        int end = 0;
        while(end < len)
        {
            while(str[end] != delim && end < len) { end++; }

            char* substr = (char*)System::MemoryMgr.AllocateSilent(end - start + 1, System::ResourceType::String);
            Memory::Copy(substr, str + start, end - start);
            start = end + 1;
            end++;
            str_array[str_offset] = substr;
            str_offset++;
        }

        //return necessary data now
        *arr_length = str_offset;
        return str_array;
    }
    
    char* Split(char* text, int index, char sep)
    {     
        size_t len = Length(text);
        char* str = nullptr;
        int last = -1;
        for (int i = 0, ind = 0; i < (int)len; i += IndexOf(text + i, sep) + 1, ind++)
        {
            if (i == last) { if (str != nullptr) { System::MemoryMgr.FreeSilent(str); } break; }
            if (index == ind)
            {
                int len = IndexOf(text+i, sep);
                if (len < 0)
                    len = Length(text+i);
                str = (char*)System::MemoryMgr.AllocateSilent(len + 1, System::ResourceType::String);
                for (int xi = 0; xi < len; xi++)
                    str[xi] = (text+i)[xi];
                str[len] = 0;
                return str;
            }
            last = i;
            if (str != nullptr) { System::MemoryMgr.FreeSilent(str); str = nullptr; }
        }
        
        return (char*)nullptr;
    }
    
    bool StartsWith(char* text, char _char)
    {
        if (text == nullptr || Length(text) == 0) return false;
        return text[0] == _char;
    }

    bool EndsWith(char* text, char _char)
    {
        if (text == nullptr || Length(text) == 0) return false;
        return text[Length(text)] == _char;
    }

    bool StartsWith(char* text, char* start)
    {
        int tlen = Length(text);
        int slen = Length(start);
        for(int i = 0; i < slen && i < tlen; i++) { if (text[i] != start[i]) { return false; } }
        return true;
    }

    bool EndsWith(char* text, char* end)
    {
        for (int i = Length(text), s = Length(end); s > 0 && i > 0; i--, s--) { if (text[i] != end[s]) { return false; } }
        return true;
    }
}