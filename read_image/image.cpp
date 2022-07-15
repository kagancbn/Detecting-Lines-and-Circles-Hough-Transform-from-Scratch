#include "image.h"
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/include/stb_image.h"
#include <iostream>
#include <iostream>
#define PI 3.141592


image load_image(const char* filename)
{
	int w, h, c;
	int channel = 3;
	//w = width, h = height, c = # 8 - bit components per pixel ...
	unsigned char* data = stbi_load(filename, &w, &h, &c, channel);    // without OpenCV
	
	if (!data) {
		exit(EXIT_FAILURE);
	}
	
	image out;
	out.data = data;
	out.h = h;
	out.w = w;
	out.c = c;
	return out;
}//load_image

image make_image(int w, int h, int c)
{
	image out;
	out.w = w;
	out.h = h;
	out.c = c;
	out.data = new unsigned char[w * h * c];
	return out;
}
image make_empty_image(int w, int h, int c)
{
	image out;
	out.w = w;
	out.h = h;
	out.c = c;
	return out;
}
image Rgb_to_gray(image input)
{
	image out = make_image(input.w, input.h, 1);

	for (int row = 0; row < out.h; row++)
		for (int col = 0; col < out.w; col++){
			out.data[row * out.w + col] = input.data[row * input.w * 3 + col * 3] * 0.29 + input.data[row * input.w * 3 + col * 3 + 1] * 0.58 + input.data[row * input.w * 3 + col * 3 + 2] * 0.11;
		}
	return out;
}
image Smoothing(image input) 
{
	int kernel_w = 3;
	int kernel_h = 3;
	int stride = 1;
	int padding = kernel_h / 2;

	int* kernel = new int[kernel_h * kernel_w];
	kernel[0] = 1; kernel[1] = 2; kernel[2] = 1;
	kernel[3] = 2; kernel[4] = 4; kernel[5] = 2;
	kernel[6] = 1; kernel[7] = 2; kernel[8] = 1;

	int out_w = (input.w - kernel_w + 2) / stride + 1;
	int out_h = (input.h - kernel_h + 2) / stride + 1;

	image out = make_image(out_w, out_h, 1);

	for (int k = padding; k < out.h - padding; k++)
		for (int l = padding; l < out.w - padding; l++){
			int sum = 0;
			for (int m = 0; m < kernel_h; m++)
				for (int n = 0; n < kernel_w; n++){
					
					sum += input.data[(k + m - padding) * input.w + (l + n - padding)] * kernel[m * kernel_w + n];
				}
			out.data[k * out.w + l] = sum / 16;
		}
	return out;
}
double* Gradient(image input, double*& angles)
{
	int kernel_w = 3;
	int kernel_h = 3;
	int padding = kernel_h / 2;

	int* KernelX;
	KernelX = new int[kernel_h * kernel_w];// horizontal direction sobel kernel (Gx)
	KernelX[0] = -1; KernelX[1] = 0; KernelX[2] = 1;
	KernelX[3] = -2; KernelX[4] = 0; KernelX[5] = 2;
	KernelX[6] = -1; KernelX[7] = 0; KernelX[8] = 1;

	int* KernelY = new int[kernel_h * kernel_w];// vertical direction sobel kernel(Gy)
	KernelY[0] = 1; KernelY[1] = 2; KernelY[2] = 1;
	KernelY[3] = 0; KernelY[4] = 0; KernelY[5] = 0;
	KernelY[6] = -1; KernelY[7] = -2; KernelY[8] = -1;

	double* Gradient = new double[input.w * input.h]; // Gx^2 + Gy^2 gradient array
	long int gradiaent_sum = 0;
	for (int k = padding; k < input.h-padding; k++)
		for (int l = padding; l < input.w - padding; l++) {
			double sumx = 0, sumy = 0;

			for (int m = 0; m < kernel_h; m++)
				for (int n = 0; n < kernel_w; n++) {

					sumx += input.data[(k + m - padding) * input.w + (l + n - padding)] * KernelX[m * kernel_w + n];
					sumy += input.data[(k + m - padding) * input.w + (l + n - padding)] * KernelY[m * kernel_w + n];
				}
			//Gradient calculation
			Gradient[k * input.w + l] = sqrt(pow(sumx, 2) + pow(sumy, 2)); // Gx^2 + Gy^2 gradient array
			Gradient[k * input.w + l] = Gradient[k * input.w + l] > 255 ? 255 : Gradient[k * input.w + l];
			gradiaent_sum += Gradient[k * input.w + l];
			//Angle calculation
			angles[k * input.w + l] = atan2(sumy, sumx) * 180 / PI; // arctan2(Gy,Gx)
		}
	return Gradient;
}
image CannyEdge(double* EdgeImage, int width, int height, double* angles) 
{	
	image out=make_image(width,height,1);

	//Non-max supression

	for (int k = 1; k < height - 1; k++)
		for (int l = 1; l < width - 1; l++) {
			
			int	q = 255, r = 255;
			if (-22.5 <= angles[k * width + l] < 22.5 || -157.5 <= angles[k * width + l] < 157.5) {

				q = EdgeImage[k * width + l - 1];
				r = EdgeImage[k * width + l + 1];
			}
			else if (22.5 <= angles[k * width + l] < 67.5 || -157.5 <= angles[k * width + l] < -112.5) {

				q = EdgeImage[k * (width - 1) + l + 1];
				r = EdgeImage[k * (width + 1) + l - 1];
			}
			else if (67.5 <= angles[k * width + l] < 112.5 || -112.5 <= angles[k * width + l] < -67.5) {

				q = EdgeImage[k * (width - 1) + l];
				r = EdgeImage[k * (width + 1) + l];
			}
			else if (-67.5 <= angles[k * width + l] < -22.5 || 112.5 <= angles[k * width + l] < 157.5) {

				q = EdgeImage[k * (width + 1) + l + 1];
				r = EdgeImage[k * (width - 1) + l - 1];
			}

			if (EdgeImage[k * width + l] >= q && EdgeImage[k * width + l] >= r){

				out.data[k * width + l] = EdgeImage[k * width + l];
			}
			else{
				out.data[k * width + l] = 0;
			}
		}

	//Double Thresholding
	int low_T = 20;
	int high_T = 80;
	
	int weak = 175;
	int strong = 255;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++){

			if (out.data[row * width + col] < low_T) {
				out.data[row * width + col] = 0;
			}
			else if (out.data[row * width + col] >= low_T && out.data[row * width + col] <= high_T){
				out.data[row * width + col] = weak;
			}
			else{
				out.data[row * width + col] = 255;
			}
		}

	//Hysteresis Tresholding
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++){
			if (out.data[row * width + col] == weak) { // check weak values is edge or not
				bool isEdge = false;
				for (int i = 0; i < 3; i++){
					for (int j = 0; j < 3; j++) {

						if (out.data[(row + i - 1) * width + col + j - 1] == strong) { // Checking 8-neighbor

							isEdge = true;// if it has a strong neighbor pixel it is an edge
							break;
						}
					}
					if (isEdge) break;
				}
				if (isEdge) {

					out.data[row * width + col] = 255; // it is edge
				}
				else{

					out.data[row * width + col] = 0; //it is not an edge, remove it from image
				}
			}
		}
	for (int i = 0; i <width ; i++){
		out.data[0 * width + i] = 0;
		out.data[1 * width + i] = 0;
		out.data[height - 1 * width + i] = 0;
		out.data[height - 2 * width + i] = 0;
	}
	for (int i = 2; i < height; i++){
		out.data[i * width + 0] = 0;
		out.data[i * width + 1] = 0;
		out.data[i * width - 1] = 0;
		out.data[i * width - 2] = 0;
	}

	return out;
}
/*
	d  =  x* cosθ + y * sinθ
	(row indice=y) x*cosθ - (col indice=y) y*sinθ = d (perpendicular distance from line to origin) (Θ = angle the perpendicular makes with the x-axis)

	Point in image space --> sinusoid segment in Hough space

	1- initialize	HoughSpace[d][theta]=0;

	2- for ( theta =min;  theta = max; theta++)
		d= x cosθ - ysinθ
		HoughSpace[d][theta]+=1;
	3- Find the value(s) of (d, Θ) where H[d, Θ] is maximum

	4- The detected line in the image is given by  d= x cosθ + ysinθ
	x cosθ = y sin
*/
int* HoughLine(image BinaryEdgeImage, int Width, int Height, double* angles, int& HoughWidth, int& HoughHeight) {
	HoughWidth = 180;
	HoughHeight = 2 * sqrt(pow(Width, 2) + pow(Height, 2));// max distance 2* for negatif values
	
	int* HoughSpace = new int[HoughWidth*HoughHeight]; // Accumulator array (votes)

	for (int row = 0; row < HoughHeight; row++)
		for (int col = 0; col < HoughWidth; col++){
			HoughSpace[row * HoughWidth + col] = 0;
		}
	for (int i = 0; i < Width; i++){// first and second row last and last-1 are 0
		BinaryEdgeImage.data[0 * Width + i] = 0;
		BinaryEdgeImage.data[1 * Width + i] = 0;
		BinaryEdgeImage.data[Height -1 * Width + i] = 0;
		BinaryEdgeImage.data[Height  -2* Width + i] = 0;
	}
	for (int i = 2; i < Height; i++){//right and right+1 left and lef+1 are 0
		BinaryEdgeImage.data[i * Width + 0] = 0;
		BinaryEdgeImage.data[i * Width + 1] = 0;
		BinaryEdgeImage.data[i * Width -1 ] = 0;
		BinaryEdgeImage.data[i * Width -2] = 0;
	}

	for (int row = 1; row < Height-1; row++)
		for (int col = 1; col < Width-1; col++){
			if (BinaryEdgeImage.data[row * Width + col] == 255) {
 				for (int theta = 0; theta < HoughWidth; theta++)
				{
					int distance = col * cos(theta *PI / 180) + row* sin(theta * PI / 180) + (HoughHeight / 2) ; // add HoughHeight / 2 --> include negatif values 
					HoughSpace[distance * HoughWidth + theta]++;
				}
			}
			
		}
	return HoughSpace;
}