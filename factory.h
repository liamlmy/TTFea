#ifndef __FACTORY_H__
#define __FACTORY_H__
#include <iostream>
#include <string>
#include <unordered_map>

namespace ttfea {
class FeaBase;
typedef FeaBase* (*newBaseFunc)(void);
#define DECLARE_CLASS(name) \
    protected:    \
    static Registor _rgt;    \
    public:    \
    static FeaBase* create();

#define IMPLEMENT_CLASS_COMMON(name, func)    \
    Registor name::_rgt((#name),    \
        (newBaseFunc) func);

#define IMPLEMENT_CLASS(name)    \
    FeaBase* name::create()    \
        {return new name;}

class Factory {
public:
    static std::unordered_map<std::string, newBaseFunc>* g_feabase_map;
    static int regist(std::string name, newBaseFunc func);
    static FeaBase* create(std::string name);
};

class Registor {
public:
    Registor(std::string name, newBaseFunc pfunc) {
        Factory::regist(name, pfunc);
    }
};
}

#endif