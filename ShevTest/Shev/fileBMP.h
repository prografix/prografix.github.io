
#ifndef FILEBMP_H
#define FILEBMP_H

struct tagBITMAPINFO;

typedef unsigned __int8 nat8;

void loadBMP ( const char * fileName, tagBITMAPINFO * & bmi, nat8 * & map );

#endif