#include <vector>
#include <complex>
#include <string>
#include <iostream>
#include "bmp_write.h"
using namespace std;

typedef complex<double> compl;

int main() {
	/*PLOTTING PARAMETERS*/
	int maxiters = 30;
	double minreal = -3.2, maxreal = 3.2, minimag = -1.8, maximag = 1.8;
	int imagewidth = 1920;
	int imageheight = 1080;
	double tolerance = 0.0001;
	vector<compl> coeffs = { 1,0,-2,2 };
	string filename = "x3 - 2x + 2 (30 iterations).bmp";
	compl a(1, 0);
	/*END OF PLOTTING PARAMETERS*/

	/*WORKING VARIABLES*/
	compl z,f_z,dfdz_z,delta;
	int degree = coeffs.size() - 1, n;
	int converged;
	int iters;
	char r, g, b;
	int absi, argi, colour_no;
	double fact;

	vector<char> blank_pixel(3, 0);
	vector<char> current_pixel(3, 0);
	vector<vector<char> > blank_row(imagewidth,blank_pixel);
	vector<vector<vector<char> > > bitmap(imageheight,blank_row);
	/*END OF WORKING VARIABLES*/
	
	for (int y = 0; y < imageheight; y++) {
		for (int x = 0; x < imagewidth; x++) {
			z = compl((maxreal-minreal)*((double)x / (double)imagewidth) +minreal, (maximag-minimag)*((double)y / (double)imageheight) + minimag);
			converged = 0;

			for (iters = 0; iters < maxiters; iters++) {
				f_z = compl(0, 0);
				dfdz_z = compl(0, 0);
				n = degree;

				for (auto it = coeffs.begin(); it != coeffs.end(); it++) {
					dfdz_z += (*it) * (double)n * pow(z, n - 1);
					f_z += (*it) * pow(z, n);
					n -= 1;
				}

				if (abs(dfdz_z)) {
					delta = a * f_z / dfdz_z;

					if (abs(delta) < tolerance) {
						/*Point converged to a root.*/
						converged = iters;
						break;
					}
					z -= delta;
				}
				else {
					/*Point cannot converge*/
					converged = 0;
					break;
				}
			}
			if (converged) {
				current_pixel = hsl_to_rgb((arg(z) + 3.1415927) / 6.2831854, 0.75, 0.5);
				fact = 1 - (((double)converged + abs(delta)/tolerance) / (double)maxiters);
				fact *= fact;
				current_pixel[0] *= fact;
				current_pixel[1] *= fact;
				current_pixel[2] *= fact;
			}
			else {
				current_pixel = { 0,0,0 };
			}
			bitmap[y][x] = current_pixel;
		}
		if (!(y % 50)) cout << y * 100 / imageheight << "% done..." << endl;
	}
	write_to_file(filename, bitmap);
	return 0;
}