/*
 * Convenient way to quickly dumps a buffer in hex/ASCII.
 *
 * Usage:
 *
 *   #include "data-dump.inc.c"
 *   const char s[] = "hello world; have a nice day";
 *   data_dump(s, sizeof(s));
 *
 * Yields:
 *
 *   0000: 68 65 6c 6c 6f 20 77 6f 72 6c 64 3b 20 68 61 76     hello world; hav
 *   0010: 65 20 61 20 6e 69 63 65 20 64 61 79 00              e a nice day.
 *
 * License:
 *
 *   Beerware, unlicense.org, public domain... up to you.
 *
 * Author:
 *
 *   Rink Springer <rink@rink.nu>, https://github.com/zhmu/snippets
 */
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static unsigned char s_datadump_hextab[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

static void data_dump(const unsigned char* buf, int buf_len)
{
	/* Dump */
#define LINE_MAX 255
	char output[LINE_MAX + 1];
	output[LINE_MAX] = 0;
#define BYTES_PER_LINE 16
#define HEX_ASCII_SPACER 4
	memset(output, ' ', BYTES_PER_LINE * 3);
	for (int n = 0; n < HEX_ASCII_SPACER; n++)
		output[BYTES_PER_LINE * 3 + n] = ' ';
	int m = 0;
	// Enable for full packet dump including header
	for (int n = 0; n < buf_len; n++) {
		uint8_t b = buf[n];

		/* Hex value */
		output[m * 3 + 0] = s_datadump_hextab[b >>  4];
		output[m * 3 + 1] = s_datadump_hextab[b & 0xf];

		/* ASCII, if printable */
		output[BYTES_PER_LINE * 3 + HEX_ASCII_SPACER + m] = isprint(b) ? b : '.';
		output[BYTES_PER_LINE * 3 + HEX_ASCII_SPACER + m + 1] = '\0';

		/* Display line if we need to */
		m++;
		if (m == BYTES_PER_LINE) {
			printf("%04x: %s\n", (n - m) + 1, output);
			m = 0;
		}
	}

	/* If we still have data left, show it */
	if (m > 0) {
		/* Clear fields that are not present */
		for (unsigned int n = m * 3; n < BYTES_PER_LINE * 3; n++) {
			output[n] = ' ';
		}
		printf("%04x: %s\n", buf_len - m, output);
	}
}
