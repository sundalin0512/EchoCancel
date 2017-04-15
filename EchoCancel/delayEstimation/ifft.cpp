//
// File: ifft.cpp
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
#include "ifft.h"
#include "delayEstimation_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_creal32_T *x
//                int n1_unsigned
//                const emxArray_real32_T *costab
//                const emxArray_real32_T *sintab
//                emxArray_creal32_T *y
// Return Type  : void
//
void b_r2br_r2dit_trig(const emxArray_creal32_T *x, int n1_unsigned, const
  emxArray_real32_T *costab, const emxArray_real32_T *sintab, emxArray_creal32_T
  *y)
{
  int j;
  int nRowsD2;
  int nRowsD4;
  int iDelta2;
  int iy;
  int ix;
  int ju;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  float r;
  float twid_im;
  int ihi;
  j = x->size[0];
  if (!(j < n1_unsigned)) {
    j = n1_unsigned;
  }

  nRowsD2 = n1_unsigned / 2;
  nRowsD4 = nRowsD2 / 2;
  iDelta2 = y->size[0];
  y->size[0] = n1_unsigned;
  emxEnsureCapacity((emxArray__common *)y, iDelta2, sizeof(creal32_T));
  if (n1_unsigned > x->size[0]) {
    iy = y->size[0];
    iDelta2 = y->size[0];
    y->size[0] = iy;
    emxEnsureCapacity((emxArray__common *)y, iDelta2, sizeof(creal32_T));
    for (iDelta2 = 0; iDelta2 < iy; iDelta2++) {
      y->data[iDelta2].re = 0.0F;
      y->data[iDelta2].im = 0.0F;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 1; i < j; i++) {
    y->data[iy] = x->data[ix];
    iDelta2 = n1_unsigned;
    tst = true;
    while (tst) {
      iDelta2 >>= 1;
      ju ^= iDelta2;
      tst = ((ju & iDelta2) == 0);
    }

    iy = ju;
    ix++;
  }

  y->data[iy] = x->data[ix];
  if (n1_unsigned > 1) {
    for (i = 0; i <= n1_unsigned - 2; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
  }

  iy = 2;
  iDelta2 = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iDelta2) {
      temp_re = y->data[i + iy].re;
      temp_im = y->data[i + iy].im;
      y->data[i + iy].re = y->data[i].re - temp_re;
      y->data[i + iy].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      r = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + ix;
      while (i < ihi) {
        temp_re = r * y->data[i + iy].re - twid_im * y->data[i + iy].im;
        temp_im = r * y->data[i + iy].im + twid_im * y->data[i + iy].re;
        y->data[i + iy].re = y->data[i].re - temp_re;
        y->data[i + iy].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iDelta2;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iy = iDelta2;
    iDelta2 += iDelta2;
    ix -= iy;
  }

  if (y->size[0] > 1) {
    r = 1.0F / (float)y->size[0];
    iDelta2 = y->size[0];
    emxEnsureCapacity((emxArray__common *)y, iDelta2, sizeof(creal32_T));
    iy = y->size[0];
    for (iDelta2 = 0; iDelta2 < iy; iDelta2++) {
      y->data[iDelta2].re *= r;
      y->data[iDelta2].im *= r;
    }
  }
}

//
// Arguments    : int n1
//                boolean_T useRadix2
//                int *N2blue
//                int *nRows
// Return Type  : void
//
void get_algo_sizes(int n1, boolean_T useRadix2, int *N2blue, int *nRows)
{
  int nn1m1;
  int pmax;
  int pmin;
  boolean_T exitg1;
  int p;
  int pow2p;
  *N2blue = 1;
  if (useRadix2) {
    *nRows = n1;
  } else {
    if (n1 > 0) {
      nn1m1 = (n1 + n1) - 1;
      pmax = 31;
      if (nn1m1 <= 1) {
        pmax = 0;
      } else {
        pmin = 0;
        exitg1 = false;
        while ((!exitg1) && (pmax - pmin > 1)) {
          p = (pmin + pmax) >> 1;
          pow2p = 1 << p;
          if (pow2p == nn1m1) {
            pmax = p;
            exitg1 = true;
          } else if (pow2p > nn1m1) {
            pmax = p;
          } else {
            pmin = p;
          }
        }
      }

      *N2blue = 1 << pmax;
    }

    *nRows = *N2blue;
  }
}

//
// Arguments    : const emxArray_creal32_T *x
//                emxArray_creal32_T *y
// Return Type  : void
//
void ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y)
{
  int n1;
  emxArray_real32_T *costab1q;
  int nInt2;
  boolean_T useRadix2;
  int N2blue;
  int nd2;
  float e;
  int nRowsD4;
  int k;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  emxArray_creal32_T *wwc;
  int nInt2m1;
  int b_y;
  float denom_re;
  emxArray_creal32_T *fy;
  float wwc_im;
  emxArray_creal32_T *fv;
  float wwc_re;
  float fv_im;
  float b_wwc_im;
  float fv_re;
  float b_fv_im;
  float b_fv_re;
  n1 = x->size[0];
  if (x->size[0] == 0) {
    nInt2 = y->size[0];
    y->size[0] = 0;
    emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
  } else {
    emxInit_real32_T(&costab1q, 2);
    useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
    get_algo_sizes(x->size[0], useRadix2, &N2blue, &nd2);
    e = 6.28318548F / (float)nd2;
    nRowsD4 = nd2 / 2 / 2;
    nInt2 = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = nRowsD4 + 1;
    emxEnsureCapacity((emxArray__common *)costab1q, nInt2, sizeof(float));
    costab1q->data[0] = 1.0F;
    nd2 = nRowsD4 / 2;
    for (k = 1; k <= nd2; k++) {
      costab1q->data[k] = std::cos(e * (float)k);
    }

    for (k = nd2 + 1; k < nRowsD4; k++) {
      costab1q->data[k] = std::sin(e * (float)(nRowsD4 - k));
    }

    costab1q->data[nRowsD4] = 0.0F;
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    if (!useRadix2) {
      nd2 = costab1q->size[1] - 1;
      nRowsD4 = (costab1q->size[1] - 1) << 1;
      nInt2 = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nRowsD4 + 1;
      emxEnsureCapacity((emxArray__common *)costab, nInt2, sizeof(float));
      nInt2 = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nRowsD4 + 1;
      emxEnsureCapacity((emxArray__common *)sintab, nInt2, sizeof(float));
      costab->data[0] = 1.0F;
      sintab->data[0] = 0.0F;
      nInt2 = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = nRowsD4 + 1;
      emxEnsureCapacity((emxArray__common *)sintabinv, nInt2, sizeof(float));
      for (k = 1; k <= nd2; k++) {
        sintabinv->data[k] = costab1q->data[nd2 - k];
      }

      for (k = costab1q->size[1]; k <= nRowsD4; k++) {
        sintabinv->data[k] = costab1q->data[k - nd2];
      }

      for (k = 1; k <= nd2; k++) {
        costab->data[k] = costab1q->data[k];
        sintab->data[k] = -costab1q->data[nd2 - k];
      }

      for (k = costab1q->size[1]; k <= nRowsD4; k++) {
        costab->data[k] = -costab1q->data[nRowsD4 - k];
        sintab->data[k] = -costab1q->data[k - nd2];
      }
    } else {
      nd2 = costab1q->size[1] - 1;
      nRowsD4 = (costab1q->size[1] - 1) << 1;
      nInt2 = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nRowsD4 + 1;
      emxEnsureCapacity((emxArray__common *)costab, nInt2, sizeof(float));
      nInt2 = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nRowsD4 + 1;
      emxEnsureCapacity((emxArray__common *)sintab, nInt2, sizeof(float));
      costab->data[0] = 1.0F;
      sintab->data[0] = 0.0F;
      for (k = 1; k <= nd2; k++) {
        costab->data[k] = costab1q->data[k];
        sintab->data[k] = costab1q->data[nd2 - k];
      }

      for (k = costab1q->size[1]; k <= nRowsD4; k++) {
        costab->data[k] = -costab1q->data[nRowsD4 - k];
        sintab->data[k] = costab1q->data[k - nd2];
      }

      nInt2 = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = 0;
      emxEnsureCapacity((emxArray__common *)sintabinv, nInt2, sizeof(float));
    }

    emxFree_real32_T(&costab1q);
    if (useRadix2) {
      b_r2br_r2dit_trig(x, x->size[0], costab, sintab, y);
    } else {
      emxInit_creal32_T(&wwc, 1);
      nInt2m1 = (x->size[0] + x->size[0]) - 1;
      nInt2 = wwc->size[0];
      wwc->size[0] = nInt2m1;
      emxEnsureCapacity((emxArray__common *)wwc, nInt2, sizeof(creal32_T));
      nd2 = x->size[0];
      nRowsD4 = 0;
      wwc->data[x->size[0] - 1].re = 1.0F;
      wwc->data[x->size[0] - 1].im = 0.0F;
      nInt2 = x->size[0] << 1;
      for (k = 1; k < n1; k++) {
        b_y = (k << 1) - 1;
        if (nInt2 - nRowsD4 <= b_y) {
          nRowsD4 += b_y - nInt2;
        } else {
          nRowsD4 += b_y;
        }

        e = 3.14159274F * (float)nRowsD4 / (float)x->size[0];
        if (e == 0.0F) {
          denom_re = 1.0F;
          e = 0.0F;
        } else {
          denom_re = std::cos(e);
          e = std::sin(e);
        }

        wwc->data[nd2 - 2].re = denom_re;
        wwc->data[nd2 - 2].im = -e;
        nd2--;
      }

      nd2 = 0;
      for (k = nInt2m1 - 1; k >= n1; k--) {
        wwc->data[k] = wwc->data[nd2];
        nd2++;
      }

      nRowsD4 = x->size[0];
      nd2 = x->size[0];
      nInt2 = y->size[0];
      y->size[0] = nd2;
      emxEnsureCapacity((emxArray__common *)y, nInt2, sizeof(creal32_T));
      nd2 = 0;
      for (k = 0; k + 1 <= nRowsD4; k++) {
        denom_re = wwc->data[(n1 + k) - 1].re;
        e = wwc->data[(n1 + k) - 1].im;
        wwc_im = x->data[nd2].re;
        wwc_re = x->data[nd2].im;
        fv_im = x->data[nd2].im;
        b_wwc_im = x->data[nd2].re;
        y->data[k].re = denom_re * wwc_im + e * wwc_re;
        y->data[k].im = denom_re * fv_im - e * b_wwc_im;
        nd2++;
      }

      while (nRowsD4 + 1 <= n1) {
        y->data[nRowsD4].re = 0.0F;
        y->data[nRowsD4].im = 0.0F;
        nRowsD4++;
      }

      emxInit_creal32_T(&fy, 1);
      emxInit_creal32_T(&fv, 1);
      r2br_r2dit_trig_impl(y, N2blue, costab, sintab, fy);
      r2br_r2dit_trig(wwc, N2blue, costab, sintab, fv);
      nInt2 = fy->size[0];
      emxEnsureCapacity((emxArray__common *)fy, nInt2, sizeof(creal32_T));
      nd2 = fy->size[0];
      for (nInt2 = 0; nInt2 < nd2; nInt2++) {
        e = fy->data[nInt2].re;
        wwc_im = fy->data[nInt2].im;
        fv_re = fv->data[nInt2].re;
        b_fv_im = fv->data[nInt2].im;
        fy->data[nInt2].re = e * fv_re - wwc_im * b_fv_im;
        fy->data[nInt2].im = e * b_fv_im + wwc_im * fv_re;
      }

      b_r2br_r2dit_trig(fy, N2blue, costab, sintabinv, fv);
      nd2 = 0;
      denom_re = (float)x->size[0];
      k = (int)(float)x->size[0] - 1;
      emxFree_creal32_T(&fy);
      while (k + 1 <= wwc->size[0]) {
        e = wwc->data[k].re;
        fv_re = fv->data[k].re;
        wwc_im = wwc->data[k].im;
        b_fv_im = fv->data[k].im;
        wwc_re = wwc->data[k].re;
        fv_im = fv->data[k].im;
        b_wwc_im = wwc->data[k].im;
        b_fv_re = fv->data[k].re;
        y->data[nd2].re = e * fv_re + wwc_im * b_fv_im;
        y->data[nd2].im = wwc_re * fv_im - b_wwc_im * b_fv_re;
        e = wwc->data[k].re;
        fv_re = fv->data[k].re;
        wwc_im = wwc->data[k].im;
        b_fv_im = fv->data[k].im;
        wwc_re = wwc->data[k].re;
        fv_im = fv->data[k].im;
        b_wwc_im = wwc->data[k].im;
        b_fv_re = fv->data[k].re;
        y->data[nd2].re = e * fv_re + wwc_im * b_fv_im;
        y->data[nd2].im = wwc_re * fv_im - b_wwc_im * b_fv_re;
        e = y->data[nd2].re;
        wwc_im = y->data[nd2].im;
        if (wwc_im == 0.0F) {
          y->data[nd2].re = e / denom_re;
          y->data[nd2].im = 0.0F;
        } else if (e == 0.0F) {
          y->data[nd2].re = 0.0F;
          y->data[nd2].im = wwc_im / denom_re;
        } else {
          y->data[nd2].re = e / denom_re;
          y->data[nd2].im = wwc_im / denom_re;
        }

        nd2++;
        k++;
      }

      emxFree_creal32_T(&fv);
      emxFree_creal32_T(&wwc);
    }

    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

//
// Arguments    : const emxArray_creal32_T *x
//                int n1_unsigned
//                const emxArray_real32_T *costab
//                const emxArray_real32_T *sintab
//                emxArray_creal32_T *y
// Return Type  : void
//
void r2br_r2dit_trig(const emxArray_creal32_T *x, int n1_unsigned, const
                     emxArray_real32_T *costab, const emxArray_real32_T *sintab,
                     emxArray_creal32_T *y)
{
  int j;
  int nRowsD2;
  int nRowsD4;
  int iy;
  int iDelta;
  int ix;
  int ju;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  float twid_re;
  float twid_im;
  int ihi;
  j = x->size[0];
  if (!(j < n1_unsigned)) {
    j = n1_unsigned;
  }

  nRowsD2 = n1_unsigned / 2;
  nRowsD4 = nRowsD2 / 2;
  iy = y->size[0];
  y->size[0] = n1_unsigned;
  emxEnsureCapacity((emxArray__common *)y, iy, sizeof(creal32_T));
  if (n1_unsigned > x->size[0]) {
    iDelta = y->size[0];
    iy = y->size[0];
    y->size[0] = iDelta;
    emxEnsureCapacity((emxArray__common *)y, iy, sizeof(creal32_T));
    for (iy = 0; iy < iDelta; iy++) {
      y->data[iy].re = 0.0F;
      y->data[iy].im = 0.0F;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 1; i < j; i++) {
    y->data[iy] = x->data[ix];
    iDelta = n1_unsigned;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }

    iy = ju;
    ix++;
  }

  y->data[iy] = x->data[ix];
  if (n1_unsigned > 1) {
    for (i = 0; i <= n1_unsigned - 2; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
  }

  iDelta = 2;
  iy = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iy) {
      temp_re = y->data[i + iDelta].re;
      temp_im = y->data[i + iDelta].im;
      y->data[i + iDelta].re = y->data[i].re - temp_re;
      y->data[i + iDelta].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + ix;
      while (i < ihi) {
        temp_re = twid_re * y->data[i + iDelta].re - twid_im * y->data[i +
          iDelta].im;
        temp_im = twid_re * y->data[i + iDelta].im + twid_im * y->data[i +
          iDelta].re;
        y->data[i + iDelta].re = y->data[i].re - temp_re;
        y->data[i + iDelta].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iy;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iDelta = iy;
    iy += iy;
    ix -= iDelta;
  }
}

//
// Arguments    : const emxArray_creal32_T *x
//                int unsigned_nRows
//                const emxArray_real32_T *costab
//                const emxArray_real32_T *sintab
//                emxArray_creal32_T *y
// Return Type  : void
//
void r2br_r2dit_trig_impl(const emxArray_creal32_T *x, int unsigned_nRows, const
  emxArray_real32_T *costab, const emxArray_real32_T *sintab, emxArray_creal32_T
  *y)
{
  int j;
  int nRowsD2;
  int nRowsD4;
  int iy;
  int iDelta;
  int ix;
  int ju;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  float twid_re;
  float twid_im;
  int ihi;
  j = x->size[0];
  if (!(j < unsigned_nRows)) {
    j = unsigned_nRows;
  }

  nRowsD2 = unsigned_nRows / 2;
  nRowsD4 = nRowsD2 / 2;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity((emxArray__common *)y, iy, sizeof(creal32_T));
  if (unsigned_nRows > x->size[0]) {
    iDelta = y->size[0];
    iy = y->size[0];
    y->size[0] = iDelta;
    emxEnsureCapacity((emxArray__common *)y, iy, sizeof(creal32_T));
    for (iy = 0; iy < iDelta; iy++) {
      y->data[iy].re = 0.0F;
      y->data[iy].im = 0.0F;
    }
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 1; i < j; i++) {
    y->data[iy] = x->data[ix];
    iDelta = unsigned_nRows;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }

    iy = ju;
    ix++;
  }

  y->data[iy] = x->data[ix];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= unsigned_nRows - 2; i += 2) {
      temp_re = y->data[i + 1].re;
      temp_im = y->data[i + 1].im;
      y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
      y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }
  }

  iDelta = 2;
  iy = 4;
  ix = 1 + ((nRowsD4 - 1) << 2);
  while (nRowsD4 > 0) {
    for (i = 0; i < ix; i += iy) {
      temp_re = y->data[i + iDelta].re;
      temp_im = y->data[i + iDelta].im;
      y->data[i + iDelta].re = y->data[i].re - temp_re;
      y->data[i + iDelta].im = y->data[i].im - temp_im;
      y->data[i].re += temp_re;
      y->data[i].im += temp_im;
    }

    ju = 1;
    for (j = nRowsD4; j < nRowsD2; j += nRowsD4) {
      twid_re = costab->data[j];
      twid_im = sintab->data[j];
      i = ju;
      ihi = ju + ix;
      while (i < ihi) {
        temp_re = twid_re * y->data[i + iDelta].re - twid_im * y->data[i +
          iDelta].im;
        temp_im = twid_re * y->data[i + iDelta].im + twid_im * y->data[i +
          iDelta].re;
        y->data[i + iDelta].re = y->data[i].re - temp_re;
        y->data[i + iDelta].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
        i += iy;
      }

      ju++;
    }

    nRowsD4 /= 2;
    iDelta = iy;
    iy += iy;
    ix -= iDelta;
  }
}

//
// File trailer for ifft.cpp
//
// [EOF]
//
