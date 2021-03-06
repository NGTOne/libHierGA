#include <random>
#include <sstream>
#include "nodes/EANode/systems/SSGA.hpp"
#include "core/utils/HierGC.hpp"

SSGA::SSGA(
	SelectionStrategy* strategy,
	CrossoverOperation* cross,
	MutationOperation* mutation
) : EvolutionarySystem(strategy, cross, mutation) {
	this->niching = NULL;
}

SSGA::SSGA(
	SelectionStrategy* strategy,
	CrossoverOperation* cross,
	MutationOperation* mutation,
	NichingStrategy* niching
) : EvolutionarySystem(strategy, cross, mutation) {
	this->niching = niching;
}

void SSGA::registerInternalObjects() {
	EvolutionarySystem::registerInternalObjects();
	HierGC::registerObject(this->niching);
}

std::vector<Genome*> SSGA::breedMutateSelect(
	std::vector<Genome*> initialPopulation,
	std::vector<Fitness>& populationFitnesses,
	std::vector<ObjectiveFunction*> objectives,
	std::string speciesNode
) {
	std::vector<Genome*> finalPopulation(initialPopulation.size(), NULL);
	std::vector<unsigned int> parentIndices;
	std::vector<Genome*> parents, children;

	for (int i = 0; i < 2; i++) {
		parentIndices.push_back(this->getParent(
			initialPopulation,
			populationFitnesses
		));
		parents.push_back(initialPopulation[parentIndices[i]]);
	}

	children = this->produceChildren(parents, speciesNode);

	std::vector<unsigned int> replacementIndices = parentIndices;
	if (niching != NULL)
		replacementIndices = this->niching->getIndices(
			initialPopulation,
			children
		);

	std::vector<Fitness> newFitnesses;

	for (unsigned int i = 0; i < children.size(); i++) {
		newFitnesses.push_back(this->evaluateFitness(
			children[i],
			objectives
		));

		if (newFitnesses[i]
			> populationFitnesses[replacementIndices[i]]
		) {
			finalPopulation[replacementIndices[i]] = children[i];
			populationFitnesses[replacementIndices[i]]
				= newFitnesses[i];
		} else {
			delete(children[i]);
		}
	}

	for (unsigned int i = 0; i < finalPopulation.size(); i++)
		if (finalPopulation[i] == NULL)
			finalPopulation[i] = new Genome(initialPopulation[i]);

	return finalPopulation;
}

bool SSGA::hasNiching() {
	return true;
}

NichingStrategy* SSGA::getNiching() {
	return this->niching;
}
