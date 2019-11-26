#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void concatena (int q,char *p,char **argv) {
	for (int i = 1;i < q;i++) {
		strcat(p,*(argv+i));
		if (i < q-1) {
			strcat(p," ");
		}
	}
}

void imprime (char *p) {
	printf("{");

	for (int i = 0;i < strlen(p);i++) {
		printf(" 0x%02X", *(p+i));
		if (i < strlen(p)-1)
			putchar(0x2c);
	}

	printf(", 0x00 }\n");

}

void uso (void) {
	printf("\nstr2hex\n\n");
	printf("    Transforma um texto em um array de bytes.\n");
	printf("    Uso:\n");
	printf("    str2hex <texto>\n\n");
	exit(0);
}

int main (int argc, char **argv) {

	if (argc < 2) uso();
	
	char *saida = malloc(argc * sizeof(char *));

	concatena(argc,saida,argv);
	imprime(saida);
	
	free(saida); 
	return 0;
}
