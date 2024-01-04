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

#define arr_size 10000000
#define trig_cand_length 128
#define trig_head_length 440
#define hw_sig_length 96
#define Compression  404 //kUseAnalysis
using namespace soa;

constexpr char const* filename = "myfile_rntuple.root";
constexpr char const* filename_tree = "myfile_tree.root";
constexpr int NWriterThreads = 10;
const int EvtsPerThreads = 250000;

void CompareStuff(){
    DUNETriggerData::soa_simple* Soa;

    auto model = RNTupleModel::Create();    
    auto soa = model->MakeField<DUNETriggerData::soa_simple>("Trig0");
    auto ntuple = RNTupleReader::Open(std::move(model),"NTuple",filename);

  auto _rfile = new TFile(filename_tree,"READONLY");
  TTree *fTree = _rfile->Get<TTree>("myTree");
  
  auto _cf = _rfile->GetCompressionFactor();
  auto _clevel = _rfile->GetCompressionLevel();
  Soa = new DUNETriggerData::soa_simple;
  fTree->SetBranchAddress("Soa",&Soa);
  const int entries_tree = fTree->GetEntries();
  const int entries_rntuple = ntuple->GetNEntries();
  
  printf("Entries TTree %d RNTuple %d \n",entries_tree,entries_rntuple);

  for(int i = 0;i<entries_rntuple;i++){
  fTree->GetEntry(i);
  printf("TTree size of each wib %lu \n",(Soa->wib0).size());

  ntuple->LoadEntry(i);
  printf("RNTuple size of each wib %lu \n",soa->wib0.size());
  }
    
}

void ReadRNTuple(){
   auto model = RNTupleModel::Create();
  // auto ntuple = RNTupleReader::Open("NTuple",filename);
  
   auto soa = model->MakeField<DUNETriggerData::soa_simple>("Trig0");
   
  
 auto ntuple = RNTupleReader::Open(std::move(model), "NTuple", filename);
 const int entries_rntuple = ntuple->GetNEntries();
 printf("Entries RNTuple %d \n",entries_rntuple);

 std::vector<uint32_t>sum_entries;
 std::vector<size_t>size_entries;
 for(int i = 0;i<entries_rntuple; i++){
  ntuple->LoadEntry(i);
  auto vec_entries = soa->wib0;
  auto vec_size = soa->wib0.size();
  size_entries.push_back(vec_size);
  uint32_t sum_tot = 0;
  for(auto val:vec_entries){
    sum_tot+= val;
  }
  sum_entries.push_back(sum_tot);

 }
 
 for(size_t i=0;i<entries_rntuple;i++){
 printf("Size and Sum  of Vector  %d %d \n",size_entries[i], sum_entries[i]);
 }
 ntuple->PrintInfo(ENTupleInfo::kStorageDetails);
 ntuple->PrintInfo();
 

 // ntuple->Show(0);
  
}   

void WriteEntries(int ID, std::vector<std::unique_ptr<REntry>>& entries, std::unique_ptr<RNTupleWriter> &Ntuple ){
  static std::mutex gLock;
  auto prng = std::make_unique<TRandom3>();
  prng->SetSeed();
  //DUNETrigger::soa_simple d_simple;
  auto _val = entries[ID]->Get<DUNETriggerData::soa_simple>("Trig0");
  for(int j=0;j<arr_size;j++){
      Double_t arr[10];
      prng->RndmArray(10,arr);
      uint32_t v0 = uint32_t(arr[0]*10);

      _val->wib0.emplace_back(v0);
      
  }
  std::lock_guard<std::mutex>guard(gLock);
  Ntuple->Fill(*entries[ID]);  
}
void CreateRNTupleSimple(){
    auto model = RNTupleModel::Create();
    auto ftrig_vec = model->MakeField<DUNETriggerData::soa_simple>("Trig0");

    auto ntuple = RNTupleWriter::Recreate(std::move(model),"NTuple",filename);

    std::vector<std::unique_ptr<REntry>>entries;
    std::vector<std::thread>threads;

    for(int i=0;i<NWriterThreads;i++)
        entries.emplace_back(ntuple->CreateEntry());
   
    for(int i=0;i<NWriterThreads;i++){
      threads.emplace_back([i,&entries,&ntuple](){
      WriteEntries(i, entries, ntuple);
    }); //end of lambda function

    } //end of for loop
    for(auto &thread:threads)
        thread.join();
    
}
int main(int argc, char* argv[]){
  
  if(atoi(argv[1])==1)  CreateRNTupleSimple();
  if(atoi(argv[1])==2)  ReadRNTuple();
  if(atoi(argv[1])==3)  CompareStuff();
    return 1;
}