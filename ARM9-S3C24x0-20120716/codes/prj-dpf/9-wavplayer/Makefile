PRJ = 9-wavplayer
SRC = main.c uart.c stdio.c printf.c command.c nand.c
OBJ = $(SRC:.c=.o)

all:
	armcc -c $(SRC)
	armasm start.s -o start.o
	armlink start.o $(OBJ) -first start.o -ro-base 0x0 -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c -d -s $(PRJ).axf -o $(PRJ).txt
	dir *.bin 
	
clean:
	del *.o *.bin *.axf *.txt