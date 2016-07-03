#include <vector>
#include <random>
#include "../../../nodes/EANode/MutationOperation.hpp"
#pragma once

class GeneScrambler {
	private:

	protected:
	MutationOperation * mutator;

	GeneScrambler(MutationOperation * mutator);

	public:
	virtual Genome scramble(Genome * target);
	Genome scramble(Genome target);
};
