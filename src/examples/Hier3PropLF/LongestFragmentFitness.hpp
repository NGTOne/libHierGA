#include <libHierGA/HierGA.hpp>
#include <string>
#pragma once

class LongestFragmentToString : public ToStringFunction {
	private:

	protected:

	public:
	std::string toString(GeneNode ** pools, int * indexes, int genomeLength);
};