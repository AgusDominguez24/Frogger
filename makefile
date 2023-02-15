CC := gcc
OPTS = -g -Wall

ifdef ALLEGRO

allegro: backend.o frontendAl.o
	${CC} ${OPTS} backend.o frontendAl.o -I/usr/include/allegro5 -lallegro -lallegro_main -lallegro_image \
-lallegro_ttf -lallegro_font -lallegro_primitives -lpthread -o frogger -D ALLEGRO

backend.o: backend.c backend.h constantes.h estructuras.h
	${CC} ${OPTS} -D ALLEGRO -c backend.c

frontendAl.o: frontendAl.c frontendAl.h constantes.h variablesGlobales.h estructuras.h
	${CC} ${OPTS} -D ALLEGRO -c frontendAl.c

endif

ifdef RASPI

raspi: backend.o frontendRa.o disdrv.o joydrv.o
	${CC} ${OPTS} backend.o frontendRa.o disdrv.o joydrv.o -lpthread -o frogger -D RASPI

backend.o: backend.c backend.h constantes.h estructuras.h
	${CC} ${OPTS} -D RASPI -c backend.c 

frontendRa.o: frontendRa.c constantes.h variablesGlobales.h estructuras.h disdrv.h joydrv.h
	${CC} ${OPTS} -D RASPI -c frontendRa.c

endif

clean:
	rm backend.o frontendRa.o frontendAl.o
