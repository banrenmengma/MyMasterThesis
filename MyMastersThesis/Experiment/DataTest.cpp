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
	std::vector<std::vector<int> > Initiate() {	
		const int TaskNum = rand() % 10 + 4;
		std::vector<std::vector<int> > matrix;
		std::vector<int> vec(TaskNum, 0);
		for (int i = 0; i < TaskNum; ++i) {
			matrix.push_back(vec);
		}
		int temp = (int)(TaskNum - 3)/2;
		if (temp == 0) temp++;
		int loop_num = (int)(1.5 * TaskNum - 1) - (int)(rand() % temp) + (int)(rand() % temp);

		bool flag;
		std::vector<int> row_col;
		int maxloop = 10;
		int count = 0;
		std::vector<int> v;
		while (noZeroNum(matrix) < loop_num) {
			row_col = r_c(TaskNum);
			while (matrix[row_col[0]][row_col[1]] != 0) {
				row_col = r_c(TaskNum);
			}
			matrix[row_col[0]][row_col[1]] = 1;
			flag = existLoop(matrix);
			while (flag) {
				matrix[row_col[0]][row_col[1]] = 0;
				row_col = r_c(TaskNum);

				matrix[row_col[0]][row_col[1]] = 1;
				flag = existLoop(matrix);
			}
			// cout << row_col[0] << '\t' << row_col[1] << '\t';
			ValidConvert(matrix);

			v.push_back(noZeroNum(matrix));
			if (count > 10) {
				if (v[count] == v[count - maxloop])
					break;
			}
			count++;
			
		}
		cout << TaskNum << '\t' << loop_num << '\t' << noZeroNum(matrix) << endl;

		// for (int i = 0; i < TaskNum; ++i) {
		// 	for (int j = 0; j < TaskNum; ++j) {
		// 		if (matrix[i][j] != 0) {
		// 			matrix[i][j] = 0;
		// 			matrix[j][i] = 1;
		// 			bool flag_temp = existLoop(matrix);
		// 			if (!flag_temp) {
		// 				matrix[i][j] = 1;
		// 			} 
		// 			matrix[j][i] = 0;
		// 		}
		// 	}
		// }

		std::vector<std::vector<int> > prec;

		return matrix;
	}
private:
	std::vector<int> r_c(int TaskNum) {
		std::vector<int> vec;
		int row;
		int col;
		row = rand() % TaskNum;
		col = rand() % TaskNum;
		while (col == row) {
			col = rand() % TaskNum;
		}
		vec.clear();
		vec.push_back(row);
		vec.push_back(col);
		return vec;
	}

	bool isIn(std::vector<int> &vec, int n) {
		if (vec.empty()) return false;
		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] == n) return true;
		}
		return false;
	}

	int zero_index(std::vector<std::vector<int> > &vec) {
		if (vec.empty()) return -1;
		for (size_t i = 0; i < vec[0].size(); ++i) {
			size_t count = 0;
			for (size_t j = 0; j < vec.size(); ++j) {
				if (vec[i][j] == 0) {
					count++;
				} else {
					break;
				}
			}
			if (count == vec.size()) {
				return i;
			}
		}
		return -1;
	}

	bool existLoop(std::vector<std::vector<int> > &vec_const) {
		std::vector<std::vector<int> > vec = vec_const;
		int num = vec.size();
		int index;
		index = zero_index(vec);
		while (index >= 0) {
			for (size_t i = 0; i < vec.size(); ++i) {
				vec[i].erase(vec[i].begin() + index);
			}
			vec.erase(vec.begin() + index);
			index = zero_index(vec);
			num--;
		}
		if (num > 0) {
			return true;
		} else {
			return false;
		}
	}

	void ValidConvert(std::vector<std::vector<int> > &matrix) {
		for (size_t i = 0; i < matrix.size(); ++i) {
			for (size_t j = 0; j < matrix.size(); ++j) {
				if (matrix[i][j] != 0) {
					matrix[i][j] = 0;
					matrix[j][i] = 1;
					bool flag_temp = existLoop(matrix);
					if (!flag_temp) {
						matrix[i][j] = 1;
					}	
					matrix[j][i] = 0;
				}
			}
		}
	}

	int noZeroNum(std::vector<std::vector<int> > &vec) {
		if (vec.empty()) return 0;
		int count = 0;
		for (size_t i = 0; i < vec.size(); ++i) {
			for (size_t j = 0; j < vec[i].size(); ++j) {
				if (vec[i][j] > 0) {
					count++;
				}
			}
		}
		return count;
	}
};

int main(int argc, char const *argv[])
{
	srand((unsigned)time(NULL));
	DataInitialization di;
	std::vector<std::vector<int> > vec = di.Initiate();
	// cout << vec.size();
	for (size_t i = 0; i < vec.size(); ++i) {
		for (size_t j = 0; j < vec[i].size(); ++j)
			cout << vec[i][j] << '\t';
		cout << endl;
	}
	return 0;
}
