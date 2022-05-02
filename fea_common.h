#ifndef __FEACOOMON_H__
#define __FEACOMMON_H__
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdint.h>
namespace ttfea {
class FeaOut {
public:
    int64_t length;
    int64_t pos;
    double val;
    bool is_multi;
    std::unordered_map<int64_t, double> fea_map;
    FeaOut() : length(1), pos(0), val(0.0), is_multi(false) {}
    FeaOut(int size) : length(size), pos(0), val(0.0), is_multi(false) {}
    void set_length(int64_t len);
    void set_multi(bool is_multi);
    int set_val(int64_t pos, double val);
    void clear();
    void clear_val();
    int copy_val(FeaOut& a);
    static FeaOut combine(FeaOut& a, FeaOut& b);
};
}