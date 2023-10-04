#undef R__HAS_VDT
#include "include/DUNETriggerData.h"
    
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <TRandom.h>

#include "TInterpreter.h"
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TRandom3.h>

R__LOAD_LIBRARY(ROOTNTuple)
using ENTupleInfo = ROOT::Experimental::ENTupleInfo;
using REntry = ROOT::Experimental::REntry;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include<vector>
#include<thread>

#define arr_size 50000
#define trig_cand_length 128
#define trig_head_length 440
#define hw_sig_length 96
#define Compression  404 //kUseAnalysis
using namespace soa;

constexpr char const* filename = "myfile_rntuple_arrSoA.root";
constexpr char const* filename_tree = "myfile_tree_arrSoA.root";
constexpr int NWriterThreads = 4;
const int EvtsPerThreads = 250000;

void CompareStuff(){
    DUNETriggerData::soa_trigger* Soa;

    auto model = RNTupleModel::Create();    
    auto soa = model->MakeField<DUNETriggerData::soa_trigger>("Trig0");
    auto ntuple = RNTupleReader::Open(std::move(model),"NTuple",filename);

  auto _rfile = new TFile(filename_tree,"READONLY");
  TTree *fTree = _rfile->Get<TTree>("myTree");
  
  auto _cf = _rfile->GetCompressionFactor();
  auto _clevel = _rfile->GetCompressionLevel();
  Soa = new DUNETriggerData::soa_trigger;
  fTree->SetBranchAddress("Soa",&Soa);
  const int entries_tree = fTree->GetEntries();
  const int entries_rntuple = ntuple->GetNEntries();
  
  printf("Entries TTree %d RNTuple %d \n",entries_tree,entries_rntuple);

  for(int i = 0;i<entries_rntuple;i++){
  fTree->GetEntry(i);
  printf("TTree size of each wib %lu \n",Soa->trig_scalar);

  ntuple->LoadEntry(i);
  printf("RNTuple size of each wib %lu \n",soa->trig_scalar);
  }
    
}

void ReadRNTuple(){
   auto model = RNTupleModel::Create();
  // auto ntuple = RNTupleReader::Open("NTuple",filename);
  
   auto soa = model->MakeField<DUNETriggerData::soa_trigger>("Trig0");
  
 auto ntuple = RNTupleReader::Open(std::move(model), "NTuple", filename);
 ntuple->PrintInfo(ENTupleInfo::kStorageDetails);
 ntuple->PrintInfo();
  
  //ntuple->Show(0);
  
}   

void CreateRNTuple(){
    auto model = RNTupleModel::Create();
    auto ftrig_vec = model->MakeField<DUNETriggerData::soa_trigger>("Trig0");

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
        auto _val = entries[i]->Get<DUNETriggerData::soa_trigger>("Trig0");
        for(int j=0;j<arr_size;j++){
            double arr[10],xx,yy;
            prng->RndmArray(10,arr);
            _val->wib0[j] = int(xx*10); 
            _val->wib1[j] = int(yy*10);  
            _val->wib2[j] = int(arr[2]*10); 
            _val->wib3[j] = int(arr[3]*10); 
            _val->wib4[j] = int(arr[4]*10); 
            _val->wib5[j] = int(arr[5]*10); 
            _val->wib6[j] = int(arr[6]*10); 
            _val->wib7[j] = int(arr[7]*10); 
            _val->wib8[j] = int(arr[8]*10); 
            _val->wib9[j] = int(arr[9]*10);  
            
            
        }
        _val->trig_scalar=arr_size;
        std::lock_guard<std::mutex>guard(gLock);
        ntuple->Fill(*entries[i]);

    }); //end of lambda function

    } //end of for loop
    for(auto &thread:threads)
        thread.join();
    
}
int main(int argc, char* argv[]){
  
  if(atoi(argv[1])==1)  CreateRNTuple();
  if(atoi(argv[1])==2)  ReadRNTuple();
  if(atoi(argv[1])==3)  CompareStuff();
    return 1;
}