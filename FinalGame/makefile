CC = gcc
CFLAGS = -Wall

all: jogoUI motor_jogo

jogoUI: jogoUI.o motor_jogo.o
    $(CC) $(CFLAGS) -o jogoUI jogoUI.o motor_jogo.o

motor_jogo: motor_jogo.o jogoUI.o
    $(CC) $(CFLAGS) -o motor_jogo motor_jogo.o jogoUI.o

jogoUI.o: jogoUI.c motor_jogo.h
    $(CC) $(CFLAGS) -c jogoUI.c

motor_jogo.o: motor_jogo.c jogoUI.h
    $(CC) $(CFLAGS) -c motor_jogo.c

clean:
    rm -f jogoUI motor_jogo *.o
