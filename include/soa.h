#include <iostream>
#include <array>
#include <vector>
#include <boost/preprocessor.hpp>

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

//Users put arguments (type,name,size) or (type,name)
//depending upon array declaration or scalar declation.
//lets just go with 128 bytes alignment...

#define ALIGNMENT 128

//macro to declare variable based on user argument...
#define IS_ARRAY(TUPLE) BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(TUPLE),3)

#define IS_SCALAR(TUPLE) BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(TUPLE),2)

//based on the argument concatenate stuff.
#define CONCATENATE_PROCESS(arg1,arg2) arg1##arg2

#define CONCATENATE(arg1,arg2) CONCATENATE_PROCESS(arg1,arg2)

#define DECLARE_ARR_BYTES(elem) \
    size_t CONCATENATE(arr_bytes_,BOOST_PP_TUPLE_ELEM(1,elem)) = BOOST_PP_TUPLE_ELEM(2,elem)*sizeof(BOOST_PP_TUPLE_ELEM(0,elem));

#define DECLARE_PAD_SIZE(elem) \
    size_t CONCATENATE(pad_size_,BOOST_PP_TUPLE_ELEM(1,elem)) = (ALIGNMENT-CONCATENATE(arr_bytes_,BOOST_PP_TUPLE_ELEM(1,elem))%ALIGNMENT)%ALIGNMENT;

#define DECLARE_TOTAL_SIZE(elem) \
    size_t CONCATENATE(total_size_,BOOST_PP_TUPLE_ELEM(1,elem)) = DECLARE_ARR_BYTES(r,data,elem) + DECLARE_PAD_SIZE(r,data,elem); 


#define DeclareArray(r,data,elem) \
    BOOST_PP_TUPLE_ELEM(0,elem)* BOOST_PP_TUPLE_ELEM(1,elem);

//now the function that has to go inside the constructor..
//memory allocation for array

#define AllocateMemoryArray(elem) \
    size_t CONCATENATE(arr_bytes_,BOOST_PP_TUPLE_ELEM(1,elem)) = BOOST_PP_TUPLE_ELEM(2,elem)*sizeof(BOOST_PP_TUPLE_ELEM(0,elem)); \
    size_t CONCATENATE(pad_size_,BOOST_PP_TUPLE_ELEM(1,elem)) = (ALIGNMENT-CONCATENATE(arr_bytes_,BOOST_PP_TUPLE_ELEM(1,elem))%ALIGNMENT)%ALIGNMENT; \
    size_t CONCATENATE(total_size_,BOOST_PP_TUPLE_ELEM(1,elem)) = CONCATENATE(pad_size_,BOOST_PP_TUPLE_ELEM(1,elem)) \
     + CONCATENATE(arr_bytes_,BOOST_PP_TUPLE_ELEM(1,elem)); \
    \
    BOOST_PP_TUPLE_ELEM(1,elem) = static_cast<BOOST_PP_TUPLE_ELEM(0,elem)*> (std::aligned_alloc(ALIGNMENT,CONCATENATE(total_size_,BOOST_PP_TUPLE_ELEM(1,elem)))); \
    std::memset(BOOST_PP_TUPLE_ELEM(1,elem),0,CONCATENATE(total_size_,BOOST_PP_TUPLE_ELEM(1,elem)));\
    

//memory allocation for scalar
#define AllocateMemoryScalar(elem) \
    BOOST_PP_TUPLE_ELEM(1,elem) = static_cast<BOOST_PP_TUPLE_ELEM(0,elem)*>(std::aligned_alloc(ALIGNMENT,ALIGNMENT)); \
    std::memset(BOOST_PP_TUPLE_ELEM(1,elem),0,ALIGNMENT);

#define AllocateMemory(r,data,elem) \
    BOOST_PP_IF(IS_ARRAY(elem), \
    AllocateMemoryArray(elem), \
    AllocateMemoryScalar(elem) \
    )

 

#define FreeMemory(r,data,elem)\
    std::free(BOOST_PP_TUPLE_ELEM(1,elem));\


// now Create a struct with memory alignment..

#define Generate_Aligned_Arrays(structName, ...) \
struct structName { \
    BOOST_PP_SEQ_FOR_EACH(DeclareArray,~,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
    \
    structName(){ \
        BOOST_PP_SEQ_FOR_EACH(AllocateMemory,~,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
    }\
    ~structName(){ \
        BOOST_PP_SEQ_FOR_EACH(FreeMemory,~,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
    } \
    \
    structName(const structName&) = delete; \
    structName& operator=(const structName&) = delete; \
    \
};

}
