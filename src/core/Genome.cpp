#include "core/Genome.hpp"
#include "core/FakeGenome.hpp"
#include "core/Locus.hpp"
#include "loci/PopulationLocus.hpp"
#include "loci/FakePopulationLocus.hpp"
#include "exception/ValueOutOfRangeException.hpp"
#include "exception/ComponentNotPresentException.hpp"
#include <cmath>
#include <sstream>

using namespace std;

Genome::Genome(vector<Locus*> loci, std::string speciesNode) {
	this->loci = loci;
	this->speciesNode = speciesNode;
	generateRandomGenes();
}

Genome::Genome(
	vector<unsigned int> genes,
	vector<Locus*> loci,
	std::string speciesNode
) {
	this->genes = genes;
	this->loci = loci;
	this->speciesNode = speciesNode;
}

Genome::Genome(GenomeTemplate geneTemplate, std::string speciesNode) {
	this->genes = geneTemplate.getGenes();
	this->loci = geneTemplate.getLoci();
	this->speciesNode = speciesNode;
}

Genome::Genome(Genome * other) {
	this->loci = other->getLoci();
	this->genes = other->getGenome();
	this->speciesNode = other->getSpeciesNode();
}

Genome::Genome(Genome * other, bool randomize) {
	this->loci = other->getLoci();
	this->speciesNode = other->getSpeciesNode();
	if (randomize) {
		this->generateRandomGenes();
	} else {
		this->genes = other->getGenome();
	}
}

Genome::Genome(Genome * other, unsigned int rawGenes[]) {
	this->loci = other->getLoci();
	this->speciesNode = other->getSpeciesNode();
	vector<unsigned int> genes(rawGenes, rawGenes + loci.size());
	for (unsigned int i = 0; i < genes.size(); i++)
		if (this->loci[i]->outOfRange(genes[i]))
			throw ValueOutOfRangeException();
	this->genes = genes;
}

Genome::~Genome() {}

void Genome::generateRandomGenes() {
	genes.clear();
	for (unsigned int i = 0; i < loci.size(); i++)
		genes.push_back(loci[i]->randomIndex());
}

vector<unsigned int> Genome::getGenome() {
	return this->genes;
}

unsigned int Genome::genomeLength() {
	return this->genes.size();
}

vector<Locus*> Genome::getLoci() {
	return loci;
}

std::string Genome::getSpeciesNode() {
	return this->speciesNode;
}

int Genome::difference(Genome * otherGenome) {
	vector<unsigned int> otherGenes = otherGenome->getGenome();
	int difference = 0;
	unsigned int shorterGenome = fmin(
		this->genes.size(),
		otherGenes.size()
	);
	unsigned int longerGenome = fmax(
		this->genes.size(),
		otherGenes.size()
	);

	for (unsigned int i = 0; i < shorterGenome; i++)
		difference += abs(this->genes[i] - otherGenes[i]);

	// We want to account for genes of different lengths somehow
	if (longerGenome != shorterGenome)
		difference += longerGenome - shorterGenome;

	return difference;
}

string Genome::flatten() {
	stringstream ss;

	for (unsigned int i = 0; i < this->genes.size(); i++)
		ss << this->loci[i]->flatten(this->genes[i]) << " ";

	return ss.str();
}

template <>
Genome * Genome::getIndex<Genome*>(unsigned int index) {
	return ((PopulationLocus*)this->loci[index])
		->getIndex(this->genes[index]);
}

Genome Genome::flattenGenome(Genome * target, bool exclude) {
	vector<unsigned int> rawGenome;
	vector<Locus*> rawLoci;

	for (unsigned int i = 0; i < this->genomeLength(); i++) {
		Locus * tempLocus = this->loci[i];
		if (!tempLocus->isConstructive()) {
			rawGenome.push_back(this->genes[i]);
			rawLoci.push_back(this->loci[i]);
		} else {
			Genome * temp = ((PopulationLocus*)tempLocus)->getIndex(
				this->genes[i]
			);

			if (temp == target) {
				if (!exclude) {
					rawGenome.push_back(this->genes[i]);
					rawLoci.push_back(this->loci[i]);
				}
			} else {
				Genome tempFlattened = temp->flattenGenome();
				vector<unsigned int> tempGenome =
					tempFlattened.getGenome();
				vector<Locus*> tempLoci =
					tempFlattened.getLoci();

				for (
					unsigned int k = 0;
					k < tempGenome.size();
					k++
				) {
					rawGenome.push_back(tempGenome[k]);
					rawLoci.push_back(tempLoci[k]);
				}
			}
		}
	}

	return Genome(rawGenome, rawLoci, this->speciesNode);
}

