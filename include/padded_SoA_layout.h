#ifndef PADDED_SOA_LAYOUT_H
#define PADDED_SOA_LAYOUT_H
//This class is supposed to have actual SOAs that can be attached to the padded_SOA stuff...

#define AddScalar(type, name) \
type name;

#define AddArray(type, name, size) \
type name[size];

#define Addvector(type,name) \
std::vector<type>name;

#define Generate_Arrays(structName, ...) \
struct structName{ \
 __VA_ARGS__ \
\
};

class SoA_Layout{
    public:
        
        Generate_Arrays(myArray,
                AddArray(uint32_t, x, msize)
                AddArray(uint32_t, y, msize)
                AddArray(uint32_t, z, msize)
                AddScalar(int, id)
                AddScalar(int, r)
                )

    
    private:
         int msize = 500;
    


};

#endif