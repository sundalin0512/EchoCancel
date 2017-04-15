//
// File: LMS.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 14-Apr-2017 20:04:32
//
#ifndef LMS_H
#define LMS_H

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
extern void LMS(const emxArray_real32_T *xn, const emxArray_real32_T *dn, float
                M, float mu, float itr, emxArray_real32_T *b_yn,
                emxArray_real32_T *W, emxArray_real32_T *en);

#endif

//
// File trailer for LMS.h
//
// [EOF]
//
