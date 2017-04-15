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
// NLMS(Normalized Least Mean Squre)�㷨
//  �������:
//      xn   ������ź�����      (������)
//      dn   ����������Ӧ����    (������)
//      M    �˲����Ľ���        (����)
//      mu   ��������(����)      (����)     Ҫ�����0,С��xn����ؾ����������ֵ�ĵ���
//      itr  ��������            (����)     Ĭ��Ϊxn�ĳ���,M<itr<length(xn)
//  �������:
//      W    �˲�����Ȩֵ����     (������)
//           ��СΪM
//      en   �������(itr x 1)    (������)
//      yn   ʵ���������         (������)
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

  //  ������������Ϊ4����5��
  //  5��ʱ����M<itr<length(xn)
  if ((itr > 4000.0F) || (itr < M)) {
    itr = 4000.0F;
  }

  //  ��ʼ������
  en_size[0] = (int)itr;
  loop_ub = (int)itr;
  for (i4 = 0; i4 < loop_ub; i4++) {
    en_data[i4] = 0.0F;
  }

  //  �������,en(k)��ʾ��k�ε���ʱԤ�������ʵ����������
  i4 = W->size[0] * W->size[1];
  W->size[0] = 1;
  W->size[1] = (int)M;
  emxEnsureCapacity((emxArray__common *)W, i4, sizeof(float));
  loop_ub = (int)M;
  for (i4 = 0; i4 < loop_ub; i4++) {
    W->data[i4] = 0.0F;
  }

  //  ÿһ�д���һ����Ȩ����,ÿһ�д���-�ε���,��ʼΪ0
  yn_size[0] = 1;
  yn_size[1] = (int)itr;
  loop_ub = (int)itr;
  for (i4 = 0; i4 < loop_ub; i4++) {
    yn_data[i4] = 0.0F;
  }

  //  ��������
  i4 = (int)(itr + (1.0F - M));
  k = 0;
  emxInit_real32_T1(&b, 1);
  while (k <= i4 - 1) {
    b_k = M + (float)k;

    //  ��k�ε���
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

    //  �˲���M����ͷ������
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

    //  �˲��������
    en_data[(int)b_k - 1] = dn[(int)b_k - 1] - yn_data[(int)b_k - 1];

    //  ��k�ε��������
    //  �˲���Ȩֵ����ĵ���ʽ
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
