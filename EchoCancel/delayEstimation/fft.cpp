//
// File: fft.cpp
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
#include "fft.h"
#include "delayEstimation_emxutil.h"
#include "ifft.h"

// Function Declarations
static void generate_twiddle_tables(int nRows, boolean_T useRadix2,
  emxArray_real32_T *costab, emxArray_real32_T *sintab, emxArray_real32_T
  *sintabinv);

// Function Definitions

//
// Arguments    : int nRows
//                boolean_T useRadix2
//                emxArray_real32_T *costab
//                emxArray_real32_T *sintab
//                emxArray_real32_T *sintabinv
// Return Type  : void
//
static void generate_twiddle_tables(int nRows, boolean_T useRadix2,
  emxArray_real32_T *costab, emxArray_real32_T *sintab, emxArray_real32_T
  *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  int nRowsD4;
  int nd2;
  int k;
  int n2;
  emxInit_real32_T(&costab1q, 2);
  e = 6.28318548F / (float)nRows;
  nRowsD4 = nRows / 2 / 2;
  nd2 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nRowsD4 + 1;
  emxEnsureCapacity((emxArray__common *)costab1q, nd2, sizeof(float));
  costab1q->data[0] = 1.0F;
  nd2 = nRowsD4 / 2;
  for (k = 1; k <= nd2; k++) {
    costab1q->data[k] = std::cos(e * (float)k);
  }

  for (k = nd2 + 1; k < nRowsD4; k++) {
    costab1q->data[k] = std::sin(e * (float)(nRowsD4 - k));
  }

  costab1q->data[nRowsD4] = 0.0F;
  if (!useRadix2) {
    nRowsD4 = costab1q->size[1] - 1;
    n2 = (costab1q->size[1] - 1) << 1;
    nd2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)costab, nd2, sizeof(float));
    nd2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)sintab, nd2, sizeof(float));
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    nd2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)sintabinv, nd2, sizeof(float));
    for (k = 1; k <= nRowsD4; k++) {
      sintabinv->data[k] = costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      sintabinv->data[k] = costab1q->data[k - nRowsD4];
    }

    for (k = 1; k <= nRowsD4; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      costab->data[k] = -costab1q->data[n2 - k];
      sintab->data[k] = -costab1q->data[k - nRowsD4];
    }
  } else {
    nRowsD4 = costab1q->size[1] - 1;
    n2 = (costab1q->size[1] - 1) << 1;
    nd2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)costab, nd2, sizeof(float));
    nd2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)sintab, nd2, sizeof(float));
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    for (k = 1; k <= nRowsD4; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[nRowsD4 - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      costab->data[k] = -costab1q->data[n2 - k];
      sintab->data[k] = -costab1q->data[k - nRowsD4];
    }

    nd2 = sintabinv->size[0] * sintabinv->size[1];
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)sintabinv, nd2, sizeof(float));
  }

  emxFree_real32_T(&costab1q);
}

