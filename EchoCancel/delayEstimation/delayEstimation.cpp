//
// File: delayEstimation.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 14-Apr-2017 20:04:32
//

// Include Files
#include "rt_nonfinite.h"
#include "LMS.h"
#include "NLMS.h"
#include "RLS.h"
#include "delayEstimation.h"
#include "relop.h"
#include "delayEstimation_emxutil.h"
#include "ifft.h"
#include "fft.h"

// Function Definitions

//
// Arguments    : const emxArray_real32_T *farEndSound
//                const emxArray_real32_T *nearEndSound
// Return Type  : float
//
float delayEstimation(const emxArray_real32_T *farEndSound, const
                      emxArray_real32_T *nearEndSound)
{
  float delay;
  emxArray_creal32_T *X1;
  emxArray_creal32_T *X2;
  emxArray_creal32_T *b_X1;
  int ixstart;
  int n;
  float X2_re;
  float X2_im;
  float X1_re;
  int dim;
  float X1_im;
  creal32_T xtmp;
  int ia;
  int vlend2;
  int vstride;
  int k;
  int ib;
  boolean_T exitg1;
  int midoffset;
  creal32_T c_X1;
  int ic;
  emxInit_creal32_T(&X1, 1);
  emxInit_creal32_T(&X2, 1);
  emxInit_creal32_T(&b_X1, 1);

  //
  fft(farEndSound, 2.0F * (float)farEndSound->size[0] - 1.0F, X1);
  fft(nearEndSound, 2.0F * (float)farEndSound->size[0] - 1.0F, X2);
  ixstart = b_X1->size[0];
  b_X1->size[0] = X1->size[0];
  emxEnsureCapacity((emxArray__common *)b_X1, ixstart, sizeof(creal32_T));
  n = X1->size[0];
  for (ixstart = 0; ixstart < n; ixstart++) {
    X2_re = X2->data[ixstart].re;
    X2_im = -X2->data[ixstart].im;
    X1_re = X1->data[ixstart].re;
    X1_im = X1->data[ixstart].im;
    b_X1->data[ixstart].re = X1_re * X2_re - X1_im * X2_im;
    b_X1->data[ixstart].im = X1_re * X2_im + X1_im * X2_re;
  }

  emxFree_creal32_T(&X2);
  ifft(b_X1, X1);
  emxFree_creal32_T(&b_X1);
  for (dim = 0; dim < 2; dim++) {
    if (dim + 1 <= 1) {
      ixstart = X1->size[0];
    } else {
      ixstart = 1;
    }

    if (!(ixstart <= 1)) {
      vlend2 = ixstart / 2;
      vstride = 1;
      k = 1;
      while (k <= dim) {
        vstride *= X1->size[0];
        k = 2;
      }

      midoffset = vlend2 * vstride;
      if (vlend2 << 1 == ixstart) {
        ixstart = -1;
        for (n = 1; n <= vstride; n++) {
          ixstart++;
          ia = ixstart;
          ib = ixstart + midoffset;
          for (k = 1; k <= vlend2; k++) {
            xtmp = X1->data[ia];
            X1->data[ia] = X1->data[ib];
            X1->data[ib] = xtmp;
            ia += vstride;
            ib += vstride;
          }
        }
      } else {
        ixstart = -1;
        for (n = 1; n <= vstride; n++) {
          ixstart++;
          ia = ixstart;
          ib = (ixstart + midoffset) + 1;
          xtmp = X1->data[ib - 1];
          for (k = 1; k <= vlend2; k++) {
            ic = ib + vstride;
            X1->data[ib - 1] = X1->data[ia];
            X1->data[ia] = X1->data[ic - 1];
            ia += vstride;
            ib = ic;
          }

          X1->data[ib - 1] = xtmp;
        }
      }
    }
  }

  //  Cxy=Cxy(1:N);
  ixstart = 1;
  n = X1->size[0];
  xtmp = X1->data[0];
  ia = 1;
  if (X1->size[0] > 1) {
    if (rtIsNaNF(X1->data[0].re) || rtIsNaNF(X1->data[0].im)) {
      ib = 2;
      exitg1 = false;
      while ((!exitg1) && (ib <= n)) {
        ixstart = ib;
        if (!(rtIsNaNF(X1->data[ib - 1].re) || rtIsNaNF(X1->data[ib - 1].im))) {
          xtmp = X1->data[ib - 1];
          ia = ib;
          exitg1 = true;
        } else {
          ib++;
        }
      }
    }

    if (ixstart < X1->size[0]) {
      while (ixstart + 1 <= n) {
        c_X1 = X1->data[ixstart];
        if (relop(c_X1, xtmp)) {
          xtmp = X1->data[ixstart];
          ia = ixstart + 1;
        }

        ixstart++;
      }
    }
  }

  emxFree_creal32_T(&X1);
  delay = (float)farEndSound->size[0] - (float)ia;

  //
  //  Cxy2=xcorr(farEndSound,nearEndSound);
  //  [max1,location1]=max(Cxy2);
  //  delay = N - location1;
  return delay;
}

//
// File trailer for delayEstimation.cpp
//
// [EOF]
//
