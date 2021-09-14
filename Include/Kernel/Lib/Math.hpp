#pragma once
#include <Kernel/Lib/Types.hpp>

namespace MathUtil
{
    double      Floor(double x);
    double      Round(double x);
    float       Round(float x);
    long double Round(long double x);
    double      Pow(double base, int exp);
    float       Pow(float base, int exp);
}