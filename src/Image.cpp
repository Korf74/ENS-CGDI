#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <cmath>

#include "Image.hpp"
#include "Pixel.hpp"
#include "Histogram.hpp"

using namespace std;

// Constructors and Destructor
Image::Image() : size(0), rows(0), cols(0), intensity(0), px(NULL), byte_array(NULL), histo(NULL)
{
}

Image::Image(Image const& other) : size(other.size), rows(other.rows), cols(other.cols), intensity(other.intensity)
{
	if(other.px != NULL) {
		if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) {
			cerr << "Allocation error" << endl;
			exit(1);
		}
		
		for(uint32_t i = 0; i < size; ++i) {
			px[i] = Pixel(other.px[i]);
		}
	}
	
	histo = NULL;
	byte_array = NULL;
}

Image::~Image()
{
	if(px != NULL) free(px);
	if(byte_array != NULL) free(byte_array);
	if(histo != NULL) free(histo);
}

// Private functions
void Image::to_byte_array()
{
	
	if(byte_array == NULL) {
	
		if((byte_array = (uint8_t*)malloc(size * 3 * 2 * sizeof(uint8_t))) == NULL) {
			cerr << "Allocation error" << endl;
			free(px);
			exit(1);
		}
		
		for(size_t i = 0; i < size * 6; i += 6) {
			uint64_t pixel = px[i].get();
			for(int32_t j = 5; j >= 0; --j) {
				byte_array[i + j] = (uint8_t)(pixel & 0xFF);
				pixel = pixel >> 8;
			}
		}
	}
	
}

void Image::make_histogram()
{	
	histo = new Histogram(intensity, size, px);
}

void Image::histogram_print()
{
	if(histo != NULL) histo -> printHistogram();
}

void Image::histogram_inversion()
{
	if(histo != NULL) histo -> inversion();
}

void Image::histogram_gamma_correction(double gamma)
{
	if(histo != NULL) histo -> gammaCorrection(gamma);
}

void Image::histogram_interpolation()
{
	if(histo != NULL) histo -> interpolation();
}

void Image::histogram_equalization()
{
	if(histo != NULL) histo -> equalization();
}

void Image::binomial_filter()
{
	int32_t kernel[3] = {1, 2, 1};
	
	convolute_x(3, kernel, -1);
	convolute_y(3, kernel, -1);
}

void Image::sobel_filter_x()
{
	int32_t kernel_x[3] = {1, 0, -1}; 
	int32_t kernel_y[3] = {1, 2, 1};
	
	convolute_x(3, kernel_x, 2);
	convolute_y(3, kernel_y, 2);
}

void Image::sobel_filter_y()
{
	int32_t kernel_x[3] = {1, 2, 1}; 
	int32_t kernel_y[3] = {1, 0, -1};
	
	convolute_x(3, kernel_x, 2);
	convolute_y(3, kernel_y, 2);
}

void Image::sobel_grad()
{
	Image sob_y(*this);

	sob_y.sobel_filter_y();
	sobel_filter_x();
	
	for(uint32_t i = 0; i < size; ++i) {
		Pixel p = px[i];
		Pixel q = sob_y.px[i];
		cout << p.getR() << " " << q.getR() << endl;
		px[i].setR(sqrt(p.getR() * p.getR() + q.getR() * q.getR()));
		px[i].setG(sqrt(p.getG() * p.getG() + q.getG() * q.getG()));
		px[i].setB(sqrt(p.getB() * p.getB() + q.getB() * q.getB()));
	}
}

uint32_t Image::mod(int32_t a, uint32_t m)
{
	if(a < 0) {
		while(a < 0) a+= m;
		return (uint32_t)a;
	} else {
		while((uint32_t)a >= m) a -= m;
		return (uint32_t)a;
	}
}


void Image::convolute_y(uint32_t n, int32_t* kernel, int32_t coeff) {
	
	uint32_t sum(0);
	
	Pixel* px_copy = px;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) return;
	
	if(coeff == -1) {
		for(uint32_t i = 0; i < n; ++i) sum += kernel[i];
	} else {
		sum = (uint32_t)coeff;
	}
		
	for(int32_t i = 0; (uint32_t)i < rows; ++i) {
		for(int32_t j = 0; (uint32_t)j < cols; ++j) {
			
			int32_t tmpR(0), tmpG(0), tmpB(0);
			
			for(uint32_t k = 0; k < n; ++k) {
				Pixel p = px_copy[(mod(i - n / 2 + k, rows)) * cols + j];
				tmpR += (p.getR() * kernel[k]);
				tmpG += (p.getG() * kernel[k]);
				tmpB += (p.getB() * kernel[k]);
			}
			
			tmpR += intensity;
			tmpG += intensity;
			tmpB += intensity;
			
			tmpR /= sum;
			tmpG /= sum;
			tmpB /= sum;
			
			px[i * cols + j].setR(tmpR);
			px[i * cols + j].setG(tmpG);
			px[i * cols + j].setB(tmpB);
			
		}
	}
	
	free(px_copy);
	
}

