#pragma once
#include <Kernel/Lib/Types.hpp>

namespace String
{
    size_t Length(char* text);
    char*  ToUpper(char* text);
    char*  ToLower(char* text);
    char*  Append(char* src, char* add);
    char*  Append(char* src, char* add, size_t len);
    char*  Append(char* src, char c);
    char*  InsertNew(char* src, int index, char c);
    char*  InsertNew(char* src, int index, char* val);
    char*  Insert(char* src, int index, size_t len, char c);
    char*  Insert(char* src, int index, size_t len, char* val);
    char*  DeleteAt(char* src, int index, size_t len);
    char*  DeleteAt(char* src, int index, size_t del_len, size_t len);
    char*  Copy(char* dest, char* src);
    char*  Copy(char* dest, char* src, size_t len);
    int    Compare(char* str1, char* str2);
    int    Compare(char* str1, char* str2, size_t len);
    bool   Equals(char* str1, char* str2);
    char*  Reverse(char* text);
    char*  FromDecimal(int num, char* text);
    char*  FromHex(uint num, char* text, bool prefix);
    char*  FromHex(uint num, char* text, bool prefix, byte bytes);
    char*  FromFloat(float num, char* text, int afterdot);
    int    ToDecimal(char* text);
    uint   ToHex(char* text);
    char*  Clear(char* text);
    bool   Contains(char* text, char* find);
    char*  Delete(char* text);
    char*  Delete(char* text, size_t len);
    char*  StartAt(char* src, char* find);
    int    IndexOf(char* text, char c);
    char*  Replace(char* src, char* rep, char* with);
    char** Split(char* str, char delim, uint* arr_length);
    char*  Split(char* text, int index, char sep);
    bool   StartsWith(char* text, char _char);
    bool   EndsWith(char* text, char _char);
    bool   StartsWith(char* text, char* start);
    bool   EndsWith(char* text, char* end);
}