#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "Data.h"
using namespace std;

class RCPSP_GA
{
public:
	RCPSP_GA(): nind(100), ggap(0.9), xovr(0.8), mutr(0.6) {}
	RCPSP_GA(int n, float g, float x, float m): nind(n), ggap(g), xovr(x), mutr(m) {}
	~RCPSP_GA(){}

	std::vector<std::vector<int> > Initiate(int nind, int TaskNum) {
		std::vector<std::vector<int> > chrom;
		// srand((unsigned)time(NULL)); 
		map<int, int> Index_Seq;
		map<int, int>::iterator it;
		std::vector<int> Seq;
		std::vector<int> Index;

		for (int j = 0; j < nind; ++j) {
			for(int i = 0; i < TaskNum - 1; ++i) {
				if (i == 0) {
					Seq.push_back(-1);
				} else {
					Seq.push_back(rand());
				}

				Index_Seq.insert(make_pair(Seq[i], i));
			}  

			for(auto it = Index_Seq.begin(); it != Index_Seq.end(); ++it) {
				Index.push_back(it->second);
			}
			Index.insert(Index.end(), TaskNum - 1);
			chrom.push_back(Index);

			Index_Seq.clear();
			Seq.clear();
			Index.clear();
		}
		return chrom;
	}

	map<int, std::vector<int> > Objv(DataSet &AllData, std::vector<std::vector<int> > &chrom, int nind) {
		map<int, std::vector<int> > objective;
		// **********

		return objective;
	}

	// std::vector<int> Rank(std::vector<int> objective) {
	// 	std::vector<int> fitnv = objective;
	// 	sort(fitnv.begin(), fitnv.end());
	// 	return fitnv;
	// }
	
	std::vector<std::vector<int> > Select(std::vector<std::vector<int> > &chrom, map<int, std::vector<int> > fitnv, float ggap) {
		std::vector<std::vector<int> > selch;
		// srand((unsigned)time(NULL));
		std::vector<double> wheel;
		double prob;
		int obj_sum = 0;
		double r_n;
		for (map<int, std::vector<int> >::iterator it = fitnv.begin(); it != fitnv.end(); ++it) 
			obj_sum += it->first;

		for (map<int, std::vector<int> >::iterator it = fitnv.begin(); it != fitnv.end(); ++it) {
			prob = it->first/obj_sum;
			if (it == fitnv.begin()) {
				wheel.push_back(prob);
			} else {
				wheel.push_back(wheel.back() + prob);
			}
		}

		for (size_t i = 0; i < (fitnv.size() * ggap); ++i) {
			r_n = (double)rand()/RAND_MAX;
			// std::vector<double>::iterator it = std::find_if (wheel.begin(), wheel.end(), (*it >= r_n && *it < r_n));
			std::vector<double>::iterator it;
			for (it = wheel.begin(); it != wheel.end(); ++it) {
				if (*it >= r_n && *it < r_n) break;
			}
			int pos = it - wheel.begin();
			int j = 0;
			for (map<int, std::vector<int> >::iterator map_it = fitnv.begin(); map_it != fitnv.end(); ++map_it, ++j) {
				if (pos == j) {
					selch.push_back(map_it->second);
					break;
				}
			}
		}

		return selch;
	}

	std::vector<std::vector<int> > Crossover(std::vector<std::vector<int> > &selch, int nind, float xovr) {
		// for (size_t i = 0; i < selch.size(); ++i) {
		// 	for (size_t j = 0; j < selch[0].size(); ++j) {
		// 		cout << selch[i][j] << '\t';
		// 	}
		// 	cout << endl;
		// }

		std::vector<int> rand_perm;
		for (int i = 0; i < nind; ++i) {
			rand_perm.push_back(i);
		}
		random_shuffle(rand_perm.begin(), rand_perm.end());
		int num = nind - 1;
		if (nind % 2 == 1) num = nind - 2;

		for (int i = 0; i <= num; i = i + 2) {
			double r_n = (double)rand()/RAND_MAX;
			if (r_n < xovr) {
				std::vector<int> Father = selch[rand_perm[i]];
				std::vector<int> Mother = selch[rand_perm[i + 1]];
				int xovr_pos = rand() % (selch[0].size() - 3) + 1;
				std::vector<int> Son, Daughter;
				std::vector<int> son_temp(Father.begin(), Father.begin() + xovr_pos + 1);
				std::vector<int> daughter_temp(Mother.begin(), Mother.begin() + xovr_pos + 1);
				for (size_t j = 0; j < selch[0].size(); ++j) {
					if ((int)j <= xovr_pos) {
						Son.push_back(Father[j]);
						Daughter.push_back(Mother[j]);
					} else {
						std::vector<int>::iterator it_son, it_daughter;
						it_son = find(Father.begin(), Father.begin() + xovr_pos + 1, Mother[j]);
						if (it_son != Father.begin() + xovr_pos + 1) {
							for (size_t k = 0; k < daughter_temp.size(); ++k) {
								std::vector<int>::iterator it_temp = find(son_temp.begin(), son_temp.end(), daughter_temp[k]);
								if (it_temp == son_temp.end()) {
									Son.push_back(daughter_temp[k]);
									daughter_temp.erase(daughter_temp.begin() + k);
									break;
								}
							}
						} else {
							Son.push_back(Mother[j]);
						}

						it_daughter = find(Mother.begin(), Mother.begin() + xovr_pos + 1, Father[j]);
						if (it_daughter != Mother.begin() + xovr_pos + 1) {
							for (size_t k = 0; k < son_temp.size(); ++k) {
								std::vector<int>::iterator it_temp = find(daughter_temp.begin(), daughter_temp.end(), son_temp[k]);
								if (it_temp == daughter_temp.end()) {
									Daughter.push_back(son_temp[k]);
									son_temp.erase(son_temp.begin() + k);
									break;
								}
							}
						} else {
							Daughter.push_back(Father[j]);
						}
					}
				}
				selch[rand_perm[i]] = Son;
				selch[rand_perm[i + 1]] = Daughter; 
			}	
		}	

		// cout << endl;
		// for (size_t i = 0; i < selch.size(); ++i) {
		// 	for (size_t j = 0; j < selch[0].size(); ++j) {
		// 		cout << selch[i][j] << '\t';
		// 	}
		// 	cout << endl;
		// }
		return selch;
	}

	std::vector<std::vector<int> > Mut(std::vector<std::vector<int> > &selch, int nind, float mutr) {
		return selch;
	}

	std::vector<std::vector<int> > Reinsert(std::vector<std::vector<int> > &chrom, std::vector<std::vector<int> > &selch) {
		return chrom;
	}

private: 
	int nind;
	float ggap;
	float xovr;
	float mutr;
};

int main(int argc, char const *argv[])
{
	srand((unsigned)time(NULL)); 
	RCPSP_GA ga(10, 0.9, 0.8, 0.6);
	std::vector<std::vector<int> > vec = ga.Initiate(10, 10);
	// for (size_t i = 0; i < vec.size(); ++i) {
	// 	for (size_t j = 0; j < vec[i].size(); ++j)
	// 		cout << vec[i][j] << '\t';
	// 	cout << endl;
	// }
	// cout << endl;
	
	std::vector<std::vector<int> > selch = ga.Crossover(vec, 10, 0.8);
	// for (size_t i = 0; i < selch.size(); ++i) {
	// 	for (size_t j = 0; j < selch[i].size(); ++j)
	// 		cout << selch[i][j] << '\t';
	// 	cout << endl;
	// }


	return 0;
}