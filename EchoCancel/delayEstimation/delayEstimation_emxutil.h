//
// File: delayEstimation_emxutil.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 27-Apr-2017 21:17:15
//
#ifndef DELAYESTIMATION_EMXUTIL_H
#define DELAYESTIMATION_EMXUTIL_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "delayEstimation_types.h"

// Function Declarations
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, unsigned
  int elementSize);
extern void emxFree_creal32_T(emxArray_creal32_T **pEmxArray);
extern void emxFree_real32_T(emxArray_real32_T **pEmxArray);
extern void emxInit_creal32_T(emxArray_creal32_T **pEmxArray, int numDimensions);
extern void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);
extern void emxInit_real32_T1(emxArray_real32_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for delayEstimation_emxutil.h
//
// [EOF]
//
