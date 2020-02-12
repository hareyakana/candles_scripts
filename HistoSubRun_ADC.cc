{
  Int_t Run;
  cout << "Run?" << endl;
  scanf("%d",& Run);
  Int_t energy=1;
    
  cout << "SubRun start?" << endl;
  Int_t start;
  scanf("%d", &start);
  
  cout << "SubRun end?" << endl;
  Int_t END;
  scanf("%d", &END);
  
  TCut cut1 = Form("PSDPara[1]<1.5&&CrystalN[1]!=0"); //Just Run009
  TCut cut2 = "DataQuality[0]==0&&DataQuality[1]==0&&DataQuality[2]==0&&DataQuality[3]==0";
  TCut cut3 = "((TriggerCounter[33]|TriggerCounter[36])&0x4)!=0";
  for(Int_t SubRun=start; SubRun<=END; SubRun++){
    TString file = Form("Run%03d/Run%03d-%03i.root", Run, Run, SubRun);
    TString outputfile = Form("Run%03d_histogram/ADChistogram_Run%03d-%03i.root", Run, Run, SubRun);
//    TString outputfile = Form("Run%03d_histogram/NPEhistogram_Run%03d-%03i.root", Run, Run, SubRun);
    TFile* data = new TFile(file);
    if (data -> GetNkeys() != 0){
      TTree* tree = (TTree*) data -> Get("tree");
      if (tree != NULL){
        TH1F* histo = new TH1F("histo","",1200,0,6000);
//      	TString gram = Form("Energy[%d]>>h1(1200,0,6000)",energy);
      	TString gram = Form("ADCEnergy[%d]>>h1(1200,0,6000)",energy);
				tree->Draw(gram,cut1+cut3+cut2);
				TH1F* h1 = (TH1F*)gDirectory->Get("h1");
				histo -> Add(h1);
        histo->Draw();
        TFile histogram(outputfile, "RECREATE");
        histo->Write();
        histogram.Close();
        histo->Clear();
      }
    }
  }
}
