#ifndef Polynomial_h
#define Polynomial_h

#define BIN_PRECISION 0.0000000001

#include <vector>

class Polynomial {
	private:
		double * coefs;
		int numCoefs;
		double binarySearch(double min, double max, bool inc);
	public:
		Polynomial(double * cfs, int numCfs);
		std::vector<double> getRoots();
		double getValue(double x);
};

#endif
