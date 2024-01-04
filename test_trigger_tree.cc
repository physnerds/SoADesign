//RNTUPLE STUFF...

#include "TInterpreter.h"
#include <TROOT.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TTree.h>
#include <TFile.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <thread>

#include "DUNETriggerData.h"


using namespace soa;
constexpr char* const _fname = "myfile_tree.root";

#define arr_size 3866696
#define trig_cand_length 128
#define trig_head_length 440
#define hw_sig_length 96
#define Compression  404 //kUseAnalysis

constexpr int NWriterThreads = 4;
const int EvtsPerThreads = 250000;

int readTree(){
 //DuneSoADataModel::soa_arrays

  DUNETriggerData::soa_trigvec* Soa;
  
  auto _rfile = new TFile(_fname,"READONLY");
  TTree *fTree = _rfile->Get<TTree>("myTree");
  
  auto _cf = _rfile->GetCompressionFactor();
  Soa = new DUNETriggerData::soa_trigvec;
  fTree->SetBranchAddress("Soa",&Soa);
  const int entries = fTree->GetEntries();
  std::cout<<"Total Entries "<<entries<<std::endl;
  std::cout<<"Compression Factor "<<_cf<<std::endl;
  fTree->GetEntry(0);
  fTree->Print();
  /*
  for(int i = 0;i<10;i++)
    std::cout<<Soa->wib0[i]<<" "<<Soa->wib1[i]<<" "<<Soa->wib2[i]<<std::endl;
  */
  delete Soa;
  
  return 1;
}

int createTree(){
   TFile *fOutFile;
  TTree* fOutTree;
  DUNETriggerData::soa_trigvec* Soa;
 // auto Soa = std::make_unique<DUNETriggerData::soa_trigvec>();
  Soa = new DUNETriggerData::soa_trigvec;
  //Soa = new DUNETriggerData::soa_trigger;
  fOutFile = new TFile(_fname,"RECREATE");
  fOutFile->SetCompressionSettings(Compression);
  fOutTree = new TTree("myTree","Test of SoA in Tree");
  fOutTree->Branch("Soa","soa::DUNETriggerData::soa_trigvec",&Soa, 32000,99);
  
  std::vector<std::thread>threads;
  
  for(int i=0;i<NWriterThreads;i++){
  //  threads.emplace_back([i,&fOutTree,&Soa](){
   static std::mutex gLock;
    auto prng = std::make_unique<TRandom3>();
    prng->SetSeed();
       Soa->wib0.clear();
       Soa->wib1.clear();
       Soa->wib2.clear();
       Soa->wib3.clear();
       Soa->wib4.clear();
       Soa->wib5.clear();
       Soa->wib6.clear();
       Soa->wib7.clear();
       Soa->wib8.clear();
      Soa->wib9.clear();
    for(int j=0;j<arr_size;j++){

        Double_t arr[10];
        prng->RndmArray(10,arr);
        uint32_t v0 = uint32_t(arr[0]*10);
        uint32_t v1 = uint32_t(arr[1]*10);
        uint32_t v2 = uint32_t(arr[2]*10);
        uint32_t v3 = uint32_t(arr[3]*10);
        uint32_t v4 = uint32_t(arr[4]*10);
        uint32_t v5 = uint32_t(arr[5]*10);
        uint32_t v6 = uint32_t(arr[6]*10);
        uint32_t v7 = uint32_t(arr[7]*10);
        uint32_t v8 = uint32_t(arr[8]*10);
        uint32_t v9 = uint32_t(arr[9]*10);
       
        Soa->wib0.emplace_back(v0);
        Soa->wib1.emplace_back(v1);    
        Soa->wib2.emplace_back(v2);
        Soa->wib3.emplace_back(v3);
        Soa->wib4.emplace_back(v4);
        Soa->wib5.emplace_back(v5);
        Soa->wib6.emplace_back(v6);
        Soa->wib7.emplace_back(v7);
        Soa->wib8.emplace_back(v8);
        Soa->wib9.emplace_back(v9); 

       // gLock.unlock();
        
      }
      Soa->trig_scalar = NWriterThreads;
      
      std::lock_guard<std::mutex>guard(gLock);
      //gLock.lock();
      fOutTree->Fill();
      //gLock.unlock();
  }  
  //});
  
 // for(auto& thread:threads)
 //   thread.join();
  
  fOutTree->Write();
  fOutFile->Close();
//  delete Soa;
  delete fOutFile;
  return 1;
}

int main(int argc, char* argv[]){
 int arg=atoi(argv[1]);

  if(arg==1) createTree();
   
 if(arg==2) readTree();
  return 1;
}
 


