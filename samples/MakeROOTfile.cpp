void MakeROOTfile()
{
   TH1D *his1D = new TH1D("his1D", "1D histogram", 1000, 0., 100.);
   TH2D *his2D = new TH2D("his2D", "2D histogram", 180, -90., 90., 1000, 0., 100.);
   for(Int_t i = 0; i < 1000000; i++){
      his1D->Fill(gRandom->Exp(5));
      his2D->Fill(gRandom->Gaus(0., 15.), gRandom->Exp(5));
   }

   TString fileName = "test.root";
   TFile *file = new TFile(fileName, "RECREATE");
   his1D->Write();
   his2D->Write();
   file->Close();
}
