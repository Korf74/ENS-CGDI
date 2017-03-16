#ifndef PBM_HPP
#define PBM_HPP

#include <sys/types.h>
#include <string>

#include "Image.hpp"


class PBM : public Image
{
	
	protected:
	
	
	public:
	
	PBM();
	PBM(const std::string& filename);
	PBM(size_t c, size_t r, bool ascii_format);
	PBM(size_t c, size_t r);
	PBM(size_t c, size_t r, bool ascii_format, unsigned char* p);
	PBM(PBM const& other);
	~PBM();
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
