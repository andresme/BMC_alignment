CC=gcc
CFLAGS= -std=gnu99 -g `pkg-config --cflags gtk+-3.0` -Wl,--export-dynamic -pthread -lm
LIBS   = `pkg-config --libs gtk+-3.0 gmodule-export-2.0` -lm
CHECK=-Wall -Wextra

all: main.o utils.o alignments.o encodeUtils.o file.o
	$(CC) $(CHECK) $(CFLAGS)  -o main main.o utils.o alignments.o encodeUtils.o file.o $(LIBS)

clean:
	rm -f *.o main
