CFLAGS += -O3 -I/opt/quantis/include
LDFLAGS += -lm -L/opt/quantis/lib -lquantis-usb

 
all: qrng 

.cpp.o:
	${CC} -c ${CFLAGS} $<


qrng: qrng.o
	$(CC) -o $@ $< $(LDFLAGS)
	
qrng-tst: qrng-tst.o
	$(CC) -o $@ $< $(LDFLAGS)

mt19937-tst-init-qrng: mt19937-tst-init-qrng.o
	$(CC) -o $@ $< $(LDFLAGS)


clean:
	- rm -rf *.o *~ qrng well19937-tst lpt-tst
