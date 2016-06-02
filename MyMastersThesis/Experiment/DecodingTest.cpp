#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
using namespace std;

// const int TaskNum = 11;

struct DataSet
{
	const int TaskNum;
	std::vector<std::vector<int> > prec;
	std::vector<int> proc_time;
	std::vector<std::vector<int> > opt_srvcs;
	const int RsrcNum;
	const int SrvcNum;
	std::vector<int> RsrcCpcty;
	std::vector<int> SrvcCpcty;
	std::vector<std::vector<int> > CoeMatrix;
};

class DataInitialization
{
public:
	DataSet Initiate() {
		const int TaskNum = 11;

		std::vector<std::vector<int> > prec;
		int arr_prec[] = {0, 0, 0, 2, 2, 0, 1, 4, 2, 3, 5, 6, 7, 8, 9};
		int arr_prec_count[TaskNum] = {0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 4};
		int prec_cum = 0;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec_prec(arr_prec + prec_cum, arr_prec + prec_cum + arr_prec_count[i]);
			prec.push_back(vec_prec);
			vec_prec.clear();
			prec_cum += arr_prec_count[i];
		}
		
		int arr_proc_time[TaskNum] = {0, 2, 6, 1, 3, 2, 2, 5, 4, 4, 0};
		std::vector<int> proc_time(arr_proc_time, arr_proc_time + TaskNum);

		std::vector<std::vector<int> > opt_srvcs;
		int arr_opt_srvcs[] = {0, 0, 2, 3, 1, 4, 3, 4, 0, 0, 0, 0};
		int arr_opt_srvcs_count[TaskNum] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		int opt_srvcs_cum = 0;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec_opt_srvcs(arr_opt_srvcs + opt_srvcs_cum, arr_opt_srvcs + opt_srvcs_cum + arr_opt_srvcs_count[i]);
			opt_srvcs.push_back(vec_opt_srvcs);
			vec_opt_srvcs.clear();
			opt_srvcs_cum += arr_opt_srvcs_count[i];
		}

		const int RsrcNum = 7;
		const int SrvcNum = 5;

		int arr_RsrcCpcty[RsrcNum] = {10, 5, 8, 6, 4, 2, 5};
		std::vector<int> RsrcCpcty(arr_RsrcCpcty, arr_RsrcCpcty + RsrcNum);

		int arr_CoeMatrix[RsrcNum][SrvcNum] = {{2,0,0,3,0}, {1,2,0,0,5},{0,1,0,4,0},{0,0,3,0,0},{1,0,0,0,0},{0,1,0,0,0},{0,0,2,1,2}};
		std::vector<std::vector<int> > CoeMatrix;
		for (int i = 0; i < RsrcNum; ++i) {
			std::vector<int> temp(arr_CoeMatrix[i], arr_CoeMatrix[i] + SrvcNum);
			CoeMatrix.push_back(temp);
			temp.clear();
		}

		std::vector<int> SrvcCpcty;
		int temp_max[SrvcNum];
		for (int i = 0; i < SrvcNum; ++i) {
			temp_max[i] = INT_MAX;
			for (int j = 0; j < RsrcNum; ++j) {
				if (CoeMatrix[i][j] > 0) {
					temp_max[i] = min(temp_max[i], RsrcCpcty[j]/CoeMatrix[i][j]);
				}
			}
			SrvcCpcty.push_back(temp_max[i]);
		}

		DataSet AllData = {TaskNum, prec, proc_time, opt_srvcs, RsrcNum, SrvcNum, RsrcCpcty, SrvcCpcty, CoeMatrix};
		return AllData;
	}
};

class SeqGeneration
{
public:
	std::vector<int> RandSeq(DataSet &AllData) {
		int TaskNum = AllData.TaskNum;
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
	// map<std::vector<int>, int> LegalSeq(std::vector<int> &OriginalSeq, int TaskNum, std::vector<vector<int> > &prec, 
	// 	// std::vector<vector<int> > &succ, 
	// 	std::vector<int> &proc_time, std::vector<vector<int> > &opt_srvcs) {

	// std::vector<int> LegalSeq(std::vector<int> &OriginalSeq, int TaskNum, std::vector<vector<int> > &prec) {
		// std::vector<vector<int> > &succ, 
		// std::vector<int> &proc_time, std::vector<vector<int> > &opt_srvcs 

	std::vector<int> LegalSeq(std::vector<int> &OriginalSeq, DataSet &AllData) {
		// map<std::vector<int>, int> result;
		// int makespan = 0;
		int TaskNum = AllData.TaskNum;
		std::vector<std::vector<int> > prec = AllData.prec;

		std::vector<int> remov;
		std::vector<int> remain;
		std::vector<int> present;
		for (int i = 0; i < TaskNum; ++i)
			remain.push_back(i);
		std::vector<int> SeqTemp(OriginalSeq.begin(), OriginalSeq.begin() + OriginalSeq.size());
		std::vector<int> pre_act_prec;
		int temp_index;
		int pre_act;

		while (remain.size() > 0) {
			temp_index = 0;
			pre_act = SeqTemp[0];
			pre_act_prec = prec[pre_act];
			// std::vector<int> pre_act_succ(succ[0].begin(), succ[0].end());
			bool ok = isSubset(pre_act_prec, remov);

			while (!ok) {
				++temp_index;
				pre_act = SeqTemp[temp_index];
				pre_act_prec = prec[pre_act];
				// pre_act_prec(prec[pre_act].begin(), prec[pre_act].begin() + prec[pre_act].size());
				ok = isSubset(pre_act_prec, remov);
			} 

			remov.push_back(pre_act);
			remain.erase(find(remain.begin(), remain.end(), pre_act));
			SeqTemp.erase(find(SeqTemp.begin(), SeqTemp.end(), pre_act));
			
			pre_act_prec.clear();
		}
		return remov;
		// result.insert(make_pair(remov, makespan));
		// return result;
	}

	// int makespan(std::vector<int> &OriginalSeq, int TaskNum, std::vector<vector<int> > &prec, 
	// // std::vector<vector<int> > &succ, 
	// std::vector<int> &proc_time, std::vector<vector<int> > &opt_srvcs) {

	// }

	int makespan(std::vector<int> &OriginalSeq, DataSet &AllData) {
		const int TaskNum = AllData.TaskNum;
		std::vector<std::vector<int> > prec = AllData.prec;
		std::vector<int> proc_time = AllData.proc_time;
		std::vector<std::vector<int> > opt_srvcs = AllData.opt_srvcs;
		const int RsrcNum = AllData.RsrcNum;
		const int SrvcNum = AllData.SrvcNum;
		std::vector<int> RsrcCpcty = AllData.RsrcCpcty;
		std::vector<int> SrvcCpcty = AllData.SrvcCpcty;
		std::vector<std::vector<int> > CoeMatrix = AllData.CoeMatrix;

		
	}

private:
	bool isSubset(std::vector<int>& vec1, std::vector<int>& vec2) {
		if (vec1.size() == 0) return true;
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

int main(int argc, char const *argv[])
{
	DataInitialization di;
	SeqGeneration sg;
	SeqDecoding sd;
	DataSet AllData = di.Initiate();
	std::vector<int> Org = sg.RandSeq(AllData);
	for (int i = 0; i < Org.size(); ++i)
		cout << Org[i] << '\t';
	cout << endl;
	std::vector<int> Lgl = sd.LegalSeq(Org, AllData);
	for (int i = 0; i < Lgl.size(); ++i)
		cout << Lgl[i] << '\t';
	cout << endl;
	return 0;
}


/*
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

*/