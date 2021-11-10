// Stdlib header file for input and output.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
using namespace Pythia8;

int main() {

// Create and open a text file
ofstream MyFile("myfile.txt");

	Pythia pythia;
	pythia.readString("Top:gg2ttbar = on");
	pythia.readString("Top:qqbar2ttbar = on");	
	pythia.readString("Charmonium:all = on");
	pythia.readString("Beams:eCM = 14000.");
	pythia.init();

// Create file on which histogram(s) can be saved.
	Event *event = &pythia.event; 
	Hist mumu("Massa invariante dos muons", 30., 2.5, 5.5);
	TH1F *histo_mumu = new TH1F("histo_mumu", "Massa invariante de muons", 30.0, 2.5, 5.5);

//Declaracao de variaveis
	int muon1, muon2; 
	double jpsi_mass;
	double mu1_pt, mu2_pt, mu1_e, mu1_px, mu1_py, mu1_pz, mu2_e, mu2_px, mu2_py, mu2_pz;
	
// Begin event loop. Generate event; skip if generation aborted.
	for (int iEvent = 0; iEvent < 1000; ++iEvent) {
		if (!pythia.next()) continue;
		
		int iMuon = 0;
		for (int i = 0; i < pythia.event.size(); ++i) {

			int id = pythia.event[i].id();
			int id_jpsi = pythia.event[i].id();
			int status = pythia.event[i].status();
			if (abs(id) == 13) { //id do muon ou antimuon
				if (id > 0. && status > 30. && status < 40.0) {
					mu1_px = pythia.event[i].px();
					mu1_py = pythia.event[i].py();
					mu1_pz = pythia.event[i].pz();
					mu1_e = pythia.event[i].e();
					mu1_pt = pythia.event[i].pT();
					muon1 = pythia.event[i].mother1();
					//jpsi_mass = pythia.event[i].m();
				}else{
					mu2_px = pythia.event[i].px();
					mu2_py = pythia.event[i].py();
					mu2_pz = pythia.event[i].pz();
					mu2_e = pythia.event[i].e();
					mu2_pt = pythia.event[i].pT();
					muon2 = pythia.event[i].mother2();
				}
				//if (id_jpsi = 443) {
				jpsi_mass = sqrt( pow(mu1_e+mu2_e, 2)-pow(mu1_px+mu2_px,2)+pow(mu1_py+mu2_py,2)+pow(mu1_pz+mu2_pz, 2));
				mumu.fill(jpsi_mass);
				histo_mumu->Fill(jpsi_mass);
				//}				
			}

		}
		cout << "m1_pT: " << mu1_pt <<
			" m2_pT: " << mu2_pt << endl;
		MyFile << mu1_pt << " " << mu2_pt  << " " << jpsi_mass << endl ;
	}
	cout << mumu;

//Create canvas
	TCanvas* c1 = new TCanvas("c1","Mumu Mass",800,800);  
	histo_mumu->GetXaxis()->SetTitle("m_{#mu#mu}");
	histo_mumu->GetYaxis()->SetTitle("Eventos");
	histo_mumu->SetFillColor(kRed);
	histo_mumu->Draw("");
// Save in pdf format
  c1->SaveAs("mumu_mass.pdf","pdf");	

// Statistics on event generation.
	pythia.stat();

// Close the file (no final do programa)
	MyFile.close();

  // Done.
  return 0;
}