#ifndef OPERACIJE_NAD_DATOTEKOM_H
#define OPERACIJE_NAD_DATOTEKOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "slog.h"

void kreirajDatoteku(char *filename);//
FILE *otvoriDatoteku(char *filename);//
SLOG *pronadjiSlog(FILE *fajl, int sifraPregleda);//
void dodajSlog(FILE *fajl, SLOG *slog);//
void ispisiSveSlogove(FILE *fajl);//
void obrisiSlogFizicki(FILE *fajl, int sifraPregleda);//

void ispisiSlog(SLOG *slog);//

void najkraciPregled(FILE *fajl);//

void korigujPreglede(FILE *fajl);//

void listaPregledaZaLekara(FILE *fajl);

#endif
