#include <map>
#include <string>
#include <strategy_common/TTFea/factory.h>

namespace ttfea {
std::unordered_map<std::string, newBaseFunc>* Factory::g_feabase_map = NULL;

int Factory::regist(std::string name, newBaseFunc func) {
	if (g_feabase_map == NULL) {
		g_feabase_map = new std::unordered_map<std::string, newBaseFunc>();
	}
	if (g_feabase_map->find(name) == g_feabase_map->end()) {
		g_feabase_map->insert(std::unordered_map<std::string, newBaseFunc>::value_type(name, func));
	}
	return 0;
};

FeaBase* Factory::create(std::string name) {
	std::unordered_map<std::string, newBaseFunc>::const_iterator iter = g_feabase_map->find(name);
	if (g_feabase_map->end() != iter) {
		return (*iter->second)();
	}
	return NULL;
};
}