void Image::convolute_x(uint32_t n, int32_t* kernel, int32_t coeff) {
	
	uint32_t sum(0);
	
	Pixel* px_copy = px;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) return;
	
	if(coeff == -1) {
		for(uint32_t i = 0; i < n; ++i) sum += kernel[i];
	} else {
		sum = (uint32_t)coeff;
	}
	
	for(int32_t i = 0; (uint32_t)i < rows; ++i) {
		for(int32_t j = 0; (uint32_t)j < cols; ++j) {
			
			int32_t tmpR(0), tmpG(0), tmpB(0);
			
			for(uint32_t k = 0; k < n; ++k) {
				Pixel p = px_copy[i * cols + mod(j - n / 2 + k, cols)];
				tmpR += (p.getR() * kernel[k]);
				tmpG += (p.getG() * kernel[k]);
				tmpB += (p.getB() * kernel[k]);
			}
			
			tmpR += intensity;
			tmpG += intensity;
			tmpB += intensity;
			
			tmpR /= sum;
			tmpG /= sum;
			tmpB /= sum;
			
			px[i * cols + j].setR(tmpR);
			px[i * cols + j].setG(tmpG);
			px[i * cols + j].setB(tmpB);
			
		}
	}
	
	free(px_copy);
	
}

void Image::convolute2D(uint32_t n, int32_t* kernel, int32_t coeff) {
	
	int32_t sum(0);
	
	Pixel* px_copy = px;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) return;
	
	if(coeff == -1) {
		for(uint32_t i = 0; i < n; ++i) sum += kernel[i];
	} else {
		sum = coeff;
	}
		
	for(int32_t i = 0; (uint32_t)i < rows; ++i) {
		for(int32_t j = 0; (uint32_t)j < cols; ++j) {
			
			int32_t tmpR(0), tmpG(0), tmpB(0);
			
			for(uint32_t k = 0; k < n; ++k) {
				for(uint32_t l = 0; l < n; ++l) {
					Pixel p = px_copy[(mod(i - n / 2 + k, rows)) * cols + (mod(j - n / 2 + l, cols))];
					tmpR += (p.getR() * kernel[k * n + l]);
					tmpG += (p.getG() * kernel[k * n + l]);
					tmpB += (p.getB() * kernel[k * n + l]);
				}
			}
			
			px[i * cols + j].setR(tmpR / sum);
			px[i * cols + j].setG(tmpG / sum);
			px[i * cols + j].setB(tmpB / sum);
		}
	}
	
	free(px_copy);
	
}

uint16_t Image::extractR(uint64_t p) {
	return (p >> 32);
}
uint16_t Image::extractG(uint64_t p) {
	return ((p >> 16) & 0xFF);
}
uint16_t Image::extractB(uint64_t p) {
	return (p & 0xFF);
}

// Public functions
void Image::read_pbm(const string& filename)
{
	
	char ich1, ich2;
	bool ascii;
	
	ifstream in(filename.c_str());
	
	if(!(in >> ich1)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich1 != 'P') {
		cerr << "Error : The file must start by the character P";
		exit(1);
	}
	
	if(!(in >> ich2)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich2 != '1' && ich2 != '4') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '1') {
		ascii = true;
	} else {
		ascii = false;
	}

	
	if(!(in >> cols >> rows)) {
		cerr << "Eror : Bad file format";
	}
	
	size = cols * rows;
	intensity = 1;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	uint8_t tmp;
	
	for(size_t i = 0; i < size; ++i) {
		if(!(in >> tmp)) {
			cerr << "Bad file format";
			free(px);
			if(byte_array) free(byte_array);
			exit(1);
		}
		px[i] = Pixel((uint16_t)(ascii ? tmp - '0': tmp));
	}

}

void Image::read_pgm(const string& filename)
{
	
	char ich1, ich2;
	bool ascii;
	
	ifstream in(filename.c_str());
	
	if(!(in >> ich1)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich1 != 'P') {
		cerr << "Error : The file must start by the character P";
		exit(1);
	}
	
	if(!(in >> ich2)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich2 != '2' && ich2 != '5') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '2') {
		ascii = true;
	} else {
		ascii = false;
	}

	if(!(in >> cols >> rows)) {
		cerr << "Eror : Bad file format";
	}
	
	if(!(in >> intensity)) {
		cerr << "Error : Bad file format";
		exit(1);
	}
	
	size = cols * rows;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	in.get();
	
	
	if(ascii) {
		for(uint32_t i = 0; i < size; ++i) {
			uint16_t tmp(0);
			
			if(!(in >> tmp)) {
				cerr << "Bad file format";
				free(px);
				if(byte_array) free(byte_array);
				exit(1);
			}
					
			px[i] = Pixel(tmp);
			
			tmp = 0;
		}
	} else {
		for(uint32_t i = 0; i < size; ++i) {
			uint16_t tmp(0);
			
			if(!(in.read((char*)&tmp, (intensity >= 256 ? 2 : 1)))) {
				cerr << "Bad file format";
				free(px);
				if(byte_array) free(byte_array);
				exit(1);
			}
					
			px[i] = Pixel(tmp);
			
		}
	}

}

