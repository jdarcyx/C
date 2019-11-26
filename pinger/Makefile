all: pinger

pinger:
	gcc --std=c11 -o pinger pinger.c

install: all
	install -m 0655 pinger /usr/bin/pinger

uninstall:
	rm -rf /usr/bin/pinger

clean:
	rm -rf *.o *~ pinger
