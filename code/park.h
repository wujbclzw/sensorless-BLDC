#ifndef PARK_H
#define PARK_H

//------------------  C API for Park Clarke routines ---------------------

typedef struct {
    short   qAngle;
    short   qSin;
    short   qCos;
    short   qIa;
    short   qIb;
    short   qIalpha;
    short   qIbeta;
    short   qId;
    short   qIq;
    short   qVd;
    short   qVq;
    short   qValpha;
    short   qVbeta;
    short   qV1;
    short   qV2;
    short   qV3;
} tParkParm;


typedef struct 
{
    short   qAngle;
    short   qSin;
    short   qCos;
} tSincosParm;
    
extern tParkParm        ParkParm;
extern tSincosParm      SincosParm;

void SinCos(void);      // Calculate qSin,qCos from iAngle
void ClarkePark(void);  // Calculate qId,qIq from qCos,qSin,qIa,qIb
void InvPark(void);     // Calculate qValpha, qVbeta from qSin,qCos,qVd,qVq
void InvClarke(void);   // Calculate qV1, qV2, qV3 from qValpha, qVbeta

#endif


