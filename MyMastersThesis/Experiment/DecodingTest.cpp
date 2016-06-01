#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;



const int TaskNum = 11;
const int SrvcNum;

class SeqGeneration
{
public:
	std::vector<int> RandSeq(int TaskNum) {
		map<int, int> Index_Seq;
		map<int, int>::iterator it;
		std::vector<int> Seq;
		std::vector<int> Index;
		srand((unsigned)time(NULL));  
		for(int i = 0; i < TaskNum - 1; ++i ) {
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
		return Index;
	}
};

class SeqDecoding
{
public:
	map<std::vector<int>, int> LegalSeq(std::vector<int> &OriginalSeq, int TaskNum, std::vector<vector<int> > &prec, 
		// std::vector<vector<int> > &succ, 
		std::vector<int> &proc_time, std::vector<vector<int> > &opt_srvcs) {

		map<std::vector<int>, int> result;
		int makespan = 0;
		std::vector<int> remov;
		std::vector<int> remain;
		std::vector<int> present;
		for (int i = 0, i < TaskNum, ++i)
			remain.push_back(i);
		std::vector<int> SeqTemp(OriginalSeq.begin(), OriginalSeq.begin() + TaskNum);

		while (remain.size() > 0) {
			int temp_index = 0;
			int pre_act = 0;
			std::vector<int> pre_act_prec;
			// std::vector<int> pre_act_succ(succ[0].begin(), succ[0].end());
			bool ok = isSubset(pre_act_prec, remov);

			while (!ok) {
				++temp_index;
				pre_act = SeqTemp[temp_index];
				pre_act_prec(prec[pre_act].begin(), prec[pre_act].end());
				ok = isSubset(pre_act_prec, remov);
			} 

			remov.push_back(pre_act);
			remain.erase(find(remain.begin(), remain.end(), pre_act));
			SeqTemp.erase(find(SeqTemp.begin(), SeqTemp.end(), pre_act));
			
			pre_act_prec.clear();
		}
		result.insert(make_pair(remov, makespan));
		return result;
	}

private:
	bool isSubset(std::vector<int>& vec1, std::vector<int>& vec2) {
		std::vector<int>::iterator it;
		for (int i = 0; i < vec1.size(); ++i) {
			it = find(vec2.begin(), vec2.end(), vec1[i]);
			if (it == vec2.end()) {
				return false;
			} 
		}
		return true;
	}
};

bool isSubset(std::vector<int>& vec1, std::vector<int>& vec2) {
	std::vector<int>::iterator it;
	for (int i = 0; i < vec1.size(); ++i) {
		it = find(vec2.begin(), vec2.end(), vec1[i]);
		if (it == vec2.end()) {
			return false;
		} 
	}
	return true;
}

int main() 
{  
	
	map<int, int> Index_Seq;
	map<int, int>::iterator it;
	std::vector<int> Seq;
	std::vector<int> Index;
	srand((unsigned)time(NULL));  
	for(int i = 0; i < TaskNum - 1; ++i ) {
		if (i == 0) {
			Seq.push_back(-1);
		} else {
			Seq.push_back(rand());
		}

		Index_Seq.insert(make_pair(Seq[i], i));
	}  

	for(auto it = Index_Seq.begin(); it != Index_Seq.end(); ++it) {
		Index.push_back(it->second);
		// cout << it->second;
	}
	// cout << Index.size();
	// Index.insert(Index.begin(), 0);
	Index.insert(Index.end(), TaskNum - 1);
	// cout << *Index.begin() << *(Index.end() - 1) << Index.size();

	for (int i = 0; i < Index.size(); ++i) {
		cout << Index[i] << '\t';
	}
	cout << endl;

	map<int, std::vector<int> > prec;
	map<int, std::vector<int> > succ;
	int arr_prec[] = {0, 0, 0, 2, 2, 0, 1, 4, 2, 3, 5, 6, 7, 8, 9};
	int arr_prec_count[TaskNum] = {0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 4};
	int arr_succ[] = {1, 2, 3, 6, 7, 4, 5, 8, 9, 7, 9, 10, 10, 10, 10};
	int arr_succ_count[TaskNum] = {4, 1, 3, 1, 1, 1, 1, 1, 1, 1, 0};

	int prec_cum = 0;
	int succ_cum = 0;
	for (int i = 0; i < TaskNum; ++i) {
		std::vector<int> vec_prec(arr_prec + prec_cum, arr_prec + prec_cum + arr_prec_count[i]);
		// for (auto it = vec_prec.begin(); it != vec_prec.end(); ++it) {
		// 	cout << *it << '\t';
		// }
		// cout << endl;
		prec.insert(make_pair(i, vec_prec));
		vec_prec.clear();
		prec_cum += arr_prec_count[i];

		std::vector<int> vec_succ(arr_succ + succ_cum, arr_succ + succ_cum + arr_succ_count[i]);
		succ.insert(make_pair(i, vec_succ));
		vec_succ.clear();
		succ_cum += arr_succ_count[i];
	}
	
	int arr_init[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> remov;
	std::vector<int> remain(arr_init, arr_init + TaskNum);
	std::vector<int> present;

	// while (remain.size() > 0) {
	// 	int *pointer = &*Index.begin();
	// 	int count_Loop = 0;
	// 	if (count_Loop == 0) {
	// 		int pre_act = *pointer;
	// 		std::vector<int> pre_act_prec = prec[pre_act];
	// 		bool ok = isSubset(pre_act_prec, remov);
	// 		if (ok) {
	// 			remov.push_back(pre_act);
	// 			remain.erase(find(remain.begin(), remain.end(), pre_act));
	// 			Index.erase(find(Index.begin(), Index.end(), pre_act));
	// 		} else {
	// 			++pointer;
	// 		}
	// 	}
	// }
	
	while (remain.size() > 0) {
		int *pointer = &*Index.begin();
		int pre_act = *pointer;
		std::vector<int> pre_act_prec = prec[pre_act];
		bool ok = isSubset(pre_act_prec, remov);

		while (!ok) {
			++pointer;
			pre_act = *pointer;
			pre_act_prec = prec[pre_act];
			ok = isSubset(pre_act_prec, remov);
		} 

		remov.push_back(pre_act);
		remain.erase(find(remain.begin(), remain.end(), pre_act));
		Index.erase(find(Index.begin(), Index.end(), pre_act));
		
		pre_act_prec.clear();
	}

	for (int i = 0; i < remov.size(); ++i) {
		cout << remov[i] << '\t';
	}
	      
	return 0; 
}