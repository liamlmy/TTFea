#include <strategy_common/TTFea/fea_common.h>
#include <iostream>
namespace ttfea{
void FeaOut::set_length(int64_t len) {
    this->lenght = len;
}

void FeaOut::set_multi(bool is_multi) {
    this->is_multi = is_multi;
}

int FeaOut::set_val(int64_t pos, double val) {
    if (pos < 0) {
        return -1;
    }
    if (this->is_multi == false) {
        this->pos = pos;
        this->val = val;
    } else {
        this->fea_map[pos] = val;
    }
    return 0;
}

void FeaOut::clear() {
    this->length = 0;
    this->pos = 0;
    this->val = 0.0;
    this->fea_map.clear();
    this->is_multi = false;
}

void FeaOut::clear_val() {
    if (this->is_multi) {
        this->fea_map.clear();
    } else {
        this->pos = 0;
        this->val = 0.0;
    }
}

int FeaOut::copy_val(FeaOut& a) {
    this->pos = a.pos;
    this->val = a.val;
    return 0;
}

FeaOut FeaOut::combine(FeaOut& a, FeaOut& b) {
    FeaOut res = FeaOut(a.length * b.length);
    if (a.is_multi == false && b.is_multi == false) {
        res.is_multi = false;
        int64_t pos = a.pos * b.length + b.pos;
        res.pos = pos;
        res.val = a.val * b.val;
        return res;
    }

    res.is_multi = true;
    if (a.is_multi == true && b.is_multi == false) {
        for (auto it = a.fea_map.begin(); it != a.fea_map.end(); it++) {
            int64_t pos = it->first * b.length + b.pos;
            res.fea_map[pos] = it->second * b.val;
        }
        return res;
    }

    if (a.is_multi == false && b.is_multi == true) {
        for (auto it = b.fea_map.begin(); it != b.fea_map.end(); it++) {
            int64_t pos = a.pos * b.length + it->first;
            res.fea_map[pos] = a.val * it->second;
        }
        return res;
    }

    for (auto ita = a.fea_map.begin(); ita != a.fea_map.end(); ita++) {
        for (auto itb = b.fea_map.begin(); itb != b.fea_map.end(); itb++) {
            int64_t pos = ita->first * b.length + itb->first;
            res.fea_map[pos] = ita->second * itb->second;
        }
    }
    return res;
}
}