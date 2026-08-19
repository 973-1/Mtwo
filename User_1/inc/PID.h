#ifndef PID_H
#define PID_H

#include "main.h"

#define PIDINC  0
#define PIDPOS  1
#define ABS(x) ((x)<0 ? -(x):(x))

typedef struct 
{
    float SetVal;
    float CurVal;
    float output;
    float KP;
    float KI;
    float KD;
    float err[3];
    uint8_t mode;
}PIDType;

float PID_Caculate(PIDType *pid);
void PID_Init(PIDType *pid_change, float kp, float ki, float kd,uint8_t mode_change);
int GetSign(float x);
void PID_Reset(PIDType *pid_change);

#endif