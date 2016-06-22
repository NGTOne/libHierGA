#include "core/PopulationNode.hpp"
#include <sstream>
#include <iostream>

using namespace std;

PopulationNode::PopulationNode(
	unsigned int populationSize,
	vector<Locus*> loci,
	vector<ObjectiveFunction*> objectives,
	ToStringFunction * populationToString,
	vector<EndCondition*> conditions,
	string nodeName
) {
	this->init(
		populationSize,
		loci,
		objectives,
		populationToString,
		conditions,
		nodeName,
		1
	);
}

PopulationNode::PopulationNode(
	unsigned int populationSize,
	vector<Locus*> loci,
	vector<ObjectiveFunction*> objectives,
	ToStringFunction * populationToString,
	vector<EndCondition*> conditions,
	string nodeName,
	unsigned int accelerationFactor
) {
	this->init(
		populationSize,
		loci,
		objectives,
		populationToString,
		conditions,
		nodeName,
		accelerationFactor
	);
}

PopulationNode::~PopulationNode() {
	for (unsigned int i = 0; i < this->population.size(); i++)
		delete(this->population[i]);
}

void PopulationNode::init(
	unsigned int populationSize,
	vector<Locus*> loci,
	vector<ObjectiveFunction*> objectives,
	ToStringFunction * populationToString,
	vector<EndCondition*> conditions,
	string nodeName,
	unsigned int accelerationFactor
) {
	this->initialPopulationSize = populationSize;
	this->objectives = objectives;
	this->populationToString = populationToString;
	this->conditions = conditions;
	this->nodeName = nodeName;
	this->accelerationFactor = accelerationFactor;
	this->currentIteration = 0;
	this->readOnce = false;

	this->createLoci(loci);
}

int PopulationNode::evaluateFitness(Genome * target) {
	int total = 0;
	for (unsigned int i = 0; i < this->objectives.size(); i++)
		total += this->objectives[i]->checkFitness(target);

	return total;
}

int PopulationNode::evaluateFitness(int solutionIndex) {
	Genome * genome = this->getIndex(solutionIndex);
	return this->evaluateFitness(genome);
}

void PopulationNode::createLoci(vector<Locus*> loci) {
	for (unsigned int i = 0; i < this->population.size(); i++) 
		delete(this->population[i]);
	this->population.clear();
	this->fitnesses = vector<int>(this->initialPopulationSize, 0);

	for (unsigned int i = 0; i < this->initialPopulationSize; i++)
		this->population.push_back(new Genome(loci, this->nodeName));
	this->evaluateFitnesses();
}

void PopulationNode::addObjective(ObjectiveFunction * objective) {
	this->objectives.push_back(objective);
	this->evaluateFitnesses();
}

void PopulationNode::addObjectives(vector<ObjectiveFunction*> objectives) {
	for (unsigned int i = 0; i < objectives.size(); i++)
		this->objectives.push_back(objectives[i]);
	this->evaluateFitnesses();
}

void PopulationNode::setObjective(ObjectiveFunction * objective) {
	this->objectives.clear();
	this->addObjective(objective);
}

void PopulationNode::setObjectives(vector<ObjectiveFunction*> objectives) {
	this->objectives.clear();
	this->addObjectives(objectives);
}

void PopulationNode::addEndCondition(EndCondition * condition) {
	this->conditions.push_back(condition);
}

void PopulationNode::addEndConditions(vector<EndCondition*> conditions) {
	for (unsigned int i = 0; i < conditions.size(); i++)
		this->conditions.push_back(conditions[i]);
}

void PopulationNode::setEndCondition(EndCondition * condition) {
	this->conditions.clear();
	this->addEndCondition(condition);
}

void PopulationNode::setEndConditions(vector<EndCondition*> conditions) {
	this->conditions.clear();
	this->addEndConditions(conditions);
}

void PopulationNode::addLoci(vector<Locus*> loci) {
	vector<Locus*> templateLoci = this->population[0]->getLoci();
	for (unsigned int i = 0; i < loci.size(); i++)
		templateLoci.push_back(loci[i]);

	this->createLoci(templateLoci);
}

