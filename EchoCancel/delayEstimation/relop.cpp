//
// File: relop.cpp
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

// Function Declarations
static float rt_atan2f_snf(float u0, float u1);
static float rt_hypotf_snf(float u0, float u1);

// Function Definitions

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_atan2f_snf(float u0, float u1)
{
  float y;
  int b_u0;
  int b_u1;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = ((real32_T)rtNaN);
  } else if (rtIsInfF(u0) && rtIsInfF(u1)) {
    if (u0 > 0.0F) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0F) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = std::atan2((float)b_u0, (float)b_u1);
  } else if (u1 == 0.0F) {
    if (u0 > 0.0F) {
      y = RT_PIF / 2.0F;
    } else if (u0 < 0.0F) {
      y = -(RT_PIF / 2.0F);
    } else {
      y = 0.0F;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
static float rt_hypotf_snf(float u0, float u1)
{
  float y;
  float a;
  float b;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = b;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

//
// Arguments    : const creal32_T a
//                const creal32_T b
// Return Type  : boolean_T
//
boolean_T relop(const creal32_T a, const creal32_T b)
{
  boolean_T p;
  float absbi;
  float y;
  float absxk;
  int exponent;
  float absar;
  float absbr;
  float Ma;
  int b_exponent;
  int c_exponent;
  int d_exponent;
  if ((std::abs(a.re) > 1.70141173E+38F) || (std::abs(a.im) > 1.70141173E+38F) ||
      (std::abs(b.re) > 1.70141173E+38F) || (std::abs(b.im) > 1.70141173E+38F))
  {
    absbi = rt_hypotf_snf(a.re / 2.0F, a.im / 2.0F);
    y = rt_hypotf_snf(b.re / 2.0F, b.im / 2.0F);
  } else {
    absbi = rt_hypotf_snf(a.re, a.im);
    y = rt_hypotf_snf(b.re, b.im);
  }

  absxk = y / 2.0F;
  if ((!rtIsInfF(absxk)) && (!rtIsNaNF(absxk))) {
    if (absxk <= 1.17549435E-38F) {
      absxk = 1.4013E-45F;
    } else {
      std::frexp(absxk, &exponent);
      absxk = std::ldexp(1.0F, exponent - 24);
    }
  } else {
    absxk = ((real32_T)rtNaN);
  }

  if ((std::abs(y - absbi) < absxk) || (rtIsInfF(absbi) && rtIsInfF(y) &&
       ((absbi > 0.0F) == (y > 0.0F)))) {
    p = true;
  } else {
    p = false;
  }

  if (p) {
    absar = std::abs(a.re);
    absxk = std::abs(a.im);
    absbr = std::abs(b.re);
    absbi = std::abs(b.im);
    if (absar > absxk) {
      Ma = absar;
      absar = absxk;
    } else {
      Ma = absxk;
    }

    if (absbr > absbi) {
      absxk = absbr;
      absbr = absbi;
    } else {
      absxk = absbi;
    }

    if (Ma > absxk) {
      if (absar < absbr) {
        absbi = Ma - absxk;
        y = (absar / 2.0F + absbr / 2.0F) / (Ma / 2.0F + absxk / 2.0F) * (absbr
          - absar);
      } else {
        absbi = Ma;
        y = absxk;
      }
    } else if (Ma < absxk) {
      if (absar > absbr) {
        y = absxk - Ma;
        absbi = (absar / 2.0F + absbr / 2.0F) / (Ma / 2.0F + absxk / 2.0F) *
          (absar - absbr);
      } else {
        absbi = Ma;
        y = absxk;
      }
    } else {
      absbi = absar;
      y = absbr;
    }

    absxk = std::abs(y / 2.0F);
    if ((!rtIsInfF(absxk)) && (!rtIsNaNF(absxk))) {
      if (absxk <= 1.17549435E-38F) {
        absxk = 1.4013E-45F;
      } else {
        std::frexp(absxk, &b_exponent);
        absxk = std::ldexp(1.0F, b_exponent - 24);
      }
    } else {
      absxk = ((real32_T)rtNaN);
    }

    if ((std::abs(y - absbi) < absxk) || (rtIsInfF(absbi) && rtIsInfF(y) &&
         ((absbi > 0.0F) == (y > 0.0F)))) {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      absbi = rt_atan2f_snf(a.im, a.re);
      y = rt_atan2f_snf(b.im, b.re);
      absxk = std::abs(y / 2.0F);
      if ((!rtIsInfF(absxk)) && (!rtIsNaNF(absxk))) {
        if (absxk <= 1.17549435E-38F) {
          absxk = 1.4013E-45F;
        } else {
          std::frexp(absxk, &c_exponent);
          absxk = std::ldexp(1.0F, c_exponent - 24);
        }
      } else {
        absxk = ((real32_T)rtNaN);
      }

      if ((std::abs(y - absbi) < absxk) || (rtIsInfF(absbi) && rtIsInfF(y) &&
           ((absbi > 0.0F) == (y > 0.0F)))) {
        p = true;
      } else {
        p = false;
      }

      if (p) {
        if (absbi > 0.78539816339744828) {
          if (absbi > 2.3561944901923448) {
            absbi = -a.im;
            y = -b.im;
          } else {
            absbi = -a.re;
            y = -b.re;
          }
        } else if (absbi > -0.78539816339744828) {
          absbi = a.im;
          y = b.im;
        } else if (absbi > -2.3561944901923448) {
          absbi = a.re;
          y = b.re;
        } else {
          absbi = -a.im;
          y = -b.im;
        }

        absxk = std::abs(y / 2.0F);
        if ((!rtIsInfF(absxk)) && (!rtIsNaNF(absxk))) {
          if (absxk <= 1.17549435E-38F) {
            absxk = 1.4013E-45F;
          } else {
            std::frexp(absxk, &d_exponent);
            absxk = std::ldexp(1.0F, d_exponent - 24);
          }
        } else {
          absxk = ((real32_T)rtNaN);
        }

        if ((std::abs(y - absbi) < absxk) || (rtIsInfF(absbi) && rtIsInfF(y) &&
             ((absbi > 0.0F) == (y > 0.0F)))) {
          p = true;
        } else {
          p = false;
        }

        if (p) {
          absbi = 0.0F;
          y = 0.0F;
        }
      }
    }
  }

  return absbi > y;
}

//
// File trailer for relop.cpp
//
// [EOF]
//
