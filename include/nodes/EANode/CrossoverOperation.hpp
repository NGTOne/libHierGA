#ifndef NODES_EANODE_CrossoverOperation
#define NODES_EANODE_CrossoverOperation

#include "../../core/Genome.hpp"
#include "../../core/GenomeTemplate.hpp"
#include "../../core/EABehaviourObject.hpp"
#include <random>
#include <string>
#include <vector>

#define DEFAULT_NUM_OFFSPRING 2

/*
* This class serves as an abstract base for crossover operators, allowing
* us to create whatever crossover operators we need to.
*/

class CrossoverOperation : public EABehaviourObject {
	private:

	protected:
	unsigned int numOffspring;

	void init(unsigned int numOffspring);
	unsigned int maxPairings(
		unsigned int numParents,
		unsigned int pairingSize
	);
	std::vector<unsigned int> getParents(
		unsigned int numAvailableParents,
		unsigned int desiredParents,
		std::vector<std::vector<unsigned int>> & previousPairings
	);

	std::vector<GenomeTemplate> getTemplates(std::vector<Genome*> parents);
	unsigned int shortestGenome(std::vector<Genome*> genomes);
	unsigned int shortestGenome(std::vector<GenomeTemplate> genomes);
	virtual std::vector<GenomeTemplate> crossOver(
		std::vector<GenomeTemplate> parents
	)=0;

	public:
	CrossoverOperation();
	CrossoverOperation(unsigned int numOffspring);
	virtual ~CrossoverOperation();

	std::vector<Genome*> crossOver(
		std::vector<Genome*> parents,
		std::string speciesNode
	);

	virtual std::string toString()=0;
};

#endif
