#include "main.h"

int get_precision(const char *format, int *i, va_list list)
{
    int curr_i = *i + 1;
    int precision = -1;

    if (format[curr_i] == '.') 
    {
        precision = 0;
        curr_i++;

        while (isdigit(format[curr_i])) 
        {
            precision = precision * 10 + format[curr_i] - '0';
            curr_i++;
        }

        if (format[curr_i] == '*') 
        {
            curr_i++;
            precision = va_arg(list, int);
        }
    }

    *i = curr_i - 1;
    return precision;
}
