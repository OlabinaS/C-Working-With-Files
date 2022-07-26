//Stefan Olabina PR 11/2019
#include <stdio.h>
#include <stdlib.h>

#include "operacije.h"

// gcc *.c && ./a.out

int main() {

	int running = 1;
	int userInput;

	FILE *fajl = NULL;

	while (running) {
		printf("Odaberite opciju:\n");
		printf("1 - Formiranje datoteke\n");//
		printf("2 - Otvaranje datoteke\n");//
		printf("3 - Pretraga datoteke\n");//
		printf("4 - Unos sloga\n");//
		printf("5 - Ispis svih slogova\n");//
		printf("6 - Brisanje sloga (fiziko)\n");//
		printf("7 - Lekar sa najkracim pregledom\n");
		printf("8 - Koriguj vreme pregleda\n");
		printf("9 - Lekari i njihovi pregledi\n");
		printf("0 - Izlaz\n");//
		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &userInput);
		getc(stdin);

		switch(userInput) {
		    case 1:
				{
					char filename[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", filename);
					kreirajDatoteku(filename);
					printf("\n");
					break;
				}
            case 2:
                {
                    char filename[20];
                    printf("Unesite ime datoteke za otvaranje: ");
                    scanf("%s", filename);
                    fajl = otvoriDatoteku(filename);
                    printf("\n");
                    break;
                }
            case 3:
                {
                    int sifraPregleda;
                    printf("Unesite sifru pregleda: ");
                    scanf("%d", &sifraPregleda);
                    //SLOG *pronadjiSlog(FILE *fajl, int sifraPregleda)
                    SLOG *slog = pronadjiSlog(fajl, sifraPregleda);
                    if(slog == NULL)
                    {
                        printf("Nema tog sloga u datoteci.\n");
                    }
                    else
                    {
                        printf("Sif.Pregleda    Lekar          Br.Licence  Duz.Pregleda     Pacijent         Br.Kartona \n");
                        ispisiSlog(slog);
                        printf("\n");
                    }
                    free(slog);
                    printf("\n");
                    break;
                }
            case 4:
                {
                    SLOG slog;
                    printf("Sifra pregleda: ");
                    scanf("%d", &slog.sifraPregleda);
                    printf("Prezime lekara: ");
                    scanf("%s", slog.prezimeLekara);
                    printf("Broj licence lekara: ");
                    scanf("%d", &slog.brojLicenceLekara);
                    printf("Duzina trajanja pregleda(minuti): ");
                    scanf("%d", &slog.duzinaPregleda);
                    printf("Prezime pacijenta: ");
                    scanf("%s", slog.prezimePacijenta);
                    printf("Broj kartona: ");
                    scanf("%d", &slog.brojKartonaPacijenta);
                    slog.deleted = 0;

                    dodajSlog(fajl, &slog);
                    printf("\n");
                    break;
                }
            case 5:
                {
                    ispisiSveSlogove(fajl);
                    printf("\n");
                    break;
                }
            case 6:
                {
                    int sifraPregleda;
                    printf("Unesite sifru pregleda za brisanje: ");
                    scanf("%d", &sifraPregleda);
                    obrisiSlogFizicki(fajl, sifraPregleda);
                    printf("\n");
                    break;
                }
            case 7:
                {
                    //const char *lekar = najkraciPregled(fajl);
                    printf("Lekar sa najkracim pregledom je: ");
                    najkraciPregled(fajl);
                    printf("\n");
                    break;
                }
            case 8:
                {
                    korigujPreglede(fajl);
                    printf("Korigovanje pregleda...\n\n");
                    break;
                }
            case 9:
                {
                    listaPregledaZaLekara(fajl);
                    printf("\n\n");
                    break;
                }
            case 0:
                {
                    running = 0;
                    if(fajl != NULL)
                        fclose(fajl);
                }
		}
	}

	return 0;

}




