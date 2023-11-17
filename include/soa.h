#include <iostream>
#include <array>
#include <vector>
/*
 Attributes of the soa....
 1. Pointers should be smart enough to access correct members


 Things to note:
 1. How are the pointer address accessed?
 2. How are the objects stored?
 3. What should be the layout of the object?
 */

//Rudimentary macros to create class...
//note that I should not make it over-complicated....

namespace soa {

#define AddScalar(type, name) \
type name;

#define AddArray(type, name, size) \
type name[size];

#define Addvector(type,name) \
std::vector<type>name;

#define AddSoA(type, name) \
type name;

#define Generate_Arrays(structName, ...) \
struct structName{ \
 __VA_ARGS__ \
\
};

}
