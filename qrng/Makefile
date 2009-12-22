CFLAGS += -O3 -I/opt/quantis/include
LDFLAGS += -lm -L/opt/quantis/lib -lquantis-usb

 
all: qrng 

.cpp.o:
	${CC} -c ${CFLAGS} $<


qrng: qrng.o
	$(CC) -o $@ $< $(LDFLAGS)
	
qrng-tst: qrng-tst.o
	$(CC) -o $@ $< $(LDFLAGS)

lpt-tst: lpt-tst.o
	$(CC) $(LDFLAGS) -o $@ $<


clean:
	- rm -rf *.o *~ qrng well19937-tst lpt-tst
