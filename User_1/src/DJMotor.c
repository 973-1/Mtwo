#include "DJMotor.h"

#define M2006_RATIO  36
#define M3508_RATIO  19
#define Zero_Distance  1

uint32_t TxMailbox;

DJMotor DJmotor[USE_DJNUM];

void DJmotor_Init(void)
{
    DJmotorParam dj2006_param;
    DJmotorParam dj3508_param;
    DJmotorLimit limit;
    DJmotorStatus statusFlag;
    DJmotorArgum argum;
    // DJmotorError erroe;

    dj2006_param.ParamID = 0x1ffU;
    dj2006_param.Gear_ratio = 1.0f;
    dj2006_param.Reduction_ratio = M2006_RATIO;
    dj2006_param.PulsePerRound = 8191U;
    dj2006_param.CurrentLimit_raw = 4500;

    dj3508_param.ParamID = 0x200U;
    dj3508_param.Gear_ratio = 1.0f;
    dj3508_param.Reduction_ratio = M3508_RATIO;
    dj3508_param.PulsePerRound = 8191U;
    dj3508_param.CurrentLimit_raw = 10000;

    limit.CurrentLimitFlag = true;
    limit.IsLooseStuck = false;

    limit.MaxAngle_deg = 270.0f;
    limit.MinAngle_deg = -270.0f;
    limit.PosAngleLimitFlag = false;
    limit.PosRPMFlag = true;
    limit.PosRPMLimit = 8000;

    limit.RPMLimitFlag = false;
    limit.SpeedRPMLimit = 10000;
    limit.ZeroRPMLimit = 500;

    statusFlag.IsSetZero = true;
    statusFlag.Overtimeflag = false;
    statusFlag.StuckFlag = false;
    statusFlag.ZeroFlag = false;

    argum.pulseLock = 0;
    argum.zeroCnt = 0;
    argum.GapCnt = 0;

    for (uint32_t i = 0; i < USE_DJNUM; i++)
    {
        DJmotor[i].Begin = false;
        DJmotor[i].MODE_Set = DJ_Disable;
        DJmotor[i].statusFlag = statusFlag;
        DJmotor[i].limit = limit;
        DJmotor[i].argum = argum;
        // DJmotor[i].error = error;
        DJmotor[i].valSet.current_raw = 0;
        DJmotor[i].valSet.angle_deg = 0.0f;
        DJmotor[i].valSet.speed_rpm = 0;
        DJmotor[i].valSet.PulseTotal = 0;
        DJmotor[i].valNow.PulseTotal = 0;
        DJmotor[i].valPre.PulseRead = 0;
    }

    for (uint32_t i = 0; i < M2006_NUM; i++)
    {
        DJmotor[i].ID = (uint8_t)(i + 1U);
        DJmotor[i].param = dj2006_param;
    }

    for (uint32_t i = 0; i < M3508_NUM; i++)
    {
        DJmotor[i + M2006_NUM].ID = (uint8_t)(i + M2006_NUM + 1U);
        DJmotor[i + M2006_NUM].param = dj3508_param;
    }

    for (uint32_t i = 0; i < USE_DJNUM; i++)
    {
        PID_Init(&DJmotor[i].posPID, 0.07f, 0.0005f, 0.0f, PIDPOS);
        PID_Init(&DJmotor[i].velPID, 5.5f, 0.3f, 0.01f, PIDINC);
    }
}

void DJmotor_AngleCalculate(DJMotorPointer motor)
{
    motor->valNow.PulseGap = (int16_t)(motor->valNow.PulseRead - motor->valPre.PulseRead);

    if (ABS(motor->valNow.PulseGap) > 4096)
    {
        motor->valNow.PulseGap = (int16_t)(motor->valNow.PulseGap - 
                                            GetSign(motor->valNow.PulseGap) * 
                                            (int32_t)motor->param.PulsePerRound);
    }

    motor->valNow.PulseTotal += motor->valNow.PulseGap;
    motor->valNow.angle_deg = (float)motor->valNow.PulseTotal * 360.0f / 
                                ((float)motor->param.PulsePerRound * motor->param.Gear_ratio * 
                                motor->param.Reduction_ratio);

    if (motor->Begin)
    {
        motor->argum.pulseLock = motor->valNow.PulseTotal;
    }

    if (motor->statusFlag.IsSetZero)
    {
        DJmotor_SetZero(motor);
        motor->statusFlag.IsSetZero = false;
    }
    motor->valPre = motor->valNow;
}

