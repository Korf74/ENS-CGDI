#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include "Pixel.hpp"

#include <stdint.h>
#include <stddef.h>

class Histogram
{
	
	private:
	uint16_t intensity;
	size_t size;
	double minValue[3];
	double maxValue[3];
	double* freq;
	double* cumul;
	
	
	public:
	
	// Constructors and Destructor
	Histogram();
	Histogram(uint16_t it, size_t sz, Pixel* px);
	~Histogram();
	
	// Public functions
	void printHistogram();
	void inversion();
	void gammaCorrection(double gamma);
	void interpolation();
	void equalization();
	
	
};

#endif
