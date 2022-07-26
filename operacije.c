#include "operacije.h"

// Ovde implementirati funkcije
void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {
		BLOK blok;
		blok.slogovi[0].sifraPregleda = OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

FILE *otvoriDatoteku(char *filename)
{
    FILE *fajl = fopen(filename, "rb+");
    if(fajl == NULL)
    {
        printf("Doslo je do greske! Moguce da datoteka \"%s\" ne postoji.\n", filename);
    }
    else
    {
        printf("Datoteka \"%s\" otvorena.\n", filename);
    }

    return fajl;
}

SLOG *pronadjiSlog(FILE *fajl, int sifraPregleda)
{
    if (fajl == NULL)
    {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraPregleda == OZNAKA_KRAJA_DATOTEKE)
            {
				return NULL;
			}

			if ((blok.slogovi[i].sifraPregleda == sifraPregleda) && !blok.slogovi[i].deleted)
            {
				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				return slog;
			}
		}
	}

	return NULL;
}

void ispisiSlog(SLOG *slog)
{
    printf("%12d %15s %12d %12d %15s %8d ", slog->sifraPregleda, slog->prezimeLekara, slog->brojLicenceLekara,
                                                slog->duzinaPregleda, slog->prezimePacijenta, slog->brojKartonaPacijenta);
}

void dodajSlog(FILE *fajl, SLOG *slog)
{
    if(fajl == NULL)
    {
        printf("Datoteka nije otvorena\n");
        return;
    }

    SLOG *stariSlog = pronadjiSlog(fajl, slog->sifraPregleda);
    if(stariSlog != NULL)
    {
        printf("Vec postoji slog sa tom sifrom pregleda!\n");
        return;
    }

    BLOK blok;
    fseek(fajl, -sizeof(BLOK), SEEK_END);
    fread(&blok, sizeof(BLOK), 1, fajl);

    int i;
    for(i=0; i<FBLOKIRANJA; i++)
    {
        if(blok.slogovi[i].sifraPregleda == OZNAKA_KRAJA_DATOTEKE)
        {
            memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
            break;
        }
    }

    i++;
    if(i < FBLOKIRANJA)
    {
        blok.slogovi[i].sifraPregleda = OZNAKA_KRAJA_DATOTEKE;

        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        fflush(fajl);
    }
    else
    {
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);

        BLOK noviBlok;
        noviBlok.slogovi[0].sifraPregleda = OZNAKA_KRAJA_DATOTEKE;

        fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
    }

    if(ferror(fajl))
    {
        printf("Greska pri upisu u fajl\n");
    }
    else
    {
        printf("Upis novog sloga je zavrsen\n");
    }
}

void ispisiSveSlogove(FILE *fajl)
{
    if (fajl == NULL)
    {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;

	printf("BL SL Sif.Pregleda    Lekar          Br.Licence  Duz.Pregleda     Pacijent         Br.Kartona \n");
	while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraPregleda == OZNAKA_KRAJA_DATOTEKE) {
				printf("B%d S%d *\n", rbBloka, i);
                break;
			}


			if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
		}

		rbBloka++;
	}
}

void obrisiSlogFizicki(FILE *fajl, int sifraPregleda)
{
    SLOG *slog = pronadjiSlog(fajl, sifraPregleda);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji!\n");
        return;
    }

    BLOK blok, naredniBlok;
    int sifraPregledaZaBrisanje = sifraPregleda;

    fseek(fajl, 0, SEEK_SET);

    while(fread(&blok, 1, sizeof(BLOK), fajl))
    {
        for(int i=0; i < FBLOKIRANJA; i++)
        {
            if (blok.slogovi[i].sifraPregleda == OZNAKA_KRAJA_DATOTEKE)
            {

                if (i == 0)
                {
                    printf("(skracujem fajl...)\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl);
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }

            if(blok.slogovi[i].sifraPregleda == sifraPregledaZaBrisanje)
            {
                for (int j = i+1; j < FBLOKIRANJA; j++)
                {
                    memcpy(&(blok.slogovi[j-1]), &(blok.slogovi[j]), sizeof(SLOG));
                }

                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);

                if (podatakaProcitano)
                {
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);

                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));

                    sifraPregledaZaBrisanje = naredniBlok.slogovi[0].sifraPregleda;
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fflush(fajl);

                if (!podatakaProcitano)
                {
                    printf("Slog je fizicki obrisan.\n");
                    return;
                }

                break;
            }
        }
    }
}

