PRJ = 1-uart-demo
C_SRC = main.c uart.c lib.c
C_OBJ = $(C_SRC:.c=.o)

all:
	armasm start.s
	armcc -c $(C_SRC)	
	armlink -first start.o start.o $(C_OBJ) -o $(PRJ).axf	
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt	
	dir *.bin
	
clean:
	del *.bin
	del *.o
	del *.axf
	del *.txt