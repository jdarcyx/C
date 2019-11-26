all: cpfcalc

icpfcalc:
	gcc --std=c11 -o cpfcalc cpfcalc.c

install: all
	install -m 0655 cpfcalc /usr/bin/cpfcalc

uninstall:
	rm -rf /usr/bin/cpfcalc

clean:
	rm -rf *.o *~ cpfcalc
