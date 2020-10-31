#include <Windows.h>
#include <math.h>
#include <stdio.h>


int main(void) {
	float C0_frequenza = 16.35;
	double a = pow(2, ((double)1/12));
	int numero_mezzitasti = 2;
	double Nota_x_frequenza;
	char nota[4];
	int nota_usata = 0;
	int C = 11;
	int D = 9;
	int E = 7;
	int F = 6;
	int G = 4;
	int A = 2;
	int B = 0;
	int melodia[100];
	int continua = 0;
	int x = 0;


	printf("Quante note vuoi usare?\n");
	scanf_s("%d%*c", &continua);
	int note_melodia = continua;
	do {
		printf("Scrivi la nota %d\n", (note_melodia - continua));
		scanf_s("%s", nota, 4);
		switch (nota[0]) {
		case 'A':
			nota_usata = A;
			break;
		case 'B':
			nota_usata = B;
			break;
		case 'C':
			nota_usata = C;
			break;
		case 'D':
			nota_usata = D;
			break;
		case 'E':
			nota_usata = E;
			break;
		case 'F':
			nota_usata = F;
			break;
		case 'G':
			nota_usata = G;
			break;
		default:
			printf("Errore");
			break;
		}

		int ottave = -(48 - nota[1]);

		switch (nota[2]) {
		case 'b':
			numero_mezzitasti = C - nota_usata + (12 * ottave) - 1;
			break;
		case '#':
			numero_mezzitasti = C - nota_usata + (12 * ottave) + 1;
			break;
		case '\0':
			numero_mezzitasti = C - nota_usata + (12 * ottave);
			break;
		default:
			printf("cacca");
			break;
		}
		
		melodia[x] = numero_mezzitasti;
		//printf("%d\n", melodia[x]);
		x++;
		continua--;
	} while (continua >= 0);
	

	for (int j = 0; j <= note_melodia; j++) {
		Nota_x_frequenza = C0_frequenza * pow(a, melodia[j]);
		// Se non suona qualche nota aumentare il secondo numero 
		Beep(Nota_x_frequenza, 400);
	}
	

	getchar();
	return(0);
}