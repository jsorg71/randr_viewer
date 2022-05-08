
OBJS=randr_viewer.o

CFLAGS=-O2 -Wall

LDFLAGS=

LIBS=-lX11 -lXrandr

randr_viewer: $(OBJS)
	$(CC) -o randr_viewer $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) randr_viewer

