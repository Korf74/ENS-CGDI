#include "PBM.hpp"
#include "PGM.hpp"
#include "PPM.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

using namespace std;

PBM::PBM() : Image()
{
}

PBM::PBM(const string& filename) : Image()
{
	read(filename);
}

PBM::PBM(size_t c, size_t r, bool ascii_format) : Image(c, r, ascii_format)
{
}

PBM::PBM(size_t c, size_t r) : Image(c, r)
{
}

PBM::PBM(size_t c, size_t r, bool ascii_format, unsigned char* p) : Image(c, r, ascii_format, p)
{
}

PBM::PBM(PBM const& other) : Image(other.cols, other.rows, other.ascii)
{
	if((px = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < rows * cols; ++i) {
		px[i] = other.px[i];
	}
}

PBM::~PBM()
{
}

Image* PBM::to_ascii_pbm() {
	if(ascii && format) return new PBM(*this);
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		p[i] = px[i] /*+ '0'*/;
	}
		
	return new PBM(cols, rows, true, p);
}
Image* PBM::to_bin_pbm() {
	if(!ascii && format) return new PBM(*this);
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		p[i] = px[i]/* - '0'*/;
	}
	
	return new PBM(cols, rows, false, p);
}

Image* PBM::to_ascii_pgm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		p[i] = (ascii && format ? px[i] : px[i] + '0');
	}
	
	return new PGM(cols, rows, true, false, 1, p);
}
Image* PBM::to_bin_pgm() {
	
	unsigned char* p;
	
	if((p = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		p[i] = (!ascii && format ? px[i] : px[i] - '0');
	}
	
	return new PGM(cols, rows, false, false, 1, p);
}
Image* PBM::to_ascii_ppm() {
	
	uint64_t* p;
	
	if((p = (uint64_t*)malloc(cols * rows * sizeof(uint64_t))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = ascii && format ? px[i] : px[i] + '0';
		p[i] = (tmp << 32) | (tmp << 16) | (tmp);
	}
	
	return new PPM(cols, rows, true, false, 1, p);
}
Image* PBM::to_bin_ppm() {
	
	uint64_t* p;
	
	if((p = (uint64_t*)malloc(cols * rows * sizeof(uint64_t))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	for(uint32_t i = 0; i < cols * rows; ++i) {
		uint64_t tmp = !ascii && format ? px[i] : px[i] - '0';
		p[i] = (tmp << 32) | (tmp << 16) | (tmp);
	}
	
	return new PPM(cols, rows, false, false, 1, p);
}

void PBM::read(const std::string& filename)
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
	
	if(ich2 == '1' && !ascii && format) {
		cerr << "Error : Trying to read an ascii PBM into a binary PBM";
		exit(1);
	} else if(ich2 == '4' && ascii && format) {
		cerr << "Error : Trying to read a binary PBM into an ascii PBM";
		exit(1);
	} else if(ich2 != '1' && ich2 != '4') {
		cerr << "Error : Wrong file format";
		exit(1);
	} else if(ich2 == '1') {
		ascii = true;
		format = true;
	} else {
		ascii = false;
		format = true;
	}

	
	if(!(in >> cols >> rows)) {
		cerr << "Eror : Bad file format";
	}
	
	if((px = (unsigned char*)malloc(cols * rows * sizeof(unsigned char))) == NULL) {
		cerr << "Allocation error";
		exit(1);
	}
	
	uint8_t tmp;
	
	for(size_t i = 0; i < rows; ++i) {
		for(size_t j = 0; j < cols; ++j) {
			if(!(in >> tmp)) {
				cerr << "Bad file format";
				free(px);
				exit(1);
			}
			
			px[i * cols + j] = tmp;
			
		}
	}

}

void PBM::write(const std::string& filename) {
	
	std::ofstream out(filename.c_str());
	
	out << 'P' << (ascii && format ? '1' : '4') << endl;
	out << cols << " " << rows << " " << endl;
	
	for(uint32_t i = 0; i < rows; ++i) {
		for(uint32_t j = 0; j < cols; ++j) {
			uint8_t pixel = px[i * cols + j] + (ascii && format ? '0' : 0);
			out << pixel;
			if(ascii && format) out << " ";
			cout << px[i * cols + j] << " ";
		}
		cout << endl;
		out << endl;
	}
}
