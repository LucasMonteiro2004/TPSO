all: motor_jogo jogoUI bot

motor_jogo: motor_jogo.c motor_jogo.h
	gcc -o motor_jogo motor_jogo.c

jogoUI: jogoUI.c
	gcc -o jogoUI jogoUI.c -lncurses

bot: bot.c
	gcc -o bot bot.c

clean:
	rm -f motor_jogo jogoUI bot
