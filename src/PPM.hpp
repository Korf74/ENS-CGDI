#ifndef PPM_HPP
#define PPM_HPP

#include <sys/types.h>
#include <string>
#include <stdint.h>

#include "Image.hpp"


class PPM : public Image
{
	
	protected:
	
	bool two_bytes;
	uint16_t intensity;
	uint64_t* px;
	
	
	public:
	
	PPM();
	PPM(const std::string& filename);
	PPM(size_t c, size_t r, bool ascii_format);
	PPM(size_t c, size_t r);
	PPM(size_t c, size_t r, bool ascii, bool bytes, uint16_t intst, uint64_t* p);
	PPM(PPM const& other);
	~PPM();
	Image* to_ascii_pbm();
	Image* to_bin_pbm();
	Image* to_ascii_pgm();
	Image* to_bin_pgm();
	Image* to_ascii_ppm();
	Image* to_bin_ppm();
	void read(const std::string& filename);
	void write(const std::string& filename);
	
};

#endif
