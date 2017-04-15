//
// File: NLMS.cpp
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
// NLMS(Normalized Least Mean Squre)算法
//  输入参数:
//      xn   输入的信号序列      (行向量)
//      dn   所期望的响应序列    (行向量)
//      M    滤波器的阶数        (标量)
//      mu   收敛因子(步长)      (标量)     要求大于0,小于xn的相关矩阵最大特征值的倒数
//      itr  迭代次数            (标量)     默认为xn的长度,M<itr<length(xn)
//  输出参数:
//      W    滤波器的权值向量     (行向量)
//           大小为M
//      en   误差序列(itr x 1)    (行向量)
//      yn   实际输出序列         (行向量)
// Arguments    : const float xn[4000]
//                const float dn[4000]
//                float M
//                float mu
//                float itr
//                float yn_data[]
//                int yn_size[2]
//                emxArray_real32_T *W
//                float en_data[]
//                int en_size[1]
// Return Type  : void
//
void NLMS(const float xn[4000], const float dn[4000], float M, float mu, float
          itr, float yn_data[], int yn_size[2], emxArray_real32_T *W, float
          en_data[], int en_size[1])
{
  int loop_ub;
  int i4;
  int k;
  emxArray_real32_T *b;
  float b_k;
  float c;
  int b_W;
  int i5;
  int i6;
  int x_size_idx_1;
  int i7;
  float x_data[4001];

  //  参数个数必须为4个或5个
  //  5个时满足M<itr<length(xn)
  if ((itr > 4000.0F) || (itr < M)) {
    itr = 4000.0F;
  }

  //  初始化参数
  en_size[0] = (int)itr;
  loop_ub = (int)itr;
  for (i4 = 0; i4 < loop_ub; i4++) {
    en_data[i4] = 0.0F;
  }

  //  误差序列,en(k)表示第k次迭代时预期输出与实际输入的误差
  i4 = W->size[0] * W->size[1];
  W->size[0] = 1;
  W->size[1] = (int)M;
  emxEnsureCapacity((emxArray__common *)W, i4, sizeof(float));
  loop_ub = (int)M;
  for (i4 = 0; i4 < loop_ub; i4++) {
    W->data[i4] = 0.0F;
  }

  //  每一行代表一个加权参量,每一列代表-次迭代,初始为0
  yn_size[0] = 1;
  yn_size[1] = (int)itr;
  loop_ub = (int)itr;
  for (i4 = 0; i4 < loop_ub; i4++) {
    yn_data[i4] = 0.0F;
  }

  //  迭代计算
  i4 = (int)(itr + (1.0F - M));
  k = 0;
  emxInit_real32_T1(&b, 1);
  while (k <= i4 - 1) {
    b_k = M + (float)k;

    //  第k次迭代
    c = b_k - M;
    if (c + 1.0F > b_k) {
      b_W = 1;
      i5 = 1;
      i6 = 0;
    } else {
      b_W = (int)b_k;
      i5 = -1;
      i6 = (int)(c + 1.0F);
    }

    x_size_idx_1 = div_s32_floor(i6 - b_W, i5) + 1;
    loop_ub = div_s32_floor(i6 - b_W, i5);
    for (i7 = 0; i7 <= loop_ub; i7++) {
      x_data[i7] = xn[(b_W + i5 * i7) - 1];
    }

    //  滤波器M个抽头的输入
    i7 = b->size[0];
    b->size[0] = x_size_idx_1;
    emxEnsureCapacity((emxArray__common *)b, i7, sizeof(float));
    for (i7 = 0; i7 < x_size_idx_1; i7++) {
      b->data[i7] = x_data[i7];
    }

    if ((W->size[1] == 1) || (b->size[0] == 1)) {
      c = 0.0F;
      for (i7 = 0; i7 < W->size[1]; i7++) {
        c += W->data[W->size[0] * i7] * b->data[i7];
      }
    } else {
      c = 0.0F;
      for (i7 = 0; i7 < W->size[1]; i7++) {
        c += W->data[W->size[0] * i7] * b->data[i7];
      }
    }

    yn_data[(int)b_k - 1] = c;

    //  滤波器的输出
    en_data[(int)b_k - 1] = dn[(int)b_k - 1] - yn_data[(int)b_k - 1];

    //  第k次迭代的误差
    //  滤波器权值计算的迭代式
    c = mu * en_data[(int)b_k - 1];
    i7 = b->size[0];
    b->size[0] = x_size_idx_1;
    emxEnsureCapacity((emxArray__common *)b, i7, sizeof(float));
    for (i7 = 0; i7 < x_size_idx_1; i7++) {
      b->data[i7] = x_data[i7];
    }

    if ((div_s32_floor(i6 - b_W, i5) + 1 == 1) || (b->size[0] == 1)) {
      b_k = 0.0F;
      for (b_W = 0; b_W < x_size_idx_1; b_W++) {
        b_k += x_data[b_W] * b->data[b_W];
      }
    } else {
      b_k = 0.0F;
      for (b_W = 0; b_W < x_size_idx_1; b_W++) {
        b_k += x_data[b_W] * b->data[b_W];
      }
    }

    b_W = W->size[0] * W->size[1];
    W->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)W, b_W, sizeof(float));
    loop_ub = W->size[0];
    b_W = W->size[1];
    loop_ub *= b_W;
    for (b_W = 0; b_W < loop_ub; b_W++) {
      W->data[b_W] += c * x_data[b_W] / (1.0F + b_k);
    }

    k++;
  }

  emxFree_real32_T(&b);
}

//
// File trailer for NLMS.cpp
//
// [EOF]
//
