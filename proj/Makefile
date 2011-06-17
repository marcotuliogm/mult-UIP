UIPDIR=../uip

CC=arm-elf-gcc
OBJCOPY=arm-elf-objcopy
OBJDUMP=arm-elf-objdump
LDSCRIPT=ldscript.ld
TARGET=SELivre-eth-multServ

LINKER_FLAGS=-nostartfiles -Xlinker -o${TARGET}.elf -Xlinker -M -Xlinker -Map=${TARGET}.map

DEBUG=-g
OPTIM=-O1

ENABLE_TELNET = true
ENABLE_HTTPD = false

CFLAGS= \
		$(DEBUG) \
		$(OPTIM) \
		-T $(LDSCRIPT) \
		-I /usr/local/arm-elf/include/ \
		-I ./eth \
		-I $(UIPDIR) \
		-mcpu=arm7tdmi-s \
		-fomit-frame-pointer \
		#-mthumb-interwork \
											
THUMB_SOURCE= \
		 

ARM_SOURCE= \
		main.c \
		syscalls.c \
		./uart/uart.c \
		./mprintf/mprintf.c \
		./timer/timer.c \
		./rtc/rtc.c \
		./i2c/i2c.c \
		./i2c/i2c_isr.c \
		./mac/emac.c \
		./mac/emac_isr.c \
		./mult/mult_uip.c \
		./sft/grlib/grlib.c \
		./sft/sftd.c \
		./dhcp/dhcpc.c \
		./httpd/httpd.c \
		./httpd/httpd-cgi.c \
		./httpd/httpd-fs.c \
		./httpd/http-strings.c \
		./sockets/cli_webSock.c \
		./eth/eth.c \
		./lcd/lcd.c \
		./cli/cli.c \
		./cli/cli_func.c \
		./cli/debug.c \
		./telnetd/telnetd.c \
		./memb/memb.c \
		./telnetd/shell.c \
		$(UIPDIR)/uip_arp.c \
		$(UIPDIR)/psock.c \
		$(UIPDIR)/timer.c \
		$(UIPDIR)/uip.c \

#	./dhcp/dhcpc.c \
		./ftp/ftp.c \
				
THUMB_OBJS = $(THUMB_SOURCE:.c=.o)				
ARM_OBJS = $(ARM_SOURCE:.c=.o)


all: ${TARGET}.hex ${TARGET}.lst #isp

${TARGET}.bin : ${TARGET}.hex
	$(OBJCOPY) ${TARGET}.elf -O binary ${TARGET}.bin
	 
${TARGET}.hex : ${TARGET}.elf
	$(OBJCOPY) ${TARGET}.elf -O ihex ${TARGET}.hex

${TARGET}.lst : ${TARGET}.elf
	$(OBJDUMP) -h -S -C ${TARGET}.elf > ${TARGET}.lst

${TARGET}.elf : $(ARM_OBJS) $(THUMB_OBJS) boot.s Makefile
	$(CC) $(CFLAGS) $(ARM_OBJS) $(THUMB_OBJS) $(LIBS) boot.s $(LINKER_FLAGS) -o ${TARGET}.elf

$(THUMB_OBJS) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) -mthumb $< -o $@

$(ARM_OBJS) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm -f $(ARM_OBJS) $(THUMB_OBJS) ${TARGET}.elf ${TARGET}.hex ${TARGET}.bin ${TARGET}.map ${TARGET}.lst

isp:
	lpc21isp ${TARGET}.hex /dev/ttyUSB0 115200 12000


