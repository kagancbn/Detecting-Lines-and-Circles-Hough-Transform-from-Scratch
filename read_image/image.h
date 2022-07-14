#pragma once

#include <string.h>

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	unsigned char* data;
} image;

image load_image(const char* filename);
image make_image(int w, int h, int c);
image make_empty_image(int w, int h, int c);
image Rgb_to_gray(image input);
image Smoothing(image input);
double* Gradient(image input, double*& angles);
image CannyEdge(double* EdgeImage, int width, int height, double* angles);
int *HoughLine(image BinaryEdgeImage, int Width, int Height,double* angles, int& HoughWidth, int& HoughHeight);

