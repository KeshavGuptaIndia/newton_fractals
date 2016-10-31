#pragma once
#include <string>
#include <fstream>
#include <vector>
using namespace std;

void write_to_file(string filename, vector<vector<vector<char> > > & bmp_data) {
	
	ofstream outbitmap(filename, ios::binary);
	
	int imageheight = bmp_data.size();
	int imagewidth = bmp_data[0].size();
	
	int paddedwidth = (imagewidth * 3) + ((imagewidth * 3) % 4 ? ((-imagewidth * 3) % 4 + 4) : 0);
	int padcells = paddedwidth - 3 * imagewidth;
	int sizeImg = paddedwidth*imageheight + 54;
	char padding = 0;

	char bmpHdr[54] = { 66,  77,  sizeImg % 256,  (sizeImg / 256) % 256,   (sizeImg / 65536) % 256,   (sizeImg / 16777216) % 256,   0,   0,   0,   0,  54,   0,   0,   0,  40,   0,
		0,   0,   imagewidth % 256,   imagewidth / 256,   0,   0,   imageheight % 256,   imageheight / 256,   0,   0,   1,   0,  24,   0,   0,   0,
		0,   0,   (sizeImg - 54) % 256,  ((sizeImg - 54) / 256) % 256,   ((sizeImg - 54) / 65536) % 256,   ((sizeImg - 54) / 16777216) % 256,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0
	};

	outbitmap.write((char*)bmpHdr, sizeof(bmpHdr));

	for (auto y = bmp_data.begin(); y != bmp_data.end(); y++)
	{
		for (auto x = y->begin(); x != y->end(); x++){
			outbitmap.write(&x->operator[](2), 1);
			outbitmap.write(&x->operator[](1), 1);
			outbitmap.write(&x->operator[](0), 1);
		}			
		for (int i = 0; i < padcells; i++){
			outbitmap.write(&padding, 1);
		}
	}
	outbitmap.close();
}

vector<char> hsl_to_rgb(double h, double sl, double l)
{
	double v;
	double r, g, b;

	r = l;   // default to gray
	g = l;
	b = l;
	v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
	if (v > 0)
	{
		double m;
		double sv;
		int sextant;
		double fract, vsf, mid1, mid2;

		m = l + l - v;
		sv = (v - m) / v;
		h *= 6.0;
		sextant = (int)h;
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant)
		{
		case 0:
			r = v;
			g = mid1;
			b = m;
			break;
		case 1:
			r = mid2;
			g = v;
			b = m;
			break;
		case 2:
			r = m;
			g = v;
			b = mid1;
			break;
		case 3:
			r = m;
			g = mid2;
			b = v;
			break;
		case 4:
			r = mid1;
			g = m;
			b = v;
			break;
		case 5:
			r = v;
			g = m;
			b = mid2;
			break;
		case 6:
			r = v;
			g = m;
			b = mid2;
			break;
		}
	}
	vector<char> rgb;
	rgb.push_back(r * 255.0);
	rgb.push_back(g * 255.0);
	rgb.push_back(b * 255.0);
	return rgb;
}