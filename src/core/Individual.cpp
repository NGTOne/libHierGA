#include "Individual.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//Empty constructor
Individual::Individual() {}

//Basic constructor - lets us have a completely generic Individual that doesn't
//know what the heck is going on inside it
Individual::Individual(GenePool ** newGenePools, int newGenomeLength, CrossoverOperation * newCrossover, MutationOperation * newMutation, FitnessFunction * newFitness) {
	init(newGenePools, newGenomeLength, newCrossover, newMutation, newFitness);
}
//Constructor that lets us create an Individual with a fully specified genome
//Necessary for crossover/mutation
Individual::Individual(GenePool ** newGenePools, int newGenomeLength, CrossoverOperation * newCrossover, MutationOperation * newMutation, FitnessFunction * newFitness, int newGenome[]) {
	init(newGenePools, newGenomeLength, newCrossover, newMutation, newFitness);

	for (int i = 0; i < genomeLength; i++) {
		genome[i] = newGenome[i];
        }
}

Individual::~Individual() {
	free(genome);
}

void Individual::init(GenePool ** newGenePools, int newGenomeLength, CrossoverOperation * newCrossover, MutationOperation * newMutation, FitnessFunction * newFitness) {
	if (newGenePools != NULL) {
                genomeLength = newGenomeLength;

                if (genomeLength >= 2) {
                        myGenePools = newGenePools;
                        genome = (int*)malloc(sizeof(int)*genomeLength);

                        //This way, we don't actually need to know how many
                        //genes are in each pool - it can be 2 or 2000
                        for (int i = 0; i < genomeLength; i++) {
                                genome[i] = myGenePools[i]->chooseRandomMember();
                        }
                }
        }

        myCrossover = newCrossover;
        myMutation = newMutation;
        myFunction = newFitness;
}

//Exactly what it says on the tin - wraps around the CrossoverOperation
//and spits out two offspring (which are new instances of Individual)
//Returns NULL if the two parents have different gene pools
Individual ** Individual::crossoverOperation(Individual * otherParent) {
	int * otherGuysGenome;
	int ** kidsGenome;
	Individual ** kids = (Individual**)malloc(sizeof(Individual*)*2);
	int otherGuysLength;

	if (sameSpecies(otherParent) == false) {
		return NULL;
	}

	otherGuysGenome = otherParent->getGenome();
	otherGuysLength = otherParent->getGenomeLength();

	kidsGenome = myCrossover->crossOver(genome, otherGuysGenome, genomeLength, otherGuysLength);
	
	kids[0] = new Individual(myGenePools, genomeLength, myCrossover, myMutation, myFunction, kidsGenome[0]);
	kids[1] = new Individual(myGenePools, genomeLength, myCrossover, myMutation, myFunction, kidsGenome[1]);
	
	free(kidsGenome[0]);
	free(kidsGenome[1]);
	free(kidsGenome);

	return kids;
}

//Wraps around the MutationOperation object and spits out a mutant
//version of this Individual (which is a new instance of Individual)
Individual * Individual::mutationOperation() {
	int * mutantGenome;
	int maxValues[genomeLength];

	for (int i = 0; i < genomeLength; i++) {
		maxValues[i] = myGenePools[i]->getPopulationSize()-1;
	}

	mutantGenome = myMutation->mutate(genome, maxValues, genomeLength);
	Individual * mutant = new Individual(myGenePools, genomeLength, myCrossover, myMutation, myFunction, mutantGenome);

	free(mutantGenome);

	return mutant;
}

int Individual::checkFitness() {
	int myFitness;

	myFitness = myFunction->checkFitness(myGenePools, genome, genomeLength);

	return myFitness;
}

Individual * Individual::deepCopy() {
	Individual * myCopy = makeSpecifiedCopy(genome);

	return myCopy;
}

//For populating HierarchicalGenePools - basically, use this Individual as a
//template, just generate a new genome for it
Individual * Individual::makeRandomCopy() {
	Individual * myCopy = new Individual(myGenePools, genomeLength, myCrossover, myMutation, myFunction);

	return myCopy;
}

//In case we need to copy a given Individual's gene pool/function set, but with
//a new genome
//WARNING - THIS IS VERY DANGEROUS IF DONE MANUALLY
//If the chromosomes in newGenome are out of range for their gene pools, bad
//things will happen
Individual * Individual::makeSpecifiedCopy(int newGenome[]) {
	Individual * myCopy = new Individual(myGenePools, genomeLength, myCrossover, myMutation, myFunction, newGenome);

	return myCopy;
}

//Necessary for species verification
GenePool ** Individual::getGenePoolList() {
	return myGenePools;
}

//Necessary for crossover
int * Individual::getGenome() {
	return genome;
}

void Individual::runHierarchicalGenerations() {
	for (int i = 0; i < genomeLength; i++) {
		myGenePools[i]->runGenerations();
	}
}

//In a hierarchical GA, it's important to know whether or not two individuals
//are from the same part of the hierarchy before attempting to breed them
//Comparing which gene pools they draw from provides an easy way to do this
bool Individual::sameSpecies(Individual * otherIndividual) {
	int otherGuysLength = otherIndividual->getGenomeLength();
	GenePool ** otherGuysPools = otherIndividual->getGenePoolList();

	//First, a rough check
	if (otherGuysLength != genomeLength) {
		return false;
	}

	//Now, more detailed inspection
	for (int i = 0; i < genomeLength; i++) {
		if (myGenePools[i]->equals(otherGuysPools[i]) == false) {
			return false;
		}
	}

	return true;
}

int Individual::getGenomeLength() {
	return genomeLength;
}

string Individual::toGenomeString() {
	return myFunction->toString(myGenePools, genome, genomeLength);
}

string Individual::toString() {
	string returnString;
	stringstream ss;

	ss << "Genome: " << toGenomeString() << "\nCrossover information:\n" << myCrossover->toString() << "Mutation information:\n" << myMutation->toString() << "";

	returnString = ss.str();

	return returnString;
}
