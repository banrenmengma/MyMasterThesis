#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
using namespace std;

struct DataSet
{
	const int TaskNum;
	std::vector<std::vector<int> > prec;
	std::vector<int> proc_time;
	std::vector<std::vector<int> > opt_srvcs;
	std::vector<std::vector<int> > srvcs_requst;
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
		std::vector<std::vector<int> > srvcs_requst;
		int arr_opt_srvcs[] = {0, 0, 2, 3, 1, 4, 3, 4, 0, 0, 0};
		int arr_srvcs_requst[] = {0, 3, 2, 1, 1, 1, 2, 1, 3, 1, 0};
		int arr_opt_srvcs_count[TaskNum] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		int opt_srvcs_cum = 0;
		for (int i = 0; i < TaskNum; ++i) {
			std::vector<int> vec_opt_srvcs(arr_opt_srvcs + opt_srvcs_cum, arr_opt_srvcs + opt_srvcs_cum + arr_opt_srvcs_count[i]);
			opt_srvcs.push_back(vec_opt_srvcs);
			vec_opt_srvcs.clear();
			

			std::vector<int> vec_srvcs_requst(arr_srvcs_requst + opt_srvcs_cum, arr_srvcs_requst + opt_srvcs_cum + arr_opt_srvcs_count[i]);
			srvcs_requst.push_back(vec_srvcs_requst);
			vec_srvcs_requst.clear();

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

		DataSet AllData = {TaskNum, prec, proc_time, opt_srvcs, srvcs_requst, RsrcNum, SrvcNum, RsrcCpcty, SrvcCpcty, CoeMatrix};
		return AllData;
	}
};

// const int GroupNum = rand() % 4 + 2;
		// std::vector<int> TaskAllocate;
		// for (int i = 0; i < TaskNum; ++i) {
		// 	TaskAllocate.push_back(i);
		// }
		// random_shuffle(TaskAllocate.begin() + 1, TaskAllocate.end() - 1);
		// std::vector<int> vec_split;
		// vec_split.push_back(1);
		// vec_split.push_back(TaskNum - 1);
		// for (int i = 0; i < GroupNum - 1; ++i) {
		// 	int r_n = rand() % (TaskNum - 1) + 1;
		// 	while (isIn(vec_split, r_n)) {
		// 		r_n = rand() % (TaskNum - 1) + 1;
		// 	}
		// 	vec_split.push_back(r_n);
		// }
		// sort(vec_split.begin(), vec_split.end());
		// std::vector<std::vector<int> > vec_allocate;
		// for (int i = 0; i < GroupNum; ++i) {
		// 	std::vector<int> vec(TaskAllocate.begin() + vec_split[i], TaskAllocate.begin() + vec_split[i + 1]);
		// 	vec_allocate.push_back(vec);
		// 	vec.clear();
		// }

		// std::vector<std::vector<int> > prec;
		// for (int i = 0; i < TaskNum; ++i) {
		// 	std::vector<int> vec;
		// 	prec.push_back(vec);
		// }

		// for (int i = 0; i < GroupNum; ++i) {
		// 	for (size_t j = 0; j < vec_allocate[i].size(); ++j) {
		// 		if (j == 0) {
		// 			prec[vec_allocate[i][j]].push_back(0);
		// 		} else {
		// 			prec[vec_allocate[i][j]].push_back(vec_allocate[i][j - 1]);
		// 		}
		// 	}
		// 	prec[TaskNum - 1].push_back(vec_allocate[i][vec_allocate[i].size() - 1]);
		// }