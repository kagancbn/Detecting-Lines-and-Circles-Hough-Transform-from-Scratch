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
image RgbToGray(image input)
{
	image out = make_image(input.w, input.h, 1);

	for (int row = 0; row < out.h; row++)
		for (int col = 0; col < out.w; col++){
			out.data[row * out.w + col] = input.data[row * input.w * 3 + col * 3] * 0.29 + input.data[row * input.w * 3 + col * 3 + 1] * 0.58 + input.data[row * input.w * 3 + col * 3 + 2] * 0.11;
		}
	return out;
}
image Smoothing(image input,int kernelSize) 
{	
	int out_w = (input.w - kernelSize + 1);
	int out_h = (input.h - kernelSize + 1);

	image out = make_image(out_w, out_h, 1);

	int kernel3x[9] = { 1,2,1,
						2,4,2,
						1,2,1 };
	
	int kernel5x[25] = { 1,4,7,4,1,
						4,16,26,16,4,
						7,26,41,26,7,
						4,16,26,16,4,
						1,4,7,4,1 };
	int* kernel;
	int div;
	switch (kernelSize)
	{
	case 5:
		kernel = kernel5x;
		div = 273;
		break;
	default:
		kernel = kernel3x;
		div = 16;
		break;
	}

	for (int row = 0; row < out.h; row++)
		for (int col = 0; col < out.w; col++){
			int sum = 0;
			for (int krow = 0; krow < kernelSize; krow++)
				for (int kcol = 0; kcol < kernelSize; kcol++){
					
					sum += input.data[(row + krow ) * input.w + (col + kcol )] * kernel[krow * kernelSize + kcol];
				}
			out.data[row * out.w + col] = (int) sum / div;
		}
	return out;
}

int directionQuantize(double direction)
{
	direction = fmod(direction , 180);

	if ( direction < 22.5 || direction > 157.5 )
		return 0;
	else if (67.5 > direction)
		return 45;
	else if (112.5 > direction)
		return 90;
	else
		return 135;
}

gradient Gradient(image input, gradient grad)
{
	int KernelX[9] = { -1,0,1,-2,0,2,-1,0,1 };
	// horizontal direction sobel kernel (Gx)

	int KernelY[9] = { 1,2,1,0,0,0,-1,-2,-1 };
	// vertical direction sobel kernel(Gy)

	int kernelSize = 3;
	int padding = 1;

	grad.w = input.w - 2;
	grad.h = input.h - 2;
	grad.magnititude = new double[grad.h * grad.w];				// G|x| + G|y| gradient array
	grad.direction = new double[grad.h * grad.w];				// G|x| + G|y| gradient orientation array
	grad.quantizeDir = new int[grad.h * grad.w];				// G|x| + G|y| gradient orientation array

	for (int i = 0; i < grad.h * grad.w; i++)
	{
		grad.magnititude[i] = 0;
		grad.quantizeDir[i] = 0;
		grad.direction[i] = 0;
	}


	for (int row = 0; row < grad.h; row++)
		for (int col = 0; col < grad.w; col++) {

			double sumx = 0, sumy = 0;
			double direction = 0;

			for (int m = 0; m < kernelSize; m++)
				for (int n = 0; n < kernelSize; n++) {

					sumx += input.data[(row + m) * input.w + (col + n)] * KernelX[m * kernelSize + n];
					sumy += input.data[(row + m) * input.w + (col + n)] * KernelY[m * kernelSize + n];
				}

			//Gradient magnititude calculation

			grad.magnititude[row * grad.w + col] = abs(sumx) + abs(sumy);				// G|x| + G|y| gradient magnititude array

			//Angle calculation

			direction = atan2(sumy, sumx) * 180 / PI;						 // arctan2(Gy,Gx);
			direction = fmod(direction + 360, 360);							// convert angle to [0-360]

			grad.direction[row * grad.w + col] = direction;

			grad.quantizeDir[row * grad.w + col] = directionQuantize(direction);					// convert angle to [0, 45, 90, 135]
		}
	return grad;
}

void hysteresisThreshold(int pos, int width, int height, int* data)
{
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		{
			if(pos + i * width + j > 0 && pos + i * width + j < width * height )
				if(data[pos + i * width + j ] == 175 )
				{
					data[pos + i * width + j] = 255;
					hysteresisThreshold(pos + i * width + j, width, height, data);
				}
		}
}

