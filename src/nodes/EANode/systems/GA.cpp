#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include "nodes/EANode/systems/GA.hpp"
#include "core/HierRNG.hpp"

using namespace std;

GA::GA(
	unsigned int numElites,
	bool randomElitePlacement,
	SelectionStrategy * strategy,
	CrossoverOperation * cross,
	MutationOperation * mutation
) : EvolutionarySystem(strategy, cross, mutation) {
	this->init(numElites, randomElitePlacement);
}

GA::GA(
	unsigned int numElites,
	SelectionStrategy * strategy,
	CrossoverOperation * cross,
	MutationOperation * mutation
) : EvolutionarySystem(strategy, cross, mutation) {
	this->init(numElites, false);
}

void GA::init(unsigned int numElites, bool randomElitePlacement) {
	this->numElites = numElites;
	this->randomElitePlacement = randomElitePlacement;
}

void GA::placeElites(
	vector<Genome*> initialPopulation,
	vector<float> initialPopulationFitnesses,
	vector<Genome*> & newPopulation,
	vector<float> & newPopulationFitnesses
) {
	vector<unsigned int> eliteLocations = this->findElites(
		initialPopulationFitnesses
	);

	for (unsigned int i = 0; i < eliteLocations.size(); i++) {
		if (!this->randomElitePlacement) {
			newPopulation[i] = new Genome(
				initialPopulation[eliteLocations[i]]
			);
			newPopulationFitnesses[i] =
				initialPopulationFitnesses[eliteLocations[i]];
		} else {
			unsigned int index;

			do {
				index = HierRNG::index(
					initialPopulation.size() - 1
				);
			} while (newPopulation[index] != NULL);
			newPopulation[index] = new Genome(
				initialPopulation[eliteLocations[i]]
			);
			newPopulationFitnesses[index] =
				initialPopulationFitnesses[eliteLocations[i]];
		}
	}
}

vector<unsigned int> GA::findElites(vector<float> fitnesses) {
	unsigned int populationSize = fitnesses.size(), bestFitnessIndex = 0;
	float bestFitness;
	bool eliteLocations[populationSize];
	vector<unsigned int> eliteIndexes;
	for (unsigned int i = 0; i < populationSize; i++)
		eliteLocations[i] = false;
	unsigned int trueNumElites = this->numElites >= fitnesses.size() ?
		fitnesses.size()/2 : this->numElites;

	for (unsigned int i = 0; i < trueNumElites; i++) {
		bestFitness = 0;
		for (unsigned int k = 0; k < populationSize; k++) {
			if (
				fitnesses[k] > bestFitness
				&& eliteLocations[k] == false
			) {
				bestFitnessIndex = k;
				bestFitness  = fitnesses[k];
			}
		}

		eliteLocations[bestFitnessIndex] = true;
		eliteIndexes.push_back(bestFitnessIndex);
	}

	return eliteIndexes;
}

vector<Genome*> GA::breedMutateSelect(
	vector<Genome*> initialPopulation,
	vector<float> & populationFitnesses,
	vector<ObjectiveFunction*> objectives,
	std::string speciesNode
) {
	vector<Genome*> newPopulation(initialPopulation.size(), NULL);
	vector<float> newFitnesses(initialPopulation.size(), 0);

	this->placeElites(
		initialPopulation,
		populationFitnesses,
		newPopulation,
		newFitnesses
	);

	unsigned int numOffspring = 0;
	while(numOffspring < initialPopulation.size()) {
		vector<Genome*> parents, children;
		for (int i = 0; i < 2; i++) parents.push_back(
			initialPopulation[this->getParent(
				initialPopulation,
				populationFitnesses
			)]
		);

		children = this->produceChildren(parents, speciesNode);

		for (unsigned int i = 0; i < children.size(); i++) {
			while (newPopulation[numOffspring] != NULL)
				numOffspring++;
			if (numOffspring < initialPopulation.size())
				newPopulation[numOffspring++] = children[i];
		}
	}

	for (unsigned int i = 0; i < newPopulation.size(); i++)
		populationFitnesses[i] = this->evaluateFitness(
			newPopulation[i],
			objectives
		);

	return newPopulation;
}
