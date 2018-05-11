
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <iomanip>

using namespace std;

// Here we assume the image is of size 256*256 and is of raw format
// You will need to make corresponding changes to accommodate images of different sizes and types


//for Walter
#define Size 16
#define BytesPerPixel 1
#define PI 3.141592653

int Q50[8][8] = {
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
};


double calC( int x,int y, int u, int v ){
	return cos((double)((2 * x + 1)* u)* PI / 16)*cos((double)((2 * y + 1)*v)*PI / 16);
}
int main(int argc, char *argv[])

{
	// file pointer
	FILE *file;
	//if (argc < 3) {
	//	cout << "usage: program_name input_image.raw output_image.raw " << endl;
	//	return 0;
	//}
	//if (argc < 4) {
	//	BytesPerPixel = 1; 	// 1: gray image, 3: color image
	//}
	//else {
	//	BytesPerPixel = atoi(argv[3]);
	//}

	bool debug = true;
	// image data array
	unsigned char Imagedata[Size][Size];

	// read image "ride.raw" into image data matrix
	if (!(file = fopen(argv[1], "rb")))
	{
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	

	// do some image processing task...

	int imageshiftData[16][16];
	//shift the magnitude
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			imageshiftData[i][j] = Imagedata[i][j] - 128;
		}
	}

	//=====================generate 4 blocks===========================================//
	int block[8][8][4];
	for (int rindex = 0; rindex < 2; rindex++) {
		for (int cindex = 0; cindex < 2; cindex++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					block[i][j][rindex*2 + cindex] = imageshiftData[i + rindex * 8][j + cindex * 8];
				}
			}
		}
	}

	//test the 4 blocks

	if (debug = true) {
		cout << "the blocks:" << endl;
		for (int layer = 0; layer < 4; layer++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << block[i][j][layer] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	//==================================================================================//
	//===================================DCT============================================//
	double DCTcoeff[8][8][4];
	double sum = 0;
	for (int bindex = 0; bindex < 4; bindex++) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						sum += block[x][y][bindex] * calC(x, y, i, j);
					}
				}
				double a;
				i > 0 ? a = 1 : a = 1 / sqrt(2);
				double b;
				j > 0 ? b = 1 : b = 1 / sqrt(2);
				DCTcoeff[i][j][bindex] = 0.25 * a * b * sum;
				sum = 0;
			}
		}
	}

	//test dct
	if (debug = true) {
		cout << "the dct coefficients:" << endl;
		for (int layer = 0; layer < 4; layer++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << setw(10) <<DCTcoeff[i][j][layer] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	//================================================================================//

	//=================================quantize=======================================//
	int DctQ50[8][8][4] = { 0 };
	int DctQ90[8][8][4] = { 0 };
	int DctQ10[8][8][4] = { 0 };
	for (int layer = 0; layer < 4; layer++) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				DctQ50[i][j][layer] = round(DCTcoeff[i][j][layer]/Q50[i][j]) ;
				DctQ90[i][j][layer] = round(DCTcoeff[i][j][layer] / ((double)Q50[i][j]*0.2));
				DctQ10[i][j][layer] = round(DCTcoeff[i][j][layer] / ((double)Q50[i][j]*5));
			}
		}
	}
	
	if (debug = true) {
		cout << "the dct coefficients after Q50:" << endl;
		for (int layer = 0; layer < 4; layer++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << setw(5) << DctQ50[i][j][layer] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}

	if (debug = true) {
		cout << "the dct coefficients after Q10:" << endl;
		for (int layer = 0; layer < 4; layer++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << setw(5) << DctQ10[i][j][layer] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	if (debug = true) {
		cout << "the dct coefficients after Q90:" << endl;
		for (int layer = 0; layer < 4; layer++) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << setw(5) << DctQ90[i][j][layer] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}


	//=================================================================================//


	// write image data to "~.raw"

	//if (!(file = fopen(argv[2], "wb")))
	//{
	//	cout << "Cannot open file: " << argv[2] << endl;
	//	exit(1);
	//}
	//fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	//fclose(file);

	return 0;
}









