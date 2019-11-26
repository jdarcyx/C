/*
 *	pinger.c
 *	Baseado em um software de mesmo nome, este foi escrito do zero
 * com o objetivo de aperfeiçoar o conhecimento deste desenvolvedor.
 *
 *	João Darcy Araújo
 *
 *	Iniciado em:	THE, 11/11/2019 às 13:43
 *	Finalizado em:	THE, 12/11/2019 às 12:01
 *
*/

#include <ctype.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define N_PKG 1000
#define T_PKG 32
#define P_DST 0
#define P_OUT 0x2a

void erro_fatal (int err_no, char *err_msg);
bool eh_numero (const char *s);
bool ip_valido (const char *ip);
void enviar_pacotes (char *dst, unsigned int n_pkt, unsigned int t_pkt,  uint16_t p_dst, bool quiet);
uint16_t numero_random (void);


void erro_fatal (int err_no, char *err_msg) {
	fprintf(stderr, "%s\n", err_msg);
	exit(err_no);
}

void uso (void) {
	printf("\nUso:\n");
	printf("  pinger [opções] <destino>\n\n");
	printf("<destino>\t\tEndereço de destino\n");
	printf("\nOpções:\n");
	printf("  -n <número>\t\tNúmero de pacotes a ser enviados. 0 para infinito. (Padrão: 1000)\n");
	printf("  -s <tamanho>\t\tTamanho do pacote em bytes. (Padrão: 32)\n");
	printf("  -p <porta>\t\tPorta de destino. 0 para porta aleatória. (Padrão: 0)\n");
	printf("  -q\t\t\tNão imprime na tela a cada pacote enviado. Poupa processamento enviando os pacotes em um intervalo ainda menor.\n\n");
	exit(1);
}

bool eh_numero(const char *s) {
	while (*s) if (!(isdigit(*s++))) return false;
	return true;
}

bool ip_valido(const char *ip) {
	int n, d = 0;
	char *p;
	char *ipw = malloc((strlen(ip)+1) * sizeof(char));

	if (ipw == NULL) erro_fatal(-55, "Memória insuficiente.");

	strcpy(ipw, ip);

	p = strtok(ipw, ".");
	if (p == NULL) {
		free(ipw);
		return false;
	}

	while (p) {
		if (!(eh_numero(p))) {
			free(ipw);
			return false;
		}
		
		n = atoi(p);
		if ((n >= 0) && (n<=255)) {
			p = strtok(NULL, ".");
			if (p) d++;
		} else {
			free(ipw);
			return false;
		}
	}

	free(ipw);

	if (d != 3) return false;

	return true;
}

uint16_t numero_random (void) {
	time_t hora;

	srand((unsigned) time(&hora));

	return (rand() % UINT16_MAX);
}

void enviar_pacotes (char *dst, unsigned int n_pkt, unsigned int t_pkt, uint16_t p_dst, bool quiet) {
	int a, sock;
	unsigned int i = 0;
	char *msg;
	struct sockaddr_in destino;

	msg = malloc(t_pkt * sizeof(char));
	if (msg == NULL) erro_fatal(-55, "Memória insuficiente.");

	memset(msg, P_OUT, sizeof msg);
	memset(&destino, 0, sizeof destino);

	destino.sin_family = AF_INET;
	destino.sin_port = htons(p_dst);
	inet_aton(dst, &destino.sin_addr);

	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) erro_fatal(sock, "Falha na criação do Socket.");

	while (true) {

		if (n_pkt > 0) {
			if (i == n_pkt)
				break;
			i++;
		}

		if (p_dst == 0) destino.sin_port = numero_random();
		a = sendto(sock, msg, strlen(msg)+1, 0, (struct sockaddr *) &destino, sizeof destino);

		if (!(quiet))
			if (a > 0)
				putc(P_OUT, stdout);
			else
				erro_fatal(a, "Falha ao enviar pacote.");
	} 

	free(msg);

	printf((quiet)?"%d pacotes enviados.\n":"\n%d pacotes enviados.\n", n_pkt);
}

int main (int argc, char **argv) {

	int opt;
	unsigned int n_pkt = N_PKG;
	unsigned int t_pkt = T_PKG;
	uint16_t p_dst = P_DST;
	bool quiet = false;

	while ((opt = getopt(argc, argv, "n:s:p:q")) > 0) {
		switch (opt) {
			case 'n':
				if (!(eh_numero(optarg)))
					erro_fatal(126, "Número inválido de pacotes.");
				else if (atoi(optarg) < 0)
					erro_fatal(126, "Número inválido de pacotes.");
				n_pkt = atoi(optarg);
				break;
			case 's':
				if (!(eh_numero(optarg)))
					erro_fatal(127, "Tamanho inválido de pacote.");
				else if (atoi(optarg) < 1)				
					erro_fatal(127, "Tamanho inválido de pacote.");
				t_pkt = atoi(optarg);
				break;
			case 'p':
				if (!(eh_numero(optarg)))
					erro_fatal(123, "A porta deve ser um valor numérico.");
				else if ((atoi(optarg) < 0) || (atoi(optarg) > 65535))
					erro_fatal(122, "A porta deve ser de 0 a 65535");
				p_dst = atoi(optarg);
				break;
			case 'q':
				quiet = true;
				break;
			default:
				printf("%d: opção inválida\n", opt);
				exit(-1);
		}

	}

	if ((argc - optind) != 1) {
		fprintf(stderr, "Falta um parâmetro necessário\n");
		uso();
	}

	if (!(ip_valido(argv[optind]))) {
		erro_fatal(125, "Endereço de IP de destino inválido.");
	}
	
	enviar_pacotes(argv[optind], n_pkt, t_pkt, p_dst, quiet);
	
	return 0;
}
