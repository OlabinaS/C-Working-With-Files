
#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1

//Svaki slog predstavlja jedan pregled.

typedef struct Slog {
	int sifraPregleda; //koristi se kao kljuc sloga
	char prezimeLekara[16];
	int brojLicenceLekara;
	int duzinaPregleda;
	char prezimePacijenta[16];
	int brojKartonaPacijenta;
	int deleted;
} SLOG;

typedef struct Blok {
	SLOG slogovi[FBLOKIRANJA];
} BLOK;
