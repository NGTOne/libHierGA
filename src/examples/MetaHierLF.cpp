#include <cstdio>
#include <iostream>
#include <sstream>
#include <libHierGA/HierGA.hpp>
#include "LongestFragmentFitness.hpp"

using namespace std;

int main(void) {
	HierarchicalEA ea(100);

	ea.addConstructiveTree<EANode, PopulationLocus>(
		new ConstructiveLocusCountPopFormula(),
		{
			vector<Locus*>(
				16,
				new NumericCollectionLocus<int>({0, 1})
			),
			vector<Locus*>(
				8,
				new NumericCollectionLocus<int>({0, 1})
			)
		},
		{
			new LongestFragmentToString(
				HierarchicalToString::omitPlaceholders
			),
			new LongestFragmentToString()
		},
		{{new IterationCountEnd(100)}, {new IterationCountEnd(100)}},
		TreeBuilder("P1").addSubNodes("P1", {"P2", "P3", "P4", "P5"}),
		{true, false},
		{true, false},
		new GA(
			2,
			false,
			new TournamentSelection(0.95, 4),
			new NPointCrossover(2),
			new UniformMutation(0.2)
		)
	);

	ea.addMetaPopulation<EANode>(
		true,
		4,
		std::make_tuple(
			std::vector<std::string>(
				{"P1", "P2", "P3", "P4", "P5"}
			),
			new LongestFragmentApportionment
		),
		{new LongestFragmentFitness()},
		new LongestFragmentToString(
			HierarchicalToString::printPlaceholders
		),
		{new IterationCountEnd(100)},
		"M1",
		new GA(
			2,
			false,
			new TournamentSelection(0.95, 4),
			new NPointCrossover(2),
			new UniformMutation(0.2)
		)
	);

	ea.setEvolutionOrder({"M1", "P5", "P4", "P3", "P2", "P1"});
	ea.setNodesToPrint({"M1", "P1", "P2", "P3", "P4", "P5"});
	ea.run(true);
	ea.deleteAllNodes();
}

