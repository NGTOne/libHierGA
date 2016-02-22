#include "core/HierarchicalEA.hpp"
#include "exception/NodeAlreadyExistsException.hpp"
#include "exception/InvalidNodeException.hpp"
#include "exception/NoNodesException.hpp"
#include "exception/NoEvolutionOrderException.hpp"

#include <iostream>
#include <algorithm>

HierarchicalEA::HierarchicalEA(int maxEpochs) {
	this->maxEpochs = maxEpochs;
}

HierarchicalEA::~HierarchicalEA() {}

void HierarchicalEA::addNode(PopulationNode * node, string name, bool print) {
	for (int i = 0; i < nodeNames.size(); i++) {
		if (nodeNames[i] == name or nodes[i] == node) {
			throw NodeAlreadyExistsException();
		}
	}
	nodes.push_back(node);
	nodeNames.push_back(name);
	if (print) nodesToPrint.push_back(name);
}

void HierarchicalEA::removeNode(string name) {
	for (int i = 0; i < nodeNames.size(); i++) {
		string nodeName = nodeNames[i];
		if (nodeName == name) {
			nodeNames.erase(nodeNames.begin()+i);
			nodes.erase(nodes.begin()+i);
			break;
		}
	}

	for (int i = 0; i < nodesToPrint.size(); i++) {
		if ((nodesToPrint[i]) == name) {
			nodesToPrint.erase(nodesToPrint.begin()+i);
		}
	}
}

void HierarchicalEA::checkNodesExist(vector<string> names) {
	for (int i = 0; i < names.size(); i++) {
		bool exists = false;

		for (int k = 0; k < nodeNames.size(); k++) {
			if (nodeNames[k] == names[i]) exists = true;
		}

		if (!exists) throw InvalidNodeException();
	}
}

void HierarchicalEA::setNodesToPrint(vector<string> names) {
	checkNodesExist(names);
	nodesToPrint.clear();
	nodesToPrint = names;
}

void HierarchicalEA::setEvolutionOrder(vector<string> names) {
	checkNodesExist(names);
	evolutionOrder.clear();
	evolutionOrder = names;
}

void HierarchicalEA::buildNodeSet(
	vector<string> targetNames,
	vector<PopulationNode *> * targetSet
) {
	if (nodes.empty()) throw NoNodesException();
	checkNodesExist(targetNames);

	for (int i = 0; i < targetNames.size(); i++) {
		string name = targetNames[i];
		for (int k = 0; k < nodeNames.size(); k++) {
			string realName = nodeNames[k];
			if (name == realName) {
				targetSet->push_back(nodes[k]);
				break;
			}
		}
	}
}

void HierarchicalEA::buildEvolutionNodes() {
	if (evolutionOrder.empty()) throw NoEvolutionOrderException();
	evolutionNodes.clear();
	buildNodeSet(evolutionOrder, &evolutionNodes);
}

void HierarchicalEA::buildPrintNodes() {
	printNodes.clear();
	buildNodeSet(nodesToPrint, &printNodes);
}

bool HierarchicalEA::done(int currentEpoch) {
	bool done = true;
	if (currentEpoch >= maxEpochs - 1) {
		cout << "Hierarchical EA ended because maximum epoch count was reached.\n";
		return done;
	}

	for (int i = 0; i < evolutionNodes.size(); i++) {
		if (!evolutionNodes[i]->done()) {
			done = false;
			break;
		}
	}

	if (done) cout << "Hierarchical EA ended because all population nodes reported reaching their specified ending conditions.\n";

	return done;
}

// TODO: Make verbosity option actually mean something
void HierarchicalEA::run(bool verbose) {
	buildEvolutionNodes();
	buildPrintNodes();
	cout << "Before:\n";
	cout << string(80, '=') << "\n";
	for (int i = 0; i < printNodes.size(); i++) {
		cout << "Node " << nodesToPrint[i] << ":\n";
		cout << printNodes[i]->toString();
		cout << string(80, '-') << "\n";
	}

	for (int i = 0; i < maxEpochs; i++) {
		for (int k = 0; k < evolutionNodes.size(); k++) {
			if (!evolutionNodes[k]->done()) {
				evolutionNodes[k]->runGenerations();
			}
		}

		// Because humans count from 1, we add 1 to our epoch counter
		cout << "After epoch " << i+1 << ":\n";
		cout << string(80, '=') << "\n";
		for (int k = 0; k < printNodes.size(); k++) {
			cout << "Node " << nodesToPrint[k] << ":\n";
			cout << printNodes[k]->toString();
			cout << string(80, '-') << "\n";
		}

		if (done(i)) break;
	}
}