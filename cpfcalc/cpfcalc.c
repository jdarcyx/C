#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define TCSD 9 		// Tamanho do CPF sem dígitos verificadores
#define TC1D TCSD+1 	// Tamanho do CPF com 1º dígito verificador
#define TCPF TC1D+1	// Tamanho do CPF completo
#define ASCII_OFFSET 48 // Diferença do Caracter '0' para o valor 0;

// Teste

#define DEBUG

void erro(int errno, char *err_msg) {
	fprintf(stderr, err_msg);
	putchar('\n');
	exit(errno);
}

char *calcdg(const char *cpf) {
	if (strlen(cpf) != TCSD) erro(1, "O CPF sem dígitos verificadores deve conter 9 caracteres.");

	unsigned char *aux = malloc(11 * sizeof(char));

	if (aux == NULL) erro(127, "Memória insuficiente.");

	memset(aux,0,sizeof aux);

	// Converte array de CHAR para array de INT
	for (int i = 0;i < TCSD;i++) {
		*(aux+i) = *cpf++ - ASCII_OFFSET;
	}

	// 1º DV
	for (int i = 0;i < TCSD;i++) {
		*(aux+TCSD) += *(aux+i) * (i+1);
	}

	*(aux+TCSD) %= 11;
	if (*(aux+TCSD) == 10) *(aux+TCSD) = 0;

	// 2º DV
	for (int i = 0;i < TC1D;i++) {
		*(aux+TC1D) += *(aux+i) * i;
	}

	*(aux+TC1D) %= 11;
	if (*(aux+TC1D) == 10) *(aux+TC1D) = 0;

	unsigned char *dg = malloc(2 * sizeof(char));

	if (dg == NULL) erro(127, "Memória insuficiente.");

	*dg = *(aux+9);
	*(dg+1) = *(aux+10);

	free(aux);

	return dg;

}

bool valida(const char *cpf) {

	if (strlen(cpf) < TCPF) erro(2, "O CPF deve conter 11 caracteres.");

	char *cpfsd = malloc(TCPF * sizeof(char));
	strncpy(cpfsd, cpf, TCSD);
	
	char *dg = calcdg(cpfsd);
	bool ret = ((*dg == (*(cpf+TCSD) - ASCII_OFFSET)) && (*(dg+1) == (*(cpf+TC1D) - ASCII_OFFSET)));

	free(dg);
	free(cpfsd);
	return ret;

}

int main(int argc, char **argv) {
	char *cpf = malloc(11 * sizeof(char));

	if (argc < 2) {
		printf("Digite o cpf (somente números): ");
		scanf("%s", cpf);
	} else 
		strcpy(cpf, argv[1]);

	if (valida(cpf)) printf("%s: Válido\n", cpf);
	else printf("%s: Inválido\n", cpf);

	free(cpf);

	return 0;
}
