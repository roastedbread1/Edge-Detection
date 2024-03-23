#include "image.h"

int main(int argc, char** argv) {
	Image test("PETA2.png");
	


	Image gray = test;
	gray.grayscale();
	gray.write("gray_lum.jpg");
	gray.edge_detection();
	gray.write("gray.jpg");

	Image bww = test;
	bww.blacknwhite();
	bww.write("bww.jpg");
	bww.edge_detection();
bww.write("bw.jpg");


	return 0;
}