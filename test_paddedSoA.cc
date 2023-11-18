#undef R__HAS_VDT

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <TRandom.h>

#include "TInterpreter.h"
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TRandom3.h>

#include "include/ASoA.h"
R__LOAD_LIBRARY(ROOTNTuple)
using ENTupleInfo = ROOT::Experimental::ENTupleInfo;
using REntry = ROOT::Experimental::REntry;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;


#include<vector>
#include<thread>

#define arr_size 100

constexpr char const* fname = "myfile_rntuple.root";
constexpr char const* mname = "MyASoA";
constexpr int NWriterThreads = 4;

using namespace soa;


void ReadRNTuple(){
   auto model = RNTupleModel::Create();
  
   auto soa = model->MakeField<ASoA::asoa>(mname);
  
 auto ntuple = RNTupleReader::Open(std::move(model), "NTuple", fname);
 ntuple->PrintInfo(ENTupleInfo::kStorageDetails);
 ntuple->PrintInfo();
  
  ntuple->Show(0);
  
}   


void CreateRNTuple(){

    ASoA::asoa* asoa;

    auto model = RNTupleModel::Create();
    auto soa = model->MakeField<ASoA::asoa>(mname);
    
    auto ntuple = RNTupleWriter::Recreate(std::move(model),"NTuple",fname);

    std::cout<<"Created the ntuple "<<std::endl;
    std::vector<std::unique_ptr<REntry>>entries;
    std::vector<std::thread>threads;   

    for(int i=0;i<NWriterThreads;i++)
        entries.emplace_back(ntuple->CreateEntry());

    for(int i = 0;i<NWriterThreads;i++){
    //    std::cout<<"Printing Thread Loop "<<i<<std::endl;
        threads.emplace_back([i,&entries,&ntuple](){
            static std::mutex glock;

            auto prng = std::make_unique<TRandom3>();

            prng->SetSeed();

            auto _val = entries[i]->Get<ASoA::asoa>(mname);

            for(int j=0;j<arr_size;j++){
            //    std::cout<<"Printing array loop "<<j<<std::endl;
                double arr[3];
                prng->RndmArray(3,arr);

                uint32_t v0 = uint32_t(arr[0]*10);
                uint32_t v1 = uint32_t(arr[1]*10);
                uint32_t v2 = uint32_t(arr[2]*10);  

                _val->aa.emplace_back(v0);
                _val->subsoa.a.emplace_back(v1);
                _val->subsoa.b.emplace_back(v2);
            }
            std::lock_guard<std::mutex>guard(glock);
            ntuple->Fill(*entries[i]);
                
        });
    }

    for(auto &thread:threads)
        thread.join();
}

int main(int argc, char* argv[]){
  
  if(atoi(argv[1])==1)  CreateRNTuple();
  if(atoi(argv[1])==2)  ReadRNTuple();
    return 1;
}