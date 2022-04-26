#include "functions.h"
void generateMandelbrotSet(MandelbrotParams* params)
{
	int index { }; //Indeks tablicy
	for (int y = params->startY; y < params->dim; y++) { //
		for (int x = 0; x < params->dim; x++) {
			double a = remap(x, 0, params->dim, -params->scale, params->scale),
				b = remap(y, 0, params->dim, -params->scale, params->scale),
				initA = a, 
				initB = b;
			int n{};
			while (n < params->iterations) {
				double tempA = a * a - b * b,
					tempB = 2.0 * a * b;
				a = tempA + initA;
				b = tempB + initB;
				if (a * a + b * b > params->depth)
					break;
				n++;
			}
			int brightness = remap(std::sqrt(remap(n, 0.0, params->iterations, 0.0, 1.0)), 0.0, 1.0, 0.0, 255.0);
			if (n == params->iterations)
				brightness = 0;
			params->arr[index++] = brightness;
			if (index == params->size)
				return;
		}
	}
}

void generateJuliaSet(JuliaParams* params)
{
		int ind{};
		for (int i = params->startY; i < params->dim; i++) {
			double y = params->minimal;
			for (int j = 0; j < params->dim; j++) {
				double a = y,
					b = params->x;
				int n{};
				while (n < params->iterations) {
					double twoab = 2.0 * a * b;
					a = (a * a - b * b) + params->real;
					b = twoab + params->imaginary;
					if (a * a + b * b > params->depth)
						break;
					n++;
				}
				int brightness = remap(std::sqrt(remap(n, 0.0, params->iterations, 0.0, 1.0)), 0.0, 1.0, 0.0, 255.0);
				if (n == params->iterations)
					brightness = 0;
				y += params->d;
				params->arr[ind++] = brightness;
				if (ind == params->size)
					return;
			}
			params->x += params->d;
		}
}
