#include "mw/util.h"

static const uint8_t enc_table[256] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x2E, 0x00,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x5F,
	0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x00, 0x00, 0x00, 0x7E, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char hex_upper[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const char hex_lower[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

int urlencode(char *out, const char *in)
{
	int i, j;
	unsigned char c;

	for (i = 0, j = 0; (c = in[i]); i++) {
		if (enc_table[c]) {
			out[j++] = c;
		} else {
			out[j++] = '%';
			out[j++] = hex_upper[(c>>4) & 0xF];
			out[j++] = hex_upper[c & 0xF];
		}
	}
	out[j] = '\0';

	return j;
}

// String length must be at least (2 * bin_len + 1)
void bin_to_str(const uint8_t *bin, unsigned bin_len, char *str)
{
	int i, j;

	for (i = 0, j = 0; i < bin_len; i++) {
		str[j++] = hex_lower[bin[i]>>4];
		str[j++] = hex_lower[bin[i] & 0xF];
	}

	str[j] = '\0';
}

int ipv4_to_str(uint32_t ipv4, char str[14])
{
	return sprintf(str, "%d.%d.%d.%d", 0xFF & ipv4, 0xFF & (ipv4>>8),
		    0xFF & (ipv4>>16), ipv4>>24);
}

/// Similar to strcpy, but returns a pointer to the last character of the
/// src input string (the ending '\0')
const char *StrCpySrc(char *dst, const char *src) {
	while ('\0' != *src) *dst++ = *src++;
	*dst = '\0';

	return src;
}

/// Similar to strcpy, but returns a pointer to the last character of the
/// dst output string (the ending '\0')
char *StrCpyDst(char *dst, const char *src) {
	while ('\0' != *src) *dst++ = *src++;
	*dst = '\0';

	return dst;
}

int itemizer(const char *input, const char **item, int max_tokens)
{
	int i;

	for (i = 0; i < max_tokens && *input; i++) {
		item[i] = input;
		input += strlen(input) + 1;
	}

	return i;
}

