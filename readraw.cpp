// The programe reads the image data from an image file "~.raw"
// Last updated on 02/20/2010 by Steve Cho

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// Here we assume the image is of size 256*256 and is of raw format
// You will need to make corresponding changes to accommodate images of different sizes and types

#define Size 256


int main(int argc, char *argv[])

{
	// file pointer
	FILE *file;
	int BytesPerPixel;

	if (argc < 3) {
		cout << "usage: program_name input_image.raw output_image.raw [BytesPerPixel]" << endl;
		return 0;
	}
	if (argc < 4) {
		BytesPerPixel = 1; 	// 1: gray image, 3: color image
	}
	else {
		BytesPerPixel = atoi(argv[3]);
	}


	// image data array
	unsigned char Imagedata[Size][Size][channel];


	// read image "ride.raw" into image data matrix
	if (!(file = fopen(argv[1], "rb")))
	{
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);



	// do some image processing task...





	// write image data to "~.raw"

	if (!(file = fopen(argv[2], "wb")))
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}









