#include "Pixel.hpp"

#include <iostream>

// Constructors and Destructor
Pixel::Pixel() : r(0), g(0), b(0)
{
}

Pixel::Pixel(uint16_t _r, uint16_t _g, uint16_t _b) : r(_r), g(_g), b(_b)
{
}

Pixel::Pixel(uint64_t rgb) : r(rgb >> 32), g((rgb >> 16) & 0xFFFF), b(rgb & 0xFFFF)
{
}

Pixel::Pixel(uint16_t v) : r(v), g(v), b(v)
{
}

Pixel::Pixel(Pixel const& other) : r(other.r), g(other.g), b(other.b)
{
}

Pixel::~Pixel()
{
}

// Public functions
uint16_t Pixel::getR()
{
	return r;
}

uint16_t Pixel::getG()
{
	return g;
}

uint16_t Pixel::getB()
{
	return b;
}

void Pixel::setR(uint16_t val)
{
	r = val;
}

void Pixel::setG(uint16_t val)
{
	g = val;
}

void Pixel::setB(uint16_t val)
{
	b = val;
}

uint16_t Pixel::getMean()
{
	return (r + g + b) / 3;
}

uint64_t Pixel::get()
{
	return ((uint64_t)r << 32) | ((uint64_t)g << 16) | (uint64_t)b;
}
