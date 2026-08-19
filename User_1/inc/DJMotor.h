#ifndef DJMOTOR_H
#define DJMOTOR_H

#include "main.h"
#include "PID.h"
#include "CAN_IRQHandler.h"
#include <stdbool.h>

#define USE_DJNUM  4
#define M2006_NUM  4
#define M3508_NUM  0
#define USE_DJ     1
#define FDCAN_STANDARD_ID  0x00000000
#define FDCAN_DLC_BYTES_8  0X00000008
#define ABS(x) ((x)<0 ? -(x):(x))

typedef enum
{
    DJ_Disable = 0,
    DJ_RPM = 1,
    DJ_Position = 2,
    DJ_Zero= 3,
    DJ_Current = 4,
} DJmotor_mode_t;

typedef struct 
{
    volatile int16_t current_raw;
    volatile float angle_deg;
    volatile int16_t speed_rpm;
    volatile float current_A;
    volatile int16_t PulseRead;
    volatile int16_t PulseGap;
    volatile int32_t PulseTotal;
    volatile int8_t temperature_C;
} DJmotorVal;

typedef struct 
{
    uint16_t PulsePerRound;
    float Gear_ratio;
    float Reduction_ratio;
    uint32_t ParamID;
    int16_t CurrentLimit_raw;
}DJmotorParam;

typedef struct 
{
    bool RPMLimitFlag;
    bool PosAngleLimitFlag;
    bool PosRPMFlag;
    bool CurrentLimitFlag;
    float MaxAngle_deg;
    float MinAngle_deg;
    int16_t SpeedRPMLimit;
    int32_t PosRPMLimit;
    int16_t ZeroRPMLimit;
    int16_t ZeroCurrentLimit_raw;
    bool IsLooseStuck;
}DJmotorLimit;

typedef struct 
{
    bool IsSetZero;
    bool Overtimeflag;
    bool StuckFlag;
    bool ZeroFlag;
}DJmotorStatus;

typedef struct 
{
    uint8_t pulseLock;
    uint8_t zeroCnt;
    uint8_t GapCnt;
}DJmotorArgum;

typedef struct 
{
    uint8_t ID;
    volatile bool Begin;
    volatile DJmotor_mode_t MODE_Set;
    volatile DJmotor_mode_t MODE_Cur;

    DJmotorParam param;
    DJmotorVal valSet;
    DJmotorVal valNow;
    DJmotorVal valPre;
    DJmotorStatus statusFlag;
    DJmotorLimit limit;
    DJmotorArgum argum;
    //DJmotor error;
    PIDType posPID;
    PIDType velPID;
}DJMotor,*DJMotorPointer;

/*
typedef struct 
{
    uint8_t Identifier;
    uint32_t IdType;
    uint32_t TxFrameType;
    uint32_t DataLength;
    uint32_t ErrorStateIndicator;
    uint32_t BitRataaSwitch;
    uint32_t FDFormat;
    uint32_t TxEventFiControl;
    uint32_t MessageMarker;
}FDCAN_TxHeaderTypeDef;
*/
#if USE_DJ
    extern DJMotor DJmotor[USE_DJNUM];

    void DJmotor_Init(void);
    void DJmotor_AngleCalculate(DJMotorPointer motor);
    void DJmotor_Func(void);
    void DJmotor_Receive(CAN_RxHeaderTypeDef RxHeader, uint8_t *Rx_data);
    //void DJmotor_PID_Reload(DJMotorPointer motor,DJmotorPID pid_reload);
    void DJmotor_SetZero(DJMotorPointer motor);
    int16_t ClampPeak(int16_t current_raw, int16_t CurrentLimit_raw);

    void DJmotor_CurrentTransmit(DJMotorPointer motor);
    void DJmotor_SpeedMode(DJMotorPointer motor);
    void DJmotor_PositionMode(DJMotorPointer motor);
    void DJmotor_ZeroMode(DJMotorPointer motor);

#endif
#endif