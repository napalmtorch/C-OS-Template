#include <Kernel/Lib/Math.hpp>

namespace MathUtil
{
    // floor - double
    double Floor(double x)
    {
        if (x >= INT64_MAX || x <= INT64_MIN || x != x) { return x; }
        ulong n = (ulong)x;
        double d = (double)n;
        if (d == x || x >= 0) { return d; } else { return d - 1; }
        return 0;
    }

    // round - double
    double Round(double x)
    {
        ulong n = (ulong)n;
        double d = (double)n;
        if (d < 0.0) { return d - 0.5; } else { return d + 0.5; }
        return 0;
    }

    // round - float
    float Round(float x)
    {
        ulong n = (ulong)n;
        float d = (float)n;
        if (d < 0.0f) { return d - 0.5f; } else { return d + 0.5f; }
        return 0;
    }

    // round - long double
    long double Round(long double x)
    {
        ulong n = (ulong)n;
        float d = (float)n;
        if (d < 0.0) { return d - 0.5; } else { return d + 0.5; }
        return 0;
    }

    // power - double
    double Pow(double base, int exp)
    {
        double temp;
        if (exp == 0)
        return 1;
        temp = Pow(base, exp / 2);
        if ((exp % 2) == 0) { return temp * temp; } 
        else 
        {
            if (exp > 0) { return base * temp * temp; }
            else { return (temp * temp) / base; }
        }
    }

    // power - float
    float Pow(float base, int exp)
    {
        float temp;
        if (exp == 0)
        return 1;
        temp = Pow(base, exp / 2);
        if ((exp % 2) == 0) { return temp * temp; } 
        else 
        {
            if (exp > 0) { return base * temp * temp; }
            else { return (temp * temp) / base; }
        }
    }
}