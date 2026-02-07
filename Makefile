clean:
	rm *.out

game:
	gcc arq.c/main.c -o game.out \
		arq.c/utils.c \
		arq.c/renderer.c \
		arq.c/baralho.c \
		arq.c/carta.c \
		-Iarq.h \
		-lm -g\
		`pkg-config allegro-5 allegro_main-5 \
		allegro_font-5 \
		allegro_image-5 allegro_primitives-5 \
		 --libs --cflags`

game-win:
	gcc arq.c/main.c -o game.out \
		arq.c/utils.c \
		arq.c/renderer.c \
		arq.c/baralho.c \
		arq.c/carta.c \
		-Iarq.h \
		-Iusr/include \
		-lm -g\
		`pkg-config allegro-5 allegro_main-5 \
		allegro_font-5 \
		allegro_image-5 allegro_primitives-5 \
		 --libs --cflags`
		$(shell pkg-config allegro-5 allegro_main-5 \
		allegro_font-5 \
		allegro_image-5 allegro_primitives-5 \
		 --libs --cflags)
