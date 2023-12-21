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
using RNTupleMetrics = ROOT::Experimental::Detail::RNTupleMetrics;

#include<vector>
#include<thread>
#include <cassert>
#include <memory>
#include <sstream>

constexpr char const* tree_name = "caf";
constexpr char const* file_name = "../CAF_FHC_TTree.root";
constexpr char const* file_name_rntuple = "CAF_FHC_RNtuple.root";
constexpr char const* ofile_name = "test_file.root";
constexpr char const* ofile_name_tree  = "test_tree_file.root";

void Convert2(){

}


void Convert(){
    std::string rfname = "blaa";
    auto rmetrics = RNTupleMetrics(rfname);
    rmetrics.Enable();
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
     //   std::cout<<"Convert leaf "<<l->GetName()<<" ["<<l->GetTypeName()<<" ]"<<
     //   " --> "<<"field "<<field->GetName()<<" ["<<field->GetType()<<" ]"<<std::endl;

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
    std::cout<<"Wrote all "<<nEntries<<" entries"<<std::endl;
    rmetrics.Print(std::cout);
    std::cout<<"Metrics "<<rfname<<std::endl;
}

//@brief Convert RNTuple back to TTree
void ConvertBack(){
    std::unique_ptr<TFile> tf(TFile::Open(file_name));
    auto old_caf = tf->Get<TTree>(tree_name);

    auto new_caf = old_caf->CloneTree(0);

    //we will write info in the new caf file...
    std::unique_ptr<TFile> f(TFile::Open(ofile_name));
    assert(f && ! f->IsZombie());

    auto ntuple = RNTupleReader::Open(tree_name,file_name_rntuple);

    auto n_descript = ntuple->GetDescriptor();

    auto n_fields = n_descript->GetNFields();

    TFile newfile(ofile_name_tree,"recreate");
    for(int i = 0;i<n_fields-1; i++){
        auto f_name = n_descript->GetQualifiedFieldName(i);
        auto branch= new_caf->GetBranch(f_name.c_str());
        std::cout<<i<<" "<<f_name<<" "<<std::endl;
        for(auto j : ntuple->GetEntryRange()){
            
        }
    }

    std::cout<<"Q NAME "<<n_descript->GetQualifiedFieldName(0)<<" "<<n_fields<<std::endl;
    //get the collection of views
   // auto v_coll = ntuple->GetViewCollection();
    

}

void ReadRNTuple(){
    auto ntuple = RNTupleReader::Open(tree_name,file_name_rntuple);
    ntuple->PrintInfo(ENTupleInfo::kStorageDetails);
    ntuple->PrintInfo();

  //  ntuple->Show(0);
}

void ReadTree(){
    std::unique_ptr<TFile> f(TFile::Open(ofile_name));
    assert(f && ! f->IsZombie());
    auto tree = f->Get<TTree>(tree_name);
    auto _cf = f->GetCompressionFactor();
    const int entries = tree->GetEntries();

    std::cout<<"Total Entries "<<entries<<std::endl;
    std::cout<<"Compression Factor "<<_cf<<std::endl;

}
void WriteCAFTree(){
    std::unique_ptr<TFile> f(TFile::Open(file_name));
    assert(f && ! f->IsZombie());

    auto tree = f->Get<TTree>(tree_name);
    TFile newfile(ofile_name, "recreate");
    auto ntree = tree->CloneTree();
    ntree->Print();
    newfile.Write();
}

int main(int argc, char* argv[]){
    if(argc<2){
        printf("Need 1 for %s \n 2 for %s \n 3 for %s \n 4 for %s \n 5 for %s \n ",
        "Convert()","WriteCAFTree()","ReadRNTuple()","ReadTree()","ConvertBack()");
    }
    else{
    if(atoi(argv[1])==1) Convert();
    if(atoi(argv[1])==2) WriteCAFTree();
    if(atoi(argv[1])==3) ReadRNTuple();
    if(atoi(argv[1])==4) ReadTree();
    if(atoi(argv[1])==5) ConvertBack();
    }
    return 1;
}
