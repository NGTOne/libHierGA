#include "../core/GenerationModel.hpp"
#include <random>
#include <string>
#pragma once

class ESGeneration : public GenerationModel {
	private:

	protected:

	public:

	ESGeneration(int newNumElites, SelectionStrategy * newStrategy);

	ESGeneration(unsigned newSeed, int newNumElites, SelectionStrategy * newStrategy);

	Individual ** breedMutateSelect(Individual ** initialPopulation, int * populationFitnesses, int populationSize);
};
