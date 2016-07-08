#include "pop-formulae/LocusBasePopFormula.hpp"
#include "exception/ValueOutOfRangeException.hpp"
#include <cmath>

LocusBasePopFormula::LocusBasePopFormula(unsigned int exponent) {
	if (exponent <= 1) throw ValueOutOfRangeException();
	this->exponent = exponent;
}

unsigned int LocusBasePopFormula::getPopulationSize(unsigned int numLoci) {
	return std::pow(numLoci, this->exponent);
}
