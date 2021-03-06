#ifndef CORE_GENES_Gene
#define CORE_GENES_Gene

#include "../Locus.hpp"
#include <boost/any.hpp>

class Locus;

class Gene {
	private:

	protected:
	Locus* target;
	double index;

	bool valueOutOfRange(double newIndex);

	Gene(Locus* target);
	Gene(Locus* target, double index);
	Gene(Gene* other);

	virtual void add(double addend)=0;
	virtual void set(double value)=0;

	public:
	virtual ~Gene() {}
	template <typename T>
	T getValue();

	template<typename T>
	void addToIndex(T addend);

	template<typename T>
	void setIndex(T value);

	double getIndex();
	Locus* getLocus();
	bool isConstructive();

	std::string flatten();
	virtual Gene* copy()=0;
	virtual Gene* copy(double newIndex)=0;

	virtual Gene* increment()=0;
	virtual Gene* decrement()=0;
};

#endif
