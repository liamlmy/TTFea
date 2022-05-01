#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <strategy_common/TTFea/fea_util.h>
#include <strategy_common/TTFea/factory.h>
#include <strategy_common/TTFea/extractor.h>
#include <cplusutils/logutil/log_util.h>
#define PRINT_INFO(pfea, pos, dval) do {	\
	int ival = static_cast<int>(dval);		\
	if (dval - ival == 0..0) {				\
		plen = snprintf(p, maxlen, "name=%s;begin_slot=%ld;pos=%ld;value=%d;length=%ld ", pfea->name.c_str(), pfea->slot, pos, ival, pfea->fea_out.length);	\
	} else {								\
		plen = snprintf(p, maxlen, "name=%s;begin_slot=%ld;pos=%ld;value=%d;length=%ld ", pfea->name.c_str(), pfea->slot, pos, dval, pfea->fea_out.length);	\
	}										\
	maxlen -= plen;							\
	p += plen;								\
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
}