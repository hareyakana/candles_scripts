//root_combine.cc
//produce a set of command that combine all subsubrun into subrun for each sub run
//to run the dat file produced
//  $ sh root_combine.dat
//Future improvement:run tis script that automatically combine the subsubrun rather
//than produced a set of commands

{
  Int_t Run,SubRun;
  cout << "Run = " << endl;
  scanf("%d",&Run);
  
  cout << "SubRun start?" << endl;
  Int_t start;
  scanf("%d", &start);
  
  cout << "SubRun end?" << endl;
  Int_t end;
  scanf("%d", &end);
  
  ofstream fout("deletelist.dat");
  
  for(SubRun=69;SubRun<=end;SubRun++){
    TString fin_name = Form("Run%03d/Run%03d-%03d-001.root",Run,Run,SubRun);
    TString fileout = Form("Run%03d/Run%03d-%03d.root",Run,Run,SubRun);
    TString filein = Form("Run%03d/Run%03d-%03d-*.root",Run,Run,SubRun);
    TFile *tfile = new TFile(fin_name);
    if (tfile -> GetNkeys()!=0){
//      fout << "hadd " << fileout << " " << filein << endl;
      fout << "rm -f " << filein << endl;
    }
  }
  fout.close();
}
