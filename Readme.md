# AMP

  C implementation of the uber simple [AMP](https://github.com/visionmedia/node-amp) protocol.
  
  Forked from [here](https://github.com/clibs/amp).
  
## Warning

  Remember `amp_encode` and `amp_encode_clean` must be matched, so do `amp_decode_arg` and `amp_decode_arg_clean`.

## Example

```c
typedef struct {
	int age;
	char name[16];
} person_t;

person_t kumakichi = {
    .age = 27,
    .name = "kumakichi"
};

arg_t args[] = {
    {"some", strlen("some")},
    {&kumakichi, sizeof(kumakichi)}
};

// encode
int len;
unsigned char *buf = amp_encode(args, 2, &len);

// decode header
amp_t msg = {0};
amp_decode(&msg, buf);
assert(AMP_VERSION == msg.version);
assert(2 == msg.argc);

// decode args
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
    //clean arg
	amp_decode_arg_clean(&arg);
}
...
//clean encoder
amp_encode_clean(&buf);
```

## Implementations

 - c: this library (~10m ops/s)
 - [node](https://github.com/visionmedia/node-amp) ~(1.5m ops/s)

# License

  MIT