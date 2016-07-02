#include "../nodes/EANode/MutationOperation.hpp"
#pragma once

/*
* Gaussian mutation changes the value of the allele by randomly generating
* a number on the normal distribution and adding it to the allele's value
*/

class GaussianMutation : public MutationOperation {
	private:

	protected:
	bool endReflection;

	virtual unsigned int getNewLocusValue(
		unsigned int currentValue,
		unsigned int largestPossibleValue
	);

	void init(bool endReflection);

	public:
	GaussianMutation();
	GaussianMutation(bool endReflection);
	GaussianMutation(double mutationRate);
	GaussianMutation(double mutationRate, bool endReflection);
	GaussianMutation(double mutationRate, unsigned int seed);
	GaussianMutation(
		double mutationRate,
		bool endReflection,
		unsigned int seed
	);
};
