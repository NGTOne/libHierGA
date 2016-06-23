#include <random>
#include "systems/StrongIterativeReplacingGA.hpp"

using namespace std;

StrongIterativeReplacingGA::StrongIterativeReplacingGA(
	SelectionStrategy * strategy
) : ReplacingGA(strategy) {
	this->scramble = false;
}

StrongIterativeReplacingGA::StrongIterativeReplacingGA(
	SelectionStrategy * strategy,
	unsigned seed
) : ReplacingGA(strategy, seed) {
	this->scramble = false;
}

StrongIterativeReplacingGA::StrongIterativeReplacingGA(
	SelectionStrategy * strategy,
	bool scramble
) : ReplacingGA(strategy) {
	this->scramble = scramble;
}

StrongIterativeReplacingGA::StrongIterativeReplacingGA(
	SelectionStrategy * strategy,
	bool scramble,
	unsigned seed
) : ReplacingGA(strategy, seed) {
	this->scramble = scramble;
}

void StrongIterativeReplacingGA::scramblePopulation(
	std::vector<Genome*> & population,
	std::vector<int> & fitnesses
) {
	uniform_int_distribution<unsigned int> placementDist(
		0,
		population.size() - 1
	);
	Genome * temp;
	unsigned int index;
	int tempFitness;

	for (unsigned int i = 0; i < population.size(); i++) {
		index = placementDist(this->generator);
		temp = population[i];
		tempFitness = fitnesses[i];
		population[i] = population[index];
		fitnesses[i] = fitnesses[index];
		population[index] = temp;
		fitnesses[index] = tempFitness;
	}
}

vector<Genome*> StrongIterativeReplacingGA::breedMutateSelect(
	vector<Genome*> initialPopulation,
	vector<int> & populationFitnesses,
	CrossoverOperation * cross,
	MutationOperation * mutation,
	vector<ObjectiveFunction*> objectives,
	std::string speciesNode
) {
	vector<Genome*> newPopulation, children;
	vector<unsigned int> parentIndices(2, 0);

	for (unsigned int i = 0; i < initialPopulation.size(); i++)
		newPopulation.push_back(new Genome(initialPopulation[i]));

	for (unsigned int i = 0; i < initialPopulation.size(); i++) {
		parentIndices[0] = i;
		parentIndices[1] = this->getParent(
			initialPopulation,
			populationFitnesses
		);

		vector<Genome*> parents;
		for (unsigned int k = 0; k < parentIndices.size(); k++)
			parents.push_back(initialPopulation[parentIndices[k]]);

		children = this->produceChildren(
			parents,
			cross,
			mutation,
			speciesNode
		);

		for (unsigned int k = 0; k < children.size(); k++) {
			int childFitness = this->evaluateFitness(
				children[k],
				objectives
			);

			if (
				childFitness >
				populationFitnesses[parentIndices[k]]
			) {
				delete(newPopulation[parentIndices[k]]);
				newPopulation[parentIndices[k]] = children[k];
				populationFitnesses[parentIndices[k]] =
					childFitness;
			} else {
				delete(children[k]);
			}
		}
	}

	if (this->scramble)
		this->scramblePopulation(newPopulation, populationFitnesses);

	return(newPopulation);
}
