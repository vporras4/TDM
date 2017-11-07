// root script file

#include "TFile.h"
#include "TTree.h"
#include <cmath>

void GetTDM_Totals(const char* TDMRootFileName){
	Long64_t Entries;
	Double_t Edep = 0;
	Double_t Edep_total = 0;
	Double_t Dodep = 0;
	Double_t Dodep_total = 0;

	TFile *TDMRootFile = new TFile( TDMRootFileName );
	TTree *TDMTree = (TTree*)TDMRootFile->Get("B4;1");
	Entries = TDMTree->GetEntriesFast();	
	TDMTree->SetBranchAddress("Eabs",&Edep);
	TDMTree->SetBranchAddress("Dodep",&Dodep);

	for(Long64_t i = 0; i < Entries ; i++ )
	{
		TDMTree->GetEntry(i);
		Edep_total += Edep;
		Dodep_total += Dodep;
	}

	cout << "Número de entradas: "<< Entries << endl;
	cout << "Energía depositada: " << Edep_total*1000 << " KeV"<< endl;
	cout << "Dosis: " << Dodep_total*(1e12) << " Gy"<< endl;

} 

