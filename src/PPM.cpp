#include "PBM.hpp"
#include "PGM.hpp"
#include "PPM.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

PPM::PPM() : Image(), two_bytes(false)
{
	px = NULL;
}

PPM::PPM(const string& filename) : Image(), two_bytes(false), intensity(0)
{
	px = NULL;
	read(filename);
}

PPM::PPM(size_t c, size_t r, bool ascii_format) : Image(c, r, ascii_format), two_bytes(false)
{
	px = NULL;
}

PPM::PPM(size_t c, size_t r) : Image(c, r), two_bytes(false)
{
	px = NULL;
}

PPM::PPM(size_t c, size_t r, bool ascii, bool bytes, uint16_t intst, uint64_t* p) : Image(c, r, ascii), two_bytes(bytes), intensity(intst)
{
	px = p;
}

PPM::PPM(PPM const& other) : Image(other.cols, other.rows, other.ascii), two_bytes(other.two_bytes), intensity(other.intensity)
{
	if((px = (uint64_t*)malloc(cols * rows * sizeof(uint64_t))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < rows * cols; ++i) {
		px[i] = other.px[i];
	}
}

PPM::~PPM()
{
	if(px != NULL) free(px);

	px = NULL;
}

Image* PPM::to_ascii_pbm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = px[i];
		uint16_t r(0), g(0), b(0);
		
		b = tmp & 0xFFFF;
		tmp = tmp >> 16;
		g = tmp & 0xFFFF;
		tmp = tmp >> 16;
		r = tmp;
		
		uint16_t mean = (r + g + b) / 3;
		
		p[i] = (mean > intensity ? '1' : '0'); 
	}
	
	return new PBM(cols, rows, true, p);
}
Image* PPM::to_bin_pbm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = px[i];
		uint16_t r(0), g(0), b(0);
		
		b = tmp & 0xFFFF;
		tmp = tmp >> 16;
		g = tmp & 0xFFFF;
		tmp = tmp >> 16;
		r = tmp;
		
		uint16_t mean = (r + g + b) / 3;
		
		p[i] = (mean > intensity ? 1 : 0); 
	}
	
	return new PBM(cols, rows, false, p);
}
Image* PPM::to_ascii_pgm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * (two_bytes ? 2 : 1) * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = px[i];
		uint16_t r(0), g(0), b(0);
		
		b = tmp & 0xFFFF;
		tmp = tmp >> 16;
		g = tmp & 0xFFFF;
		tmp = tmp >> 16;
		r = tmp;
		
		uint16_t mean = (r + g + b) / 3;
		
		if(two_bytes) {
			p[i + 1] = (mean % 10) + '0';
			p[i] = (mean / 10) + '0';
		} else {
			p[i] = (mean % 10) + '0';
		}
	}
	
	return new PGM(cols, rows, true, two_bytes, intensity, p);
}
Image* PPM::to_bin_pgm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * (two_bytes ? 2 : 1) * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = px[i];
		uint16_t r(0), g(0), b(0);
		
		b = tmp & 0xFFFF;
		tmp = tmp >> 16;
		g = tmp & 0xFFFF;
		tmp = tmp >> 16;
		r = tmp;
		
		uint16_t mean = (r + g + b) / 3;
		
		if(two_bytes) {
			p[i + 1] = (mean % 10);
			p[i] = (mean / 10);
		} else {
			p[i] = (mean % 10);
		}
	}
	
	return new PGM(cols, rows, false, two_bytes, intensity, p);
}
Image* PPM::to_ascii_ppm() {
	return new PPM(*this);
}
Image* PPM::to_bin_ppm() {
	return new PPM(*this);
}

void PPM::read(const std::string& filename)
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
	
	if(ich2 == '3' && !ascii && format) {
		cerr << "Error : Trying to read an ascii PPM into a binary PPM";
		exit(1);
	} else if(ich2 == '6' && ascii && format) {
		cerr << "Error : Trying to read a binary PPM into an ascii PPM";
		exit(1);
	} else if(ich2 != '3' && ich2 != '6') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '3') {
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
	
	if((px = (uint64_t*)malloc(cols * rows * sizeof(uint64_t))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	if(!ascii) {
	
		unsigned char tmp;
		uint64_t pixel;
	
		for(size_t i = 0; i < rows; ++i) {
			for(size_t j = 0; j < cols; ++j) {
				for(int c = 0; c < 3; ++c) {
					for(int k = 0; k < (two_bytes ? 2 : 1); ++k) {
						if(!(in >> tmp)) {
							cerr << "Bad file format";
							free(px);
							exit(1);
						}
						
						pixel = (pixel << (two_bytes ? 8 : 16)) | tmp;
						
					}
				}
				
				px[i * cols + j] = pixel;	
				
			}
		}
		
	} else {
		
		uint16_t tmp;
		
		for(size_t i = 0; i < rows; ++i) {
			for(size_t j = 0; j < cols; ++j) {
				
				uint64_t pixel = 0;
				for(int c = 0; c < 3; ++c) {
					
					if(!(in >> tmp)) {
						perror("Bad file format");
						free(px);
						exit(1);
					}
					
					pixel = (pixel << 16) | tmp;
										
				}
				px[i * cols + j] = pixel;	
			}
		}
		
	}

}

void PPM::write(const std::string& filename) {
	
	std::ofstream out(filename.c_str());
	
	out << "P" << (ascii && format ? "3" : "6") << endl;
	out << cols << " " << rows << " " << endl;
	out << intensity << endl;
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint64_t pixel = px[i * cols + j];
			uint16_t r(0), g(0), b(0);
			
			b = pixel & 0xFFFF;
			pixel = pixel >> 16;
			g = pixel & 0xFFFF;
			pixel = pixel >> 16;
			r = pixel;
			
			out << r;
			if(ascii && format) out << " ";
			out << g;
			if(ascii && format) out << " ";
			out << b;
			if(ascii && format) out << " ";
		}
		out << endl;
	}
}
