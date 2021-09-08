%module Olympics
%include "typemaps.i"
%include "Olympics.h"
%{
#include "Olympics.h"
typedef struct olympics* Olympics;
%}

Olympics OlympicsCreate();

void OlympicsUpdateCompetitionResults(Olympics o, const char*
 gold_country, const char* silver_country, const char* bronze_country);

void OlympicsWinningCountry(Olympics o);

void OlympicsDestroy(Olympics o);
