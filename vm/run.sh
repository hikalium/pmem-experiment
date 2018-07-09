# -cdrom ~/Downloads/ubuntu-18.04-server-amd64.iso \
echo "Connect vm with ssh localhost -p 2222"
qemu-system-x86_64 \
	-cpu host -smp 4 -machine q35 \
	-vnc :5,password \
	-monitor stdio \
	-boot c \
	-m 8G \
	-bios /usr/share/ovmf/OVMF.fd \
	-enable-kvm \
	-net nic \
	-net user,hostfwd=tcp::2222-:22 \
	-k ja \
	disk.img

