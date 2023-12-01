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

#include "include/padded_SoA.h"
//#include "include/padded_SoAView.h"

R__LOAD_LIBRARY(ROOTNTuple)
using ENTupleInfo = ROOT::Experimental::ENTupleInfo;
using REntry = ROOT::Experimental::REntry;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include<vector>
#include<thread>

#define arr_size 100

constexpr char const* fname = "myfile_rntuple_aligned.root";
constexpr char const* mname = "MyASoA";
constexpr int NWriterThreads = 4;

using namespace soa;


void ReadRNTuple(){

  
}   


void CreateRNTuple(){
    SoALayout* asoa;

    auto model = RNTupleModel::Create();
    auto soa = model->MakeField<SoALayout>(mname);

    auto ntuple = RNTupleWriter::Recreate(std::move(model),"NTuple",fname);

    printf("Created NTuple %s in %s \n",mname,fname);



}

int main(int argc, char* argv[]){
  
  if(atoi(argv[1])==1)  CreateRNTuple();
  if(atoi(argv[1])==2)  ReadRNTuple();
    return 1;
}