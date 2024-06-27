#ifndef __ALGORITHM__
#define __ALGORITHM__

#include <istream>
#include <ostream>

class Algorithm{
public:
    Algorithm() = default;
    virtual ~Algorithm() = default;

    virtual void Encode(std::istream& in, std::ostream& out);
    virtual void Decode(std::istream& in, std::ostream& out);    
};


#endif