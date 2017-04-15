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
// LMS(Least Mean Squre)算法
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

	//  参数个数必须为4个或5个
	//  5个时满足M<itr<length(xn)
	if ((itr > xn->size[1]) || (itr < M)) {
		itr = (float)xn->size[1];
	}

	//  初始化参数
	i0 = en->size[0];
	en->size[0] = (int)itr;
	emxEnsureCapacity((emxArray__common *)en, i0, sizeof(float));
	loop_ub = (int)itr;
	for (i0 = 0; i0 < loop_ub; i0++) {
		en->data[i0] = 0.0F;
	}

	//  误差序列,en(k)表示第k次迭代时预期输出与实际输入的误差
	i0 = W->size[0] * W->size[1];
	W->size[0] = 1;
	W->size[1] = (int)M;
	emxEnsureCapacity((emxArray__common *)W, i0, sizeof(float));
	loop_ub = (int)M;
	for (i0 = 0; i0 < loop_ub; i0++) {
		W->data[i0] = 0.0F;
	}

	//  每一行代表一个加权参量,每一列代表-次迭代,初始为0
	i0 = b_yn->size[0] * b_yn->size[1];
	b_yn->size[0] = 1;
	b_yn->size[1] = (int)(float)xn->size[1];
	emxEnsureCapacity((emxArray__common *)b_yn, i0, sizeof(float));
	loop_ub = (int)(float)xn->size[1];
	for (i0 = 0; i0 < loop_ub; i0++) {
		b_yn->data[i0] = 0.0F;
	}

	//  迭代计算
	i0 = (int)(itr + (1.0F - M));
	k = 0;
	emxInit_real32_T(&x, 2);
	emxInit_real32_T1(&b, 1);
	while (k <= i0 - 1) {
		b_k = M + (float)k;

		//  第k次迭代
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

		//  滤波器M个抽头的输入
		//      y = W(:,k-1)' * x';        % 滤波器的输出
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

		//      en(k) = dn(k) - y ;        % 第k次迭代的误差
		en->data[(int)b_k - 1] = dn->data[(int)b_k - 1] - b_yn->data[(int)b_k - 1];

		//  滤波器权值计算的迭代式
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
