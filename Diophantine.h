/*!
	\defgroup Diophantine algoritm
	\brief Solution of Diophantine equations

	Consider the diophantine (only integer solutions) equation
*/

#include <stdlib.h>
#include <time.h>

#define MAXPOP	25

/*!
	\brief It used to keep track of different sets of solutions
	

Sozdavayamaya population - population gene. This genetic structure monitors their koeffetsienty survival and the probability of being a parent.
*/
struct gene {
	int alleles[4];
	int fitness;
	float likelihood;

	// Test for equality.
	int operator==(gene gn) {
		for (int i = 0; i<4; i++) {
			if (gn.alleles[i] != alleles[i]) return false;
		}

		return true;
	}
};
/*!
	\bref Class solutions to the equation Diophantine
	\author Jarvis
	\param [in] a Positive factors of an equation
	\param [in] b Positive factors of an equation
	\param [in] c Positive factors of an equation
	\param [in] d Positive factors of an equation
	\param [in] result The result of an equation (the upper limit)
	\return Descendants of survival tend to zero


*/
class CDiophantine {
public:
	CDiophantine(int, int, int, int, int);		//< Constructor with coefficients for a,b,c,d.
	int Solve();								//< Solve the equation.

												//< Returns a given gene.
	/*!
		\param [in] i Number element
		\return The gen

		The function returns the gene
	*/
	gene GetGene(int i) { return population[i]; }

protected:
	int ca, cb, cc, cd;							//< The coefficients.
	int result;
	gene population[MAXPOP];					//< Population.

	int Fitness(gene &);						//< Fitness function.
	void GenerateLikelihoods();					//< Generate likelihoods.
	float MultInv();							//< Creates the multiplicative inverse.
	int CreateFitnesses();
	void CreateNewPopulation();
	int GetIndex(float val);

	gene Breed(int p1, int p2);

};

CDiophantine::CDiophantine(int a, int b, int c, int d, int res) : ca(a), cb(b), cc(c), cd(d), result(res) {}
/*!
	\bref Call function
	\author Jarvis
	\warning a test to verify whether the function could get a result using the initial population.


*/
int CDiophantine::Solve() {
	int fitness = -1;

	// Generate initial population.
	srand((unsigned)time(NULL));

	for (int i = 0; i<MAXPOP; i++) {						// Fill the population with numbers between
		for (int j = 0; j<4; j++) {						// 0 and the result.
			population[i].alleles[j] = rand() % (result + 1);
		}
	}

	if (fitness = CreateFitnesses()) {
		return fitness;
	}

	int iterations = 0;								// Keep record of the iterations.
	while (fitness != 0 || iterations < 50) {		// Repeat until solution found, or over 50 iterations.
		GenerateLikelihoods();						// Create the likelihoods.
		CreateNewPopulation();
		if (fitness = CreateFitnesses()) {
			return fitness;
		}

		iterations++;
	}

	return -1;
}
/*! 
	\bref Fitness function - calculates the survival rate (fitness - fitness) of each gene
	\author Jarvis
	\version 1.0
	\warning In our case it is - absolute difference between the desired result and this value
	
	This class uses two functions: the first evaluates all the factors, and the second - less (preferably make it inline) calculates the rate for any single gene.
*/
int CDiophantine::Fitness(gene &gn) {
	int total = ca * gn.alleles[0] + cb * gn.alleles[1] + cc * gn.alleles[2] + cd * gn.alleles[3];

	return gn.fitness = abs(total - result);
}
/*!
	\bref Fitness function - calculates the survival rate (fitness - fitness) of each gene
	\author Jarvis
	\version 2.0
	\warning Note that if the fitness = 0, then the solution is found - return. After evaluating the fitness (fitness), we need to calculate the probability of selection of the gene as a parent

	This class uses two functions: the first evaluates all the factors, and the second - less (preferably make it inline) calculates the rate for any single gene.
*/
int CDiophantine::CreateFitnesses() {
	float avgfit = 0;
	int fitness = 0;
	for (int i = 0; i<MAXPOP; i++) {
		fitness = Fitness(population[i]);
		avgfit += fitness;
		if (fitness == 0) {
			return i;
		}
	}

	return 0;
}
/*!
	conversion function required to float in order to avoid an integer division
*/
float CDiophantine::MultInv() {
	float sum = 0;

	for (int i = 0; i<MAXPOP; i++) {
		sum += 1 / ((float)population[i].fitness);
	}

	return sum;
}
/*!
	\bref Likelihood functions
	\author Jarvis
	\version 1.0
	\warning The last value will always be 100

	The function calculates the probability of being a parent. Probability is calculated as the sum of converted coefficients, divided by the value of the inverse of the coefficient of this value. The probabilities are cumulative (fold), which makes it very easy to calculate with the parents.
*/
void CDiophantine::GenerateLikelihoods() {
	float multinv = MultInv();

	float last = 0;
	for (int i = 0; i<MAXPOP; i++) {
		population[i].likelihood = last = last + ((1 / ((float)population[i].fitness) / multinv) * 100);
	}
}

/*!
\bref Create a random population of genes. Then we do the cycle all the genes
\author Jarvis
\version 1.5
\warning We do not need the same genes (operator in the gene)

When choosing a parent, generates a random number and then call GetIndex. GetIndex uses the idea of the cumulative probability (likelihoods), it just makes iterating through all the genes have not yet found a gene containing a number:
*/
int CDiophantine::GetIndex(float val) {
	float last = 0;
	for (int i = 0; i<MAXPOP; i++) {
		if (last <= val && val <= population[i].likelihood) return i;
		else last = population[i].likelihood;
	}

	return 4;
}
/*!
	\bref 
	\author
	\version 1.0
	\warning if the number of iterations exceeds MAXPOP^2, she chooses any parents

	Breed function returns a gene that is placed into the temporary population
*/
gene CDiophantine::Breed(int p1, int p2) {
	int crossover = rand() % 3 + 1;					// Create the crossover point (not first).
	int first = rand() % 100;						// Which parent comes first?

	gene child = population[p1];					// Child is all first parent initially.

	int initial = 0, final = 3;						// The crossover boundaries.
	if (first < 50) initial = crossover;			// If first parent first. start from crossover.
	else final = crossover + 1;						// Else end at crossover.

	for (int i = initial; i<final; i++) {				// Crossover!
		child.alleles[i] = population[p2].alleles[i];
		if (rand() % 101 < 5) child.alleles[i] = rand() % (result + 1);
	}

	return child;									// Return the kid...
}
/*!
\bref Breeding Functions - reproductive functions
\author Jarvis
\version 1.0

Propagation consists of three functions: to receive gene index corresponding to the random number from 1 to 100, directly calculate the cross-over of the two genes and the main function of generating a new generation.
*/
void CDiophantine::CreateNewPopulation() {
	gene temppop[MAXPOP];

	for (int i = 0; i<MAXPOP; i++) {
		int parent1 = 0, parent2 = 0, iterations = 0;
		while (parent1 == parent2 || population[parent1] == population[parent2]) {
			parent1 = GetIndex((float)(rand() % 101));
			parent2 = GetIndex((float)(rand() % 101));
			if (++iterations > 25) break;
		}

		temppop[i] = Breed(parent1, parent2);		// Create a child.
	}

	for (int i = 0; i<MAXPOP; i++) population[i] = temppop[i];
}
