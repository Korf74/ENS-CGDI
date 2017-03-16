#ifndef PGM_HPP
#define PGM_HPP

#include <sys/types.h>
#include <string>
#include <stdint.h>

#include "Image.hpp"


class PGM : public Image
{
	
	protected:
	
	bool two_bytes;
	uint16_t intensity;
	
	
	public:
	
	PGM();
	PGM(const std::string& filename);
	PGM(size_t c, size_t r, bool ascii);
	PGM(size_t c, size_t r);
	PGM(size_t c, size_t r, bool ascii, bool bytes, uint16_t intst, unsigned char* p);
	PGM(PGM const& other);
	~PGM();
	Image* to_ascii_pbm();
	Image* to_bin_pbm();
	Image* to_ascii_pgm();
	Image* to_bin_pgm();
	Image* to_ascii_ppm();
	Image* to_bin_ppm();
	void read(const std::string& filename);
	void write(const std::string& filename);
	void convolute(int32_t* kernel, uint32_t sum, size_t n);
	
};

#endif
