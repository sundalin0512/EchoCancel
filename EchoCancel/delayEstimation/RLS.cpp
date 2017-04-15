//
// File: RLS.cpp
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
#include "delayEstimation_emxutil.h"
#include "delayEstimation_rtwutil.h"

// Function Definitions

//
// RLS(Least Mean Squre)算法
//  输入参数:
//      xn   输入的信号序列      (行向量)
//      dn   所期望的响应序列    (行向量)
//      M    滤波器的阶数        (标量)
//      itr  迭代次数            (标量)     默认为xn的长度,M<itr<length(xn)
//  输出参数:
//      W    滤波器的权值向量     (行向量)
//           大小为M
//      en   误差序列(itr x 1)    (行向量)
//      yn   实际输出序列         (行向量)
// Arguments    : const float xn[4000]
//                const float dn[4000]
//                float M
//                float itr
//                float yn_data[]
//                int yn_size[2]
//                emxArray_real32_T *W
//                float en_data[]
//                int en_size[1]
// Return Type  : void
//
void RLS(const float xn[4000], const float dn[4000], float M, float itr, float
         yn_data[], int yn_size[2], emxArray_real32_T *W, float en_data[], int
         en_size[1])
{
  int ar;
  int i8;
  emxArray_real32_T *p;
  int d;
  int k;
  int b_p;
  int n;
  emxArray_real32_T *b_k;
  emxArray_real32_T *c;
  emxArray_real32_T *b_c;
  emxArray_real32_T *c_c;
  emxArray_real32_T *d_c;
  float b_n;
  float e_c;
  int i9;
  int i10;
  int i11;
  int x_size_idx_0;
  int i12;
  float x_data[4001];
  unsigned int c_idx_0;
  int m;
  int br;
  int i13;
  int ic;
  int ib;
  int ia;
  unsigned int unnamed_idx_0;

  //  参数个数必须为4个或5个
  //  5个时满足M<itr<length(xn)
  if ((itr > 4000.0F) || (itr < M)) {
    itr = 4000.0F;
  }

  //  初始化参数
  en_size[0] = (int)itr;
  ar = (int)itr;
  for (i8 = 0; i8 < ar; i8++) {
    en_data[i8] = 0.0F;
  }

  //  误差序列,en(n)表示第k次迭代时预期输出与实际输入的误差
  i8 = W->size[0] * W->size[1];
  W->size[0] = 1;
  W->size[1] = (int)M;
  emxEnsureCapacity((emxArray__common *)W, i8, sizeof(float));
  ar = (int)M;
  for (i8 = 0; i8 < ar; i8++) {
    W->data[i8] = 0.0F;
  }

  //  每一行代表一个加权参量,每一列代表-次迭代,初始为0
  yn_size[0] = 1;
  yn_size[1] = (int)itr;
  ar = (int)itr;
  for (i8 = 0; i8 < ar; i8++) {
    yn_data[i8] = 0.0F;
  }

  emxInit_real32_T(&p, 2);
  d = (int)M;
  i8 = p->size[0] * p->size[1];
  p->size[0] = (int)M;
  p->size[1] = (int)M;
  emxEnsureCapacity((emxArray__common *)p, i8, sizeof(float));
  ar = (int)M * (int)M;
  for (i8 = 0; i8 < ar; i8++) {
    p->data[i8] = 0.0F;
  }

  if (d > 0) {
    for (k = 0; k + 1 <= d; k++) {
      p->data[k + p->size[0] * k] = 1.0F;
    }
  }

  i8 = p->size[0] * p->size[1];
  emxEnsureCapacity((emxArray__common *)p, i8, sizeof(float));
  d = p->size[0];
  b_p = p->size[1];
  ar = d * b_p;
  for (i8 = 0; i8 < ar; i8++) {
    p->data[i8] *= 0.1F;
  }

  //  迭代计算
  i8 = (int)(itr + (1.0F - M));
  n = 0;
  emxInit_real32_T1(&b_k, 1);
  emxInit_real32_T(&c, 2);
  emxInit_real32_T(&b_c, 2);
  emxInit_real32_T(&c_c, 2);
  emxInit_real32_T(&d_c, 2);
  while (n <= i8 - 1) {
    b_n = M + (float)n;

    //  第k次迭代
    e_c = b_n - M;
    if (e_c + 1.0F > b_n) {
      i9 = 1;
      i10 = 1;
      i11 = 0;
    } else {
      i9 = (int)b_n;
      i10 = -1;
      i11 = (int)(e_c + 1.0F);
    }

    x_size_idx_0 = div_s32_floor(i11 - i9, i10) + 1;
    ar = div_s32_floor(i11 - i9, i10);
    for (i12 = 0; i12 <= ar; i12++) {
      x_data[i12] = xn[(i9 + i10 * i12) - 1];
    }

    //  滤波器M个抽头的输入
    //      y = W(:,n-1)' * x';        % 滤波器的输出
    i12 = c->size[0] * c->size[1];
    c->size[0] = p->size[0];
    c->size[1] = p->size[1];
    emxEnsureCapacity((emxArray__common *)c, i12, sizeof(float));
    ar = p->size[0] * p->size[1];
    for (i12 = 0; i12 < ar; i12++) {
      c->data[i12] = 1.02040815F * p->data[i12];
    }

    if ((c->size[1] == 1) || (div_s32_floor(i11 - i9, i10) + 1 == 1)) {
      i12 = b_k->size[0];
      b_k->size[0] = c->size[0];
      emxEnsureCapacity((emxArray__common *)b_k, i12, sizeof(float));
      ar = c->size[0];
      for (i12 = 0; i12 < ar; i12++) {
        b_k->data[i12] = 0.0F;
        br = c->size[1];
        for (i13 = 0; i13 < br; i13++) {
          b_k->data[i12] += c->data[i12 + c->size[0] * i13] * x_data[i13];
        }
      }
    } else {
      k = c->size[1];
      c_idx_0 = (unsigned int)c->size[0];
      i12 = b_k->size[0];
      b_k->size[0] = (int)c_idx_0;
      emxEnsureCapacity((emxArray__common *)b_k, i12, sizeof(float));
      m = c->size[0];
      d = b_k->size[0];
      i12 = b_k->size[0];
      b_k->size[0] = d;
      emxEnsureCapacity((emxArray__common *)b_k, i12, sizeof(float));
      for (i12 = 0; i12 < d; i12++) {
        b_k->data[i12] = 0.0F;
      }

      if (c->size[0] != 0) {
        b_p = 0;
        while ((m > 0) && (b_p <= 0)) {
          for (ic = 1; ic <= m; ic++) {
            b_k->data[ic - 1] = 0.0F;
          }

          b_p = m;
        }

        br = 0;
        b_p = 0;
        while ((m > 0) && (b_p <= 0)) {
          ar = 0;
          i12 = br + k;
          for (ib = br; ib + 1 <= i12; ib++) {
            if (x_data[ib] != 0.0F) {
              ia = ar;
              for (ic = 0; ic + 1 <= m; ic++) {
                ia++;
                b_k->data[ic] += x_data[ib] * c->data[ia - 1];
              }
            }

            ar += m;
          }

          br += k;
          b_p = m;
        }
      }
    }

    i12 = b_c->size[0] * b_c->size[1];
    b_c->size[0] = 1;
    b_c->size[1] = x_size_idx_0;
    emxEnsureCapacity((emxArray__common *)b_c, i12, sizeof(float));
    for (i12 = 0; i12 < x_size_idx_0; i12++) {
      b_c->data[b_c->size[0] * i12] = 1.02040815F * x_data[i12];
    }

    if ((b_c->size[1] == 1) || (p->size[0] == 1)) {
      i12 = c_c->size[0] * c_c->size[1];
      c_c->size[0] = 1;
      c_c->size[1] = p->size[1];
      emxEnsureCapacity((emxArray__common *)c_c, i12, sizeof(float));
      ar = p->size[1];
      for (i12 = 0; i12 < ar; i12++) {
        c_c->data[c_c->size[0] * i12] = 0.0F;
        br = b_c->size[1];
        for (i13 = 0; i13 < br; i13++) {
          c_c->data[c_c->size[0] * i12] += b_c->data[b_c->size[0] * i13] *
            p->data[i13 + p->size[0] * i12];
        }
      }
    } else {
      k = b_c->size[1];
      c_idx_0 = (unsigned int)p->size[1];
      i12 = c_c->size[0] * c_c->size[1];
      c_c->size[0] = 1;
      c_c->size[1] = (int)c_idx_0;
      emxEnsureCapacity((emxArray__common *)c_c, i12, sizeof(float));
      d = p->size[1] - 1;
      i12 = c_c->size[0] * c_c->size[1];
      c_c->size[0] = 1;
      emxEnsureCapacity((emxArray__common *)c_c, i12, sizeof(float));
      ar = c_c->size[1];
      for (i12 = 0; i12 < ar; i12++) {
        c_c->data[c_c->size[0] * i12] = 0.0F;
      }

      if (p->size[1] != 0) {
        for (b_p = 1; b_p - 1 <= d; b_p++) {
          for (ic = b_p; ic <= b_p; ic++) {
            c_c->data[ic - 1] = 0.0F;
          }
        }

        br = 0;
        for (b_p = 0; b_p <= d; b_p++) {
          ar = 0;
          i12 = br + k;
          for (ib = br; ib + 1 <= i12; ib++) {
            if (p->data[ib] != 0.0F) {
              ia = ar;
              for (ic = b_p; ic + 1 <= b_p + 1; ic++) {
                ia++;
                c_c->data[ic] += p->data[ib] * b_c->data[ia - 1];
              }
            }

            ar++;
          }

          br += k;
        }
      }
    }

    if ((c_c->size[1] == 1) || (div_s32_floor(i11 - i9, i10) + 1 == 1)) {
      e_c = 0.0F;
      for (i12 = 0; i12 < c_c->size[1]; i12++) {
        e_c += c_c->data[c_c->size[0] * i12] * x_data[i12];
      }
    } else {
      e_c = 0.0F;
      for (i12 = 0; i12 < c_c->size[1]; i12++) {
        e_c += c_c->data[c_c->size[0] * i12] * x_data[i12];
      }
    }

    i12 = b_k->size[0];
    emxEnsureCapacity((emxArray__common *)b_k, i12, sizeof(float));
    ar = b_k->size[0];
    for (i12 = 0; i12 < ar; i12++) {
      b_k->data[i12] /= 1.0F + e_c;
    }

    if ((W->size[1] == 1) || (div_s32_floor(i11 - i9, i10) + 1 == 1)) {
      e_c = 0.0F;
      for (i12 = 0; i12 < W->size[1]; i12++) {
        e_c += W->data[W->size[0] * i12] * x_data[i12];
      }
    } else {
      e_c = 0.0F;
      for (i12 = 0; i12 < W->size[1]; i12++) {
        e_c += W->data[W->size[0] * i12] * x_data[i12];
      }
    }

    en_data[(int)b_n - 1] = dn[(int)b_n - 1] - e_c;
    e_c = en_data[(int)b_n - 1];
    i12 = W->size[0] * W->size[1];
    W->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)W, i12, sizeof(float));
    ar = W->size[1];
    for (i12 = 0; i12 < ar; i12++) {
      W->data[W->size[0] * i12] += b_k->data[i12] * e_c;
    }

    i12 = c->size[0] * c->size[1];
    c->size[0] = b_k->size[0];
    c->size[1] = x_size_idx_0;
    emxEnsureCapacity((emxArray__common *)c, i12, sizeof(float));
    ar = b_k->size[0];
    for (i12 = 0; i12 < ar; i12++) {
      for (i13 = 0; i13 < x_size_idx_0; i13++) {
        c->data[i12 + c->size[0] * i13] = 1.02040815F * b_k->data[i12] *
          x_data[i13];
      }
    }

    if ((c->size[1] == 1) || (p->size[0] == 1)) {
      i12 = d_c->size[0] * d_c->size[1];
      d_c->size[0] = c->size[0];
      d_c->size[1] = p->size[1];
      emxEnsureCapacity((emxArray__common *)d_c, i12, sizeof(float));
      ar = c->size[0];
      for (i12 = 0; i12 < ar; i12++) {
        br = p->size[1];
        for (i13 = 0; i13 < br; i13++) {
          d_c->data[i12 + d_c->size[0] * i13] = 0.0F;
          d = c->size[1];
          for (b_p = 0; b_p < d; b_p++) {
            d_c->data[i12 + d_c->size[0] * i13] += c->data[i12 + c->size[0] *
              b_p] * p->data[b_p + p->size[0] * i13];
          }
        }
      }
    } else {
      k = c->size[1];
      unnamed_idx_0 = (unsigned int)c->size[0];
      c_idx_0 = (unsigned int)p->size[1];
      i12 = d_c->size[0] * d_c->size[1];
      d_c->size[0] = (int)unnamed_idx_0;
      d_c->size[1] = (int)c_idx_0;
      emxEnsureCapacity((emxArray__common *)d_c, i12, sizeof(float));
      m = c->size[0];
      i12 = d_c->size[0] * d_c->size[1];
      emxEnsureCapacity((emxArray__common *)d_c, i12, sizeof(float));
      ar = d_c->size[1];
      for (i12 = 0; i12 < ar; i12++) {
        br = d_c->size[0];
        for (i13 = 0; i13 < br; i13++) {
          d_c->data[i13 + d_c->size[0] * i12] = 0.0F;
        }
      }

      if ((c->size[0] == 0) || (p->size[1] == 0)) {
      } else {
        d = c->size[0] * (p->size[1] - 1);
        b_p = 0;
        while ((m > 0) && (b_p <= d)) {
          i12 = b_p + m;
          for (ic = b_p; ic + 1 <= i12; ic++) {
            d_c->data[ic] = 0.0F;
          }

          b_p += m;
        }

        br = 0;
        b_p = 0;
        while ((m > 0) && (b_p <= d)) {
          ar = 0;
          i12 = br + k;
          for (ib = br; ib + 1 <= i12; ib++) {
            if (p->data[ib] != 0.0F) {
              ia = ar;
              i13 = b_p + m;
              for (ic = b_p; ic + 1 <= i13; ic++) {
                ia++;
                d_c->data[ic] += p->data[ib] * c->data[ia - 1];
              }
            }

            ar += m;
          }

          br += k;
          b_p += m;
        }
      }
    }

    i12 = p->size[0] * p->size[1];
    emxEnsureCapacity((emxArray__common *)p, i12, sizeof(float));
    d = p->size[0];
    b_p = p->size[1];
    ar = d * b_p;
    for (i12 = 0; i12 < ar; i12++) {
      p->data[i12] = 1.02040815F * p->data[i12] - d_c->data[i12];
    }

    if ((W->size[1] == 1) || (div_s32_floor(i11 - i9, i10) + 1 == 1)) {
      e_c = 0.0F;
      for (i9 = 0; i9 < W->size[1]; i9++) {
        e_c += W->data[W->size[0] * i9] * x_data[i9];
      }
    } else {
      e_c = 0.0F;
      for (i9 = 0; i9 < W->size[1]; i9++) {
        e_c += W->data[W->size[0] * i9] * x_data[i9];
      }
    }

    yn_data[(int)b_n - 1] = e_c;
    n++;
  }

  emxFree_real32_T(&d_c);
  emxFree_real32_T(&c_c);
  emxFree_real32_T(&b_c);
  emxFree_real32_T(&c);
  emxFree_real32_T(&b_k);
  emxFree_real32_T(&p);
}

//
// File trailer for RLS.cpp
//
// [EOF]
//
