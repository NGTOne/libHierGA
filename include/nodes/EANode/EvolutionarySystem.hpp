#include "../../core/Genome.hpp"
#include "SelectionStrategy.hpp"
#include "CrossoverOperation.hpp"
#include "../../core/ObjectiveFunction.hpp"
#include "MutationOperation.hpp"
#include <random>
#include <string>
#include <vector>
#pragma once

class EvolutionarySystem {
	private:

	protected:
	unsigned seed;
	std::mt19937 generator;
	SelectionStrategy * strategy;

	void init(SelectionStrategy * strategy, unsigned seed);
	void sortPopulation(
		std::vector<Genome*> &population,
		std::vector<int> &fitnesses
	);
	int evaluateFitness(
		Genome * target,
		std::vector<ObjectiveFunction*> objectives
	);

	virtual unsigned int getParent(
		std::vector<Genome*> population,
		std::vector<int> fitnesses
	);

	std::vector<Genome*> produceChildren(
		std::vector<Genome*> parents,
		CrossoverOperation * cross,
		MutationOperation * mutation
	);

	public:
	EvolutionarySystem(SelectionStrategy * strategy);
	EvolutionarySystem(SelectionStrategy * strategy, unsigned seed);
	virtual ~EvolutionarySystem();

	virtual std::vector<Genome*> breedMutateSelect(
		std::vector<Genome*> initialPopulation,
		std::vector<int> & populationFitnesses,
		CrossoverOperation * cross,
		MutationOperation * mutation,
		std::vector<ObjectiveFunction*> objectives
	)=0;

	std::string toString();
	SelectionStrategy * getSelectionStrategy();
	virtual bool hasNiching();
};
