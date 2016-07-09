#include "../EvolutionarySystem.hpp"
#include "niching/NichingStrategy.hpp"
#include <random>
#include <string>
#include <vector>
#pragma once

/*
* In the steady-state GA model, in which one pair
* of parents is directly replaced by their offspring in each generation. For
* this reason, it is much slower than other models, but can yield results
* that may not appear using them.
*/

class SSGA : public EvolutionarySystem {
	private:

	protected:
	NichingStrategy * niching;

	public:
	SSGA(SelectionStrategy * strategy);
	SSGA(SelectionStrategy * strategy, unsigned int seed);
	SSGA(SelectionStrategy * strategy, NichingStrategy * niching);
	SSGA(
		SelectionStrategy * strategy,
		NichingStrategy * niching,
		unsigned int seed
	);

	virtual std::vector<Genome*> breedMutateSelect(
		std::vector<Genome*> initialPopulation,
		std::vector<float> & populationFitnesses,
		CrossoverOperation * cross,
		MutationOperation * mutation,
		std::vector<ObjectiveFunction*> objectives,
		std::string speciesNode
	);

	bool hasNiching();
	NichingStrategy * getNiching();
};