CFLAGS += -O3 -I/opt/quantis/include

all: qrng mt19937-tst

qrng.o: qrng.c
	$(CC) -c $(CFLAGS) -o $@ qrng.c

qrng: qrng.o
	$(CC) -o $@ qrng.o -L/opt/quantis/lib -lquantis-usb
	
mt19937-tst.o: mt19937-tst.c
	$(CC) -c $(CFLAGS) -o $@ mt19937-tst.c

mt19937-tst: mt19937-tst.o
	$(CC) -o $@ mt19937-tst.o

lpt-tst.o: lpt-tst.c
	$(CC) -c $(CFLAGS) -o $@ lpt-tst.c

lpt-tst: lpt-tst.o
	$(CC) -o $@ lpt-tst.o


clean:
	- rm -rf *.o *~ qrng mt19937-tst lpt-tst
	