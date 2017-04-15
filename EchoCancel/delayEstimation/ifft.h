//
// File: ifft.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 14-Apr-2017 20:04:32
//
#ifndef IFFT_H
#define IFFT_H

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
extern void b_r2br_r2dit_trig(const emxArray_creal32_T *x, int n1_unsigned,
  const emxArray_real32_T *costab, const emxArray_real32_T *sintab,
  emxArray_creal32_T *y);
extern void get_algo_sizes(int n1, boolean_T useRadix2, int *N2blue, int *nRows);
extern void ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y);
extern void r2br_r2dit_trig(const emxArray_creal32_T *x, int n1_unsigned, const
  emxArray_real32_T *costab, const emxArray_real32_T *sintab, emxArray_creal32_T
  *y);
extern void r2br_r2dit_trig_impl(const emxArray_creal32_T *x, int unsigned_nRows,
  const emxArray_real32_T *costab, const emxArray_real32_T *sintab,
  emxArray_creal32_T *y);

#endif

//
// File trailer for ifft.h
//
// [EOF]
//
