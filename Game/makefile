CC = gcc
CFLAGS = -Wall

all: motor_jogo jogoUI bot

motor_jogo: motor_jogo.c motor_jogo.h
    $(CC) $(CFLAGS) -o motor_jogo motor_jogo.c

jogoUI: jogoUI.c
    $(CC) $(CFLAGS) -o jogoUI jogoUI.c

bot: bot.c
    $(CC) $(CFLAGS) -o bot bot.c

clean:
    rm -f motor_jogo jogoUI bot
