#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

void reduced_2000(){
	TFile *fin = new TFile("test7.root","READ");
	TTree *tree_in = (TTree*)fin->Get("tree");
	TFile *fout = new TFile("sig7.root","RECREATE");
	TTree *tree_out = new TTree("tree","");
	Int_t Eventnumber;
	Float_t Height[100000];
	Float_t Height_L[100000];
	
	tree_in -> SetBranchAddress("Height", &Height);
	tree_in -> SetBranchAddress("Height_L", &Height_L);
	tree_in -> SetBranchAddress("Eventnumber", &Eventnumber);
	
	Long64_t nentries = tree_in->GetEntries();
	
	Float_t Heat[2000];
	Float_t Light[2000];
	
	tree_out -> Branch("EventNumber", &Eventnumber, "EventNumber/I");
	tree_out -> Branch("HeightHeat", &Heat, "HeightHeat[2000]/F");
	tree_out -> Branch("HeightLight", &Light, "HeightLight[2000]/F");
	
	for (Long64_t i=0; i<nentries; i++){
		tree_in->GetEntry(i);
		for (Int_t j=0; j<2000; j++){
			Float_t sum = 0;
			Float_t sum_L = 0;
			for (Int_t k=0; k<50; k++){
				sum += Height[k+50*j];
				sum_L += Height_L[k+50*j];
			}
			Heat[j] = sum/50;
			Light[j] = sum_L/50;
//			cout << Heat[j] << endl;
		}
		tree_out -> Fill();
//		if (tree_in->GetEntry(i)){
//			 cout << Height[i] << endl;
//		}
	}
	tree_out -> Write();
	delete fin;
//	delete fout;
//	delete tree_in;
}
