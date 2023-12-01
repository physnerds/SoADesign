#ifndef PADDED_SOAVIEW_H
#define PADDED_SOAVIEW_H 1

#include "padded_SoA.h"
#include "soa.h"

namespace soa{
class SoAView {
public:
    // References to the arrays in the SoA struct
    uint32_t* x;
    uint32_t* y;
    uint32_t* z;

    //scalars....
    int* id;
    int* r;
    // The number of elements in the arrays
    size_t num_elements;

    // Constructor
    SoAView(SoALayout& soa)
        : x(soa.getX()), y(soa.getY()), z(soa.getZ()), 
          num_elements(soa.getNumElements()) {}

    // Accessors for the arrays
    uint32_t* getX() { return x; }
    uint32_t* getY() { return y; }
    uint32_t* getZ() { return z; }
    size_t getNumElements() const { return num_elements; }

    // Other view-specific member functions can be added as needed

    // Deleted copy constructor and assignment operator to prevent unintended copying
    SoAView(const SoAView&) = delete;
    SoAView& operator=(const SoAView&) = delete;
};
}
#endif
