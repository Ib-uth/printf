#include "main.h"

int handle_write_char(char c, char buffer[],
                      int flags, int width, int precision, int size)
{
    int written = 0;
    char padding = (flags & F_ZERO) ? '0' : ' ';

    buffer[0] = c;
    buffer[1] = '\0';

    if (width > 1) {
        int padding_width = width - 1;
        if (flags & F_MINUS) {
            written += write(1, &buffer[0], 1);
            written += write_padding(padding, padding_width);
        } else {
            written += write_padding(padding, padding_width);
            written += write(1, &buffer[0], 1);
        }
    } else {
        written += write(1, &buffer[0], 1);
    }

    return written;
}

int write_padding(char padding, int width)
{
    int written = 0;
    for (int i = 0; i < width; i++) {
        written += write(1, &padding, 1);
    }
    return written;
}


/************************* WRITE NUMBER *************************/

int write_number(int is_negative, int ind, char buffer[],
	int flags, int width, int precision, int size)
{
	int length = BUFF_SIZE - ind - 1;
	char padd = ' ', extra_ch = 0;

	UNUSED(size);

	if ((flags & F_ZERO) && !(flags & F_MINUS))
		padd = '0';
	if (is_negative)
		extra_ch = '-';
	else if (flags & F_PLUS)
		extra_ch = '+';
	else if (flags & F_SPACE)
		extra_ch = ' ';

	return (write_num(ind, buffer, flags, width, precision,
		length, padd, extra_ch));
}
int write_number(int is_negative, int ind, char buffer[],
    int flags, int width, int precision, int size)
{
    const int buff_size = BUFF_SIZE;
    const int length = buff_size - ind - 1;
    char padd = (flags & F_ZERO) && !(flags & F_MINUS) ? '0' : ' ';
    char extra_ch = 0;
    
    if (is_negative) {
        extra_ch = '-';
    } else if (flags & F_PLUS) {
        extra_ch = '+';
    } else if (flags & F_SPACE) {
        extra_ch = ' ';
    }
    
    // Suppress unused variable warning
    (void) size;
    
    return write_num(ind, buffer, flags, width, precision,
        length, padd, extra_ch);
}
int write_unsgnd(int is_negative, int ind,
	char buffer[],
	int flags, int width, int precision, int size)
{
	const int buff_size = BUFF_SIZE;
	int length = buff_size - ind - 1;
	char padd = (flags & F_ZERO) && !(flags & F_MINUS) ? '0' : ' ';

	UNUSED(is_negative);
	UNUSED(size);

	if (precision == 0 && ind == buff_size - 2 && buffer[ind] == '0') {
		return 0; // printf(".0d", 0)  no char is printed
	}

	if (precision > 0 && precision < length) {
		padd = ' ';
	}

	while (precision > length) {
		buffer[--ind] = '0';
		length++;
	}

	if (width > length) {
		const int padding_length = width - length;
		const int padding_start = 0;
		const int buffer_start = padding_length;
		if (flags & F_MINUS) {
			// Extra char to left of buffer [buffer>padd]
			for (int i = buffer_start; i < buff_size; i++) {
				buffer[i] = i - buffer_start < length ? buffer[ind + i - buffer_start] : padd;
			}
			return write(1, buffer, buff_size);
		} else {
			// Extra char to left of padding [padd>buffer]
			for (int i = padding_start; i < padding_length; i++) {
				buffer[i] = padd;
			}
			for (int i = buffer_start; i < buff_size; i++) {
				buffer[i] = i - buffer_start < length ? buffer[ind + i - buffer_start] : buffer[i];
			}
			return write(1, buffer, buff_size);
		}
	}

	return write(1, &buffer[ind], length);
}

int write_pointer(char buffer[], int ind, int length,
	int width, int flags, char padd, char extra_c, int padd_start)
{
	int i, result = 0;
	bool has_extra_c = extra_c != 0;

	if (width > length)
	{
		i = 3;
		while (i < width - length + 3)
			buffer[i++] = padd;
		buffer[i] = '\0';

		if (flags & F_MINUS && padd == ' ')
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (has_extra_c) buffer[--ind] = extra_c;

			result += write(1, &buffer[ind], length);
			result += write(1, &buffer[3], i - 3);
		}
		else if (!(flags & F_MINUS) && padd == ' ')
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (has_extra_c) buffer[--ind] = extra_c;

			result += write(1, &buffer[3], i - 3);
			result += write(1, &buffer[ind], length);
		}
		else if (!(flags & F_MINUS) && padd == '0')
		{
			if (has_extra_c) buffer[--padd_start] = extra_c;
			buffer[1] = '0';
			buffer[2] = 'x';

			result += write(1, &buffer[padd_start], i - padd_start);
			result += write(1, &buffer[ind], length - (1 - padd_start) - 2);
		}

		return result;
	}

	buffer[--ind] = 'x';
	buffer[--ind] = '0';
	if (has_extra_c) buffer[--ind] = extra_c;

	return write(1, &buffer[ind], BUFF_SIZE - ind - 1);
}
