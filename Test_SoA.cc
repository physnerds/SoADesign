#include "include/soa.h"
#include <iostream>

using namespace soa;


Generate_Arrays(soa_arrays,
                AddArray(int, xx, 10);
                AddArray(double, yy, 10);
                AddArray(uint32_t, zz, 10);
                );

int main(){
  soa_arrays Soa;
  for(int i=0;i<10;i++){
    Soa.xx[i] = i;
    Soa.yy[i] = (i)*3.1415;
    Soa.zz[i] = i+8;
  }
  constexpr size_t numarrays = 5;
  constexpr size_t arraysize = 4;

  // Create an instance of the SoA
  SoA<int, numarrays, arraysize> soa;
  
  // Access and modify elements in the SoA
  soa.arrays[0].Data[2] = 42;
  
  // Print the value
  std::cout << " Value at SoA.arrays[0].data[2]: " << soa.arrays[0].Data[2] << std::endl;
  
  
  double x = 3.14;
  int y = static_cast<int>(x);
  printf("x, y, %f, %d ",x,y);
  

  
 // std::cout<<"RAW ARR "<<raw_arr[0][0]<<std::endl;
  return 1;
  
}
