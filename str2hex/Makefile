all: str2hex

str2hex: 
	gcc --std=c11 -o str2hex str2hex.c

install: all
	install -m 0655 str2hex /usr/bin/str2hex

uninstall:
	rm -rf /usr/bin/str2hex

clean:
	rm -rf *.o *~ str2hex
