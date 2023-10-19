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

#include "DuneTriggerData.h"


using namespace soa;
constexpr char* const _fname = "myfile_tree_arrSoA.root";

#define arr_size 50000
#define trig_cand_length 128
#define trig_head_length 440
#define hw_sig_length 96
#define Compression  404 //kUseAnalysis

constexpr int NWriterThreads = 4;
const int EvtsPerThreads = 250000;

int readTree(){
 //DuneSoADataModel::soa_arrays

  DUNETriggerData::soa_trigger* Soa;
  
  auto _rfile = new TFile(_fname,"READONLY");
  TTree *fTree = _rfile->Get<TTree>("myTree");
  
  auto _cf = _rfile->GetCompressionFactor();
  auto _clevel = _rfile->GetCompressionLevel();
  Soa = new DUNETriggerData::soa_trigger;
  fTree->SetBranchAddress("Soa",&Soa);
  const int entries = fTree->GetEntries();
  std::cout<<"Total Entries "<<entries<<std::endl;
  std::cout<<"Compression Factor "<<_cf<<" "<<_clevel<<std::endl;
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
  DUNETriggerData::soa_trigger* Soa;
 // auto Soa = std::make_unique<DUNETriggerData::soa_trigvec>();
 //  Soa = new DUNETriggerData::soa_trigvec;
  Soa = new DUNETriggerData::soa_trigger;
  fOutFile = new TFile(_fname,"RECREATE");
  fOutFile->SetCompressionSettings(Compression);
  fOutTree = new TTree("myTree","Test of SoA in Tree");
  fOutTree->Branch("Soa","soa::DUNETriggerData::soa_trigger",&Soa, 32000,99);
  
  std::vector<std::thread>threads;
  
  for(int i=0;i<NWriterThreads;i++){
  //  threads.emplace_back([i,&fOutTree,&Soa](){
  // static std::mutex gLock;
    auto prng = std::make_unique<TRandom3>();
    prng->SetSeed();
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
       
        Soa->wib0[j]=v0;
        Soa->wib1[j]=v1;    
        Soa->wib2[j]=v2;
        Soa->wib3[j]=v3;
        Soa->wib4[j]=v4;
        Soa->wib5[j]=v5;
        Soa->wib6[j]=v6;
        Soa->wib7[j]=v7;
        Soa->wib8[j]=v8;
        Soa->wib9[j]=v9; 


        
      }
      Soa->trig_scalar = NWriterThreads;
      
     // std::lock_guard<std::mutex>guard(gLock);
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
 


