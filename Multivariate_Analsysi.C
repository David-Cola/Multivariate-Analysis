#define Multivariate_Analysis_cxx

#include "Multivariate_Analysis.h"
#include <TH2.h>
#include <TStyle.h>

#include "fit1MeV_Gaussian.C"
using namespace std;

#include <TH1.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <array>

std::array<std::array<TH1D*, 22>, 25> hists;
std::array<std::array<double, 22>, 25> fom;

TF1 * FitFunc2 = nullptr;
TH2D * Sens = nullptr;
TFile * File = nullptr; 
TCanvas * canvas = nullptr;


void Multivariate_Analysis::Begin(TTree * /*tree*/)
{

   TString option = GetOption();

}

void Multivariate_Analysis::SlaveBegin(TTree * /*tree*/)
{
   
   TString option = GetOption();

	for (double i = 0; i < 22; i++){
		for (double j = 0; j < 25; j++){
			hists[i][j] = new TH1D(TString::Format("hist_%f_%f", i, j), "Histogram", 80, 5200, 5360);
			hists[i][j]->GetXaxis()->SetTitle("Mass [MeV]");
			hists[i][j]->GetYaxis()->SetTitle("Events [per 2 MeV[");}}


	FitFunc2 = new TF1("FitFunc2", fit1MeV_Gaussian, 2600, 2680, 5);
	FitFunc2->SetParNames("total events", "mean", "sigma", "y int", "slope");
	FitFunc2->SetParameters(3000, 5280, 5, 0, 0);
  
	Sens = new TH2D("Distribution", "Distribution", 22, 1.0, 3.2, 25, 0.08, 0.33);
	Sens->GetXaxis()->SetTitle("Chi 2 Value");
	Sens->GetYaxis()->SetTitle("PID Probability Product");
	Sens->GetZaxis()->SetTitle("Figure of Merit");
  
  File = new TFile("Sensitivity.root", "RECREATE");
  gFile = File;

  canvas = new TCanvas("canvas", "Test Canvas");

}

Bool_t Multivariate_Analysis::Process(Long64_t entry)
{
   GetEntry(entry);
   fReader.SetEntry(entry);

	for (double i = 0; i < 22; i++){
		for (double j = 0; j < 25; j++){ 
   if (((*PromptK_IPCHI2_OWNPV) <= (i + 10)/10) && (((*PromptK_MC15TuneV1_ProbNNk)) >= ((j+8)/100))){
			hists[i][j]->Fill(*Xi_c0_MM);}}}

 }

void Multivariate_Analysis::SlaveTerminate()
{
  
}

void Multivariate_Analysis::Terminate()
{
	for (double i = 0; i < 22; i++){
		for (double j = 0; j < 25; j++){

			hists[i][j]->Fit("fit1MeV_Gaussian", "Q");
			fom[i][j] = ((hists[i][j]->GetEntries()) / (sqrt((hists[i][j]->GetEntries()) - fit1MeV_Gaussian->GetParameter(0)) - ((fit1MeV_Gaussian->GetParameter(2))/2)));

		std::cout << fom[i][j] << endl;

		hists[i][j]->Write();

		Sens->Fill(((i + 10)/10), ((j + 8)/100), fom[i][j]);}}

	Sens->Draw("COLZ");
	Sens->Write();
	File->Close();
 
  
