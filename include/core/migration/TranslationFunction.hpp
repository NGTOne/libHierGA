#include "../Individual.hpp"
#pragma once

class TranslationFunction {
	private:

	protected:

	public:
	virtual Individual * translate(Individual * target);
	virtual bool isNull();
};
