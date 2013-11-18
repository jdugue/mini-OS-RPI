ROOT_DIR=.
include common.mk

.PHONY: gcc clean hw music process sem start

all : kernel

clean : common_clean
	cd hw && make clean
	cd music && make clean
	cd scheduler && make clean
	cd start && make clean
	cd process && make clean
	cd sem && make clean
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s
		
hw:
	cd hw && make
	
music :
	cd music && make
	
start:
	cd start && make

scheduler:
	cd scheduler && make
	
process:
	cd process && make
	
sem:
	cd sem && make
	
kernel.elf : hw start music process sem scheduler start/memmap notmain.o
	$(LD) start/vectors.o notmain.o music/music.a hw/hw.a process/process.a sem/sem.a scheduler/sched.a -T start/memmap -o kernel.elf $(LD_FLAGS)

kernel.bin : kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.bin

kernel.hex : kernel.elf
	$(OBJCOPY) kernel.elf -O ihex kernel.hex

kernel.img : kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img
	cp kernel.img SD_Card/
	
kernel : kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img
	cp kernel.img SD_Card/
	$(OBJDUMP) -D kernel.elf > kernel.list