void najkraciPregled(FILE *fajl)
{
    if (fajl == NULL)
    {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	int duzinaPregleda = 999;
	char *lekar;
	//printf("%s %d\n", lekar, duzinaPregleda);

	while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
		for (int i = 0; i < FBLOKIRANJA; i++)
        {
            if(blok.slogovi[i].sifraPregleda != OZNAKA_KRAJA_DATOTEKE && blok.slogovi[i].duzinaPregleda != 0)
            {
                //printf("%s %d %d\n", blok.slogovi[i].prezimeLekara, blok.slogovi[i].duzinaPregleda, blok.slogovi[i].sifraPregleda);
                if (blok.slogovi[i].duzinaPregleda < duzinaPregleda)
                {
                    duzinaPregleda = blok.slogovi[i].duzinaPregleda;
                    strcpy(&lekar, blok.slogovi[i].prezimeLekara);
                    //printf("%s\n", &lekar);
                }
            }
		}
	}
    printf("%s\n", &lekar);
	//return &lekar;
}

void korigujPreglede(FILE *fajl)
{
    fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
		for (int i = 0; i < FBLOKIRANJA; i++)
        {
            if(blok.slogovi[i].sifraPregleda != OZNAKA_KRAJA_DATOTEKE)
            {
                if (blok.slogovi[i].duzinaPregleda > 60)
                {
                    //printf("%s %d\n", blok.slogovi[i].prezimeLekara, blok.slogovi[i].duzinaPregleda);
                    blok.slogovi[i].duzinaPregleda = 60;
                    //printf("%s %d\n", blok.slogovi[i].prezimeLekara, blok.slogovi[i].duzinaPregleda);
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);
                    fflush(fajl);
                }
            }
		}
	}
}

void listaPregledaZaLekara(FILE *fajl)
{
    int listaLekarskeLicence[50];
    int count = 0;
    int postoji = 0;

    fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	//Prikupljanje lekara u listu
	while (fread(&blok, sizeof(BLOK), 1, fajl))
    {
		for (int i = 0; i < FBLOKIRANJA; i++)
        {
            if(blok.slogovi[i].sifraPregleda != OZNAKA_KRAJA_DATOTEKE)
            {
                for(int k = 0; k < count; k++)
                {
                    if(listaLekarskeLicence[k] == blok.slogovi[i].brojLicenceLekara)
                    {
                        postoji = 1;
                        break;
                    }
                }
                if(postoji == 0)
                {
                    listaLekarskeLicence[count] = blok.slogovi[i].brojLicenceLekara;
                    count++;
                }
                postoji = 0;
            }
		}
	}

	//Ispis lekara sa njegovim pregledima

	for(int k; k < count; k++)
    {
        //Uzimanje potrebnog lekara
        fseek(fajl, 0, SEEK_SET);
        BLOK blok1;

        while (fread(&blok1, sizeof(BLOK), 1, fajl))
        {
            for (int i = 0; i < FBLOKIRANJA; i++)
            {
                if(blok1.slogovi[i].sifraPregleda != OZNAKA_KRAJA_DATOTEKE)
                {
                    if(blok1.slogovi[i].brojLicenceLekara == listaLekarskeLicence[k])
                    {
                        printf("\n%s: ", blok1.slogovi[i].prezimeLekara);
                        postoji = 1;
                        break;
                    }
                }
            }
            if(postoji == 1)
            {
                postoji = 0;
                break;
            }
        }

        //Ispis pregleda
        fseek(fajl, 0, SEEK_SET);
        BLOK blok2;

        while (fread(&blok2, sizeof(BLOK), 1, fajl))
        {
            for (int i = 0; i < FBLOKIRANJA; i++)
            {
                if(blok2.slogovi[i].sifraPregleda != OZNAKA_KRAJA_DATOTEKE)
                {
                    if(blok2.slogovi[i].brojLicenceLekara == listaLekarskeLicence[k])
                    {
                        printf("%d ", blok2.slogovi[i].sifraPregleda);
                    }
                }
            }
        }
    }

}














