#ifndef NODES_EANODE_SYSTEMS_SSGA
#define NODES_EANODE_SYSTEMS_SSGA

#include "../EvolutionarySystem.hpp"
#include "niching/NichingStrategy.hpp"
#include <random>
#include <string>
#include <vector>

/*
* In the steady-state GA model, in which one pair
* of parents is directly replaced by their offspring in each generation. For
* this reason, it is much slower than other models, but can yield results
* that may not appear using them.
*/

class SSGA : public EvolutionarySystem {
	private:

	protected:
	NichingStrategy* niching;

	public:
	SSGA(
		SelectionStrategy* strategy,
		CrossoverOperation* cross,
		MutationOperation* mutation
	);
	SSGA(
		SelectionStrategy* strategy,
		CrossoverOperation* cross,
		MutationOperation* mutation,
		NichingStrategy* niching
	);

	virtual void registerInternalObjects();

	virtual std::vector<Genome*> breedMutateSelect(
		std::vector<Genome*> initialPopulation,
		std::vector<Fitness>& populationFitnesses,
		std::vector<ObjectiveFunction*> objectives,
		std::string speciesNode
	);

	bool hasNiching();
	NichingStrategy* getNiching();
};

#endif
