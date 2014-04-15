#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "amp.h"

int main()
{
	int i, len;
	char *args[] = { "some", "stuff", "here" };

	// encode
	char *buf = amp_encode(args, 3, &len);

	// header
	amp_t msg = { 0 };
	amp_decode(&msg, buf);
	assert(1 == msg.version);
	assert(3 == msg.argc);

	// args
	for (i = 0; i < msg.argc; ++i) {
		char *arg = amp_decode_arg(&msg);
		switch (i) {
		case 0:
			assert(0 == strcmp("some", arg));
			break;
		case 1:
			assert(0 == strcmp("stuff", arg));
			break;
		case 2:
			assert(0 == strcmp("here", arg));
			break;
		}
		free(arg);
	}

	printf("ok\n");

	free(buf);
	return 0;
}