image CannyEdge(gradient grad)
{	
	int width = grad.w - 2;
	int height = grad.h - 2;
	//Non-max supression

	int* nonmax = new int[width*height];
	for (int i = 0; i < width*height; i++)
		nonmax[i] = 0;

	for (int row = 1; row < grad.h - 1; row++)
		for (int col = 1; col < grad.w - 1; col++) {
			int	q = 255, r = 255;
			switch (grad.quantizeDir[row * width + col])
			{
			case 0:
				q = grad.magnititude[row * width + col - 1];					// left-pixel
				r = grad.magnititude[row * width + col + 1];					//right-pixel
				break;
			case 45:
				q = grad.magnititude[row * (width + 1) + col - 1];
				r = grad.magnititude[row * (width - 1) + col + 1];
				break;
			case 90 :
				q = grad.magnititude[row * (width + 1) + col];						// up- pixel
				r = grad.magnititude[row * (width - 1) + col];						// bottom- pixel
				break;
			case 135:
				q = grad.magnititude[row * (width - 1) + col - 1];
				r = grad.magnititude[row * (width + 1) + col + 1];
				break;
			}

			if (grad.magnititude[row * grad.w + col] >= q && grad.magnititude[row * grad.w + col] >= r){ // büyük mü büyük eşitmi??

				nonmax[(row-1)* width + (col-1)] = (int)grad.magnititude[row * grad.w + col];
			}
		}

	//Double Thresholding
	int max = 0;
	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			if (nonmax[row * width + col] > max)
			{
				max = nonmax[row * width + col];
			}
		}
	
	int high_T = 0.3 * max;
	int low_T = 0.1 * max;

	
	int weak = 175;
	int strong = 255;
	
	int strongCount = 0;
	int weakCount = 0;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++){

			if (nonmax[row * width + col] <= low_T) {
				nonmax[row * width + col] = 0;
			}
			else if (nonmax[row * width + col] >= high_T){
				nonmax[row * width + col] = strong;
				strongCount++;
			}
			else{
				nonmax[row * width + col] = weak;
				weakCount++;
			}
		}
	/*image out = make_image(width, height, 1);
	for (int i = 0; i < width * height; i++)
	{
		out.data[i] = nonmax[i];
	}
	return out;*/

	//Hysteresis Tresholding

	int* strongEdgePos = new int[strongCount];
	int* weakEdgePos = new int[weakCount];
	int strongEdgeIndex = 0;
	int weakEdgeIndex = 0;
	
	for (int row = 0; row < height ; row++)
		for (int col = 0; col < width; col++) {
			int pos = row * width + col;
			if (nonmax[row * width + col] == 255)
			{
				strongEdgePos[strongEdgeIndex] = pos;
				strongEdgeIndex++;
			}
			if (nonmax[row * width + col] == 175)
			{
				weakEdgePos[weakEdgeIndex] = pos;
				weakEdgeIndex++;
			}
		}

	for (int i = 0; i < strongCount; i++)
	{
		int pos = strongEdgePos[i];
		hysteresisThreshold(pos, width, height, nonmax);
	}

	for (int i = 0; i < weakCount; i++)
	{
		int pos = weakEdgePos[i];
		if (nonmax[pos] == 175)
			nonmax[pos] = 0;
	}

	int HysWidth = width - 2;			//width ve height nonmax genisligi oluyor
	int HysHeight = height - 2;

	int* Hysteresis = new int[HysWidth * HysHeight];
	for (int i = 0; i < HysHeight * HysWidth; i++)
	{
		Hysteresis[i] = 0;
	}

	for (int row = 1; row < height - 1 ; row++)
		for (int col = 1; col < width - 1; col++)
		{
			Hysteresis[(row - 1) * HysWidth + (col - 1)] = nonmax[row * width + col];
		}

	image out = make_image(HysWidth,HysHeight,1);
	for (int i = 0; i < HysWidth*HysHeight; i++)
	{
		out.data[i] = Hysteresis[i];
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


hough HoughLine(image BinaryEdgeImage, hough HoughSpace) {

	HoughSpace.w = 360;
	HoughSpace.h = 2 * abs(BinaryEdgeImage.w) + abs(BinaryEdgeImage.h);// max distance 2* for negatif values
	
	HoughSpace.data = new int[HoughSpace.w*HoughSpace.h]; // Accumulator array (votes)

	for (int i = 0; i< HoughSpace.w * HoughSpace.h; i++)
		HoughSpace.data[i] = 0;

	for (int row = 1; row < BinaryEdgeImage.h; row++)
		for (int col = 1; col < BinaryEdgeImage.w; col++) {

			if (BinaryEdgeImage.data[row * BinaryEdgeImage.w + col] == 255) {

 				for (int theta = 0; theta < HoughSpace.w; theta++)
				{
					int distance = col * cos(theta *PI / 180) + row* sin(theta * PI / 180) + (HoughSpace.h / 2) ; // add HoughHeight / 2 --> include negatif values 
					HoughSpace.data[distance * HoughSpace.w + theta]++;
				}
			}
			
		}
	return HoughSpace;
}


bool isExist(int* distance, int* theta, int d, int t, int numberOfLines,int width, int height)
{

	int distQuantize = height/30;
	int thetaQuantize = width/20;

	for (int i = 0; i < numberOfLines; i++)
	{
		if(distance [i] != -1 && theta[i] != -1 )
			if ((d - distQuantize <= distance[i] && distance[i] <= d + distQuantize) && (t - thetaQuantize <= theta[i] && theta[i] <= t + thetaQuantize))
				return true;
	}
	return false;
}
bool isExist(int* distance, int* theta, int* radius, int d, int t, int r, int numberOfLines)
{
	int distQuantize = 15;
	int thetaQuantize = 15;
	int radiusQuant = 4;
	for (int i = 0; i < numberOfLines; i++)
	{
		if (distance[i] != -1 && theta[i] != -1)
			if ((d - distQuantize <= distance[i] && distance[i] <= d + distQuantize) && (t - thetaQuantize <= theta[i] && theta[i] <= t + thetaQuantize) && (r - radiusQuant<= radius[i] && radius[i] <= r + radiusQuant))
				return true;
	}
	return false;
}


void DrawSelectedLines(hough HoughSpace, image im, int number_of_lines) {

	int* max = new int[number_of_lines];				//Max voted points
	int* distance = new int[number_of_lines];			//Distance parameter
	int* theta = new int[number_of_lines];				//Theta parameter

	int minvote = 90 + im.w * im.h / 8000;

	for (int i = 0; i < number_of_lines; i++)
		max[i] = minvote, distance[i] = -1, theta[i] = -1;

	for (int row = 0; row < HoughSpace.h; row++)// distance indice d
		for (int col = 0; col < HoughSpace.w; col++) {//Theta indice 

			if (HoughSpace.data[row * HoughSpace.w + col] > max[number_of_lines - 1] && !isExist(distance, theta, row, col, number_of_lines, HoughSpace.w, HoughSpace.h)) {

				for (int i = 1; i < number_of_lines; i++) {// 
					max[i - 1] = max[i];
					distance[i - 1] = distance[i];
					theta[i - 1] = theta[i];
				}

				max[number_of_lines - 1] = HoughSpace.data[row * HoughSpace.w + col];// max vote
				distance[number_of_lines - 1] = row;// perpendecular distance value of max vote
				theta[number_of_lines - 1] = col;// theta value of max vote
			}
		}

	for (int i = 0; i < number_of_lines; i++)
	{
		std::cout << "-" << i << "-Max vote = " << max[i] << "\n";
		std::cout << "-" << i << "-Max distance = " << distance[i] << "\n";
		std::cout << "-" << i << "-Max theta = " << theta[i] << "\n---------------------------------------------------\n";
	}

	int psw, bufpos;
	psw = im.w * 3;

	for (int i = 0; i < number_of_lines; i++)
		for (int row = 0; row < im.h - 5; row++)
			for (int col = 0; col < im.w - 5; col++)

				if (distance[i] == int(col * cos(theta[i] * PI / 180) + row * sin(theta[i] * PI / 180) + HoughSpace.h / 2)) {
					bufpos = (row + 5) * psw + (col + 5) * im.c;
					im.data[bufpos] = 255, im.data[bufpos + 1] = 0, im.data[bufpos + 2] = 0;
				}
}

//if r is unknown and using gradient direction for optimization
hough HoughCircle(hough HoughSpace, image BinaryEdgeImage, gradient grad, int radius, int radius_scale) {
	
	HoughSpace.w = BinaryEdgeImage.w ;
	HoughSpace.h = BinaryEdgeImage.h ;

	int r_min = radius - radius_scale;
	int r_max = radius + radius_scale;

	HoughSpace.l = r_max;
	HoughSpace.data = new int[HoughSpace.w * HoughSpace.h * HoughSpace.l];

	for (int i = 0; i < HoughSpace.w * HoughSpace.h * HoughSpace.l; i++)
		HoughSpace.data[i] = 0;


	int diff = grad.w - BinaryEdgeImage.w;

	int a = 0;
	int b = 0;
	int bufpos = 0;

	for (int r = r_min; r < HoughSpace.l; r++)
		for (int row = 1; row < BinaryEdgeImage.h - 1; row++)
			for (int col = 1; col < BinaryEdgeImage.w - 1; col++) {

				if (BinaryEdgeImage.data[row * BinaryEdgeImage.w + col] == 255) {

						a = col - r * cos(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;
						b = row + r * sin(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;

						if (a > 0 && b > 0) {
							bufpos = HoughSpace.w * HoughSpace.h * r + HoughSpace.h * b + a;
							HoughSpace.data[bufpos]++;
						}

						/*a = col + r * cos(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;
						b = row - r * sin(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;

						if (a > 0 && b > 0) {
							bufpos = HoughSpace.w * HoughSpace.h * r + HoughSpace.h * b + a;
							HoughSpace.data[bufpos]++;
						}*/

						/*a = col - r * cos(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;
						b = row - r * sin(grad.direction[(row + diff) * grad.w + col + diff] * PI / 180) ;

						if (a > 0 && b > 0) {
							bufpos = HoughSpace.w * HoughSpace.h * r + HoughSpace.h * b + a;
							HoughSpace.data[bufpos]++;
						}*/
				}
			}
	return HoughSpace;
}

void DrawSelectedCircles(hough HoughSpace, image im, int radius, int number_of_circles, int radius_scale) {
	
	int* max = new int[number_of_circles];
	int* a_axis = new int[number_of_circles];//x
	int* b_axis = new int[number_of_circles];//y
	int* r_axis = new int[number_of_circles];

	for (int i = 0; i < number_of_circles; i++) {
		max[i] = 8;
		a_axis[i] = -1;
		b_axis[i] = -1;
		r_axis[i] = 0;
	}

	int r_min = radius - radius_scale;
	int bufpos = 0;

	for (int r = r_min; r < HoughSpace.l; r++)
		for (int row = 1; row < HoughSpace.h - 1; row++) // y yani b 
			for (int col = 1; col < HoughSpace.w - 1; col++) { // x yani a

				bufpos = (HoughSpace.w * HoughSpace.h * r) + (HoughSpace.h * row) + col;

				if (HoughSpace.data[bufpos] > max[number_of_circles - 1] && !isExist(a_axis, b_axis, r_axis, col, row, r, number_of_circles)) {// farklı r değerlerinde maksimum oy degerleri bulunmalı düzgün çalışmıyor!!

					for (int i = 1; i < number_of_circles; i++) {// 
						max[i - 1] = max[i];
						a_axis[i - 1] = a_axis[i];
						b_axis[i - 1] = b_axis[i];
						r_axis[i - 1] = r_axis[i];
					}

					max[number_of_circles - 1] = HoughSpace.data[bufpos];// max vote
					a_axis[number_of_circles - 1] = col;// x axsis
					b_axis[number_of_circles - 1] = row;// y axis
					r_axis[number_of_circles - 1] = r ;// r axis
				}
			}
	for (int i = 0; i < number_of_circles; i++) {

		std::cout << "-" << i << "-Max vote  " << max[i] << "\n";
		std::cout << "-" << i << "-A value " << a_axis[i] << "\n";
		std::cout << "-" << i << "-B value" << b_axis[i] << "\n";
		std::cout << "-" << i << "-R value  " << r_axis[i] << "\n---------------------------------------------------\n";
	}

	int psw;

	bufpos = 0;
	psw = im.w * 3;

	int diff = im.w - HoughSpace.w;

	for (int i = 0; i < number_of_circles; i++)
		for (int theta = 0; theta < 360; theta++) {

			int col = 0, row = 0;

			col = a_axis[i] + r_axis[i] * cos(theta * PI / 180);
			row = b_axis[i] + r_axis[i] * sin(theta * PI / 180);

			if ((col >= 1 && col < im.w - 1) && (row >= 1 && row < im.h - 1)) {

				bufpos = (row + diff) * psw + (col + diff) * im.c;
				im.data[bufpos] = 255, im.data[bufpos + 1] = 0, im.data[bufpos + 2] = 0;
			}
		}
}

////if r is known
//hough HoughCircle(hough HoughSpace, image BinaryEdgeImage, gradient grad, int radius, int radius_scale) {
//
//	HoughSpace.w = BinaryEdgeImage.w;
//	HoughSpace.h = BinaryEdgeImage.h;
//
//	int r_min = radius - radius_scale;
//	int r_max = radius + radius_scale;
//
//	HoughSpace.l = r_max;
//	HoughSpace.data = new int[HoughSpace.w * HoughSpace.h * HoughSpace.l];
//
//	for (int i = 0; i < HoughSpace.w * HoughSpace.h * HoughSpace.l; i++)
//		HoughSpace.data[i] = 0;
//
//	int a = 0, b = 0, bufpos = 0;
//
//	for (int r = r_min; r < HoughSpace.l; r++)
//		for (int row = 1; row < BinaryEdgeImage.h - 1; row++)
//			for (int col = 1; col < BinaryEdgeImage.w - 1; col++) {
//
//				if (BinaryEdgeImage.data[row * BinaryEdgeImage.w + col] == 255) {
//
//						for (int theta = 0; theta < 360; theta+=3){
//
//							int a = col /*x*/ - r * cos(theta * PI / 180) ;
//							int b = row /*y*/ - r * sin(theta * PI / 180) ;
//
//							bufpos = HoughSpace.w * HoughSpace.h * r + HoughSpace.h * b + a;
//							HoughSpace.data[bufpos]++;
//						}					
//				}
//			}
//
//	return HoughSpace;
//}