#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <stdint.h>

class Pixel
{
	
	private:
	
	uint16_t r, g, b;
	
	public:
	
	// Constructors and Destructor
	Pixel();
	Pixel(uint16_t r, uint16_t g, uint16_t b);
	Pixel(uint64_t rgb);
	Pixel(uint16_t v);
	Pixel(Pixel const& other);
	~Pixel();
	
	// Public functions
	uint16_t getR();
	uint16_t getG();
	uint16_t getB();
	void setR(uint16_t val);
	void setG(uint16_t val);
	void setB(uint16_t val);
	uint16_t getMean();
	uint64_t get();
	
	
};

#endif
