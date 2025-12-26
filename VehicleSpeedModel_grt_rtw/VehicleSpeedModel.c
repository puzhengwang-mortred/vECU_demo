/*
 * VehicleSpeedModel.c
 *
 * Code generation for model "VehicleSpeedModel".
 *
 * Model version              : 1.2
 * Simulink Coder version : 25.2 (R2025b) 28-Jul-2025
 * C source code generated on : Fri Dec 26 13:39:25 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "VehicleSpeedModel.h"
#include "VehicleSpeedModel_private.h"
#include "rtwtypes.h"
#include <string.h>

/* Block signals (default storage) */
B_VehicleSpeedModel_T VehicleSpeedModel_B;

/* Continuous states */
X_VehicleSpeedModel_T VehicleSpeedModel_X;

/* Disabled State Vector */
XDis_VehicleSpeedModel_T VehicleSpeedModel_XDis;

/* External inputs (root inport signals with default storage) */
ExtU_VehicleSpeedModel_T VehicleSpeedModel_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_VehicleSpeedModel_T VehicleSpeedModel_Y;

/* Real-time model */
static RT_MODEL_VehicleSpeedModel_T VehicleSpeedModel_M_;
RT_MODEL_VehicleSpeedModel_T *const VehicleSpeedModel_M = &VehicleSpeedModel_M_;

