#include "Histogram.hpp"

#include <cmath>
#include <stdlib.h>
#include <iostream>

using namespace std;

// Constructors and Destructor
Histogram::Histogram() : intensity(0), size(0)
{
	for(uint32_t i = 0; i < 3; ++i) {
		maxValue[i] = 0;
		minValue[i] = 0;
		freq = NULL;
		cumul = NULL;
	}
}

Histogram::Histogram(uint16_t it, size_t sz, Pixel* px) : intensity(it), size(sz)
{
	if((freq = (double*)malloc(it * 3 * sizeof(double)))) {
		cerr << "Allocation error";
		exit(1);
	}
	
	if((cumul = (double*)malloc(it * 3 * sizeof(uint32_t)))) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < 3; ++i) {
		minValue[i] = sz;
		maxValue[i] = 0;
	}
	
	for(uint32_t i = 0; i < sz; ++i) {
		Pixel p = px[i];
		++freq[p.getR()];
		++freq[it + p.getG()];
		++freq[2 * it + p.getB()];
	}
	
	for(uint32_t i = 0; i < it; ++i) {
		for(uint32_t j = 0; j < 3; ++i) {
			freq[j * it + i] /= sz;
			cumul[j * it + i] = (i == 0 ? freq[j * it + i] : freq[j * it + i] + cumul[j * it + i - 1]);
			maxValue[j] = max(maxValue[j], freq[j * it + i]);
			minValue[j] = min(minValue[j], freq[j * it + i]);
		}
	}
}

Histogram::~Histogram()
{
	if(freq != NULL) free(freq);
	if(cumul != NULL) free(cumul);
}

void Histogram::printHistogram()
{
	if(freq == NULL) {
		cerr << "The histogram is not initialized";
		exit(1);
	}
	
	for(uint32_t i = 0; i < intensity; ++i) {
		cout << i << " " << freq[i] << endl;
	}
}

void Histogram::inversion()
{
	uint32_t a, b;
	
	for(uint32_t i = 0; i < intensity / 2; ++i) {
		a = freq[i];
		b = freq[1 - i];
		
		a = a + b;
		b = a - b;
		a = a - b;
		
		freq[i] = a;
		freq[1 - i] = b;
	}
}

void Histogram::gammaCorrection(double gamma)
{
	for(uint32_t i = 0; i < intensity; ++i) {
		freq[i] = pow(freq[i], 1 / gamma);
	}
}

void Histogram::interpolation()
{/*
	for(uint32_t i = 0; i < intensity; ++i) {
		freq[i] = () / ();
	}*/
}

void Histogram::equalization()
{
	for(uint32_t i = 0; i < intensity; ++i) {
		freq[i] = intensity * cumul[i];
	}
}

