#include "core/utils/HierRNG.hpp"
#include <chrono>
#include <random>

unsigned int HierRNG::seed;
std::mt19937 HierRNG::generator;

void HierRNG::initialize() {
	HierRNG::seed = std::chrono::system_clock::now()
		.time_since_epoch().count();
	HierRNG::generator = std::mt19937(HierRNG::seed);
}

void HierRNG::initialize(unsigned int seed) {
	HierRNG::seed = seed;
	HierRNG::generator = std::mt19937(HierRNG::seed);
}

double HierRNG::gaussian(double mean, double stdDev) {
	std::normal_distribution<double> dist(mean, stdDev);
	return dist(HierRNG::generator);
}

std::vector<double> HierRNG::gaussianVector(
	double mean,
	double stdDev,
	unsigned int count
) {
	return std::vector<double>(HierRNG::gaussian(mean, stdDev), count);
}

unsigned int HierRNG::index(unsigned int maxIndex) {
	return HierRNG::uniform<unsigned int>(0, maxIndex);
}

bool HierRNG::binary() {
	return HierRNG::zeroOne<unsigned int>();
}

bool HierRNG::chooseWithProb(double probability) {
	return HierRNG::chooseWithProb(probability, true, false);
}
