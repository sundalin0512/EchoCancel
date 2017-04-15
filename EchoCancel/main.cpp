#include <stdio.h>

using namespace std;
const int audioLength10ms = 4410;

int main()
{


	FILE *fp_far = fopen("noise.wav", "rb");
	FILE *fp_near = fopen("mix.wav", "rb");
	FILE *fp_out = fopen("out.wav", "wb");

	unsigned short far_frame[audioLength10ms];
	unsigned short near_frame[audioLength10ms];
	unsigned short out_frame[audioLength10ms];

	char* data;
	data = new char[44];
	fread(far_frame, sizeof(char), 44, fp_far);
	fwrite(far_frame, sizeof(char), 44, fp_out);
	fread(near_frame, sizeof(char), 44, fp_far);

	fread(far_frame, sizeof(short), audioLength10ms, fp_far);
	fread(near_frame, sizeof(short), audioLength10ms, fp_near);

}