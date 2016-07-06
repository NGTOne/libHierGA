#include "PopulationNode.hpp"
#include "migration/MigratoryRelationship.hpp"
#include "migration/TranslationFunction.hpp"
#include "ApportionmentFunction.hpp"
#include "AggregationFunction.hpp"
#include "ObjectiveFunction.hpp"
#include "ToStringFunction.hpp"
#include "meta/MetaPopulationFactory.hpp"
#include <vector>
#include <tuple>
#include <string>
#pragma once

using namespace std;

class HierarchicalEA {
	private:

	protected:
	vector<PopulationNode *> nodes;
	vector<string> nodesToPrint;
	vector<string> evolutionOrder;
	vector<string> endDictators;
	vector<MigratoryRelationship> migrations;
	unsigned int maxEpochs;

	vector<PopulationNode *> evolutionNodes;
	vector<PopulationNode *> printNodes;
	vector<PopulationNode *> endConditionDictatorNodes;

	void buildNodeSet(
		vector<string> targetNames,
		vector <PopulationNode *> & targetSet
	);
	void buildEvolutionNodes();
	void buildPrintNodes();
	void buildEndDictators();
	void migrate();

	void checkNodesExist(vector<string> names);
	bool done(unsigned int currentEpoch);

	public:
	HierarchicalEA();
	HierarchicalEA(unsigned int maxEpochs);
	~HierarchicalEA();

	// Kept separate from the destructor because we might want to keep them
	void deleteAllNodes();

	void run(bool verbose);

	void addNode(PopulationNode * node, bool print);
	void addNode(PopulationNode * node, bool print, bool end);
	void addNodes(
		vector<PopulationNode*> nodes,
		vector<bool> print,
		vector<bool> end
	);
	void removeNode(string name);

	void setNodesToPrint(vector<string> names);
	void setEvolutionOrder(vector<string> names);
	void setEndConditionDictatorNodes(vector<string> names);
	PopulationNode * getNodeByName(string name);

	template<typename MetaNodeType, typename... params>
	void addMetaPopulation(
		unsigned int populationSize,
		std::tuple<
			std::vector<std::string>,
			ApportionmentFunction *,
			AggregationFunction *
		> nodes,
		std::vector<ObjectiveFunction*> flattenedObjectives,
		ToStringFunction * flattenedToString,
		std::vector<EndCondition*> conditions,
		std::string metaNodeName,
		params... as
	);

	template<typename MetaNodeType, typename... params>
	void addMetaPopulation(
		unsigned int populationSize,
		std::vector<std::tuple<
			std::string,
			ApportionmentFunction *,
			AggregationFunction *
		>> nodes,
		std::vector<ObjectiveFunction*> flattenedObjectives,
		ToStringFunction * flattenedToString,
		std::vector<EndCondition*> conditions,
		std::string metaNodeName,
		params... as
	);

	void addMigratoryRelationship(
		string from,
		string to,
		bool bidirectional,
		unsigned int n
	);

	void addMigratoryRelationship(
		string from,
		string to,
		unsigned int n,
		TranslationFunction toTranslate
	);

	void addMigratoryRelationship(
		string from,
		string to,
		unsigned int n,
		TranslationFunction toTranslate,
		TranslationFunction fromTranslate
	);
};

template<typename MetaNodeType, typename... params>
void HierarchicalEA::addMetaPopulation(
	unsigned int populationSize,
	std::tuple<
		std::vector<std::string>,
		ApportionmentFunction *,
		AggregationFunction *
	> nodes,
	std::vector<ObjectiveFunction*> flattenedObjectives,
	ToStringFunction * flattenedToString,
	std::vector<EndCondition*> conditions,
	std::string metaNodeName,
	params... as
) {
	std::vector<std::tuple<
		std::string,
		ApportionmentFunction*,
		AggregationFunction*
	>> fullNodes;
	for (unsigned int i = 0; i < std::get<0>(nodes).size(); i++)
		fullNodes.push_back(std::make_tuple(
			std::get<0>(nodes)[i],
			std::get<1>(nodes),
			std::get<2>(nodes)
		));

	this->addMetaPopulation<MetaNodeType>(
		populationSize,
		fullNodes,
		flattenedObjectives,
		flattenedToString,
		conditions,
		metaNodeName,
		as...
	);
}

template<typename MetaNodeType, typename... params>
void HierarchicalEA::addMetaPopulation(
	unsigned int populationSize,
	std::vector<std::tuple<
		std::string,
		ApportionmentFunction *,
		AggregationFunction *
	>> nodes,
	std::vector<ObjectiveFunction*> flattenedObjectives,
	ToStringFunction * flattenedToString,
	std::vector<EndCondition*> conditions,
	std::string metaNodeName,
	params... as
) {
	std::vector<std::tuple<
		PopulationNode*,
		ApportionmentFunction*,
		AggregationFunction *
	>> trueNodes;
	for (unsigned int i = 0; i < nodes.size(); i++)
		trueNodes.push_back(std::make_tuple(
			this->getNodeByName(std::get<0>(nodes[i])),
			std::get<1>(nodes[i]),
			std::get<2>(nodes[i])
		));

	this->addNode(
		MetaPopulationFactory::createMeta<MetaNodeType>(
			populationSize,
			trueNodes,
			flattenedObjectives,
			flattenedToString,
			conditions,
			metaNodeName,
			1,
			as...
		),
		false,
		false
	);
}
