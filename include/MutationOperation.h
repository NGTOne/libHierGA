//Abstract class for mutation operators. Lets us define any mutation operation we like.

class MutationOperation {
	private:

	protected:
	unsigned seed;
        int mutationRate;

	public:

	MutationOperation();
	MutationOperation(int newMutationRate);
	MutationOperation(int newMutationRate, unsigned newSeed);

	int * mutate(int initialGenome[], int largestPossibleValues[]);
};