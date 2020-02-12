// case1: use root in CINT
// $ root root_reduced.cc
//
// case2: complie in c++
// set following line in your shell script
//     alias rg++ 'g++ `root-config --cflags --libs`' (csh)
// $ rg++ root_reduced.cc
// $ ./a.out
//
// how to make list file
// $ find /np1c/v01/candles/CANUG/CAT/Official_Run010/ -name "Run010-*.root" | sort > Run010.list
//
// Temperature data
// $ /home/tiida/cat28.0/DataTable/

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

Int_t root_reduced(){
	Int_t Run;
	cout << "which run do you want to make?" << endl;
	scanf("%d",& Run);
  TString list_name = Form("Run%03d.list",Run);

  //---------------------------------------------------------------------------------------//
  Bool_t flag_Header            = 1; //  0
  Bool_t flag_Flag              = 1; //  1
  Bool_t flag_EventTimeStamp    = 1; //  2
  Bool_t flag_TimingDiff        = 1; //  3
  Bool_t flag_DataQuality       = 1; //  4
  Bool_t flag_EventID           = 1; //  5
  Bool_t flag_ADCTotal          = 0; //  6
  Bool_t flag_NPETotal          = 0; //  7
  Bool_t flag_ADCTotal4us       = 0; //  8
  Bool_t flag_ADCTotal4us2      = 0; //  9
  Bool_t flag_NPETotal4us       = 0; // 10
  Bool_t flag_NPEDiff4us				= 0; // 11
  Bool_t flag_ADCShort          = 0; // 12
  Bool_t flag_NPEShort          = 0; // 13
  Bool_t flag_FADCINT           = 0; // 14
  Bool_t flag_FADCINT4us        = 0; // 15
  Bool_t flag_FADCINT60ns       = 0; // 16
  Bool_t flag_FADCSho           = 0; // 17
  Bool_t flag_SCHANNEL          = 1; // 18
  Bool_t flag_Ratio             = 0; // 19
  Bool_t flag_Ratio4us          = 1; // 20
  Bool_t flag_RatioR            = 0; // 21
  Bool_t flag_PMTALL            = 0; // 22
  Bool_t flag_Pedestal          = 0; // 23
  Bool_t flag_Position_wm       = 1; // 24
  Bool_t flag_Position_wm2      = 0; // 25
  Bool_t flag_Position_wm_woLS  = 0; // 26
  Bool_t flag_ADCPosition_wm    = 0; // 27
  Bool_t flag_CrystalN          = 1; // 28
  Bool_t flag_CrystalN2         = 0; // 29
  Bool_t flag_CrystalN_adc      = 0; // 30
  Bool_t flag_CrystalN_woLS     = 0; // 31
  Bool_t flag_PSDPara           = 1; // 32
  Bool_t flag_PSDChInfo         = 0; // 33
  Bool_t flag_Energy            = 1; // 34
  Bool_t flag_ADCEnergy         = 1; // 35
  Bool_t flag_Energy_woLS       = 0; // 36
  Bool_t flag_TriggerCounter    = 1; // 37
  Bool_t flag_EnergyScaleFactor = 0; // 38
  Bool_t flag_PSDDoubleP        = 1; // 39
  Bool_t flag_UnixTime          = 1; // 40
  //---------------------------------------------------------------------------------------//

  TROOT root("root", "root");
  
  // make output
  TFile *tfin;
  TTree *tree_in; 
  TFile *tfout;

  Float_t  Header[5];           //  0
  Int_t    Flag;                //  1
  UInt_t   EventTimeStamp[3];   //  2
  Long64_t TimingDiff;          //  3
  Int_t    DataQuality[4];      //  4
  Int_t    EventID;             //  5
  Float_t  ADCTotal;            //  6
  Float_t  NPETotal;            //  7
  Float_t  ADCTotal4us;         //  8
  Float_t  ADCTotal4us2;        //  9
  Float_t  NPETotal4us;         // 10
  Float_t  ADCShort;            // 11
  Float_t  NPEShort;            // 12
  Float_t  FADCINT[75];         // 13
  Float_t  FADCINT4us[75];      // 14
  Float_t  FADCINT60ns[75];     // 15
  Float_t  FADCSho[75];         // 16
  Int_t    SCHANNEL;            // 17
  Float_t  Ratio;               // 18
  Float_t  Ratio4us;            // 19
  Float_t  RatioR[3];           // 20
  Float_t  PMTALL[4480];        // 21
  Float_t  Pedestal[75];        // 22
  Float_t  Position_wm[3];      // 23
  Float_t  Position_wm2[3];     // 24
  Float_t  Position_wm_woLS[3]; // 25
  Float_t  ADCPosition_wm[3];   // 26
  Int_t    CrystalN[6];         // 27
  Int_t    CrystalN2[6];        // 28
  Int_t    CrystalN_adc[6];     // 29
  Int_t    CrystalN_woLS[6];    // 30
  Float_t  PSDPara[34];         // 31
  Float_t  PSDChInfo[9];        // 32
  Float_t  Energy[2];           // 33
  Float_t  ADCEnergy[2];        // 34
  Float_t  Energy_woLS[2];      // 35
  UInt_t   TriggerCounter[44];  // 36
  Float_t  EnergyScaleFactor;   // 37
  Float_t  PSDDoubleP[22];       // 38
  Int_t    UnixTime;            // 39, added
  // modified parameters
  //Int_t    DataQuality1;
  //Float_t  PSDPara1;  // chi2_beta_LT
  //Float_t  PSDPara4;  // chi2_beta_HT
  //Float_t  PSDPara10; // chi2_alpha_HT
  //Float_t  PSDPara18; // chi2_betaLS_HT
  //Float_t  PSDPara22; // Shape Indicator
  //Float_t  PSDPara28; // chi2_betaLS+alpha
  //Float_t  Energy0;
  //Float_t  Energy1;
  //Float_t  Energy2;
  //Float_t  ADCEnergy1;
  //Float_t  Energy_woLS1;
  //UInt_t   TriggerCounter33;
  //UInt_t   TriggerCounter36;
  //Float_t  PSDDoubleP2[5]; // time difference

  TTree *tree_out;

  // read file
  //
  ifstream fin(list_name.Data());
  if (!fin) {
    cerr << "Error: don't exist such a data file, " << list_name << " !" << endl;
    exit(8);
  }

  char fin_name[256];
  while (fin.getline(fin_name, sizeof(fin_name), '\n')) {
    // open input file
    tfin = new TFile(fin_name, "open");
    if (tfin -> GetNkeys() == 0) {
      cerr << "Error: no such file, " << fin_name << "!" << endl;
      exit(8);
    }

    tree_in = (TTree *)tfin -> Get("tree");
    if (tree_in == NULL) {
      cerr << "Error: tree == NULL" << endl;
      exit(8);
    }

    // inactive parameters
    tree_in -> SetBranchStatus("*", 0);
    tree_in -> SetBranchStatus("Header", flag_Header);             //  0
    tree_in -> SetBranchStatus("Flag",flag_Flag);               //  1
    tree_in -> SetBranchStatus("EventTimeStamp",flag_EventTimeStamp);     //  2
    tree_in -> SetBranchStatus("TimingDiff",flag_TimingDiff);        //  3
    tree_in -> SetBranchStatus("DataQuality",flag_DataQuality);        //  4
    tree_in -> SetBranchStatus("EventID",flag_EventID);           //  5
    tree_in -> SetBranchStatus("ADCTotal",flag_ADCTotal);          //  6
    tree_in -> SetBranchStatus("NPETotal",flag_NPETotal);          //  7
    tree_in -> SetBranchStatus("ADCTotal4us",flag_ADCTotal4us);       //  8
    tree_in -> SetBranchStatus("ADCTotal4us2",flag_ADCTotal4us2);      //  9
    tree_in -> SetBranchStatus("NPETotal4us",flag_NPETotal4us);       // 10
    tree_in -> SetBranchStatus("ADCShort",flag_ADCShort);          // 11
    tree_in -> SetBranchStatus("NPEShort",flag_NPEShort);          // 12
    tree_in -> SetBranchStatus("FADCINT",flag_FADCINT);            // 13
    tree_in -> SetBranchStatus("FADCINT4us",flag_FADCINT4us);         // 14
    tree_in -> SetBranchStatus("FADCINT60ns",flag_FADCINT60ns);        // 15
    tree_in -> SetBranchStatus("FADCSho",flag_FADCSho);            // 16
    tree_in -> SetBranchStatus("SCHANNEL",flag_SCHANNEL);          // 17
    tree_in -> SetBranchStatus("Ratio",flag_Ratio);             // 18
    tree_in -> SetBranchStatus("Ratio4us",flag_Ratio4us);          // 19
    tree_in -> SetBranchStatus("RatioR",flag_RatioR);             // 20
    tree_in -> SetBranchStatus("PMTALL",flag_PMTALL);             // 21
    tree_in -> SetBranchStatus("Pedestal",flag_Pedestal);           // 22
    tree_in -> SetBranchStatus("Position_wm",flag_Position_wm);        // 23
    tree_in -> SetBranchStatus("Position_wm2",flag_Position_wm2);       // 24
    tree_in -> SetBranchStatus("Position_wm_woLS",flag_Position_wm_woLS);   // 25
    tree_in -> SetBranchStatus("ADCPosition_wm",flag_ADCPosition_wm);     // 26
    tree_in -> SetBranchStatus("CrystalN",flag_CrystalN);           // 27
    tree_in -> SetBranchStatus("CrystalN2",flag_CrystalN2);          // 28
    tree_in -> SetBranchStatus("CrystalN_adc",flag_CrystalN_adc);       // 29
    tree_in -> SetBranchStatus("CrystalN_woLS",flag_CrystalN_woLS);      // 30
    tree_in -> SetBranchStatus("PSDPara",flag_PSDPara);            // 31
    tree_in -> SetBranchStatus("PSDChInfo",flag_PSDChInfo);          // 32
    tree_in -> SetBranchStatus("Energy",flag_Energy);             // 33
    tree_in -> SetBranchStatus("ADCEnergy",flag_ADCEnergy);          // 34
    tree_in -> SetBranchStatus("Energy_woLS",flag_Energy_woLS);        // 35
    tree_in -> SetBranchStatus("TriggerCounter",flag_TriggerCounter);     // 36
    tree_in -> SetBranchStatus("EnergyScaleFactor",flag_EnergyScaleFactor); // 37
    tree_in -> SetBranchStatus("PSDDoubleP",flag_PSDDoubleP);         // 38

    if (flag_Header)            tree_in -> SetBranchAddress("Header",            &Header);             //  0
    if (flag_Flag)              tree_in -> SetBranchAddress("Flag",              &Flag);              //  1
    if (flag_EventTimeStamp)    tree_in -> SetBranchAddress("EventTimeStamp",    &EventTimeStamp);     //  2
    if (flag_TimingDiff)        tree_in -> SetBranchAddress("TimingDiff",        &TimingDiff);        //  3
    if (flag_DataQuality)       tree_in -> SetBranchAddress("DataQuality",       &DataQuality);        //  4
    if (flag_EventID)           tree_in -> SetBranchAddress("EventID",           &EventID);           //  5
    if (flag_ADCTotal)          tree_in -> SetBranchAddress("ADCTotal",          &ADCTotal);          //  6
    if (flag_NPETotal)          tree_in -> SetBranchAddress("NPETotal",          &NPETotal);          //  7
    if (flag_ADCTotal4us)       tree_in -> SetBranchAddress("ADCTotal4us",       &ADCTotal4us);       //  8
    if (flag_ADCTotal4us2)      tree_in -> SetBranchAddress("ADCTotal4us2",      &ADCTotal4us2);      //  9
    if (flag_NPETotal4us)       tree_in -> SetBranchAddress("NPETotal4us",       &NPETotal4us);       // 10
    if (flag_ADCShort)          tree_in -> SetBranchAddress("ADCShort",          &ADCShort);          // 11
    if (flag_NPEShort)          tree_in -> SetBranchAddress("NPEShort",          &NPEShort);          // 12
    if (flag_FADCINT)           tree_in -> SetBranchAddress("FADCINT",           &FADCINT);            // 13
    if (flag_FADCINT4us)        tree_in -> SetBranchAddress("FADCINT4us",        &FADCINT4us);         // 14
    if (flag_FADCINT60ns)       tree_in -> SetBranchAddress("FADCINT60ns",       &FADCINT60ns);        // 15
    if (flag_FADCSho)           tree_in -> SetBranchAddress("FADCSho",           &FADCSho);            // 16
    if (flag_SCHANNEL)          tree_in -> SetBranchAddress("SCHANNEL",          &SCHANNEL);          // 17
    if (flag_Ratio)             tree_in -> SetBranchAddress("Ratio",             &Ratio);             // 18
    if (flag_Ratio4us)          tree_in -> SetBranchAddress("Ratio4us",          &Ratio4us);          // 19
    if (flag_RatioR)            tree_in -> SetBranchAddress("RatioR",            &RatioR);             // 20
    if (flag_PMTALL)            tree_in -> SetBranchAddress("PMTALL",            &PMTALL);             // 21
    if (flag_Pedestal)          tree_in -> SetBranchAddress("Pedestal",          &Pedestal);           // 22
    if (flag_Position_wm)       tree_in -> SetBranchAddress("Position_wm",       &Position_wm);        // 23
    if (flag_Position_wm2)      tree_in -> SetBranchAddress("Position_wm2",      &Position_wm2);       // 24
    if (flag_Position_wm_woLS)  tree_in -> SetBranchAddress("Position_wm_woLS",  &Position_wm_woLS);   // 25
    if (flag_ADCPosition_wm)    tree_in -> SetBranchAddress("ADCPosition_wm",    &ADCPosition_wm);     // 26
    if (flag_CrystalN)          tree_in -> SetBranchAddress("CrystalN",          &CrystalN);           // 27
    if (flag_CrystalN2)         tree_in -> SetBranchAddress("CrystalN2",         &CrystalN2);          // 28
    if (flag_CrystalN_adc)      tree_in -> SetBranchAddress("CrystalN_adc",      &CrystalN_adc);       // 29
    if (flag_CrystalN_woLS)     tree_in -> SetBranchAddress("CrystalN_woLS",     &CrystalN_woLS);      // 30
    if (flag_PSDPara)           tree_in -> SetBranchAddress("PSDPara",           &PSDPara);            // 31
    if (flag_PSDChInfo)         tree_in -> SetBranchAddress("PSDChInfo",         &PSDChInfo);          // 32
    if (flag_Energy)            tree_in -> SetBranchAddress("Energy",            &Energy);             // 33
    if (flag_ADCEnergy)         tree_in -> SetBranchAddress("ADCEnergy",         &ADCEnergy);          // 34
    if (flag_Energy_woLS)       tree_in -> SetBranchAddress("Energy_woLS",       &Energy_woLS);        // 35
    if (flag_TriggerCounter)    tree_in -> SetBranchAddress("TriggerCounter",    &TriggerCounter);     // 36
    if (flag_EnergyScaleFactor) tree_in -> SetBranchAddress("EnergyScaleFactor", &EnergyScaleFactor); // 37
    if (flag_PSDDoubleP)        tree_in -> SetBranchAddress("PSDDoubleP",        &PSDDoubleP);         // 38
    cerr << "input  = " << fin_name  << " (" << tree_in  -> GetEntries() << " events)" << endl;

    // make output
    tree_in -> GetEntry(0);
    TString fout_name = Form("Run%03d/Run%03d-%03d-%03d.root",Run,(Int_t)Header[2], (Int_t)Header[3], (Int_t)Header[4]);
    // TString fout_name = Form("/np1c/v01/candles/CANUG/USER/kyohei/data/catroot_physics_subsubrun_reduced/Run%03d-%03d-%03d_reduced.root", (Int_t)Header[2], (Int_t)Header[3], (Int_t)Header[4]);
    //    TString fout_name = Form("/np1c/v01/candles/CANUG/USER/kyohei/data/catroot_neutron_reduced_subsubrun/Run%03d-%03d-%03d_reduced.root", (Int_t)Header[2], (Int_t)Header[3], (Int_t)Header[4]);
    tfout = new TFile(fout_name, "RECREATE");

    tree_out = new TTree("tree", "");
    if (flag_Header)            tree_out -> Branch("Header",            &Header,             "Header[5]/F");           //  0
    if (flag_Flag)              tree_out -> Branch("Flag",              &Flag,              "Flag/I");                //  1
    if (flag_EventTimeStamp)    tree_out -> Branch("EventTimeStamp",    &EventTimeStamp,     "EventTimeStamp[3]/I");   //  2
    if (flag_TimingDiff)        tree_out -> Branch("TimingDiff",        &TimingDiff,        "TimingDiff/L");          //  3
    if (flag_DataQuality)       tree_out -> Branch("DataQuality",       &DataQuality,       "DataQuality[4]/I");      //  4
    if (flag_EventID)           tree_out -> Branch("EventID",           &EventID,           "EventID/I");             //  5
    if (flag_ADCTotal)          tree_out -> Branch("ADCTotal",          &ADCTotal,          "ADCTotal/F");            //  6
    if (flag_NPETotal)          tree_out -> Branch("NPETotal",          &NPETotal,          "NPETotal/F");            //  7
    if (flag_ADCTotal4us)       tree_out -> Branch("ADCTotal4us",       &ADCTotal4us,       "ADCTotal4us/F");         //  8
    if (flag_ADCTotal4us2)      tree_out -> Branch("ADCTotal4us2",      &ADCTotal4us2,      "ADCTotal4us2/F");        //  9
    if (flag_NPETotal4us)       tree_out -> Branch("NPETotal4us",       &NPETotal4us,       "NPETotal4us/F");         // 10
    if (flag_ADCShort)          tree_out -> Branch("ADCShort",          &ADCShort,          "ADCShort/F");            // 11
    if (flag_NPEShort)          tree_out -> Branch("NPEShort",          &NPEShort,          "NPEShort/F");            // 12
    if (flag_FADCINT)           tree_out -> Branch("FADCINT",           &FADCINT,            "FADCINT[75]/F");         // 13
    if (flag_FADCINT4us)        tree_out -> Branch("FADCINT4us",        &FADCINT4us,         "FADCINT4us[75]/F");      // 14
    if (flag_FADCINT60ns)       tree_out -> Branch("FADCINT60ns",       &FADCINT60ns,        "FADCINT60ns[75]/F");     // 15
    if (flag_FADCSho)           tree_out -> Branch("FADCSho",           &FADCSho,            "FADCSho[75]/F");         // 16
    if (flag_SCHANNEL)          tree_out -> Branch("SCHANNEL",          &SCHANNEL,          "SCHANNEL/I");            // 17
    if (flag_Ratio)             tree_out -> Branch("Ratio",             &Ratio,             "Ratio/F");               // 18
    if (flag_Ratio4us)          tree_out -> Branch("Ratio4us",          &Ratio4us,          "Ratio4us/F");            // 19
    if (flag_RatioR)            tree_out -> Branch("RatioR",            &RatioR,             "RatioR[3]/F");           // 20
    if (flag_PMTALL)            tree_out -> Branch("PMTALL",            &PMTALL,             "PMTALL[4480]/F");        // 21
    if (flag_Pedestal)          tree_out -> Branch("Pedestal",          &Pedestal,           "Pedestal[75]/F");        // 22
    if (flag_Position_wm)       tree_out -> Branch("Position_wm",       &Position_wm,        "Position_wm[3]/F");      // 23
    if (flag_Position_wm2)      tree_out -> Branch("Position_wm2",      &Position_wm2,       "Position_wm2[3]/F");     // 24
    if (flag_Position_wm_woLS)  tree_out -> Branch("Position_wm_woLS",  &Position_wm_woLS,   "Position_wm_woLS[3]/F"); // 25
    if (flag_ADCPosition_wm)    tree_out -> Branch("ADCPosition_wm",    &ADCPosition_wm,     "ADCPosition_wm[3]/F");   // 26
    if (flag_CrystalN)          tree_out -> Branch("CrystalN",          &CrystalN,           "CrystalN[6]/I");         // 27
    if (flag_CrystalN2)         tree_out -> Branch("CrystalN2",         &CrystalN2,          "CrystalN2[6]/I");        // 28
    if (flag_CrystalN_adc)      tree_out -> Branch("CrystalN_adc",      &CrystalN_adc,       "CrystalN_adc[6]/I");     // 29
    if (flag_CrystalN_woLS)     tree_out -> Branch("CrystalN_woLS",     &CrystalN_woLS,      "CrystalN_woLS[6]/I");    // 30
    if (flag_PSDPara)           tree_out -> Branch("PSDPara",          &PSDPara,          "PSDPara[34]/F");            // 30, modified
    //if (flag_PSDPara)           tree_out -> Branch("PSDPara4",          &PSDPara4,          "PSDPara4/F");            // 30, modified
    //if (flag_PSDPara)           tree_out -> Branch("PSDPara10",         &PSDPara10,         "PSDPara10/F");           // 30, modified
    //if (flag_PSDPara)           tree_out -> Branch("PSDPara18",         &PSDPara18,         "PSDPara18/F");           // 30, modified
    //if (flag_PSDPara)           tree_out -> Branch("PSDPara22",         &PSDPara22,         "PSDPara22/F");           // 30, modified
    //if (flag_PSDPara)           tree_out -> Branch("PSDPara28",         &PSDPara28,         "PSDPara28/F");           // 31, modified
    if (flag_PSDChInfo)         tree_out -> Branch("PSDChInfo",         &PSDChInfo,          "PSDChInfo[9]/F");        // 32
    if (flag_Energy)            tree_out -> Branch("Energy",            &Energy,           "Energy[2]/F");              // 33, modified
    //if (flag_Energy)            tree_out -> Branch("Energy1",            &Energy1,           "Energy1/F");
    //if (flag_Energy)            tree_out -> Branch("Energy2",            &Energy2,           "Energy2/F");
    if (flag_ADCEnergy)         tree_out -> Branch("ADCEnergy",         &ADCEnergy,        "ADCEnergy[2]/F");           // 34, modified
    if (flag_Energy_woLS)       tree_out -> Branch("Energy_woLS",       &Energy_woLS,      "Energy_woLS[2]/F");         // 35, modified
    if (flag_TriggerCounter)    tree_out -> Branch("TriggerCounter",  &TriggerCounter,  "TriggerCounter[44]/i");    // 35, modified
    //if (flag_TriggerCounter)    tree_out -> Branch("TriggerCounter36",  &TriggerCounter36,  "TriggerCounter36/i");    // 36, modified
    if (flag_EnergyScaleFactor) tree_out -> Branch("EnergyScaleFactor", &EnergyScaleFactor, "EnergyScaleFactor/F");   // 37
    if (flag_PSDDoubleP)        tree_out -> Branch("PSDDoubleP",        &PSDDoubleP,        "PSDDoubleP[22]/F");       // 38, modified
    if (flag_UnixTime)          tree_out -> Branch("UnixTime",          &UnixTime,          "UnixTime/I");            // 39, added


    // start to analyze
    //
    tfin -> cd();

    for (Int_t i = 0; i < tree_in -> GetEntries(); i++) {
      tree_in  -> GetEntry(i);
      // Define Cut Condition here!
      if (DataQuality[0]==0&&DataQuality[1]==0&&DataQuality[2]==0&&DataQuality[3]==0&&CrystalN[1]!=0&&PSDPara[1]<1.5&&((TriggerCounter[33]|TriggerCounter[36])&0x4)!=0){
        tree_out -> Fill();
      }
    }

    cout << "output = " << fout_name << " (" << tree_out -> GetEntries() << " events)" << endl
	 << endl;

    // make output
    tfout -> cd();
    tree_out  -> Write();

    delete tfin;
    delete tree_out;
    delete tfout;
  }

  return 0;
}
