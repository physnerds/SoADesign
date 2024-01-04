#ifndef DUNETRIGGERDATA_H
#define DUNETRIGGERDATA_H 1
#include "soa.h"
#include<vector>
namespace soa
{
 class DUNETriggerData{
    public:
    DUNETriggerData(){};

    ~DUNETriggerData(){};
    void GenerateData(int seed);

    static const int arr_size = 10000000;
    static const int trig_cand_length = 128;
    static const int trig_head_length = 440;
    static const int hw_sig_length = 96;
    
    Generate_Arrays(soa_trigger,
                  AddArray(uint32_t, wib0,arr_size);
                  AddArray(uint32_t, wib1,arr_size);
                  AddArray(uint32_t, wib2,arr_size);
                  AddArray(uint32_t,wib3,arr_size);
                  AddArray(uint32_t, wib4,arr_size);
                  AddArray(uint32_t, wib5,arr_size);
                  AddArray(uint32_t, wib6,arr_size);
                  AddArray(uint32_t, wib7,arr_size);
                  AddArray(uint32_t, wib8,arr_size);
                  AddArray(uint32_t, wib9,arr_size);

                 // AddArray(uint32_t, trig_cand,trig_cand_length);
                //  AddArray(uint32_t, trig_header,trig_head_length);
                //  AddArray(uint32_t, hw_sig_interface,hw_sig_length);
                  AddScalar(uint32_t,trig_scalar);
                  );
    
    Generate_Arrays(soa_trigvec,
                Addvector(uint32_t, wib0);
                Addvector(uint32_t, wib1);
                
                Addvector(uint32_t, wib2);
                Addvector(uint32_t,wib3);
                Addvector(uint32_t, wib4);
                Addvector(uint32_t, wib5);
                Addvector(uint32_t, wib6);
                Addvector(uint32_t, wib7);
                Addvector(uint32_t, wib8);
                Addvector(uint32_t, wib9);
                
                Addvector(uint32_t, trig_cand);
                Addvector(uint32_t, trig_header);
                Addvector(uint32_t, hw_sig_interface);
                AddScalar(uint32_t,trig_scalar);
                );

      Generate_Arrays(soa_simple,
         Addvector(uint32_t, wib0);
         );
    private:

 };   
} // namespace soa
#endif