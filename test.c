#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "amp.h"

typedef struct {
	int age;
	char name[16];
} person_t;

int main()
{
	int i, len;
	person_t kumakichi = {
		.age = 27,
		.name = "kumakichi"
	};

	arg_t args[] = {
		{"some", strlen("some")},
		{&kumakichi, sizeof(kumakichi)}
	};

	// encode
	unsigned char *buf = amp_encode(args, 2, &len);

	// header
	amp_t msg = { 0 };
	amp_decode(&msg, buf);
	assert(1 == msg.version);
	assert(2 == msg.argc);

	// args
	for (i = 0; i < msg.argc; ++i) {
		person_t *p;
		unsigned char *arg = amp_decode_arg(&msg);
		switch (i) {
		case 0:
			assert(0 == strncmp("some", arg, 4));
			break;
		case 1:
			p = (person_t *) arg;
			assert(27 == p->age);
			assert(0 == strcmp("kumakichi", p->name));
			break;
		}
		amp_decode_arg_clean(&arg);
	}
	printf("ok\n");

	amp_encode_clean(&buf);
	return 0;
}
