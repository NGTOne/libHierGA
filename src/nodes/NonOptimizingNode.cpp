#include "nodes/NonOptimizingNode.hpp"
#include <sstream>

using namespace std;

NonOptimizingNode::NonOptimizingNode(
	unsigned int populationSize,
	vector<Locus*> loci,
	vector<ObjectiveFunction*> objectives,
	ToStringFunction * populationToString,
	vector<EndCondition*> conditions,
	string nodeName
) : PopulationNode(
	populationSize,
	loci,
	objectives,
	populationToString,
	conditions,
	nodeName
) {}

void NonOptimizingNode::nextIteration() {
	if (!done()) evaluateFitnesses();
}

node_type_t NonOptimizingNode::type() {
	return NON_OPT_TYPE;
}
