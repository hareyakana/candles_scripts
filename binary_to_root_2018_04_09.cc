
#ifndef __CINT__
#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"

#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;


Int_t binary_to_root_2018_04_09()
{

  //-----------------------------------

  //-----------------------------
 

  Int_t NNN = 0;
  Int_t Out = 0;

  Int_t FileID = 0;
  Float_t Height[100000];
  Float_t Height_L[100000];  
  Float_t Gomi[100];
  Float_t Time;  
  

  TFile *fout = TFile::Open("test4.root", "recreate");

  TTree *tree = new TTree("tree", "A TTree from rawdata.bin");

  tree->Branch("Eventnumber", &FileID, "FileID/I");
  tree->Branch("Height", Height, "Height[100000]/F");
  tree->Branch("Height_L", Height_L, "Height_L[100000]/F");


  for(FileID = 1; FileID <= 25121; FileID++){
//		TString fin_name = Form("/home/candles/np1c/CANUG/RawData/Bolometer/CaF2_2nd/20mK_nosource/DC/sig8_photon_trigger/s%07d.bin",(Int_t)FileID);
//    TString fin_name = Form("/home/candles/np1c/CANUG/RawData/Bolometer/CaF2_2nd/20mK_nosource/DC/sig6_phonon_30kohm/s%07d.bin",(Int_t)FileID);
//		TString fin_name = Form("/home/candles/np1c/CANUG/RawData/Bolometer/CaF2_2nd/20mK_Cs137/DC/sig3/s%07d.bin",(Int_t)FileID);  
					TString fin_name = Form("/home/candles/np1c/CANUG/RawData/Bolometer/CaF2_2nd/20mK_nosource/DC/sig4/s%07d.bin",(Int_t)FileID);
     
      //--------------------------------------------------------------------------//
   
    const Float_t VerticalGain    = 8.47592673380859E-06;
    const Float_t VerticalOffset  = 0.000888643960934132;;
    const Int_t   NumberOfData    = 100000;
    const Float_t TimeInterval    = 2e-6;
    const Float_t TimeOffset      = 0;
  
  //--------------------------------------------------------------------------//
  
  // set canvas style
  //
/*
  gStyle -> SetLabelSize(0.05, "XYZ");
  gStyle -> SetTitleSize(0.06, "T");
  gStyle -> SetTitleSize(0.06, "XYZ");
  gStyle -> SetTitleOffset(1.0, "X");
  gStyle -> SetTitleOffset(1.1, "Y");
  gStyle -> SetPadLeftMargin(0.14);
  gStyle -> SetPadBottomMargin(0.14);

        TCanvas *c1 = new TCanvas("c1", "", 800, 600);
        c1 -> SetFillStyle(4001);
*/  
  // open binary file
  //
  ifstream fin(fin_name.Data(), ios::in | ios::binary);
  if (!fin) {
    cerr << "Error: don't exist such a data file, " << fin_name << " !" << endl;
    return -1;
  }


  Char_t character[100000];
  Char_t L_ST;
  Char_t Date[100000];
  Char_t Others[100000];
  Char_t Nannka[100000]; 

  fin.read((Char_t* )&character, 4); 
  fin.read((Char_t* )&L_ST, 1);

   if(L_ST == 33){
    fin.read((Char_t* )&Date, 33);
     } else if(L_ST == 34){
     fin.read((Char_t* )&Date, 34);
   } else {

     cerr << "Error! Header length error:  " << endl;
   }
  fin.read((Char_t* )&Others, 70);
  
  //-----------------------

  Int_t Waveform32BitData;

//      TGraph *graph = new TGraph;
//    TGraph *graph2 = new TGraph;
  
  //---------Heat--------------
for (Int_t i = 0; i < NumberOfData; i++) {

      fin.read((Char_t* )&Waveform32BitData, sizeof(Waveform32BitData)); 

      Height[i] = Waveform32BitData * VerticalGain + VerticalOffset;

      Time = (Float_t)i * TimeInterval*1000 + TimeOffset;
      
//              graph -> SetPoint(i, Time, Height[i]);

  }

//   cerr << "ID = " <<  FileID  << endl;

 if (Height[100] < -10){

   Out += 1;

 }

 NNN += 1;

  //---------Gomi---------------

 for (Int_t j = 0; j < 10; j++) {

   fin.read((Char_t* )&Waveform32BitData, sizeof(Waveform32BitData));

   Gomi[j] += Waveform32BitData;

 }

 //----------Light---------------



 //---------------------------------

 const Float_t VerticalGain_L    = 8.52839220897295E-06;//1.68582319020061E-06;//6.24976e-05;
 const Float_t VerticalOffset_L  = -0.000852099678013474;//0;//0.98;
  //----------------------------------------

 

 for (Int_t jj = 0; jj < NumberOfData; jj++) {

   fin.read((Char_t* )&Waveform32BitData, sizeof(Waveform32BitData)); 

   Height_L[jj] = Waveform32BitData * VerticalGain_L + VerticalOffset_L;

   Time = (Float_t)jj * TimeInterval*1000 + TimeOffset;

//      graph2 -> SetPoint(jj, Time, Height_L[jj]);

  }

 /*
  // draw graph
  //
  graph -> SetLineColor(4);
  graph -> SetLineWidth(2);
  graph -> SetMarkerColor(4);
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.2);
  graph -> Draw("ALP");

  // set frame
  //
  const Double_t XMIN = 0;
  const Double_t XMAX = 1000;
  const Double_t YMIN = 0;
  const Double_t YMAX = 1000;
  //  graph -> GetXaxis() -> SetLimits(XMIN, XMAX);
  //  graph -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
  graph -> SetTitle("");
  graph -> GetXaxis() -> SetTitle("Time [ms]");
  graph -> GetYaxis() -> SetTitle("Voltage [V]");
  c1 -> Print("plot_waveform_binary.ps"); 
*/
    tree->Fill();
  }
  cerr << "-------------------------------------------"  << endl;
  cerr << "Event number =  " <<  NNN  << endl;   

  if(Out == 0){
    cerr << "finish" << endl;
  } else {
    cerr << "Error" << endl;
  }

    tree->Write();

    delete tree;
    delete fout;  
  
  return 0;
  
}


#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  binary_to_root_2018_04_09();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
