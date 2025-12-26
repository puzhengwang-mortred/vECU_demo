/*
 * File: SpeedController.c
 *
 * Code generated for Simulink model 'SpeedController'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Dec 26 13:11:41 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "SpeedController.h"

/* External inputs (root inport signals with default storage) */
ExtU_SpeedController_T SpeedController_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_SpeedController_T SpeedController_Y;

/* Real-time model */
static RT_MODEL_SpeedController_T SpeedController_M_;
RT_MODEL_SpeedController_T *const SpeedController_M = &SpeedController_M_;

/* Model step function */
void SpeedController_step(void)
{
  /* Switch: '<Root>/Brake_Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/Speed_Input'
   *  RelationalOperator: '<S1>/Compare'
   */
  if (SpeedController_U.Speed_Input > 105.0) {
    /* Outport: '<Root>/Brake_Output' incorporates:
     *  Constant: '<Root>/Brake_Constant'
     */
    SpeedController_Y.Brake_Output = 0.1;
  } else {
    /* Outport: '<Root>/Brake_Output' incorporates:
     *  Constant: '<Root>/Zero_Constant'
     */
    SpeedController_Y.Brake_Output = 0.0;
  }

  /* End of Switch: '<Root>/Brake_Switch' */

  /* Switch: '<Root>/Throttle_Switch' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/Speed_Input'
   *  RelationalOperator: '<S2>/Compare'
   */
  if (SpeedController_U.Speed_Input < 95.0) {
    /* Outport: '<Root>/Throttle_Output' incorporates:
     *  Constant: '<Root>/Throttle_Constant'
     */
    SpeedController_Y.Throttle_Output = 0.1;
  } else {
    /* Outport: '<Root>/Throttle_Output' incorporates:
     *  Constant: '<Root>/Zero_Constant'
     */
    SpeedController_Y.Throttle_Output = 0.0;
  }

  /* End of Switch: '<Root>/Throttle_Switch' */
}

/* Model initialize function */
void SpeedController_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void SpeedController_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
