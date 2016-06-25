#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
// #include "Data.h"
// #include "Decoding.h"
#include "RCPSP_GA.h"
using namespace std;

int main(int argc, char const *argv[])
{
	int max_gen, nind;
	float ggap, xovr, mutr;
	cout << "Maximum Generation: ";
	cin >> max_gen;
	cout << "Population Size: ";
	cin >> nind;
	cout << "Generation Gap: ";
	cin >> ggap;
	cout << "Crossover Rate: ";
	cin >> xovr;
	cout << "Mutation Rate: ";
	cin >> mutr;

	// int max_gen = 100, nind = 100;
	// float ggap = 0.9, xovr = 0.8, mutr = 0.6;
	srand((unsigned)time(NULL));
	double dur;
	clock_t s_time, e_time;
	s_time = clock();
	DataInitialization d_i;
	DataSet AllData = d_i.Initiate();
	RCPSP_GA r_g(nind, ggap, xovr, mutr);
	std::vector<std::vector<int> > chrom = r_g.Initiate(nind, AllData.TaskNum);
	multimap<int, std::vector<int> > fitnv = r_g.Objv(AllData, chrom, nind);

	int gen = 0;
	int makespan;
	std::vector<int> opt_seq;
	multimap<int, std::vector<int> >::iterator it;
	it = fitnv.begin();
	makespan = it->first;
	while (gen < max_gen) {
		std::vector<std::vector<int> > selch = r_g.Select(chrom, fitnv, ggap);
		selch = r_g.Crossover(selch, floor(nind * ggap), xovr);
		selch = r_g.Mut(selch, floor(nind * ggap), mutr);
		chrom = r_g.Reinsert(chrom, selch, fitnv);
		fitnv.clear();
		fitnv = r_g.Objv(AllData, chrom, nind);
		it = fitnv.begin();
		makespan = min(makespan, it->first);
		// cout << makespan << '\t';
		if (gen == max_gen - 1) {
			opt_seq = it->second;
		}
		++gen;
	}
	e_time = clock();
	dur = (double)(e_time - s_time);
    
	cout << "Makespan = " << makespan << endl;
	cout << "Optimal Sequence : ";
	for (size_t i = 0; i < opt_seq.size(); ++i) {
		cout << opt_seq[i] << " ";
	}
	cout << endl;
	cout << "Use Time: " << (dur/CLOCKS_PER_SEC) << " seconds" << endl;
	// printf("Use Time:%f\n",(dur/CLOCKS_PER_SEC));
	return 0;
}