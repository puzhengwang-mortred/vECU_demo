/*
 * File: SpeedController.h
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

#ifndef SpeedController_h_
#define SpeedController_h_
#ifndef SpeedController_COMMON_INCLUDES_
#define SpeedController_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* SpeedController_COMMON_INCLUDES_ */

#include "SpeedController_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Speed_Input;                  /* '<Root>/Speed_Input' */
} ExtU_SpeedController_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Brake_Output;                 /* '<Root>/Brake_Output' */
  real_T Throttle_Output;              /* '<Root>/Throttle_Output' */
} ExtY_SpeedController_T;

/* Real-time Model Data Structure */
struct tag_RTM_SpeedController_T {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_SpeedController_T SpeedController_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SpeedController_T SpeedController_Y;

/* Model entry point functions */
extern void SpeedController_initialize(void);
extern void SpeedController_step(void);
extern void SpeedController_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SpeedController_T *const SpeedController_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SpeedController'
 * '<S1>'   : 'SpeedController/Compare_GT_105'
 * '<S2>'   : 'SpeedController/Compare_LT_95'
 */
#endif                                 /* SpeedController_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
