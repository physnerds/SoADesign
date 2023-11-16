#include "include/padded_SoA.h"
#include "include/padded_SoA_layout.h"

int main(){

    SoA_Layout::myArray mSoa;

    for(int i = 0;i<500;i++){
        mSoa.x[i] = uint32_t(i);
        mSoa.y[i] = uint32_t(i+2);
        mSoa.z[i] = uint32_t(i+4);
    }

    SoA aligned_Soa(500);



    return 1;
}