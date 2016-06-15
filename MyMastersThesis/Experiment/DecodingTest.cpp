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
	std::vector<std::vector<int> > srvcs_requst;
	const int RsrcNum;
	const int SrvcNum;
	std::vector<int> RsrcCpcty;
	std::vector<int> SrvcCpcty;
	std::vector<std::vector<int> > CoeMatrix;
};

struct Interval
{
	int start;
	int end;
	int height;
	Interval(): start(0), end(0), height(0) {}
	Interval(int s, int e, int h): start(s), end(e), height(h) {}	
};

struct InvalidPos
{
	bool valid_flag;
	int invalid_pos;
	InvalidPos(): valid_flag(true), invalid_pos(-1) {}
	InvalidPos(bool v, int i): valid_flag(v), invalid_pos(i) {}
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
		std::vector<std::vector<int> > srvcs_requst = AllData.srvcs_requst;
		const int RsrcNum = AllData.RsrcNum;
		// const int SrvcNum = AllData.SrvcNum;
		std::vector<int> RsrcCpcty = AllData.RsrcCpcty;
		std::vector<int> SrvcCpcty = AllData.SrvcCpcty;
		std::vector<std::vector<int> > CoeMatrix = AllData.CoeMatrix;

		int result = 0;
		map<int, std::vector<Interval> > RsrcCumFunc;
		map<int, std::vector<Interval> > SrvcCumFunc;
		map<int, std::vector<Interval> >::iterator it;
		std::vector<Interval> EmptyItv;
		map<int, Interval> schd;
		Interval s_dummy(0, 0, 0);
		schd.insert(make_pair(0, s_dummy));
		srand((unsigned)time(NULL));
		for (int i = 1; i < TaskNum - 1; ++i) {
			int OpId = OriginalSeq[i];
			int srvc_index = rand() % opt_srvcs[i].size();
			int SrvcId = opt_srvcs[OpId][srvc_index];
			// cout << OpId;
			int StartTime = 0;
			for (size_t j = 0;  j < prec[OpId].size(); ++j) {
				StartTime = max(StartTime, schd[prec[OpId][j]].end);
			}
			// int StartTime = schd[i - 1].end();
			Interval new_itv;
			std::vector<int> vec_st;
			for (int j = 0; j < RsrcNum; ++j) {
				if (CoeMatrix[j][SrvcId] > 0) {
					it = RsrcCumFunc.find(j);
					if (it == RsrcCumFunc.end()) {
						RsrcCumFunc.insert(make_pair(j, EmptyItv));
					}
					// if (i == 10) cout << j;
					new_itv = newItv(StartTime, proc_time[OpId], srvcs_requst[OpId][srvc_index] * CoeMatrix[j][SrvcId], RsrcCpcty[j], RsrcCumFunc[j]);	
					// if (i == 10) cout << j;
					vec_st.push_back(new_itv.start);								
					StartTime = max(StartTime, new_itv.start);	
				} 
			}

			// cout << StartTime;

			while (!allEqual(vec_st)) {
				vec_st.clear();
				for (int j = 0; j < RsrcNum; ++j) {
					if (CoeMatrix[j][SrvcId] > 0) {
						// it = RsrcCumFunc.find(j);
						// if (it == RsrcCumFunc.end()) {
						// 	RsrcCumFunc.insert(make_pair(j, EmptyItv));
						// }

						// int RsrcRequst = srvcs_requst[i][SrvcId] * CoeMatrix[j][SrvcId];
						new_itv = newItv(StartTime, proc_time[OpId], srvcs_requst[OpId][srvc_index] * CoeMatrix[j][SrvcId], RsrcCpcty[j], RsrcCumFunc[j]);
						vec_st.push_back(new_itv.start);
						StartTime = max(StartTime, new_itv.start);
					} 
				}
			}

			for (int j = 0; j < RsrcNum; ++j) {
				if (CoeMatrix[j][SrvcId] > 0) {
					// it = RsrcCumFunc.find(j);
					// if (it == RsrcCumFunc.end()) {
					// 	RsrcCumFunc.insert(make_pair(j, EmptyItv));
					// }
					// int RsrcRequst = srvcs_requst[i][SrvcId] * CoeMatrix[j][SrvcId];

					new_itv = newItv(StartTime, proc_time[OpId], srvcs_requst[OpId][srvc_index] * CoeMatrix[j][SrvcId], RsrcCpcty[j], RsrcCumFunc[j]);
					itv_insert(RsrcCumFunc[j], new_itv);
				}
			}

			Interval itv(StartTime, StartTime + proc_time[OpId], srvcs_requst[OpId][srvc_index]);
			schd.insert(make_pair(OpId, itv));
			result = max(result, schd[OpId].end);
			// cout << result << '\t';
			
		}	
		// Interval e_dummy(schd[TaskNum - 2].end, schd[TaskNum - 2].end + 0, 0);
		// schd.push_back(e_dummy);
		// result = schd[TaskNum - 1].end;
		return result;
	}

