//
// File: LMS.cpp
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
// LMS(Least Mean Squre)�㷨
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
// Arguments    : const emxArray_real32_T *xn
//                const emxArray_real32_T *dn
//                float M
//                float mu
//                float itr
//                emxArray_real32_T *b_yn
//                emxArray_real32_T *W
//                emxArray_real32_T *en
// Return Type  : void
//
void LMS(const emxArray_real32_T *xn, const emxArray_real32_T *dn, float M,
	float mu, float itr, emxArray_real32_T *b_yn, emxArray_real32_T *W,
	emxArray_real32_T *en)
{
	int i0;
	int loop_ub;
	int k;
	emxArray_real32_T *x;
	emxArray_real32_T *b;
	float b_k;
	float c;
	int i1;
	int i2;
	int i3;
	int b_W;

	//  ������������Ϊ4����5��
	//  5��ʱ����M<itr<length(xn)
	if ((itr > xn->size[1]) || (itr < M)) {
		itr = (float)xn->size[1];
	}

	//  ��ʼ������
	i0 = en->size[0];
	en->size[0] = (int)itr;
	emxEnsureCapacity((emxArray__common *)en, i0, sizeof(float));
	loop_ub = (int)itr;
	for (i0 = 0; i0 < loop_ub; i0++) {
		en->data[i0] = 0.0F;
	}

	//  �������,en(k)��ʾ��k�ε���ʱԤ�������ʵ����������
	i0 = W->size[0] * W->size[1];
	W->size[0] = 1;
	W->size[1] = (int)M;
	emxEnsureCapacity((emxArray__common *)W, i0, sizeof(float));
	loop_ub = (int)M;
	for (i0 = 0; i0 < loop_ub; i0++) {
		W->data[i0] = 0.0F;
	}

	//  ÿһ�д���һ����Ȩ����,ÿһ�д���-�ε���,��ʼΪ0
	i0 = b_yn->size[0] * b_yn->size[1];
	b_yn->size[0] = 1;
	b_yn->size[1] = (int)(float)xn->size[1];
	emxEnsureCapacity((emxArray__common *)b_yn, i0, sizeof(float));
	loop_ub = (int)(float)xn->size[1];
	for (i0 = 0; i0 < loop_ub; i0++) {
		b_yn->data[i0] = 0.0F;
	}

	//  ��������
	i0 = (int)(itr + (1.0F - M));
	k = 0;
	emxInit_real32_T(&x, 2);
	emxInit_real32_T1(&b, 1);
	while (k <= i0 - 1) {
		b_k = M + (float)k;

		//  ��k�ε���
		c = b_k - M;
		if (c + 1.0F > b_k) {
			i1 = 1;
			i2 = 1;
			i3 = 0;
		}
		else {
			i1 = (int)b_k;
			i2 = -1;
			i3 = (int)(c + 1.0F);
		}

		b_W = x->size[0] * x->size[1];
		x->size[0] = 1;
		x->size[1] = div_s32_floor(i3 - i1, i2) + 1;
		emxEnsureCapacity((emxArray__common *)x, b_W, sizeof(float));
		loop_ub = div_s32_floor(i3 - i1, i2);
		for (i3 = 0; i3 <= loop_ub; i3++) {
			x->data[x->size[0] * i3] = xn->data[(i1 + i2 * i3) - 1];
		}

		//  �˲���M����ͷ������
		//      y = W(:,k-1)' * x';        % �˲��������
		i1 = b->size[0];
		b->size[0] = x->size[1];
		emxEnsureCapacity((emxArray__common *)b, i1, sizeof(float));
		loop_ub = x->size[1];
		for (i1 = 0; i1 < loop_ub; i1++) {
			b->data[i1] = x->data[x->size[0] * i1];
		}

		if ((W->size[1] == 1) || (b->size[0] == 1)) {
			c = 0.0F;
			for (i1 = 0; i1 < W->size[1]; i1++) {
				c += W->data[W->size[0] * i1] * b->data[i1];
			}
		}
		else {
			c = 0.0F;
			for (i1 = 0; i1 < W->size[1]; i1++) {
				c += W->data[W->size[0] * i1] * b->data[i1];
			}
		}

		b_yn->data[(int)b_k - 1] = c;

		//      en(k) = dn(k) - y ;        % ��k�ε��������
		en->data[(int)b_k - 1] = dn->data[(int)b_k - 1] - b_yn->data[(int)b_k - 1];

		//  �˲���Ȩֵ����ĵ���ʽ
		c = 2.0F * mu * en->data[(int)b_k - 1];
		i1 = W->size[0] * W->size[1];
		W->size[0] = 1;
		emxEnsureCapacity((emxArray__common *)W, i1, sizeof(float));
		loop_ub = W->size[0];
		b_W = W->size[1];
		loop_ub *= b_W;
		for (i1 = 0; i1 < loop_ub; i1++) {
			W->data[i1] += c * x->data[i1];
		}

		k++;
	}

	emxFree_real32_T(&b);
	emxFree_real32_T(&x);
}

//
// File trailer for LMS.cpp
//
// [EOF]
//
