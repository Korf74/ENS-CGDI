#include "PBM.hpp"
#include "PGM.hpp"
#include "PPM.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

PGM::PGM() : Image(), two_bytes(false)
{
}

PGM::PGM(const string& filename) : Image(), two_bytes(false), intensity(0)
{
	read(filename);
}

PGM::PGM(size_t c, size_t r, bool ascii_format) : Image(c, r, ascii_format), two_bytes(false)
{
}

PGM::PGM(size_t c, size_t r) : Image(c, r), two_bytes(false)
{
}

PGM::PGM(size_t c, size_t r, bool ascii, bool bytes, uint16_t intst, unsigned char* p) : Image(c, r, ascii), two_bytes(bytes), intensity(intst)
{
	px = p;
}

PGM::PGM(PGM const& other) : Image(other.cols, other.rows, other.ascii), two_bytes(other.two_bytes), intensity(other.intensity)
{
	if((px = (unsigned char*)malloc(cols * rows * (two_bytes ? 2 : 1) * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows * (two_bytes ? 2 : 1); ++i) {
		px[i] = other.px[i];
	}
}

PGM::~PGM()
{
}

Image* PGM::to_ascii_pbm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint16_t tmp = (two_bytes ? ((uint16_t)(px[i]) << 8) | px[i + 1] : px[i]);  
		p[i] = (tmp > intensity / 2 ? '1' : '0');
	}
	
	return new PBM(cols, rows, true, p);
}
Image* PGM::to_bin_pbm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint16_t tmp = (two_bytes ? ((uint16_t)(px[i]) << 8) | px[i + 1] : px[i]);  
		p[i] = (tmp > intensity / 2 ? 1 : 0);
	}
	
	return new PBM(cols, rows, true, p);
}
Image* PGM::to_ascii_pgm() {
	return new PGM(*this);
}
Image* PGM::to_bin_pgm() {
	return new PGM(*this);
}
Image* PGM::to_ascii_ppm() {
	
	uint64_t* p;
	
	
	if((p = (uint64_t*)malloc(cols * rows * sizeof(uint64_t))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = px[i];
		if(two_bytes) tmp = (tmp << 8) | px[i + 1];
		
		uint64_t pixel = (tmp << 32) | (tmp << 16) | tmp;
		
		p[i] = pixel;
	}
	
	return new PPM(cols, rows, ascii, two_bytes, intensity, p);
}
Image* PGM::to_bin_ppm() {
	return PGM::to_ascii_ppm();
}

void PGM::convolute(int32_t* kernel, uint32_t sum, size_t n) {
	PGM* copy = new PGM(*this);
	
	for(uint32_t i = n / 2; i < rows - n / 2; ++i) {
		for(uint32_t j = n / 2; j < cols - n / 2; ++j) {
			int32_t tmp = 0;
			for(uint32_t k = 0; k < n; ++k) {
				for(uint32_t l = 0; l < n; ++l) {
					tmp += copy -> px[(i - n / 2 + k) * cols + (j - n / 2 + l)] * kernel[k * n + l];
				}
			}
			px[i * cols + j] = tmp / sum;
		}
	}
	
	delete(copy);
}

void PGM::read(const std::string& filename)
{
	
	char ich1, ich2;
	
	
	std::ifstream in(filename.c_str());
	
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
	
	if(ich2 == '2' && !ascii && format) {
		cerr << "Error : Trying to read an ascii PGM into a binary PGM";
		exit(1);
	} else if(ich2 == '5' && ascii && format) {
		cerr << "Error : Trying to read a binary PGM into an ascii PGM";
		exit(1);
	} else if(ich2 != '2' && ich2 != '5') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '2') {
		ascii = true;
		format = true;
	} else {
		ascii = false;
		format = true;
	}
	
	if(!(in >> cols >> rows)) {
		cerr << "Eror : Bad file format";
	}
	
	if(!(in >> intensity)) {
		cerr << "Error : Bad file format";
		exit(1);
	}
	
	if(intensity >= 256) two_bytes = true;
	
	if((px = (unsigned char*)malloc(cols * rows * (two_bytes ? 2 : 1) * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	if(!ascii && format) {
	
		unsigned char tmp;
	
		for(size_t i = 0; i < rows; ++i) {
			for(size_t j = 0; j < cols; ++j) {
				for(int k = 0; k < (two_bytes ? 2 : 1); ++k) {
					//in.read(&tmp, 1);
					if(!(in >> tmp)) {
						cerr << "Bad file format";
						free(px);
						exit(1);
					}
					
					px[i * cols + j + k] = tmp;	
				}
				
			}
		}
		
		//in.read((char*)px, cols * rows * (two_bytes ? 2 : 1));
	} else {
		
		
		uint16_t tmp;
		
		for(size_t i = 0; i < rows; ++i) {
			for(size_t j = 0; j < cols; ++j) {
				if(!(in >> tmp)) {
					cerr << "Bad file format";
					free(px);
					exit(1);
				}
								
				if(two_bytes) {
					px[i * cols + j] = (unsigned char)(tmp >> 8);
					px[i * cols + j + 1] = (unsigned char)(tmp & 0xFF);
				} else {
					px[i * cols + j] = (unsigned char)(tmp);
				}
					
			}
		}
		
	}

}

void PGM::write(const std::string& filename) {
	
	std::ofstream out(filename.c_str());
	
	out << "P" << (ascii && format ? "2" : "5") << endl;
	out << cols << " " << rows << " " << endl;
	out << intensity << endl;
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint16_t tmp = px[i * cols + j];
			if(two_bytes) tmp = (tmp << 8) | px[i * cols + j + 1];
			out << tmp;
			if(ascii && format) out << " ";
		}
		out << endl;
	}
}
