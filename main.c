#include <math.h>
#include <stdio.h>
#include <Windows.h>







int main(void) {
	//Stringhe utilizzate
	char melodia[150];
	char nota[4];

	//Variabili per il primo blocco for
	int x,z;
	int fine_nota_precedente = 0;

	//Variabili usate per convertire la stringa nota in un valore int da inserire nell'array canzone per calcolarne la frequenza in seguito
	int nota_usata = 0;
	int ottava = 0;
	int numero_mezzitasti = 0;
	int durata_nota = 1;

	//Array canzone contente informazioni su nota e durata della nota in ordine [x = nota, x + 1 = durata nota, y = nota, y + 1 = durata nota]
	int canzone[100];
	int t = 0;

	//Variabili usate per il calcolo della frequenza da usare per il comando Beep
	int ms = 300;
	int k;
	double Nota_x_frequenza;
	double C0_frequenza = 16.35;
	double a = pow(2, ((double)1 / 12));
	float lunghezza_nota = 0;

	printf("Scrivi la tua melodia:\n");
	scanf_s("%s", melodia, sizeof(melodia));

	//Ciclo for che separa dalla stringa melodia le informazioni per le singole note
	for (x = 0; x < sizeof(melodia); x++) {
		//Salva in una stringa chiamata nota le informazioni della stringa melodia finché non raggiungo la virgola
		if (melodia[x] != ',' || melodia[x] != '\0') {
			z = x - fine_nota_precedente;
			nota[z] = melodia[x];
		}
		//Quando raggiungo la virgola setto una variabile fine_nota_precedente come (posizione della virgola nell'array + 1)
		//Facendo questo posso assegnare al valore z, indice di posizione per la stringa nota, un valore sempre compreso tra 0 e 4
		if (melodia[x] == ',' || melodia[x] == '\0') {
			fine_nota_precedente = x + 1;
			//Raggiunta la virgola e finito quindi di immagazinare le informazioni nella stringa nota inizio a lavorare con quest'ultima
			
			//Usando le informazioni di nota[0] e nota[1] calcolo il numero di mezzitasti tra C0 e la nota richiesta

			//Prima converto nota[0] = valori compresi tra char[A,B,C,...,G] in numeri 
			if (nota[0] >= 65 && nota[0] <= 71) {
				//Se nota[0] è una lettera compresa tra A e G la nota usata diventa un numero n t.c. -1 <= n <= 6
				nota_usata = nota[0] - 66;
				//Converto nota usata in relazione a C secondo l'ordine delle note di un pianoforte (C,D,E,F,G,A,B)
				if (nota_usata < 1) {
					nota_usata = nota_usata + 7;
				}
				//Converto la tabella (C=1,D=2,...,B=7) in questo modo (C=0,D=1,E=2,...,B=6) 
				nota_usata--;
				
				//Uso la formula nota_usata * 2 per calcolare i mezzitasti tra C ed E
				//Uso la formula (nota_usata * 2) - 1 per calcolare i mezzitasti tra C e le note dopo E
				//Quando si parla di mezzi tasti qui si intende sempre e solo per l'ottava numero 0
				if (nota_usata <= 2) {
					nota_usata = nota_usata * 2;
				}
				else {
					nota_usata = (nota_usata * 2) - 1;
				}
			
				//Controllo l'ottava in cui l'utente vuole suonare la nota convertendo il char nota[1] in int (Valori tra 0 e 7 vd. Ascii)
				//Poi moltiplico quel valore per il numero di mezzitasti tra C0 e C1, ovvero 12
				ottava = 12 * (nota[1] - 48);

				//Controllo il valore di nota[2], se esso è un intero n t.c 1<= n <= 4 (usando i valori char ASCII di 1 e 4) 
				//Quella sarà la durata della nota 
				if (nota[2] >= 49 && nota[2] <= 52) {
					durata_nota = (nota[2] - 48);
				}
				//Se nota[2] non è la durata della nota allora puo essere altre 2 cose:
				//	1) La nota può avere un alterazione, quindi calcolo +-1 in numero_mezzitasti da mettere nell'array canzone per calcolare la Frequenza
				//	2) Nota[2] = /0, quindi la stringa è terminata quindi restituisco numero_mezzitasti cosi com'è
				else {
					switch (nota[2]) {
					case 'b':
						numero_mezzitasti = nota_usata + ottava - 1;
						break;
					case '#':
						numero_mezzitasti = nota_usata + ottava + 1;
						break;
					default:
						numero_mezzitasti = nota_usata + ottava;
						break;
					}
				}
				// Se nota[2] è la durata o un alterazione controllo nota[3], se è diverso da '\0' allora sarà la durata
				if (nota[3] != '\0') {
					//Controllo per non avere errori e salvo la durata
					if (nota[2] >= 49 && nota[2] <= 52) {
						durata_nota = (nota[2] - 48);
					}
				}
		
			}
			
			//Se nota[0] è uguale a 'P' definisco una pausa nella melodia elevando il valore usato per calcolare la frequenza a 0
			if (nota[0] == 'P') {
				numero_mezzitasti = -1;
				if (nota[1] >= 49 && nota[2] <= 52) {
					durata_nota = (nota[1] - 48);
				}
			}
			
			canzone[t] = numero_mezzitasti;
			t++;
			canzone[t] = durata_nota;
			durata_nota = 1;
			t++;
		}
	}

	//Prendo in esame l'array canzone
	for (k = 0; k <= 74; k++) {
		//Se k è pari stiamo lavorando con la variabile n_mezzitasti
		//Se n_mezzitasti < 0 significa che stiamo lavorando con una pausa quindi la frequenza è zero
		if (k % 2 == 0) {
			if (canzone[k] <= 0) {
				Nota_x_frequenza = 0;
			}
			else {
				//Se n_mezzitasti > 0 usiamo la formula per calcolare la frequenza di una nota partendo da una di riferimento( Qui C0 )
				//fn = f0 * (a) ^ n
				//f0 = the frequency of one fixed note which must be defined.A common choice is setting the A above middle C(A4) at f0 = 440 Hz.
				//n = the number of half steps away from the fixed note you are. If you are at a higher note, n is positive. If you are on a lower note, n is negative.
				//fn = the frequency of the note n half steps away.
				//a = (2)1 / 12 = the twelth root of 2 = the number which when multiplied by itself 12 times equals 2 = 1.059463094359...
				Nota_x_frequenza = C0_frequenza * pow(a, canzone[k]);
			}
		}
		k++;
		//Moltiplico la variabile ms per la durata della nota
		lunghezza_nota = ms * canzone[k];
		//Finalmente uso il comando Beep
		Beep(Nota_x_frequenza, lunghezza_nota);
		Sleep(lunghezza_nota *2);
		printf("Ho fatto la nota");
	}

	return(0);
}
