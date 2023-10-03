#include "include/soa.h"
#include "include/common.h"
#include "include/val.h"
#include <iostream>

using namespace soa;


Generate_Arrays(soa_arrays,
                AddArray(int, xx, 10);
                AddArray(double, yy, 10);
                AddArray(uint32_t, zz, 10);
                );

Generate_Arrays(soa_simple,
                AddArray(myval<int>, mx,10);
                AddArray(myval<double>,my,10);
                AddArray(myval<uint32_t>,mz,10);
                );
int main(){
  soa_arrays Soa;
  for(int i=0;i<10;i++){
    Soa.xx[i] = i;
    Soa.yy[i] = (i)*3.1415;
    Soa.zz[i] = i+8;
  }
  
  soa_simple sa;
  for(int i=0;i<10;i++){
    sa.mx[i] = i;
    sa.my[i] = (i)*3.1415;
    sa.mz[i] = i+5;
  }

  soa_simple *sb;

  sb->mx[0] = sa.mx[0].val()*2;
  
  for(int i = 0;i<10;i++){
    std::cout<<"soa "<<Soa.xx[i]<<" "<<Soa.yy[i]<<" "<<Soa.zz[i]<<std::endl;
    std::cout<<"myval "<<sa.mx[i]<<" "<<sa.my[i]<<" "<<sa.mz[i]<<std::endl;
  }
 // std::cout<<"RAW ARR "<<raw_arr[0][0]<<std::endl;
  constexpr static size_t defaultAlignment = 0;


  alignas(128) double x[5] = {1.,2.,3.,4.,5.};
  double* ptr=nullptr;
  ptr = x;
  ptr = static_cast<double*>(__builtin_assume_aligned(ptr,128));

  using d_soapar = soaParametersImpl<soacolumntype::column,double>;

  d_soapar temp_val(ptr);
  using soavalueconf = soavalue<soacolumntype::column,double,defaultAlignment>;
 
  soavalueconf d_soaval(1,temp_val);
  auto val = d_soaval();
  std::cout<<*ptr<<" "<<*temp_val.addr_<<" "<<val<<std::endl;
  ++ptr;
  ++temp_val.addr_;
  std::cout<<*ptr<<" "<<temp_val.addr_<<std::endl;
  for(int i=0;i<7;i++)++ptr;
  ++temp_val.addr_;
  std::cout<<*ptr<<" "<<temp_val.addr_<<std::endl;
  
  std::cout<<"Test of myval begins"<<std::endl;

  myval<double> _val = 3.5;
  myval<double> _arrval[1] = {3.0};//{0.0,1.0,2.0,3.0,4.0};
  myval<double> *vptr = nullptr;
  vptr = _arrval;
  std::cout<<vptr<<std::endl;
  vptr++;
  std::cout<<vptr<<std::endl;
  std::cout<<"size of myval "<<sizeof(_arrval)<<" "<<sizeof(double)<<std::endl;
  return 1;

}
