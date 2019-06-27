#define DalitzTest_cxx
#include "DalitzTest.h"

#include "Fit2MeV.C"

#include <TH1.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

TF1 * FitFunc2 = nullptr;
TH1 * Res = nullptr;
TH3D * Dalitz = nullptr;
TFile * File = nullptr; 

void DalitzTest::Begin(TTree *)
{
  TString option = GetOption();
}

void DalitzTest::SlaveBegin(TTree *)
{
  TString option = GetOption();
  
  FitFunc2 = new TF1("FitFunc2", Fit2MeV, 2600, 2680, 5);
  FitFunc2->SetParNames("total events", "mean", "sigma", "y int", "slope");
  FitFunc2->SetParameters(3000, 5280, 5, 0, 0);
  
  Res = new TH1D("Res", "Res", 80, 5200, 5360);
  Res->GetXaxis()->SetTitle("Mass [MeV]");
  Res->GetYaxis()->SetTitle("Events [per 2 MeV]");
  
  Dalitz = new TH3D("Dalitz", "Dalitz", 20, 0, 10, 30, 0.4, 1, 100, 0, 1);
  Dalitz->GetXaxis()->SetTitle("Chi 2 Value");
  Dalitz->GetYaxis()->SetTitle("PID Probability Product");
  Dalitz->GetZaxis()->SetTitle("Figure of Merit");
  
  File = new TFile("DalitzTest.root", "RECREATE");
  gFile = File;
}

Bool_t DalitzTest::Process(Long64_t entry)
{
  GetEntry(entry);
  fReader.SetEntry(entry);
  //marian: i think that all these variables can go  
  Double_t sigma;
  Double_t deltasigma;
  Double_t mu;
  Double_t deltaMu;
  Double_t total;
  Double_t deltaTotal;
  Double_t sigmaStr;
  Double_t deltaSigmaStr;
  Double_t muStr;
  Double_t deltaMuStr;
  Double_t totalStr;
  Double_t deltaTotalStr;
  
      //marian: what are these loops doing?
      for (int i = 0; i < 21; i++){
      for (int j = 20; j < 51; j++){
          
          if (((*p1_MC15TuneV1_ProbNNp * *p2_MC15TuneV1_ProbNNp * *p3_MC15TuneV1_ProbNNp * *p4_MC15TuneV1_ProbNNp) >= j/50)
          &&(*B_IPCHI2_OWNPV <= i/2)){
          //marian: if i understand correctly, you want to fill 600 histograms of 
            //B_MM for different values of ProbNNp products and IPCHI2. is this right? If yes, you need vectors
            //What you are doing here is filling the same histogram with up to 600 times the same events
              Res->Fill(*B_MM);
          }
          //marian: this will fit your histogram 600 times for each event
          Res->Fit("FitFunc2");
          double tot = Res->GetEntries();
          double sign = FitFunc2->GetParameter(0);
          double sign2 = FitFunc2->GetParameter(2);
     
      double fom = sign / ((sqrt(tot - sign)) - (sign2 / 2));
      //marian: this is a misconception. Dalitz-plots are very special multidimensional plots relating the decay kinematics to the decay amplitude
      Dalitz->Fill(i/2, j/50, fom);
      if (j == 50){break;}
      }
   if (i == 20){break;}
   }
   
   return kTRUE;
}

void DalitzTest::SlaveTerminate(){}//marian: changed

void DalitzTest::Terminate()//marian: changed
{
   Dalitz->Write("SURF2");
      
      File->Close();
}
//marian: please feel free to remove all comments again
