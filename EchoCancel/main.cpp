#include <stdio.h>
#include "delayEstimation\LMS.h"
#include "delayEstimation\NLMS.h"
#include "delayEstimation\RLS.h"
#include "delayEstimation\delayEstimation.h"
#include "delayEstimation/delayEstimation_initialize.h"
#include "delayEstimation/delayEstimation_terminate.h"
#include "delayEstimation/delayEstimation_emxAPI.h"
//#include "delayEstimation\delayEstimation_terminate.h"
//#include "delayEstimation\delayEstimation_emxAPI.h"
//#include "delayEstimation\delayEstimation_initialize.h"

using namespace std;
const int audioLength10ms = 4410;

int main()
{

	delayEstimation_initialize();
	FILE *fp_far = fopen("farEndSound.wav", "rb");
	FILE *fp_near = fopen("mix.wav", "rb");
	FILE *fp_out = fopen("out.wav", "wb");

	short *far_frame;
	short *near_frame;
	short *out_frame;

	far_frame = new short[44];
	near_frame = new short[44];
	out_frame = new short[44];

	char* data;
	data = new char[44];
	fread(far_frame, sizeof(char), 44, fp_far);
	long fileBegin = ftell(fp_far);
	fwrite(far_frame, sizeof(char), 44, fp_out);
	fread(near_frame, sizeof(char), 44, fp_far);

	delete[]far_frame;
	delete[]near_frame;
	delete[]out_frame;

	fseek(fp_far, 0, SEEK_END);
	long fileEnd = ftell(fp_far);

	int fileLength = fileEnd - fileBegin;
	far_frame = new short[fileLength];
	near_frame = new short[fileLength];
	out_frame = new short[fileLength];

	fread(far_frame, sizeof(short), fileLength, fp_far);
	fread(near_frame, sizeof(short), fileLength, fp_near);

	float *farEnd_f = new float[fileLength];
	for (int i = 0; i < fileLength; i++)
	{
		farEnd_f[i] = far_frame[i] / float(65536 / 2);
	}

	emxArray_real32_T  *farEnd;
	farEnd = emxCreateWrapper_real32_T(farEnd_f, fileLength, 1);
	

	float *nearEnd_f = new float[fileLength];
	for (int i = 0; i < fileLength; i++)
	{
		nearEnd_f[i] = near_frame[i] / float(65536 / 2);
	}

	emxArray_real32_T  *nearEnd;
	nearEnd = emxCreateWrapper_real32_T(nearEnd_f, fileLength, 1);

	int delay = delayEstimation(farEnd, nearEnd);








	delayEstimation_terminate();

}