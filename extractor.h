#ifndef __EXTRACTOR_H__
#define __EXTRACTOR_H__
#include <stdint.h>
#include <strategy_common/TTFea/FeaBase.h>
#include <strategy_common/TTFea/fea_common.h>
#include <strategy_common/TTFea/parser.h>
#include <strategy_common/TTFea/resource.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
namespace ttfea {
class FeaBase;
class DataIf;
class Extractor {
public:
	unordered_map<int64_t, double> fea_result;
	int64_t fea_res_length;
	vector<FeaBase*> fea_list;
	unordered_map<string, FeaBase*> name_dict;
	const unordered_map<string, string>* u_map;
	const unordered_map<string, string>* s_map;
	unordered_map<string, double> inner_col;
	bool is_auto_slot;
	bool enable_list_cache;
	bool hit_list_cache;
	string list_cache_key;
	string list_cache_col;
	unordered_map<string, DataIf*> cache_data;
	ResourceManage* p_resc_mgr;
	Extractor() : u_map(NULL), s_map(NULL),
	              is_auto_slot(true),
	              enable_list_cache(false),
	              hit_list_cache(false),
	              p_resc_mgr(NULL) {}
	~Extractor() {this->free();}
	int append_S(FeaBase* S);
	int add_cols(const string& k, double& v);
	int add_feaout(FeaBase* S);
	int extract_fea();
	void auto_slot();
	void fill_result();
	int parse_fea_line(string& line, unordered_map<string, string>& dict);
	int make_S(unordered_map<string, string>& dict);
	int bind_fea(string& fea_conf);
	int clone_to(Extractor* ext);
	void free();
	void clear_cache();
	int debug(char* buf, int maxlen);
	int parse_map(const unordered_map<string, string>& s_map);
	int parse_map(const unordered_map<string, string>& s_map, const unordered_map<string, string>& u_map);
	ResourceManage* get_resc_mgr();
	int set_resc_mgr(ResourceManage* ptr);
	int set_list_cache_col(const string& s);
	int set_hit_list_cache();
};
}
#endif