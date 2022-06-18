# kernel data structure

## Commands


### To compile module bridgeOwn


Into the project folder type

```sh
make

```

### To compile client

```sh
gcc -o clientBridge clientBridge.c

```

### Up module

```sh
sudo ./bridge_load

```

### Down module

```sh
sudo ./bridge_unload

```

### Checking it

```sh
ls /dev/
lsmod | grep bridgeOwn
```