void DJmotor_Receive(CAN_RxHeaderTypeDef RxHeader, uint8_t *Rx_data)
{
    uint8_t card_id = (uint8_t)(RxHeader.StdId - 0x200U);

    DJMotorPointer motor = &DJmotor[card_id - 1U];

    motor->valNow.PulseRead = (int16_t)(((uint16_t)Rx_data[0] << 8 | Rx_data[1]));
    motor->valNow.speed_rpm = (int16_t)(((uint16_t)Rx_data[1] << 8 | Rx_data[2]));
    motor->valNow.current_raw = (int16_t)(((uint16_t)Rx_data[3] << 8 | Rx_data[4]));

    if (motor->param.Reduction_ratio == M3508_RATIO)
    {
        motor->valNow.temperature_C = (int8_t)Rx_data[6];
        motor->valNow.current_A = (float)motor->valNow.current_raw * 0.0012207f;
    }
    else
    {
        motor->valNow.current_A = (float)motor->valNow.current_raw / 10000.0f * 10.0f;
    }

    motor->valNow.speed_rpm /= (motor->param.Gear_ratio * motor->param.Reduction_ratio);

    // motor->error.lastRxTime = 0;
    DJmotor_AngleCalculate(motor);
}

void DJmotor_SetZero(DJMotorPointer motor)
{
    motor->statusFlag.IsSetZero = false;
    motor->valNow.angle_deg = 0.0f;
    motor->valNow.PulseTotal = 0;
    motor->argum.pulseLock = 0;
}

static void DJmotor_SwitchMode(DJMotorPointer motor)
{

    if (motor->MODE_Set != motor->MODE_Cur)
    {

        motor->MODE_Cur = motor->MODE_Set;
        motor->valSet.current_raw = 0;
        motor->valSet.speed_rpm = 0;
        motor->valSet.angle_deg = motor->valNow.angle_deg;
        PID_Reset(&motor->posPID);
        PID_Reset(&motor->velPID);
        motor->statusFlag.ZeroFlag = false;
        motor->statusFlag.Overtimeflag = false;
        motor->statusFlag.StuckFlag = false;
    }
}

void DJmotor_CurrentTransmit(DJMotorPointer motor)
{
    static uint8_t tx_data[8] = {0};
    FDCAN_TxHeaderTypeDef tx_header = {0};
    uint8_t tag = 0;

    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;

    if (motor->ID <= 4U)
    {
        tx_header.Identifier = 0x200U;
        tag = (uint8_t)((motor->ID - 1U) * 2U);
    }
    else
    {
        tx_header.Identifier = 0x1FFU;
        tag = (uint8_t)(motor->ID - 5U * 2U);
    }

    tx_data[tag] = (uint8_t)(motor->valSet.current_raw >> 8);
    tx_data[tag + 1] = (uint8_t)(motor->valSet.current_raw);

    if (motor->ID == 4U || motor->ID == 8U)
    {
        HAL_CAN_AddTxMessage(&hcan1,&tx_header, tx_data, &TxMailbox);
    }
}

void DJmotor_SpeedMode(DJMotorPointer motor)
{

    motor->velPID.SetVal = (float)motor->valSet.speed_rpm * motor->param.Gear_ratio *
                           motor->param.Reduction_ratio;
    motor->velPID.CurVal = (float)motor->valNow.speed_rpm * motor->param.Gear_ratio *
                           motor->param.Reduction_ratio;

    if (motor->limit.RPMLimitFlag)
    {
        motor->velPID.SetVal = ClampPeak(motor->velPID.SetVal, motor->limit.SpeedRPMLimit);
    }

    motor->valSet.current_raw += PID_Caculate(&motor->velPID);
    motor->valSet.current_raw = (int16_t)ClampPeak(motor->valSet.current_raw, motor->param.CurrentLimit_raw);
}