//
// Arguments    : const emxArray_real32_T *x
//                float varargin_1
//                emxArray_creal32_T *y
// Return Type  : void
//
void fft(const emxArray_real32_T *x, float varargin_1, emxArray_creal32_T *y)
{
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  emxArray_creal32_T *wwc;
  emxArray_creal32_T *fy;
  emxArray_creal32_T *fv;
  boolean_T useRadix2;
  int nInt2;
  int N2blue;
  int idx;
  int nInt2m1;
  int ihi;
  int nRowsD2;
  int rt;
  int nRowsD4;
  int j;
  float nt_im;
  float nt_re;
  float temp_re;
  float temp_im;
  float fv_re;
  float fv_im;
  float wwc_im;
  float b_fv_re;
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  emxInit_real32_T(&sintabinv, 2);
  emxInit_creal32_T(&wwc, 1);
  emxInit_creal32_T(&fy, 1);
  emxInit_creal32_T(&fv, 1);
  if ((x->size[0] == 0) || ((int)varargin_1 == 0)) {
    nInt2 = y->size[0];
    y->size[0] = (int)varargin_1;
    emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
    if ((int)varargin_1 > x->size[0]) {
      ihi = y->size[0];
      nInt2 = y->size[0];
      y->size[0] = ihi;
      emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
      for (nInt2 = 0; nInt2 < ihi; nInt2++) {
        y->data[nInt2].re = 0.0F;
        y->data[nInt2].im = 0.0F;
      }
    }
  } else {
    useRadix2 = ((!((int)varargin_1 <= 0)) && (((int)varargin_1 & ((int)
      varargin_1 - 1)) == 0));
    get_algo_sizes((int)varargin_1, useRadix2, &N2blue, &idx);
    generate_twiddle_tables(idx, useRadix2, costab, sintab, sintabinv);
    if (useRadix2) {
      idx = x->size[0];
      nInt2m1 = (int)varargin_1;
      if (idx < nInt2m1) {
        nInt2m1 = idx;
      }

      nRowsD2 = (int)varargin_1 / 2;
      nRowsD4 = nRowsD2 / 2;
      nInt2 = y->size[0];
      y->size[0] = (int)varargin_1;
      emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
      if ((int)varargin_1 > x->size[0]) {
        ihi = y->size[0];
        nInt2 = y->size[0];
        y->size[0] = ihi;
        emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
        for (nInt2 = 0; nInt2 < ihi; nInt2++) {
          y->data[nInt2].re = 0.0F;
          y->data[nInt2].im = 0.0F;
        }
      }

      rt = 0;
      nInt2 = 0;
      idx = 0;
      for (N2blue = 1; N2blue < nInt2m1; N2blue++) {
        y->data[idx].re = x->data[rt];
        y->data[idx].im = 0.0F;
        idx = (int)varargin_1;
        useRadix2 = true;
        while (useRadix2) {
          idx >>= 1;
          nInt2 ^= idx;
          useRadix2 = ((nInt2 & idx) == 0);
        }

        idx = nInt2;
        rt++;
      }

      y->data[idx].re = x->data[rt];
      y->data[idx].im = 0.0F;
      if ((int)varargin_1 > 1) {
        for (N2blue = 0; N2blue <= (int)varargin_1 - 2; N2blue += 2) {
          temp_re = y->data[N2blue + 1].re;
          temp_im = y->data[N2blue + 1].im;
          y->data[N2blue + 1].re = y->data[N2blue].re - y->data[N2blue + 1].re;
          y->data[N2blue + 1].im = y->data[N2blue].im - y->data[N2blue + 1].im;
          y->data[N2blue].re += temp_re;
          y->data[N2blue].im += temp_im;
        }
      }

      idx = 2;
      rt = 4;
      nInt2 = 1 + ((nRowsD4 - 1) << 2);
      while (nRowsD4 > 0) {
        for (N2blue = 0; N2blue < nInt2; N2blue += rt) {
          temp_re = y->data[N2blue + idx].re;
          temp_im = y->data[N2blue + idx].im;
          y->data[N2blue + idx].re = y->data[N2blue].re - temp_re;
          y->data[N2blue + idx].im = y->data[N2blue].im - temp_im;
          y->data[N2blue].re += temp_re;
          y->data[N2blue].im += temp_im;
        }

        nInt2m1 = 1;
        for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
          nt_re = costab->data[j];
          nt_im = sintab->data[j];
          N2blue = nInt2m1;
          ihi = nInt2m1 + nInt2;
          while (N2blue < ihi) {
            temp_re = nt_re * y->data[N2blue + idx].re - nt_im * y->data[N2blue
              + idx].im;
            temp_im = nt_re * y->data[N2blue + idx].im + nt_im * y->data[N2blue
              + idx].re;
            y->data[N2blue + idx].re = y->data[N2blue].re - temp_re;
            y->data[N2blue + idx].im = y->data[N2blue].im - temp_im;
            y->data[N2blue].re += temp_re;
            y->data[N2blue].im += temp_im;
            N2blue += rt;
          }

          nInt2m1++;
        }

        nRowsD4 /= 2;
        idx = rt;
        rt += rt;
        nInt2 -= idx;
      }
    } else {
      nInt2m1 = ((int)varargin_1 + (int)varargin_1) - 1;
      nInt2 = wwc->size[0];
      wwc->size[0] = nInt2m1;
      emxEnsureCapacity((emxArray__common *)wwc, nInt2, sizeof(creal32_T));
      idx = (int)varargin_1;
      rt = 0;
      wwc->data[(int)varargin_1 - 1].re = 1.0F;
      wwc->data[(int)varargin_1 - 1].im = 0.0F;
      nInt2 = (int)varargin_1 << 1;
      for (j = 1; j < (int)varargin_1; j++) {
        ihi = (j << 1) - 1;
        if (nInt2 - rt <= ihi) {
          rt += ihi - nInt2;
        } else {
          rt += ihi;
        }

        nt_im = -3.14159274F * (float)rt / (float)(int)varargin_1;
        if (nt_im == 0.0F) {
          nt_re = 1.0F;
          nt_im = 0.0F;
        } else {
          nt_re = std::cos(nt_im);
          nt_im = std::sin(nt_im);
        }

        wwc->data[idx - 2].re = nt_re;
        wwc->data[idx - 2].im = -nt_im;
        idx--;
      }

      idx = 0;
      for (j = nInt2m1 - 1; j >= (int)varargin_1; j--) {
        wwc->data[j] = wwc->data[idx];
        idx++;
      }

      idx = (int)varargin_1;
      rt = x->size[0];
      if (idx < rt) {
        rt = idx;
      }

      nInt2 = y->size[0];
      y->size[0] = (int)varargin_1;
      emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
      if ((int)varargin_1 > x->size[0]) {
        ihi = y->size[0];
        nInt2 = y->size[0];
        y->size[0] = ihi;
        emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
        for (nInt2 = 0; nInt2 < ihi; nInt2++) {
          y->data[nInt2].re = 0.0F;
          y->data[nInt2].im = 0.0F;
        }
      }

      idx = 0;
      for (j = 0; j + 1 <= rt; j++) {
        nt_re = wwc->data[((int)varargin_1 + j) - 1].re;
        nt_im = wwc->data[((int)varargin_1 + j) - 1].im;
        y->data[j].re = nt_re * x->data[idx];
        y->data[j].im = nt_im * -x->data[idx];
        idx++;
      }

      while (rt + 1 <= (int)varargin_1) {
        y->data[rt].re = 0.0F;
        y->data[rt].im = 0.0F;
        rt++;
      }

      r2br_r2dit_trig_impl(y, N2blue, costab, sintab, fy);
      r2br_r2dit_trig(wwc, N2blue, costab, sintab, fv);
      nInt2 = fy->size[0];
      emxEnsureCapacity((emxArray__common *)fy, nInt2, sizeof(creal32_T));
      idx = fy->size[0];
      for (nInt2 = 0; nInt2 < idx; nInt2++) {
        nt_re = fy->data[nInt2].re;
        nt_im = fy->data[nInt2].im;
        fv_re = fv->data[nInt2].re;
        fv_im = fv->data[nInt2].im;
        fy->data[nInt2].re = nt_re * fv_re - nt_im * fv_im;
        fy->data[nInt2].im = nt_re * fv_im + nt_im * fv_re;
      }

      b_r2br_r2dit_trig(fy, N2blue, costab, sintabinv, fv);
      idx = 0;
      for (j = (int)(float)(int)varargin_1 - 1; j + 1 <= wwc->size[0]; j++) {
        nt_re = wwc->data[j].re;
        fv_re = fv->data[j].re;
        nt_im = wwc->data[j].im;
        fv_im = fv->data[j].im;
        temp_re = wwc->data[j].re;
        temp_im = fv->data[j].im;
        wwc_im = wwc->data[j].im;
        b_fv_re = fv->data[j].re;
        y->data[idx].re = nt_re * fv_re + nt_im * fv_im;
        y->data[idx].im = temp_re * temp_im - wwc_im * b_fv_re;
        idx++;
      }
    }
  }

  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&fy);
  emxFree_creal32_T(&wwc);
  emxFree_real32_T(&sintabinv);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

//
// File trailer for fft.cpp
//
// [EOF]
//
