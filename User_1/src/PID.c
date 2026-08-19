#include "PID.h"

int GetSign(float x){
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

float PID_Caculate(PIDType *pid)
{
    pid->err[0] = pid->SetVal - pid->CurVal;

    switch (pid->mode)
    {
        case PIDINC:
            pid->output = pid->KP * (pid->err[0] - pid->err[1]) +
                          pid->KI * pid->err[0] +
                          pid->KD * (pid->err[0] - 2.0f * pid->err[1] + pid->err[2]);
            pid->err[2] = pid->err[1];
            pid->err[1] = pid->err[0];
            break;
        case PIDPOS:
            pid->err[2] = 0.5f * pid->err[0] + 0.5f * pid->err[2];
            if (ABS(pid->err[2]) > 100.0f)
            {
                pid->err[2] = GetSign(pid->err[2]) * 100.0f;
            }
            pid->output = pid->KP * pid->err[0] +
                          pid->KI * pid->err[2] +
                          pid->KD * (pid->err[0] - pid->err[1]);
            pid->err[1] = pid->err[0];
            break;
        default:
            break;
    }
    return pid->output;
}

void PID_Init(PIDType *pid_change, float kp, float ki, float kd,uint8_t mode_change)
{
    pid_change->KP = kp;
    pid_change->KI = ki;
    pid_change->KD = kd;
    pid_change->mode = mode_change;
}

void PID_Reset(PIDType *pid_change)
{
    pid_change->err[0] = 0.0f;
    pid_change->err[1] = 0.0f;
    pid_change->err[2] = 0.0f;
    pid_change->output = 0.0f;
}