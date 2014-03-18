#include <cstdio>
#include <iostream>
#include "NonHierarchicalGenePool.h"
#include "HierarchicalGenePool.h"
#include "BitwiseMutation.h"
#include "TwoPointCrossover.h"
#include "TournamentSelection.h"
#include "1maxFitness.h"

int main(void) {
	SelectionStrategy * myStrategy = new TournamentSelection(0.5, 2);

	FitnessFunction * myFunction = new OneMaxFitness();
	CrossoverOperation * myCrossover = new TwoPointCrossover();
	MutationOperation * myMutation = new BitwiseMutation();

	int binaryDigits[] = {0,1};

	GenePool ** baseGenes = (GenePool**)malloc(sizeof(GenePool*)*32);

	for (int i = 0; i < 32; i++) {
		baseGenes[i] = new NonHierarchicalGenePool<int>(binaryDigits, 2);
	}

	Individual * templateIndividual = new Individual(baseGenes, 32, myCrossover, myMutation, myFunction);

	HierarchicalGenePool * topLevelPool = new HierarchicalGenePool(64, templateIndividual, 100, 1, myStrategy);

	for (int i = 0; i < 120; i++) {
		topLevelPool->runGenerations();
	}
}
