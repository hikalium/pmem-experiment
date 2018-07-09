# -cdrom ~/Downloads/ubuntu-18.04-server-amd64.iso \
echo "Connect vm with ssh localhost -p 2222"
qemu-system-x86_64 \
	-cpu host -smp 4 -machine q35,nvdimm \
	-vnc :5,password \
	-monitor stdio \
	-boot c \
	-bios /usr/share/ovmf/OVMF.fd \
	-enable-kvm \
	-net nic \
	-net user,hostfwd=tcp::2222-:22 \
	-k ja \
	-m 8G,slots=2,maxmem=10G \
	-object memory-backend-file,id=mem1,share=on,mem-path=pmem.img,size=2G \
	-device nvdimm,id=nvdimm1,memdev=mem1 \
	disk.img

