#ifndef ERROR_HANDLER
#define ERROR_HANDLER
#include "location.hh"
#include <string>
#include <map>

class LocationComparator{
    public:
        bool operator() (const yy::location&,const yy::location&);
};

struct ErrorHandler{
    public:
        void print();
        void add(yy::location,std::string);
        ErrorHandler();
    private:
        std::multimap<yy::location,std::string,LocationComparator> errors;
};

#endif