void Image::read_ppm(const string& filename)
{
	
	char ich1, ich2;
	bool ascii;
	
	
	ifstream in(filename.c_str());
	
	if(!(in >> ich1)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich1 != 'P') {
		cerr << "Error : The file must start by the character P";
		exit(1);
	}
	
	if(!(in >> ich2)) {
		cerr << "Error : EOF encountered while reading the header";
		exit(1);
	}
	
	if(ich2 != '3' && ich2 != '6') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '3') {
		ascii = true;
	} else {
		ascii = false;
	}
	
	if(!(in >> cols >> rows)) {
		cerr << "Eror : Bad file format";
	}
	
	if(!(in >> intensity)) {
		cerr << "Error : Bad file format";
		exit(1);
	}
	
	size = rows * cols;
	
	if((px = (Pixel*)malloc(size * sizeof(Pixel))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	in.get();
	
	if(ascii) {
		for(uint32_t i = 0; i < size; ++i) {
			uint16_t tmp(0);
			uint64_t pixel(0);
			for(uint32_t j = 0; j < 3; ++j) {
				if(!(in >> tmp)) {
					cerr << "Bad file format";
					free(px);
					if(byte_array) free(byte_array);
					exit(1);
				}
				pixel = (pixel << 16) | tmp;
				
				tmp = 0;
			}
			
			px[i] = Pixel(pixel);
		}
	} else {
		for(uint32_t i = 0; i < size; ++i) {
			uint16_t tmp(0);
			uint16_t c[3];
			for(uint32_t j = 0; j < 3; ++j) {
				if(!(in.read((char*)&tmp, (intensity >= 256 ? 2 : 1)))) {
					cerr << "Bad file format";
					free(px);
					if(byte_array) free(byte_array);
					exit(1);
				}
				c[j] = tmp;
				
				tmp = 0;
				
			}
			
			px[i] = Pixel(c[0], c[1], c[2]);
		}
	}

}

void Image::write_pbm_ascii(const string& filename)
{
	ofstream out(filename.c_str());
	
	out << 'P' << '1' << endl;
	out << cols << " " << rows << endl;
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint16_t pixel = px[i * cols + j].getR();
			pixel = (pixel > intensity / 2 ? 1 : 0);
			out << (pixel & 0xFF) << " ";
		}
		out << endl;
	}
	
}

void Image::write_pgm_ascii(const string& filename)
{
	std::ofstream out(filename.c_str());
	
	out << "P" << '2' << endl;
	out << cols << " " << rows << endl;
	out << intensity << endl;
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			out << px[i * cols + j].getR() << " ";
		}
		out << endl;
	}
}

void Image::write_ppm_ascii(const string& filename)
{
	std::ofstream out(filename.c_str());
	
	out << "P" << '3' << endl;
	out << cols << " " << rows << " " << endl;
	out << intensity << endl;
			
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint64_t pixel = px[i * cols + j].get();

			out <<  extractR(pixel) << " " << extractG(pixel) << " " << extractB(pixel) << " ";
		}
		out << endl;
	}
}

void Image::write_pbm_bin(const string& filename)
{
	std::ofstream out(filename.c_str());
	
	out << 'P' << '4' << endl;
	out << cols << " " << rows << endl;
		
	uint32_t off(7);
	uint8_t b(0);
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint8_t x = (px[i * cols + j].getR() > intensity / 2 ? 1 : 0) << off;
			b |= x;
			if(off == 0 || j == cols - 1) {
				out << b;
				b = 0;
				off = 7;
			} else {
				--off;
			}
		}
	}
	
	if(off < 7) out << b;
}

void Image::write_pgm_bin(const string& filename)
{
	std::ofstream out(filename.c_str());
	
	out << "P" << '5' << endl;
	out << cols << " " << rows << endl;
	out << intensity << endl;
	
	for(uint32_t i = 0; i < size; ++i) {
		if(intensity < 256) out << (uint8_t)px[i].getR();
		else out << px[i].getR();
	}
}

void Image::write_ppm_bin(const string& filename)
{
	std::ofstream out(filename.c_str());
	
	out << "P" << '6' << endl;
	out << cols << " " << rows << endl;
	out << intensity << endl;
	
	for(uint32_t i = 0; i < size; ++i) {
		uint64_t pixel = px[i].get();
		uint16_t r = extractR(pixel);
		uint16_t g = extractG(pixel);
		uint16_t b = extractB(pixel);

		if(intensity >= 256) {
			out.write(((char*)&r), 2);
			out.write(((char*)&g), 2);
			out.write(((char*)&b), 2);
		} else {
			out << (uint8_t)r << (uint8_t)g << (uint8_t)b;
		}
	}
}
