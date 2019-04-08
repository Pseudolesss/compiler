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
        void print(std::string); //temporarly
        void add(yy::location,std::string);
        bool isempty();
        ErrorHandler();
    private:
        std::multimap<yy::location,std::string,LocationComparator> errors;
};

#endif