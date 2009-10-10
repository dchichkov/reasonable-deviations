CFLAGS += -I/opt/quantis/include

all: qfcgi

qfcgi.o: qfcgi.c
	$(CC) -c $(CFLAGS) -o $@ qfcgi.c

qfcgi: qfcgi.o
	$(CC) -o $@ qfcgi.o -L/opt/quantis/lib -lquantis-usb -l fcgi

clean:
	- rm -rf *.o *~ qfcgi