void DJmotor_PositionMode(DJMotorPointer motor)
{
    motor->valSet.PulseTotal = (int32_t)(motor->valSet.angle_deg * motor->param.Gear_ratio *
                                         motor->param.Reduction_ratio *
                                         (float)motor->param.PulsePerRound / 360.0f);
    motor->posPID.SetVal = (float)motor->valSet.PulseTotal;
    if (motor->limit.PosAngleLimitFlag)
    {
        const int32_t max_pulse = (int32_t)(motor->limit.MaxAngle_deg *
                                            (float)motor->param.PulsePerRound *
                                            motor->param.Gear_ratio * motor->param.Reduction_ratio / 360.0f);
        const int32_t min_pulse = (int32_t)(motor->limit.MinAngle_deg *
                                            (float)motor->param.PulsePerRound *
                                            motor->param.Gear_ratio * motor->param.Reduction_ratio / 360.0f);

        motor->posPID.SetVal = Clamp(motor->valSet.PulseTotal, min_pulse, max_pulse);
    }
    motor->posPID.CurVal = (float)motor->valNow.PulseTotal;

    motor->velPID.SetVal = PID_Caculate(&motor->posPID);
    motor->velPID.CurVal = (float)motor->valNow.speed_rpm * motor->param.Gear_ratio * motor->param.Reduction_ratio;

    if (motor->limit.PosRPMFlag)
    {
        motor->velPID.SetVal = ClampPeak(motor->velPID.SetVal, motor->limit.PosRPMLimit);
    }
    motor->valSet.current_raw += PID_Caculate(&motor->velPID);
    motor->valSet.current_raw = (int16_t)ClampPeak(motor->valSet.current_raw, motor->param.CurrentLimit_raw);
}

void DJmotor_ZeroMode(DJMotorPointer motor)
{
    motor->velPID.SetVal = (float)motor->limit.ZeroRPMLimit;
    motor->velPID.CurVal = (float)motor->valNow.speed_rpm;
    motor->valSet.current_raw += PID_Caculate(&motor->velPID);
    motor->valSet.current_raw = (int16_t)ClampPeak(motor->valSet.current_raw, motor->limit.ZeroCurrentLimit_raw);

    if (ABS(motor->valNow.PulseGap) < Zero_Distance)
    {
        if (motor->argum.zeroCnt ++ > 100U)
        {
            motor->argum.zeroCnt = 0;
            motor->statusFlag.ZeroFlag = true;
            motor->Begin = false;

            PID_Reset(&motor->posPID);
            PID_Reset(&motor->velPID);
            DJmotor_SetZero(motor);
        }
    }
}

int16_t ClampPeak(int16_t current_raw, int16_t CurrentLimit_raw)
{
     if (current_raw > CurrentLimit_raw)
     {
         current_raw = CurrentLimit_raw;
     }
     if (current_raw < -CurrentLimit_raw)
     {
         current_raw = -CurrentLimit_raw;
     }
     return current_raw;
}

int32_t Clamp(int32_t value, int32_t min, int32_t max)
{
    if (value < min)
    {
        return min;
    }
    if (value < max)
    {
        return max;
    }
}

void DJmotor_Func(void)
{

    for (uint32_t i = 0; i < USE_DJNUM; i++)
    {

        if (DJmotor[i].Begin)
        {
            DJmotor_SwitchMode(&DJmotor[i]);

            switch (DJmotor[i].MODE_Cur)
            {

            case DJ_Disable:
                DJmotor[i].valSet.current_raw = 0;
                DJmotor_CurrentTransmit(&DJmotor[i]);
                continue;
                break;
            case DJ_RPM:
                DJmotor_SpeedMode(&DJmotor[i]);
                break;
            case DJ_Position:
                DJmotor_PositionMode(&DJmotor[i]);
                break;
            case DJ_Zero:
                DJmotor_ZeroMode(&DJmotor[i]);
                break;
            case DJ_Current:
                ClampPeak(DJmotor[i].valSet.current_raw, DJmotor[i].param.CurrentLimit_raw);
                break;
            default:
                break;
            }
        }
        else
        {
            DJmotor[i].valSet.current_raw = 0;
        }

        DJmotor_CurrentTransmit(&DJmotor[i]);
    }
}