void PopulationNode::setLoci(vector<Locus*> loci) {
	this->createLoci(loci);
}

void PopulationNode::setToString(ToStringFunction * populationToString) {
	this->populationToString = populationToString;
}

void PopulationNode::replacePopulation() {
	vector<Genome*> tempPopulation = this->getNextPopulation();

	for (unsigned int i = 0; i < this->population.size(); i++)
		delete(this->population[i]);

	this->population = tempPopulation;
}

void PopulationNode::nextIteration() {
	unsigned int i = 0;
	while (!done() && i++ < this->accelerationFactor) {
		this->evaluateFitnesses();
		this->replacePopulation();
		this->evaluateFitnesses();
		this->currentIteration++;
	}
}

Genome * PopulationNode::getIndex(int index) {
	return this->population[index];
}

bool PopulationNode::done() {
	// TODO: Allow logical operations (AND/OR) of end conditions
	for (unsigned int i = 0; i < this->conditions.size(); i++) {
		if (this->conditions[i]->checkCondition(
			this->population,
			this->fitnesses,
			this->currentIteration
		) == false) {
			return false;
		}
	}

	return true;
}

// Since our n is reasonably small, the O(n^2) bubblesort will suffice
void PopulationNode::sortPopulation() {
	for (unsigned int i = 0; i < this->population.size(); i++) {
		for (unsigned int k = 0; k < this->population.size(); k++) {
			if (this->fitnesses[i] > this->fitnesses[k]) {
				int tempFitness = fitnesses[i];
				Genome * tempSolution = population[i];

				this->fitnesses[i] = this->fitnesses[k];
				this->population[i] = this->population[k];

				this->fitnesses[k] = tempFitness;
				this->population[k] = tempSolution;
			}
		}
	}
}

void PopulationNode::evaluateFitnesses() {
	for (unsigned int i = 0; i < this->population.size(); i++)
		this->fitnesses[i] = this->evaluateFitness(i);
}

void PopulationNode::insert(int index, Genome * target) {
	delete(this->population[index]);
	this->evaluateFitness(index);
}

string PopulationNode::name() {
	return this->nodeName;
}

unsigned int PopulationNode::populationSize() {
	return this->population.empty() ?
		this->initialPopulationSize : this->population.size();
}

string PopulationNode::populationStrings() {
	stringstream ss;
	string populationString;
	
	for (unsigned int i = 0; i < population.size(); i++) {
		populationString = this->populationToString->toString(
			this->population[i]
		);

		ss << "Member " << i << ": " << populationString
			<< " Fitness: " << this->fitnesses[i] << "\n";
	}

	return ss.str();
}

string PopulationNode::toString() {
	stringstream ss;

	ss << "Population size: " << this->population.size() << "\n";
	ss << this->populationStrings();

	this->readOnce = true;
	return ss.str();
}

set<Locus*> PopulationNode::getLoci() {
	set<Locus*> loci;
	for (unsigned int i = 0; i < this->population.size(); i++) {
		vector<Locus*> temp = this->population[i]->getLoci();
		loci.insert(temp.begin(), temp.end());
	}

	return loci;
}

set<Locus*> PopulationNode::getConstructiveLoci() {
	set<Locus*> loci;
	for (unsigned int i = 0; i < this->population.size(); i++) {
		set<Locus*> temp = this->population[i]->getConstructiveLoci();
		loci.insert(temp.begin(), temp.end());
	}

	return loci;
}

vector<EndCondition*> PopulationNode::getConditions() {
	return this->conditions;
}

vector<ObjectiveFunction*> PopulationNode::getObjectives() {
	return this->objectives;
}

ToStringFunction * PopulationNode::getToString() {
	return this->populationToString;
}

int PopulationNode::getFitnessAtIndex(int index) {
	return this->fitnesses[index];
}

bool PopulationNode::contains(Genome * target) {
	for (unsigned int i = 0; i < this->population.size(); i++)
		if (this->population[i] == target) return true;
	return false;
}
