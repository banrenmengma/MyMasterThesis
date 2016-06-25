#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
// #include "Data.h"
#include "Decoding.h"
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

	multimap<int, std::vector<int> > Objv(DataSet &AllData, std::vector<std::vector<int> > &chrom, int nind) {
		multimap<int, std::vector<int> > objective;
		// **********
		std::vector<int> lgl_seq;
		int fit;
		for (int i = 0; i < nind; ++i) {
			SeqDecoding sd;
			lgl_seq = sd.LegalSeq(chrom[i], AllData);
			fit = sd.makespan(lgl_seq, AllData);
			objective.insert(make_pair(fit, chrom[i]));
		}
		return objective;
	}

	// std::vector<int> Rank(std::vector<int> objective) {
	// 	std::vector<int> fitnv = objective;
	// 	sort(fitnv.begin(), fitnv.end());
	// 	return fitnv;
	// }
	
	std::vector<std::vector<int> > Select(std::vector<std::vector<int> > &chrom, multimap<int, std::vector<int> > &fitnv, float ggap) {
		std::vector<std::vector<int> > selch;
		// srand((unsigned)time(NULL));
		std::vector<double> wheel;
		double prob;
		int obj_sum = 0;
		double r_n;
		for (multimap<int, std::vector<int> >::iterator it = fitnv.begin(); it != fitnv.end(); ++it) 
			obj_sum += it->first;
		// cout << obj_sum;
		for (multimap<int, std::vector<int> >::iterator it = fitnv.begin(); it != fitnv.end(); ++it) {
			prob =  ((double)(it->first)/obj_sum);
			// cout << prob << endl;
			if (it == fitnv.begin()) {
				wheel.push_back(prob);
			} else {
				wheel.push_back(wheel.back() + prob);
			}
			// cout << wheel.back() << endl;
		}
		int j;
		for (size_t i = 0; i < floor(fitnv.size() * ggap); ++i) {
			r_n = (double)rand()/RAND_MAX;
			// std::vector<double>::iterator it = std::find_if (wheel.begin(), wheel.end(), (*it >= r_n && *it < r_n));
			// std::vector<double>::iterator it;
			int pos = 0;
			for (std::vector<double>::iterator it = wheel.begin(); it != wheel.end(); ++it, ++pos) {
				if (it == wheel.begin()) {
					if (r_n < *it) break;
				} else {
					if (r_n >= *(it - 1) && r_n < *it) {
						break;
					}
				}
			}

			j = 0;
			for (multimap<int, std::vector<int> >::iterator map_it = fitnv.begin(); map_it != fitnv.end(); ++map_it, ++j) {
				if (pos == j) {
					selch.push_back(map_it->second);
					break;
				}
			}
		}

		return selch;
	}

	std::vector<std::vector<int> > Crossover(std::vector<std::vector<int> > &selch, int nind, float xovr) {
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
		return selch;
	}

	std::vector<std::vector<int> > Mut(std::vector<std::vector<int> > &selch, int nind, float mutr) {
		for (size_t i = 0; i < selch.size(); ++i) {
			double r_n = (double)rand()/RAND_MAX;
			if (r_n < mutr) {
				int index1 = rand() % (selch[0].size() - 2) + 1;
				int index2 = rand() % (selch[0].size() - 2) + 1;
				while (index1 == index2) {
					index2 = rand() % (selch[0].size() - 2) + 1;
				}
				swap(selch[i][index1], selch[i][index2]);
			}
		}
		return selch;
	}

	std::vector<std::vector<int> > Reinsert(std::vector<std::vector<int> > &chrom, std::vector<std::vector<int> > &selch, multimap<int, std::vector<int> > &fitnv) {
		int reins_size = (int)(chrom.size() - selch.size());
		multimap<int, std::vector<int> >::iterator it = fitnv.begin();
		for (int i = 0; i < reins_size; ++i, ++it) {
			selch.push_back(it->second);
		}
		return selch;
	}

private: 
	int nind;
	float ggap;
	float xovr;
	float mutr;
};
