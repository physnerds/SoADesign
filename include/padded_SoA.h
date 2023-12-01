#ifndef PADDED_SoALayout_H
#define PADDED_SoALayout_H 1

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace soa{
struct SoALayout{
    //these are arrays
    uint32_t x;
    uint32_t y;
    uint32_t z;

    //the below are the scalars
    int id;
    int r;
/// @brief what is the length of arrays (all arrays have same length)
    size_t num_elements; 
    size_t pad_size; //padding at the end of arrays....

    /// @brief Constructor for the SoALayout
    SoALayout(){};
    SoALayout(size_t elements): num_elements(elements){
        size_t arr_bytes = elements* sizeof(uint32_t);
        pad_size = (128-(arr_bytes%128))%128; //Putting a padding of 128 elements....
        size_t total_size = arr_bytes + pad_size;
        

        //allocate aligned memory for arrays...
     /*
        x = static_cast<uint32_t*> (std::aligned_alloc(128,total_size));
        y = static_cast<uint32_t*> (std::aligned_alloc(128,total_size));
        z = static_cast<uint32_t*> (std::aligned_alloc(128,total_size));

        //alignment for the scalars..

        id = static_cast<int*>(std::aligned_alloc(128,128));
        r = static_cast<int*>(std::aligned_alloc(128,128));

        //Zero out the memory, make sure pointer is pointing to the beginning of the arrays....
        std::memset(x,0,total_size);
        std::memset(y,0,total_size);
        std::memset(z,0,total_size);
        std::memset(id,0,128);
        std::memset(r,0,128);
*/
    }

    //Destructor

    ~SoALayout(){
        /*
        std::free(x);
        std::free(y);
        std::free(z);
        std::free(id);
        std::free(r);
*/
    }
    //Assignment operator overloading.....
    //why this causes problem?
 //   SoALayout(const SoALayout&) = delete;

   // SoALayout& operator=(const SoALayout&) = delete;

    // Accessors for the elements
    /*
    uint32_t* getX() { return x; }
    uint32_t* getY() { return y; }
    uint32_t* getZ() { return z; }
    int* getId() { return id; }
    int* getR() { return r; }
    size_t getNumElements() const { return num_elements; }
    size_t getPadSize() const { return pad_size; }
*/
};
}
#endif //PADDED_SoALayout_H

