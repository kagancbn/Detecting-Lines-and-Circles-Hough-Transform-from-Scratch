#pragma once

#include <string.h>

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	unsigned char* data;
} image;
typedef struct gradient {
	int w;
	int h;
	double* direction;
	double* magnititude;
	int* quantizeDir;
} gradient;

typedef struct hough {
	int w;
	int h;
	int l;
	int* data;
} hough;



image load_image(const char* filename);
image make_image(int w, int h, int c);
image make_empty_image(int w, int h, int c);
image RgbToGray(image input);
image Smoothing(image input,int filterSize);
gradient Gradient(image input, gradient grad);
double* Gradient(image input, double*& angles);
image CannyEdge(gradient grad);
hough HoughLine(image BinaryEdgeImage, hough HoughSpace);
void DrawSelectedLines(hough HoughSpace, image im, int number_of_lines);
hough HoughCircle(hough HoughSpace, image BinaryEdgeImage, gradient grad, int radius, int radius_scale);
void DrawSelectedCircles(hough HoughSpace, image im, int radius, int number_of_circles, int radius_scale);

