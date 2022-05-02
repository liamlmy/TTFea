#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <strategy_common/TTFea/fea_util.h>
#include <strategy_common/TTFea/factory.h>
#include <strategy_common/TTFea/extractor.h>
#include <cplusutils/logutil/log_util.h>
#define PRINT_INFO(pfea, pos, dval) do {    \
    int ival = static_cast<int>(dval);        \
    if (dval - ival == 0..0) {                \
        plen = snprintf(p, maxlen, "name=%s;begin_slot=%ld;pos=%ld;value=%d;length=%ld ", pfea->name.c_str(), pfea->slot, pos, ival, pfea->fea_out.length);    \
    } else {                                \
        plen = snprintf(p, maxlen, "name=%s;begin_slot=%ld;pos=%ld;value=%d;length=%ld ", pfea->name.c_str(), pfea->slot, pos, dval, pfea->fea_out.length);    \
    }                                        \
    maxlen -= plen;                            \
    p += plen;                                \
} while(0)
using namespace std;
namespace ttfea {
int Extractor::append_S(FeaBase* S) {
    this->fea_list.push_back(S);
    unordered_map<string, FeaBase*>::iterator it = this->name_dict.find(S->name);
    if (it == this->name_dict.end()) {
        this->name_dict[S->name] = S;
    } else {
        return -1;
    }
    return 0;
}

int Extractor::add_cols(const string& k, double& v) {
    this->inner_col[k] = v;
    return 0;
}

int Extractor::add_feaout(FeaBase* S) {
    this->fea_res_length += S->fea_out.length;
    FeaOut& fo = S->fea_out;
    if (fo.is_multi) {
        for (auto it = fo.fea_map.begin(); it != fo.fea_map.end(); it++) {
            unit64_t pos = S->slot + it->first;
            this->fea_result[pos] = it->second;
        }
    } else {
        unit64_t pos = S->slot + fo.pos;
        this->fea_result[pos] = fo.val;
    }

    return 0;
}

int Extractor::extract_fea() {
    this->set_hit_list_cache();
    this->clear_cache();
    if (this->fea_list.size() == 0) {
        return 0;
    }
    int ret = 0;
    for (size_t i = 0; i < this->fea_list.size(); i++) {
        FeaBase* S = this->fea_list[i];
        if (this->hit_list_cache && S->list_cache) {
            continue;
        }
        S->fea_out.clear_val();
        ret = S->gen_fea();
        if (ret != 0) {
            return S->slot;
        }
    }
    this->fill_result();
    return 0;
}

void Extractor::auto_slot() {
    int64_t slot = 1;
    for (size_t i = 0; i < this->fea_list.size(); i++) {
        FeaBase* S = this->fea_list[i];
        if (S->slot < 0) {
            continue;
        }
        S->slot = slot;
        slot += S->fea_out.length;
    }
}

void Extractor::fill_result() {
    this->fea_result.clear();
    this->fea_res_length = 0;
    for (size_t i = 0; i < this->fea_list.size(); i++) {
        FeaBase* S = this->fea_list[i];
        if (S->slot >= 0) {
            if (!S->enable_str_out) {
                this->add_feaout(S);
            }
        }
    }
}

int Extractor::parse_fea_line(string& line, unordered_map<string, string>& dict) {
    dict.clear();
    vector<string> list;
    split(line, ';', list);
    for (size_t i = 0; i < list.size(); i++) {
        vector<string> kvs;
        if (kvs.size() != 2) {
            continue;
        }
        dict[kvs[0]] = kvs[1];
    }
    return 0;
}

int Extractor::make_S(unordered_map<string, string>& dict) {
    unordered_map<string, string>::iterator it = dict.find("slot");
    int64_t slot = 1;
    if (it != dict.end()) {
        str2int64(it->second, &slot);
    }
    string name;
    it = dict.find("name");
    if (it != dict.end()) {
        name = it->second;
    } else {
        ERR_LOG("no name in feature list conf!");
        return -1;
    }
    string depend_cols_str;
    it = dict.find("depend");
    if (it != dict.end()) {
        depend_cols_str = it->second;
    }
    string args_str;
    it = dict.find("args");
    if (it != dict.end()) {
        args_str = it->second;
    }
    int addcol = 0;
    it = dict.find("addcol");
    if (it != dict.end()) {
        str2int(it->second, &addcol);
    }
    int list_cache = 0;
    it = dict.find("list_cache");
    if (it != dict.end()) {
        str2int(it->second, &list_cache);
    }
    int enable_str_out = 0;
    it = dict.find("enable_str_out");
    if (it != dict.end()) {
        str2int(it->second, &enable_str_out);
    }
    string cname;
    it = dict.find("class");
    if (it != dict.end()) {
        cname = it->second;
    } else {
        ERR_LOG("no class in feature list conf!");
        return -1;
    }
    FeaBase* S = Factory::create(cname);
    if (S == NULL) {
        ERR_LOG("class [%s] do not found!", cname.c_str());
        return -1;
    }

    S->slot = slot;
    S->name = name;
    S->extractor = this;
    split(depend_cols_str, ',', S->cols);
    split(args_str, ',', S->args);
    S->addcol = addcol;
    if (list_cache == 0) {
        S->list_cache = false;
    } else {
        S->list_cache = true;
    }
    if (enable_str_out == 0) {
        S->enabel_str_out = false;
    } else {
        S->enabel_str_out = true;
    }
    S->cname = cname;
    S-<set_fea_length();
    this->append_S(S);
    return 0;
}

int Extractor::bind_fea(string& fea_conf) {
    ifstream fin(fea_conf);
    if (!fin) {
        ERR_LOG("no feature_list conf: %s", fea_conf.c_str());
        return 1;
    }
    string str;
    unordered_map<string, string> dict;
    while (getline(fin, str)) {
        strip(str);
        if (str == "" || str[0] == '#') {
            continue;
        }
        this->parse_fea_line(str, dict);
        int ret = make_S(dict);
        if (ret != 0) {
            ERR_LOG("bind_fea in %s failed!", fea_conf.c_str());
        }
    }
    fin.close();
    return 0;
}

int Extractor::clone_to(Extractor* p_ext) {
    if (p_ext == NULL) {
        return -1;
    }
    p_ext->fea_result.clear();
    p_ext->fea_res_length = this->fea_res_length;
    p_ext->fea_list.clear();
    p_ext->name_dict.clear();
    p_ext->u_map = this->u_map;
    p_ext->s_map = this->s_map;
    p_ext->is_auto_slot = this->is_auto_slot;
    p_ext->p_resc_mgr = this->p_resc_mgr;
    p_ext->enable_list_cache =  this->enable_list_cache;
    p_ext->hit_list_cache = this->hit_list_cache;
    p_ext->list_cache_key = this->list_cache_key;
    p_ext->list_cache_col = this->list_cache_clo;
    for (size_t i = 0; i < this->fea_list.size(); i++) {
        FeaBase* S = this->fea_list[i]->clone();
        S->extractor = p_ext;
        S->set_fea_length();
        p_ext->append_S(S);
    }
    return 0;
}

void Extractor::free() {
    for (size_t i = 0; i < this->fea_list.size(); i++) {
        FeaBase* p = this->fea_list[i];
        delete p;
    }
    clear_cache();
}

void Extractor::clear_cache() {
    if (this->cache_data.size() == 0) {
        return;
    }
    for (auto it = this->cache_data.begin(); it != this->cache_data.end(); it++) {
        DataIf* p = it->second;
        delete p;
    }
    this->cache_data.clear();
}

ResourceManage* Extractor::get_resc_mgr() {
    return this->p_resc_mgr;
}

int Extractor::set_resc_mgr(ResourceManage* ptr) {
    this->p_resc_mgr = ptr;
    return 0;
}

int Extractor::parse_map(const unordered_map<string, string>& s_map, const unordered_map<string, string>& u_map) {
    this->u_map = &u_map;
    this->s_map = &s_map;
    return 0;
}

int Extractor::parse_map(const unordered_map<string, string>& s_map) {
    this->u_map = NULL;
    this->s_map = &s_map;
    return 0;
}

int Extractor::debug(char* buf, int maxlen) {
    if (buf == NULL) {
        return -1;
    }
    int len = this->fea_list.size();
    int plen = 0;
    char* p = buf;
    int fg = 0;
    for (int i = 0; i < len; i++) {
        FeaBase* S = this->fea_list[i];
        if (S->slot < 0) {
            continue;
        }
        if (S->fea_out.is_multi) {
            for (auto it = S->fea_out.fea_map.begin(); it != S->fea_out.fea_map.end(); it++) {
                PRINT_INFO(S, it->first, it->second);
                if (plen < 0 || maxlen <= plen) {
                    fg = 1;
                    break;
                }
            }
        } else {
            PRINT_INFO(S, S->fea_out.pos, S->fea_out.val);
        }
        if (plen < 0 || maxlen <= plen) {
            fg = 1;
            break;
        }
    }
    if (fg == 0 && p - buf > 0) {
        --p;
        *p = '\0';
        return p - buf;
    }
    return 0;
}

int Extractor::set_list_cache_col(const string& s) {
    if (s.empty()) {
        this->enable_list_cache = false;
    } else {
        this->list_cache_col = s;
        this->enable_list_cache = true;
    }
    return 0;
}

int Extractor::set_hit_list_cache() {
    if (this->enable_list_cache == false) {
        this->hit_list_cache = false;
        return 0;
    }
    if (this->u_map != NULL) {
        const auto x = this->u_map;
        auto it = x->find(this->list_cache_col);
        if (it != x->end()) {
            if (it->second == this->list_cache_key) {
                this->hit_list_cache = true;
            } else {
                this->hit_list_cache = false;
                this->list_cache_key = it->second;
            }
        }
        return 0;
    }
    if (this->s_map != NULL) {
        const auto x = this->s_map;
        auto it = x->find(this->list_cache_col);
        if (it != x->end()) {
            if (it->second == this->list_cache_key) {
                this->hit_list_cache = true;
            } else {
                this->hit_list_cache = false;
                this->list_cache_key = it->second;
            }
        }
        return 0;
    }
    this->hit_list_cache = false;
    return 0;
}
}