/*
 * This function updates continuous states using the ODE2 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE2_IntgData *id = (ODE2_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T temp;
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  VehicleSpeedModel_derivatives();

  /* f1 = f(t + h, y + h*f0) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f0[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f1);
  VehicleSpeedModel_step();
  VehicleSpeedModel_derivatives();

  /* tnew = t + h
     ynew = y + (h/2)*(f0 + f1) */
  temp = 0.5*h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + f1[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void VehicleSpeedModel_step(void)
{
  if (rtmIsMajorTimeStep(VehicleSpeedModel_M)) {
    /* set solver stop time */
    if (!(VehicleSpeedModel_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&VehicleSpeedModel_M->solverInfo,
                            ((VehicleSpeedModel_M->Timing.clockTickH0 + 1) *
        VehicleSpeedModel_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&VehicleSpeedModel_M->solverInfo,
                            ((VehicleSpeedModel_M->Timing.clockTick0 + 1) *
        VehicleSpeedModel_M->Timing.stepSize0 +
        VehicleSpeedModel_M->Timing.clockTickH0 *
        VehicleSpeedModel_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(VehicleSpeedModel_M)) {
    VehicleSpeedModel_M->Timing.t[0] = rtsiGetT(&VehicleSpeedModel_M->solverInfo);
  }

  /* Integrator: '<Root>/Integrator' */
  /* Limited  Integrator  */
  if (VehicleSpeedModel_X.Integrator_CSTATE >=
      VehicleSpeedModel_P.Integrator_UpperSat) {
    VehicleSpeedModel_X.Integrator_CSTATE =
      VehicleSpeedModel_P.Integrator_UpperSat;
  } else if (VehicleSpeedModel_X.Integrator_CSTATE <=
             VehicleSpeedModel_P.Integrator_LowerSat) {
    VehicleSpeedModel_X.Integrator_CSTATE =
      VehicleSpeedModel_P.Integrator_LowerSat;
  }

  /* Outport: '<Root>/Speed_Output' incorporates:
   *  Integrator: '<Root>/Integrator'
   */
  VehicleSpeedModel_Y.Speed_Output = VehicleSpeedModel_X.Integrator_CSTATE;

  /* Sum: '<Root>/Add_Drag' incorporates:
   *  Gain: '<Root>/Gain_Alpha'
   *  Gain: '<Root>/Gain_Beta'
   *  Gain: '<Root>/Gain_Gamma'
   *  Inport: '<Root>/Brake_Input'
   *  Inport: '<Root>/Throttle_Input'
   *  Integrator: '<Root>/Integrator'
   *  Sum: '<Root>/Add_ThrottleBrake'
   */
  VehicleSpeedModel_B.Add_Drag = (VehicleSpeedModel_P.Gain_Alpha_Gain *
    VehicleSpeedModel_U.Throttle_Input - VehicleSpeedModel_P.Gain_Beta_Gain *
    VehicleSpeedModel_U.Brake_Input) - VehicleSpeedModel_P.Gain_Gamma_Gain *
    VehicleSpeedModel_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(VehicleSpeedModel_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(VehicleSpeedModel_M->rtwLogInfo,
                        (VehicleSpeedModel_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(VehicleSpeedModel_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(VehicleSpeedModel_M)!=-1) &&
          !((rtmGetTFinal(VehicleSpeedModel_M)-
             (((VehicleSpeedModel_M->Timing.clockTick1+
                VehicleSpeedModel_M->Timing.clockTickH1* 4294967296.0)) * 0.01))
            > (((VehicleSpeedModel_M->Timing.clockTick1+
                 VehicleSpeedModel_M->Timing.clockTickH1* 4294967296.0)) * 0.01)
            * (DBL_EPSILON))) {
        rtmSetErrorStatus(VehicleSpeedModel_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&VehicleSpeedModel_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++VehicleSpeedModel_M->Timing.clockTick0)) {
      ++VehicleSpeedModel_M->Timing.clockTickH0;
    }

    VehicleSpeedModel_M->Timing.t[0] = rtsiGetSolverStopTime
      (&VehicleSpeedModel_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      VehicleSpeedModel_M->Timing.clockTick1++;
      if (!VehicleSpeedModel_M->Timing.clockTick1) {
        VehicleSpeedModel_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void VehicleSpeedModel_derivatives(void)
{
  XDot_VehicleSpeedModel_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_VehicleSpeedModel_T *) VehicleSpeedModel_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  lsat = (VehicleSpeedModel_X.Integrator_CSTATE <=
          VehicleSpeedModel_P.Integrator_LowerSat);
  usat = (VehicleSpeedModel_X.Integrator_CSTATE >=
          VehicleSpeedModel_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (VehicleSpeedModel_B.Add_Drag > 0.0)) ||
      (usat && (VehicleSpeedModel_B.Add_Drag < 0.0))) {
    _rtXdot->Integrator_CSTATE = VehicleSpeedModel_B.Add_Drag;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<Root>/Integrator' */
}

/* Model initialize function */
void VehicleSpeedModel_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)VehicleSpeedModel_M, 0,
                sizeof(RT_MODEL_VehicleSpeedModel_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&VehicleSpeedModel_M->solverInfo,
                          &VehicleSpeedModel_M->Timing.simTimeStep);
    rtsiSetTPtr(&VehicleSpeedModel_M->solverInfo, &rtmGetTPtr
                (VehicleSpeedModel_M));
    rtsiSetStepSizePtr(&VehicleSpeedModel_M->solverInfo,
                       &VehicleSpeedModel_M->Timing.stepSize0);
    rtsiSetdXPtr(&VehicleSpeedModel_M->solverInfo, &VehicleSpeedModel_M->derivs);
    rtsiSetContStatesPtr(&VehicleSpeedModel_M->solverInfo, (real_T **)
                         &VehicleSpeedModel_M->contStates);
    rtsiSetNumContStatesPtr(&VehicleSpeedModel_M->solverInfo,
      &VehicleSpeedModel_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&VehicleSpeedModel_M->solverInfo,
      &VehicleSpeedModel_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&VehicleSpeedModel_M->solverInfo,
      &VehicleSpeedModel_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&VehicleSpeedModel_M->solverInfo,
      &VehicleSpeedModel_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&VehicleSpeedModel_M->solverInfo, (boolean_T**)
      &VehicleSpeedModel_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&VehicleSpeedModel_M->solverInfo, (&rtmGetErrorStatus
      (VehicleSpeedModel_M)));
    rtsiSetRTModelPtr(&VehicleSpeedModel_M->solverInfo, VehicleSpeedModel_M);
  }

  rtsiSetSimTimeStep(&VehicleSpeedModel_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&VehicleSpeedModel_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&VehicleSpeedModel_M->solverInfo, false);
  VehicleSpeedModel_M->intgData.y = VehicleSpeedModel_M->odeY;
  VehicleSpeedModel_M->intgData.f[0] = VehicleSpeedModel_M->odeF[0];
  VehicleSpeedModel_M->intgData.f[1] = VehicleSpeedModel_M->odeF[1];
  VehicleSpeedModel_M->contStates = ((X_VehicleSpeedModel_T *)
    &VehicleSpeedModel_X);
  VehicleSpeedModel_M->contStateDisabled = ((XDis_VehicleSpeedModel_T *)
    &VehicleSpeedModel_XDis);
  VehicleSpeedModel_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&VehicleSpeedModel_M->solverInfo, (void *)
                    &VehicleSpeedModel_M->intgData);
  rtsiSetSolverName(&VehicleSpeedModel_M->solverInfo,"ode2");
  rtmSetTPtr(VehicleSpeedModel_M, &VehicleSpeedModel_M->Timing.tArray[0]);
  rtmSetTFinal(VehicleSpeedModel_M, 10.0);
  VehicleSpeedModel_M->Timing.stepSize0 = 0.01;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    VehicleSpeedModel_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(VehicleSpeedModel_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(VehicleSpeedModel_M->rtwLogInfo, (NULL));
    rtliSetLogT(VehicleSpeedModel_M->rtwLogInfo, "tout");
    rtliSetLogX(VehicleSpeedModel_M->rtwLogInfo, "");
    rtliSetLogXFinal(VehicleSpeedModel_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(VehicleSpeedModel_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(VehicleSpeedModel_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(VehicleSpeedModel_M->rtwLogInfo, 0);
    rtliSetLogDecimation(VehicleSpeedModel_M->rtwLogInfo, 1);
    rtliSetLogY(VehicleSpeedModel_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(VehicleSpeedModel_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(VehicleSpeedModel_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &VehicleSpeedModel_B), 0,
                sizeof(B_VehicleSpeedModel_T));

  /* states (continuous) */
  {
    (void) memset((void *)&VehicleSpeedModel_X, 0,
                  sizeof(X_VehicleSpeedModel_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&VehicleSpeedModel_XDis, 0,
                  sizeof(XDis_VehicleSpeedModel_T));
  }

  /* external inputs */
  (void)memset(&VehicleSpeedModel_U, 0, sizeof(ExtU_VehicleSpeedModel_T));

  /* external outputs */
  VehicleSpeedModel_Y.Speed_Output = 0.0;

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(VehicleSpeedModel_M->rtwLogInfo, 0.0,
    rtmGetTFinal(VehicleSpeedModel_M), VehicleSpeedModel_M->Timing.stepSize0,
    (&rtmGetErrorStatus(VehicleSpeedModel_M)));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  VehicleSpeedModel_X.Integrator_CSTATE = VehicleSpeedModel_P.Integrator_IC;
}

/* Model terminate function */
void VehicleSpeedModel_terminate(void)
{
  /* (no terminate code required) */
}
