#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
using namespace std;

extern int service_num;
extern std::vector<std::vector<int> > coe_matrix;

std::map<string, int> Resource_map;
std::map<string, int> Service_map;
std::map<string, std::vector<int> > Requst_map;

