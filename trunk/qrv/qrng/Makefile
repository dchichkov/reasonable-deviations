CFLAGS += -I/opt/quantis/include

all: qrng

qrng.o: qrng.c
	$(CC) -c $(CFLAGS) -o $@ qrng.c

qrng: qrng.o
	$(CC) -o $@ qrng.o -L/opt/quantis/lib -lquantis-usb

clean:
	- rm -rf *.o *~ qrng
