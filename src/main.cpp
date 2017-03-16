#include "Image.hpp"
#include "Pixel.hpp"

#include <iostream>

using namespace std;

void tests() {
	Image pbm;
	Image pgm;
	Image ppm;
	
	pbm.read_pbm("test.pbm");
	pgm.read_pgm("test.pgm");
	ppm.read_ppm("test.ppm");
	
	pbm.write_pbm_ascii("test_ascii.pbm");
	pgm.write_pgm_ascii("test_ascii.pgm");
	ppm.write_ppm_ascii("test_ascii.ppm");
	
	pbm.write_pbm_bin("test_bin.pbm");
	pgm.write_pgm_bin("test_bin.pgm");
	ppm.write_ppm_bin("test_bin.ppm");
	
}

int main() 
{
	Image lena1, lena2, lena3;
	
	lena1.read_ppm("lena.ppm");
	
	lena1.sobel_grad();
	
	lena1.write_ppm_bin("lena_sobel_grad.ppm");
	
	lena2.read_ppm("lena.ppm");
	
	lena2.sobel_filter_x();
	
	lena2.write_ppm_bin("lena_sobel_x.ppm");
	
	lena3.read_ppm("lena.ppm");
	
	lena3.sobel_filter_y();
	
	lena3.write_ppm_bin("lena_sobel_y.ppm");
}
