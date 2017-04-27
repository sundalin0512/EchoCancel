#include <stdio.h>
#include "delayEstimation/delayEstimation.h"
#include "delayEstimation/delayEstimation_emxAPI.h"


using namespace std;
int audioLength1s = 4410;

int main()
{

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
	fread(near_frame, sizeof(char), 44, fp_near);

	delete[]far_frame;
	delete[]near_frame;
	delete[]out_frame;

	fseek(fp_far, 0, SEEK_END);
	long fileEnd = ftell(fp_far);


	fseek(fp_far, 44, SEEK_SET);
	fseek(fp_near, 44, SEEK_SET);
	int fileLength = (fileEnd - fileBegin)/2;
	//audioLength1s = fileLength;
	far_frame = new short[audioLength1s];
	near_frame = new short[audioLength1s];
	out_frame = new short[audioLength1s];

	fread(far_frame, sizeof(short), audioLength1s, fp_far);
	fread(near_frame, sizeof(short), audioLength1s, fp_near);

	float *farEnd_f = new float[audioLength1s];
	for (int i = 0; i < audioLength1s; i++)
	{
		farEnd_f[i] = far_frame[i] / float(65536 / 2);
	}

	emxArray_real32_T  *farEnd;
	farEnd = emxCreateWrapper_real32_T(farEnd_f, audioLength1s, 1);
	

	float *nearEnd_f = new float[audioLength1s];
	for (int i = 0; i < audioLength1s; i++)
	{
		nearEnd_f[i] = near_frame[i] / float(65536 / 2);
	}

	emxArray_real32_T  *nearEnd;
	nearEnd = emxCreateWrapper_real32_T(nearEnd_f, audioLength1s, 1);

	int delay = delayEstimation(farEnd, nearEnd);


	float *echo_f = new float[audioLength1s];





	return 0;

}