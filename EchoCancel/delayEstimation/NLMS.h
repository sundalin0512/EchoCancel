//
// File: NLMS.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 14-Apr-2017 20:04:32
//
#ifndef NLMS_H
#define NLMS_H

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
extern void NLMS(const float xn[4000], const float dn[4000], float M, float mu,
                 float itr, float yn_data[], int yn_size[2], emxArray_real32_T
                 *W, float en_data[], int en_size[1]);

#endif

//
// File trailer for NLMS.h
//
// [EOF]
//