Genome Genome::flattenGenome() {
	return this->flattenGenome(NULL, false);
}

Genome Genome::flattenExceptFor(Genome * target) {
	return this->flattenGenome(target, false);
}

Genome Genome::flattenWithout(Genome * target) {
	return this->flattenGenome(target, true);
}

Genome * Genome::replaceComponent(Genome * target) {
	std::vector<unsigned int> newGenes;
	std::vector<Locus*> newLoci;
	for (unsigned int i = 0; i < this->loci.size(); i++) {
		if (this->loci[i]->isConstructive()) {
			PopulationLocus * temp =
				(PopulationLocus*)this->loci[i];

			if (temp->usesSpecies(target)) {
				newLoci.push_back(new FakePopulationLocus(
					target,
					temp,
					false
				));
			} else {
				Genome * replaced = this->getIndex<Genome*>(i)
					->replaceComponent(target);
				newLoci.push_back(new FakePopulationLocus(
					replaced,
					temp,
					true
				));
				delete(replaced);
			}
		} else {
			newLoci.push_back(this->loci[i]);
		}
		newGenes.push_back(this->genes[i]);
	}
	return new FakeGenome(newGenes, newLoci, this->speciesNode);
}

std::vector<unsigned int> Genome::getFlattenedIndices(
	Genome * target,
	std::function<bool(Genome *, Genome *)> compare
) {
	std::vector<unsigned int> indices;
	unsigned int currentIndex = 0;

	for (unsigned int i = 0; i < this->genes.size(); i++) {
		if (this->loci[i]->isConstructive()) {
			Genome * temp = ((PopulationLocus*)this->loci[i])
				->getIndex(this->genes[i]);
			if (compare(target, temp)) {
				indices.push_back(currentIndex);
			} else {
				std::vector<unsigned int> componentIndices =
					temp->getFlattenedIndices(target);

				for (
					unsigned int k = 0;
					k < componentIndices.size();
					k++
				) {
					indices.push_back(
						componentIndices[k]
							+ currentIndex
					);
				}
			}
			currentIndex += temp->flattenedGenomeLength();
		} else {
			currentIndex++;
		}
	}

	return indices;
}

std::vector<unsigned int> Genome::getFlattenedIndices(Genome * target) {
	return this->getFlattenedIndices(
		target,
		[] (Genome * target, Genome * compare) -> bool {
			return target == compare;
		}
	);
}

std::vector<unsigned int> Genome::getFlattenedSpeciesIndices(Genome * target) {
	return this->getFlattenedIndices(
		target,
		[] (Genome * target, Genome * compare) -> bool {
			return target->getSpeciesNode()
				== compare->getSpeciesNode();
		}
	);
}

unsigned int Genome::flattenedGenomeLength() {
	return this->flattenGenome().genomeLength();
}

bool Genome::usesComponent(Genome * component) {
	for (unsigned int i = 0; i < this->loci.size(); i++) {
		Locus * locus = this->loci[i];
		if (locus->isConstructive()) {
			Genome * temp = ((PopulationLocus*)locus)->getIndex(
				this->genes[i]
			);

			if (
				temp == component
				|| temp->usesComponent(component)
			) return true;
		}
	}

	return false;
}

set<Locus*> Genome::getConstructiveLoci() {
	set<Locus*> constructiveLoci;
	for (unsigned int i = 0; i < this->loci.size(); i++)
		if (this->loci[i]->isConstructive())
			constructiveLoci.insert(this->loci[i]);

	return constructiveLoci;
}

GenomeTemplate Genome::getTemplate() {
	return GenomeTemplate(this->genes, this->loci);
}
