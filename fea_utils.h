#ifndef __FEAUTILS_H__
#define __FEAUTILS_H__
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdint.h>
using namespace std;
namespace ttfea{
int split(string& str, char mod, vector<string>& result);
int strip(string& str);
int strip(string& str, char ch);
int str2int(const string& str, int* pval);
int str2int64(const string& str, int64_t* pval);
int str2uint64(const string& str, uint64_t* pval);
int str2double(const string& str, double* pval);
unsigned int murMurHash(const void* key, int len);
string double2str(double val);
int double2int(double val);
string int2str(int val);
string int642str(int64_t val);
void time2str(time_t n, const char* format, char strTime[], int bufLen);
time_t str2time(std::string timeStamp, std::string format);
double get_angle(double xbegin_lng, double xbegin_lat, double xend_lng, double xend_lat, double ybegin_lng, double ybegin_lat, double yend_lng, double yend_lat);
double get_sphere_distance(double lat1, double lng1, double lat2, double lng2);
double get_direct_distance(double lat1, double lng1, double lat2, double lng2);
bool equal(double x, double y);
bool less(double x, double y);
bool less_equal(double x, double y);
bool sort_on_weight(const pair<string, double>& p1, const pair<string, double>& p2);
unsigned int BKDHash(string);
int parse_array(const string* s, vector<double>& res, const char sep=',', size_t pading_size=0, double padding_value=0.0);
int parse_array(const string* s, vector<uint64_t>& res, const char sep=',', size_t padding_size=0, uint64_t padding_value=0);

class StringSpliter {
public:
    char spt;
    size_t spt_length;
    const std::string* p_str;
    size_t now_begin;
    size_t now_end;
    size_t begin_pos;
    size_t end_pos;
    size_t field_num;
    StringSpliter() : spt('\0'), spt_length(0), p_str(NULL), now_begin(0), now_end(0), begin_pos(0), end_pos(0), field_num(0) {};
    StringSpliter(const std::string& str, char spt, size_t bpos=0, size_t length=string::npos) {
        this->spt = spt;
        this->spt_length = 1;
        this->p_str = &str;
        this->begin_pos = bpos;
        this->end_pos = str.size();
        if (length != string::npos && this->begin_pos + length < this->end_pos) {
            this->end_pos = this->begin_pos + length;
        }
        this->now_begin = bpos;
        this->now_end = bpos;
        this->field_num = 0;
    };
    StringSpliter(char sub_spt, const StringSpliter& ss) {
        this->spt = sub_spt;
        this->spt_length = 1;
        this->p_str = ss.p_str;
        this->begin_pos = ss.now_begin;
        this->end_pos = ss.now_end;
        this->now_begin = this->begin_pos;
        this->now_end = this->begin_pos;
        this->field_num = 0;
    };
    bool next();
    int get_field(string& res);
    int get_tail(string& res);
    int get_next_field(string& res);
    int field_length();
};

template <class RandomAccessIterator, class T, class Compare>
RandomAccessIterator binary_find(RandomAccessIterator first, RandomAccessIterator last, const T& val, Compare comp) {
    if (first >= last) {
        return last;
    }
    if (comp(val, *first)) {
        return first;
    }
    if (!comp(val, *(last - 1))) {
        return last;
    }
    --last;
    while (last - first > 1) {
        auto mid = first + (last - first) / 2;
        if (comp(val, *mid)) {
            last = mid;
        } else {
            first = mid;
        }
    }
    return last;
}

struct DoubleCompare {
    bool operator() (const double& x, const double& y) const {
        return less_equal(x, y);
    }
};
}
#endif
