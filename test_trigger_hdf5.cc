#undef R__HAS_VDT
#include "include/DUNETriggerData.h"
    
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <TRandom.h>

#include "TInterpreter.h"
#include <TROOT.h>
#include <TFile.h>
#include <TRandom.h>
#include <TRandom3.h>

R__LOAD_LIBRARY(ROOTNTuple)
using REntry = ROOT::Experimental::REntry;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include<vector>
#include<thread>

#define arr_size 38500
#define trig_cand_length 128
#define trig_head_length 440
#define hw_sig_length 96

using namespace soa;

constexpr char const* filename = "myfile_rntuple.root";
constexpr int NWriterThreads = 4;
const int EvtsPerThreads = 25000;
void CreateRNTuple(){
    auto model = RNTupleModel::Create();
    auto ftrig_vec = model->MakeField<DUNETriggerData::soa_trigvec>("Trig0");

    auto ntuple = RNTupleWriter::Recreate(std::move(model),"NTuple",filename);

    std::vector<std::unique_ptr<REntry>>entries;
    std::vector<std::thread>threads;

    for(int i=0;i<NWriterThreads;i++)
        entries.emplace_back(ntuple->CreateEntry());
   
    for(int i=0;i<NWriterThreads;i++){
        threads.emplace_back([i,&entries,&ntuple](){
        static std::mutex gLock;

        auto prng = std::make_unique<TRandom3>();
        prng->SetSeed();
        //DUNETrigger::soa_trigvec d_trigvec;
        auto _val = entries[i]->Get<DUNETriggerData::soa_trigvec>("Trig0");
        for(int j=0;j<100;j++){
            Double_t arr[10];
            prng->RndmArray(10,arr);
            std::cout<<arr[0]<<" "<<arr[1]<<std::endl;
            _val->wib0[j]=uint32_t(arr[0])*10;
            _val->wib1[j]=uint32_t(arr[1])*10;   
            _val->wib2[j]=uint32_t(arr[2])*10;
            _val->wib3[j]=uint32_t(arr[3])*10;
            _val->wib4[j]=uint32_t(arr[4])*10;
            _val->wib5[j]=uint32_t(arr[5])*10;
            _val->wib6[j]=uint32_t(arr[9])*10;
            _val->wib7[j]=uint32_t(arr[6])*10;
            _val->wib8[j]=uint32_t(arr[7])*10;
            _val->wib9[j]=uint32_t(arr[8])*10; 

        }


    }); //end of lambda function

    } //end of for loop
    for(auto &thread:threads)
        thread.join();
    
}
int main(){
    CreateRNTuple();
    return 1;
}