{
  Int_t Run;
  cout << "Run?" << endl;
  scanf("%d", & Run);
  Int_t status=1;
  //cout << "Energy?[?]" << endl;
  //scanf("%d", & status);
  Int_t bis=5;
  
  cout << "SubRun start?" << endl;
  Int_t start;
  scanf("%d", &start);
  
  cout << "SubRun end?" << endl;
  Int_t END;
  scanf("%d", &END);
  
  gROOT -> SetBatch(); //do not show plots
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(100000);
  // Levenberg-Marquardt Algorithm
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("GSLMultiFit", "");
//  ofstream fout(Form("Run%03d_K40/ADCRun%03d_K40.txt",Run,Run));
  ofstream fout(Form("Run%03d_K40/NPERun%03d_K40.txt",Run,Run));
  
  for (Int_t SubRun=start; SubRun<=END; SubRun++){
    gStyle -> SetOptFit(1112);
    gStyle -> SetStatFont(42);
    gStyle -> SetStatH(0.1);
    gStyle -> SetStatW(0.15);
    TCanvas* c1 = new TCanvas("c1");
    c1 -> SetGrid();
//    TString inputfile = Form("Run%03d_histogram/ADChistogram_Run%03d-%03d.root",Run,Run,SubRun);
    TString inputfile = Form("Run%03d_histogram/NPEhistogram_Run%03d-%03d.root",Run,Run,SubRun);
    TFile* data = new TFile(inputfile);
    if (data -> GetNkeys() != 0){
      TFile* file = TFile::Open(inputfile);
      TH1F* histo = (TH1F*)file->Get("histo");
      histo -> Draw();
      TString Title = Form("Run%03d, SubRun=%03d", Run, SubRun);
      TString ytitle = Form("Counts/%dkeV [#]",bis);
      histo -> SetTitle(Title);
      histo -> GetXaxis() -> SetRangeUser(1000,2000);
//      histo -> GetXaxis() -> SetTitle(Form("ADCEnergy[%d] [keV]",status));
      histo -> GetXaxis() -> SetTitle(Form("Energy[%d] [keV]",status));
      histo -> GetXaxis() -> SetTitleFont(62);
      histo -> GetXaxis() -> SetTitleSize(0.04);
      histo -> GetXaxis() -> SetTitleOffset(1.0);
      histo -> GetYaxis() -> SetTitle(ytitle);
      histo -> GetYaxis() -> SetTitleFont(62);
      histo -> GetYaxis() -> SetTitleSize(0.04);
      histo -> GetYaxis() -> SetTitleOffset(1.1);
      Float_t eventN = histo -> GetEntries();
	
      // K40 1.65MeV
      TF1* f1 = new TF1("f1","gaus",1380,1600); // par 0,1,2
      histo -> Fit("f1","L","",1380,1600);
      
      TF1* f2 = new TF1("f2","expo",1300,1800); 
      histo -> Fit("f2","+","",1300,1800);
      
      TF1* func = new TF1("func","gaus(0)+expo(3)",1300,1680);
      Double_t par[5];
      f1->GetParameters(&par[0]);
      f2->GetParameters(&par[3]);
      func -> SetParameters(par);
      histo -> Fit(func,"","",1300,1680);
      
      //c1 -> Print(Form("/home/mmoser/analysis/ReducedFiles/Run%03d/Reprocess1/histos/ADC/fitting_Run%03d-%03d.eps",Run, Run, SubRun),"recreate");
      // Save as root and as pdf //
//      TString pathEnergyHisto = Form("Run%03d_K40/ADCRun%03d-%03dK40.pdf",Run,Run,SubRun);
      TString pathEnergyHisto = Form("Run%03d_K40/NPERun%03d-%03dK40.pdf",Run,Run,SubRun);
      //TString pathEnergyHistopng = Form("/home/kenk/Peak/ADC-analysis/Run%03d/HistoADC_Run%03d-%03d.png",Run, Run, SubRun);
      //TString pathEnergyHistoroot = Form("cat28_6_009_E%d/Run%03d-%03d.root",status,Run,SubRun);
      
      c1 -> SaveAs(pathEnergyHisto, "pdf");
      //c1 -> SaveAs(pathEnergyHistopng, "png");
      //TFile* outputfile = new TFile(pathEnergyHistoroot, "recreate");
      histo -> Write();
      c1 -> Write();
      
      Float_t position_err = f1 -> GetParError(1) + func -> GetParError(1);
      Float_t sigma_err = f1 -> GetParError(2) + func -> GetParError(2);
     // compute rate
      Int_t peak;
      Float_t chi;
      Int_t sigma;
      if(f1 -> GetParameter(1)!=0){
        chi = (f1 -> GetChisquare())/(f1 -> GetNDF());
        peak = (Int_t) f1 -> GetParameter(1);
        sigma = (Int_t) f1 -> GetParameter(2);
        cout << peak << endl;
      }
      // read official datafile without any cut. RATE!!!
      TString fin2_name = Form("Run%03d/Run%03d-%03d.root",Run, Run, SubRun); // modified
      cout << "Reading" << fin2_name << endl;
      TFile* tfile = new TFile(fin2_name);
    
      if (tfile -> GetNkeys() != 0){
        TTree* tree = (TTree*) tfile -> Get("tree");
        if (tree != NULL){
	  //calculate event rate
				  Float_t Header[5], Ratio4us, Energy[2], PSDPara[34];
				  Int_t DataQuality, CrystalN[6], CrystalN_adc[6];
				  UInt_t TriggerCounter[44];
				  Double_t Event;
				  tree -> SetBranchAddress("Header",         Header);
				  tree -> SetBranchAddress("DataQuality",    &DataQuality);
				  //tree -> SetBranchAddress("Ratio4us",       &Ratio4us);
				  tree -> SetBranchAddress("CrystalN",       &CrystalN);
				  //tree -> SetBranchAddress("CrystalN_adc",       &CrystalN_adc);
				  tree -> SetBranchAddress("PSDPara",        &PSDPara);
				  tree -> SetBranchAddress("ADCEnergy",         &Energy);
				  tree -> SetBranchAddress("TriggerCounter",   &TriggerCounter);
				  tree -> SetBranchAddress("TriggerCounter",   &TriggerCounter);
	
					for(Int_t i=0; i < tree -> GetEntries(); i++){
				    tree -> GetEntry(i);
				  }
//				  TCut cut2 = Form("ADCEnergy[1]>%d&&ADCEnergy[1]<%d&&CrystalN[1]!=0&&PSDPara[1]<1.5&&DataQuality[0]==0&&DataQuality[1]==0&&DataQuality[2]==0&&DataQuality[3]==0&&((TriggerCounter[33]|TriggerCounter[36])&0x4)!=0",peak-sigma,peak+sigma);
				  TCut cut2 = Form("Energy[1]>%d&&Energy[1]<%d&&CrystalN[1]!=0&&PSDPara[1]<1.5&&DataQuality[0]==0&&DataQuality[1]==0&&DataQuality[2]==0&&DataQuality[3]==0&&((TriggerCounter[33]|TriggerCounter[36])&0x4)!=0",peak-sigma,peak+sigma);
				  Event = tree -> GetEntries(cut2);
				  Double_t Rate = Event/Header[1]*60;
				  Double_t RateError = sqrt(Event)/Header[1]*60;
				  Double_t TimeRatio = (Header[0] - Header[1])/Header[0];
	
          if(f1-> GetParameter(1)!=0){
				    fout << Run << " " << SubRun  << " " ;
				    fout << func -> GetParameter(1) << " " << position_err << " ";
				    fout << func -> GetParameter(2) << " " << sigma_err << " ";
				    fout << chi << " " << eventN << " " << Rate << " " << RateError << " " << TimeRatio << endl;
	    // Run SubRun Peak PeakError Sigma SigmaError ChiSquare EventN Rate RateError TimeRatio
          }
        }
      tfile->Close();
      }
    data->Close();
    }
  }
  fout.close();
  
}
