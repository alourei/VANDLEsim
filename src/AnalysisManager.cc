
// $Id: AnalysisManager.cc 12.16.2016 A. Fijalkowska $
//
/// \file AnalysisManager.cc
/// \brief Definition of the AnalysisManager singleton class
//
//
#include "AnalysisManager.hh"

AnalysisManager::AnalysisManager()
{
   rootManager = G4RootAnalysisManager::Instance();	
   rootManager->SetVerboseLevel(1);
   rootManager->SetFirstHistoId(0);
   rootManager->SetFirstNtupleId(0);
   rootManager->SetFirstNtupleColumnId(0);  
   nrOfCreatedTuple = 0;

}

void AnalysisManager::CreateOutput(G4String filename)
{
   rootManager->OpenFile(filename);
   CreateTuple();   
}


void AnalysisManager::SaveOutput()
{
   rootManager->Write();
   rootManager->CloseFile();	
}

void AnalysisManager::CreateTuple()
{
   //analysisManager->SetHistoDirectoryName("histograms");
   //analysisManager->SetNtupleDirectoryName("ntuple");
   CreatePMTTuple();
	
}

void AnalysisManager::CreatePMTTuple()
{
  rootManager->CreateNtuple("OptPhotons", "Detected photons");
  rootManager->CreateNtupleDColumn("eventID");//D - double
  rootManager->CreateNtupleDColumn("detID");
  rootManager->CreateNtupleDColumn("pmtID");
  rootManager->CreateNtupleDColumn("time");
  rootManager->CreateNtupleDColumn("energy");
  rootManager->FinishNtuple();
  nrOfCreatedTuple ++;
  optPhTupleId = nrOfCreatedTuple;
/*  analysisManager->CreateNtuple("OptPhotonsSum", "Nr of detected photons");
  analysisManager->CreateNtupleDColumn("eventID");
  analysisManager->CreateNtupleDColumn("detID");
  analysisManager->CreateNtupleDColumn("pmtID");
  analysisManager->CreateNtupleDColumn("nrOfPhotons");
  analysisManager->FinishNtuple();*/
	
	
}

void AnalysisManager::CreateScintilatorTuple()
{
	//TODO	
}


void AnalysisManager::AddHit(PMTHitsCollection* pmtHC, G4int eventId)
{
    G4int pmts = pmtHC->entries();
    for(G4int i=0; i!=pmts; i++)
    {
	   G4int moduleIndex = (*pmtHC)[i]->GetModuleIndex();
	   G4int pmtIndex = (*pmtHC)[i]->GetPMTIndex();
	   G4int photonsCounts = (*pmtHC)[i]->GetPhotonsCounts();
	   for(G4int photonNr=0; photonNr != photonsCounts; photonsCounts++)
       {
		   G4double energyDep = (*pmtHC)[i]->GetEnergyDeposit(photonNr);
		   G4double time = (*pmtHC)[i]->GetTime(photonNr);
		   
		   //ugly way to fill tuple forced by G4RootAnalysisManager
		   int colId = 0;
		   rootManager->FillNtupleDColumn(optPhTupleId, colId, eventId); 
		   rootManager->FillNtupleDColumn(optPhTupleId, colId++, moduleIndex);
		   rootManager->FillNtupleDColumn(optPhTupleId, colId++, pmtIndex);
		   rootManager->FillNtupleDColumn(optPhTupleId, colId++, time);
		   rootManager->FillNtupleDColumn(optPhTupleId, colId++, energyDep);
		   rootManager->AddNtupleRow(optPhTupleId);
	   }
    }	
}

AnalysisManager *AnalysisManager::s_instance = 0;
