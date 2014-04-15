# AMP

  C implementation of the uber simple [AMP](https://github.com/visionmedia/node-amp) protocol.
  
  Forked from [here](https://github.com/clibs/amp).
  
## Warning

  Remember to free memory pieces allocate by `amp_encode` and `amp_decode_arg`

## Example

```c
char *args[] = { "some", "stuff", "here" };

// encode
char *buf = amp_encode(args, 3);
// remember to free 'buf'

// decode header
amp_t msg = {0};
amp_decode(&msg, buf);
assert(1 == msg.version);
assert(3 == msg.argc);

// decode args
for (int i = 0; i < msg.argc; ++i) {
  char *arg = amp_decode_arg(&msg);
  // remember to free 'arg'
  printf("%d : %s\n", i, arg);
  free(arg);
}
```

## Implementations

 - c: this library (~10m ops/s)
 - [node](https://github.com/visionmedia/node-amp) ~(1.5m ops/s)

# License

  MIT
