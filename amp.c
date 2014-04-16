//
// amp.c
//
// Copyright (c) 2014 TJ Holowaychuk <tj@vision-media.ca>
//

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "amp.h"

/*
 * Read u32be.
 */

static uint32_t read_u32_be(char *buf)
{
	uint32_t n = 0;
	n |= buf[0] << 24;
	n |= buf[1] << 16;
	n |= buf[2] << 8;
	n |= buf[3];
	return n;
}

/*
 * Write u32be.
 */

static void write_u32_be(char *buf, uint32_t n)
{
	buf[0] = n >> 24 & 0xff;
	buf[1] = n >> 16 & 0xff;
	buf[2] = n >> 8 & 0xff;
	buf[3] = n & 0xff;
}

/*
 * Decode the `msg` header in `buf`.
 */

void amp_decode(amp_t * msg, char *buf)
{
	msg->version = buf[0] >> 4;
	msg->argc = buf[0] & 0xf;
	msg->buf = buf + 1;	/* ignore the ver/argc byte */
}

/*
 * Jump over the legth bytes(4 bytes)
 */

void jump_over_length_bytes(unsigned char **msg)
{
	*msg += LEGTH_BYTES_SIZE;
}

/*
 * Decode `msg` argument, returning a buffer
 * that must be freed by the user and progressing
 * the msg->buf cursor.
 */

unsigned char *amp_decode_arg(amp_t * msg, int *arg_len)
{
	uint32_t size = read_u32_be(msg->buf);
	jump_over_length_bytes(&msg->buf);

	unsigned char *buf = malloc(size);
	if (!buf)
		return NULL;

	if (arg_len)
		*arg_len = size;

	memcpy(buf, msg->buf, size);
	msg->buf += size;
	return buf;
}

/*
 * Encode the AMP message argv.
 *
 *         0        1 2 3 4     <length>    ...
 *   +------------+----------+------------+
 *   | <ver/argc> | <length> | <data>     | additional arguments
 *   +------------+----------+------------+
 * 
 * msg_len is used to let user know the length of encoded msg
 * for later use,e.g.,for sendto()
 *
 */

unsigned char *amp_encode(arg_t * argv, int argc, int *msg_len)
{
	int i;
	size_t total_size = 1;	/* for ver/argc byte */
	size_t size[argc];

	// length
	for (i = 0; i < argc; ++i) {
		total_size += LEGTH_BYTES_SIZE;
		size[i] = argv[i].len;
		total_size += size[i];
	}

	// alloc
	unsigned char *buf = malloc(total_size);
	unsigned char *ret = buf;
	if (!buf)
		return NULL;

	*msg_len = total_size;

	// ver/argc
	*buf++ = AMP_VERSION << 4 | argc;

	// encode
	for (i = 0; i < argc; ++i) {
		write_u32_be(buf, size[i]);
		jump_over_length_bytes(&buf);

		memcpy(buf, argv[i].data, size[i]);
		buf += size[i];
	}

	return ret;
}

/*
 * Free memory allocated by amp_encode()
 */

void amp_encode_clean(unsigned char **buf)
{
	if (*buf)
		free(*buf);
	*buf = NULL;		/* in case of double free */
}

/*
 * Free memory allocated by amp_decode_arg()
 */

void amp_decode_arg_clean(unsigned char **buf)
{
	if (*buf)
		free(*buf);
	*buf = NULL;		/* in case of double free */
}
