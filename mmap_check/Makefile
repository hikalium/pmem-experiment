CFLAGS+=-Wall -Wpedantic
default: mmap_check

run: mmap_check
	sudo ./mmap_check /dev/dax1.0

dmesg:
	dmesg | grep persistent