private:

	bool allEqual(std::vector<int> &vec) {
		if (vec.size() <= 1) {
			return true;
		} else {
			for (size_t i = 1; i < vec.size(); ++i) {
				if (vec[i] != vec[0]) {
					return false;
				} 
			}
		}
		return true;
	}

	Interval newItv(int StartTime, int ProTime, int RsrcHght, int RsrcLmt, std::vector<Interval> &itvs) {
		std::vector<Interval> const_itvs = itvs;
		Interval new_itv(StartTime, StartTime + ProTime, RsrcHght);
		std::vector<Interval> itvs_temp = itvs;
		InvalidPos IP = isValidIsert(itvs_temp, new_itv, RsrcLmt);

		while (!IP.valid_flag) {
			new_itv.start = itvs_temp[IP.invalid_pos].end;
			new_itv.end = new_itv.start + ProTime;
			IP = isValidIsert(itvs_temp, new_itv, RsrcLmt);
		}
		return new_itv;
	}

	InvalidPos isValidIsert(std::vector<Interval> &const_itvs, Interval new_itv, int RsrcLmt) {
		bool valid_flag = true;
		int invalid_pos = -1;
		std::vector<Interval> itvs = const_itvs;
		std::vector<Interval> itvs_temp = itv_insert(itvs, new_itv);
		for (size_t i = 0; i < itvs_temp.size(); ++i) {
			if (itvs_temp[i].height > RsrcLmt) {
				valid_flag = false;
				invalid_pos = i - 1;
			}
		}
		itvs.clear();
		InvalidPos IP(valid_flag, invalid_pos);
		return IP;
	}

	bool isSubset(std::vector<int>& vec1, std::vector<int>& vec2) {
		if (vec1.size() == 0) return true;
		std::vector<int>::iterator it;
		for (size_t i = 0; i < vec1.size(); ++i) {
			it = find(vec2.begin(), vec2.end(), vec1[i]);
			if (it == vec2.end()) {
				return false;
			} 
		}
		return true;
	}

	bool isOverLap(Interval itv1, Interval itv2) {
		if (itv1.start < itv2.end && itv1.end > itv2.start) {
			return true;
		} else {
			return false;
		}	 
	}

	std::vector<Interval> itv_insert(std::vector<Interval> &itvs, Interval new_itv) {
		if (itvs.empty()) {
			Interval itv_init(0, INT_MAX/2, 0);
			itvs.push_back(itv_init);
		}
		if (new_itv.height == 0) return itvs;
		// std::vector<Interval>:: iterator it = itvs.begin();
		std::vector<Interval>:: iterator it_first, it_last;
		int count = 0;
		int first_pos;
		int last_pos;
		int break_flag = 0;
		auto it = itvs.begin() + itvs.size() - 1;
		for (int i = itvs.size() - 1; it >= itvs.begin(); --it, --i) {
			if (isOverLap(*it, new_itv)) {
				++count;
				if (count == 1) {
					it_last = it;
					last_pos = i;
				}
				it_first = it;
				first_pos = i;
				break_flag = i;
			}
			if (break_flag > i) break;
		}

		Interval itv_temp2(new_itv.end, it_last->end, it_last->height);

		Interval itv_temp1(it_first->start, new_itv.start, it_first->height);

		for (auto it = it_last; it >= it_first; --it) {
			if (it == it_last) {
				it->start = it_last->start;
				it->end = new_itv.end;
			}
			if (it == it_first) {
				it->start = new_itv.start;
				it->end = it_first->end;
			}
			it->height += new_itv.height; 
		}

		itvs.insert(itvs.begin() + last_pos + 1, itv_temp2);
		itvs.insert(itvs.begin() + first_pos, itv_temp1);

		return itvs;
	}

	/*
	std::vector<Interval> itv_true_insert(std::vector<Interval> &itvs, Interval new_itv) {
		if (itvs.empty()) {
			Interval itv_init(0, INT_MAX/2, 0);
			itvs.push_back(itv_init);
		}
		if (new_itv.height == 0) return itvs;
		// std::vector<Interval>:: iterator it = itvs.begin();
		std::vector<Interval>:: iterator it_first, it_last;
		int count = 0;
		int first_pos;
		int last_pos;
		int break_flag = 0;
		auto it = itvs.begin() + itvs.size() - 1;
		for (int i = itvs.size() - 1; it >= itvs.begin(); --it, --i) {
			if (isOverLap(*it, new_itv)) {
				++count;
				if (count == 1) {
					it_last = it;
					last_pos = i;
				}
				it_first = it;
				first_pos = i;
				break_flag = i;
			}
			if (break_flag > i) break;
		}

		Interval itv_temp2(new_itv.end, it_last->end, it_last->height);

		Interval itv_temp1(it_first->start, new_itv.start, it_first->height);

		for (auto it = it_last; it >= it_first; --it) {
			if (it == it_last) {
				it->start = it_last->start;
				it->end = new_itv.end;
			}
			if (it == it_first) {
				it->start = new_itv.start;
				it->end = it_first->end;
			}
			it->height += new_itv.height; 
		}

		if (itv_temp2.start == itv_temp2.end) {
			if (itvs.end() - it_last >= 2) {
				if ((it_last + 1)->height == it_last->height) {
					(it_last + 1)->start = it_last->start;
					itvs.erase(it_last);
				}
			}			
		} else {
			itvs.insert(itvs.begin() + last_pos + 1, itv_temp2);
		}

		if (itv_temp1.start == itv_temp1.end) {
			if (it_first - itvs.begin() >= 1) {
				if ((it_first - 1)->height == it_first->height) {
					(it_first - 1)->end = it_first->end;
					itvs.erase(it_first);
				}
			}
		} else {
			itvs.insert(itvs.begin() + first_pos, itv_temp1);
		}

		return itvs;
	}
	*/
};

int main(int argc, char const *argv[])
{
	double dur;
	clock_t s_time, e_time;
	s_time = clock();
	DataInitialization di;
	SeqGeneration sg;
	SeqDecoding sd;
	DataSet AllData = di.Initiate();
	std::vector<int> Org = sg.RandSeq(AllData);
	for (size_t i = 0; i < Org.size(); ++i)
		cout << Org[i] << '\t';
	cout << endl;
	std::vector<int> Lgl = sd.LegalSeq(Org, AllData);
	for (size_t i = 0; i < Lgl.size(); ++i)
		cout << Lgl[i] << '\t';
	cout << endl;
	int makespan = sd.makespan(Lgl, AllData);
	cout << "makespan = " << makespan << endl;
	e_time = clock();
	dur = (double)(e_time - s_time);
    printf("Use Time:%f\n",(dur/CLOCKS_PER_SEC));
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