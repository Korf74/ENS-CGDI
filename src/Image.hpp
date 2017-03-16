#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <sys/types.h>
#include <string>

#include "Pixel.hpp"
#include "Histogram.hpp"

class Image
{
	
	private:
	
	Pixel* px;
	size_t size;
	size_t rows;
	size_t cols;
	uint16_t intensity;
	uint8_t* byte_array;
	Histogram* histo;
	
	// Private functions
	void to_byte_array();
	uint16_t extractR(uint64_t p);
	uint16_t extractG(uint64_t p);
	uint16_t extractB(uint64_t p);
	uint32_t mod(int32_t a, uint32_t m);
	void convolute_x(uint32_t n, int32_t* kernel, int32_t coeff);
	void convolute_y(uint32_t n, int32_t* kernel, int32_t coeff);
	void convolute2D(uint32_t n, int32_t* kernel, int32_t coeff);
	
	public:
	
	// Constructors and Destructor
	Image();
	Image(Image const& other);
	~Image();
	
	// Public functions
	void read_pbm(const std::string& filename);
	void read_pgm(const std::string& filename);
	void read_ppm(const std::string& filename);
	void write_pbm_ascii(const std::string& filename);
	void write_pgm_ascii(const std::string& filename);
	void write_ppm_ascii(const std::string& filename);
	void write_pbm_bin(const std::string& filename);
	void write_pgm_bin(const std::string& filename);
	void write_ppm_bin(const std::string& filename);
	void make_histogram();
	void histogram_print();
	void histogram_inversion();
	void histogram_gamma_correction(double gamma);
	void histogram_interpolation();
	void histogram_equalization();
	void binomial_filter();
	void sobel_filter_x();
	void sobel_filter_y();
	void sobel_grad();
	
	
};
/*
	protected:
	
	size_t cols, rows;
	unsigned char* px;
	bool ascii;
	bool format;
	
	public:
	
	Image();
	Image(const std::string& filename);
	Image(size_t c, size_t r, bool ascii_format);
	Image(size_t c, size_t r);
	Image(size_t c, size_t r, bool ascii_format, unsigned char* p);
	~Image();
	Image* to_ascii_pbm();
	Image* to_bin_pbm();
	Image* to_ascii_pgm();
	Image* to_bin_pgm();
	Image* to_ascii_ppm();
	Image* to_bin_ppm();
	void read(const std::string& filename);
	void write(const std::string& filename);

	
};*/

#endif
