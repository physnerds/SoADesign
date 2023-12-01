#undef R__HAS_VDT
    
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <TRandom.h>

#include "TInterpreter.h"
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TRandom.h>
#include <TRandom3.h>

R__LOAD_LIBRARY(ROOTNTuple)
using ENTupleInfo = ROOT::Experimental::ENTupleInfo;
using REntry = ROOT::Experimental::REntry;
using RFieldBase = ROOT::Experimental::Detail::RFieldBase;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include<vector>
#include<thread>
#include <cassert>
#include <memory>

constexpr char const* tree_name = "caf";
constexpr char const* file_name = "../CAF_FHC_TTree.root";
constexpr char const* file_name_rntuple = "CAF_FHC_RNtuple.root";

void Convert(){
    std::unique_ptr<TFile> f(TFile::Open(file_name));
    assert(f && ! f->IsZombie());

    //unique ptr to an empty RNTuple model without a default entry
    auto model = RNTupleModel::CreateBare();

    auto tree = f->Get<TTree>(tree_name);

    for(auto b : TRangeDynCast<TBranch>(*tree->GetListOfBranches())){
        assert(b);
        TLeaf *l = static_cast<TLeaf*>(b->GetListOfLeaves()->First());

        //Create an ntuple field with the same name and type than the tree branch
        auto field = RFieldBase::Create(l->GetName(),l->GetTypeName()).Unwrap();
        std::cout<<"Convert leaf "<<l->GetName()<<" ["<<l->GetTypeName()<<" ]"<<
        " --> "<<"field "<<field->GetName()<<" ["<<field->GetType()<<" ]"<<std::endl;

        model->AddField(std::move(field));
    }

    //now new ntuple to own the model...
    auto ntuple = RNTupleWriter::Recreate(std::move(model),tree_name,file_name_rntuple);
    auto entry = ntuple->GetModel()->CreateEntry();
    for(auto b : TRangeDynCast<TBranch>(*tree->GetListOfBranches())){
        auto l = static_cast<TLeaf*>(b->GetListOfLeaves()->First());
        void *fieldDataPtr = entry->GetValue(l->GetName()).GetRawPtr();
        tree->SetBranchAddress(b->GetName(),fieldDataPtr);

    }
    auto nEntries = tree->GetEntries();
    for(decltype(nEntries) i = 0;i<nEntries; ++i){
        tree->GetEntry(i);
        ntuple->Fill(*entry);

        if( i && i% 1000==0)
        std::cout<<" Wrote "<<i<<" entries "<<std::endl;
    }

}

int main(int argc, char* argv[]){
    Convert();
    return 1;
}