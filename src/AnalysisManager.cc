
// $Id: AnalysisManager.cc 12.16.2016 A. Fijalkowska $
//
/// \file AnalysisManager.cc
/// \brief Definition of the AnalysisManager singleton class
//
//
#include "AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
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
   CreateScintilatorTuple();
	
}

void AnalysisManager::CreatePMTTuple()
{
  rootManager->CreateNtuple("PMTHitInfo", "Detected photons");
  rootManager->CreateNtupleIColumn("eventID");//D - double, I - int
  rootManager->CreateNtupleIColumn("detID");
  rootManager->CreateNtupleIColumn("pmtID");
  rootManager->CreateNtupleDColumn("time");
  //rootManager->CreateNtupleDColumn("enDep");
  rootManager->FinishNtuple();
  optPhTupleId = nrOfCreatedTuple++;

//add first empty event
   int colId = 0;
   rootManager->FillNtupleIColumn(optPhTupleId, colId, 0); 
   rootManager->FillNtupleIColumn(optPhTupleId, ++colId, 0);
   rootManager->FillNtupleIColumn(optPhTupleId, ++colId, 0);
   rootManager->FillNtupleDColumn(optPhTupleId, ++colId, 0.);
   //rootManager->FillNtupleDColumn(optPhTupleId, ++colId, 0.);
   rootManager->AddNtupleRow(optPhTupleId);
	
}

void AnalysisManager::CreateScintilatorTuple()
{
   rootManager->CreateNtuple("NeutronInteractInfo", "Neutron interactions in scint");
   rootManager->CreateNtupleIColumn("eventID");
   rootManager->CreateNtupleIColumn("detID");
   rootManager->CreateNtupleDColumn("time");
   rootManager->CreateNtupleDColumn("enDep");
   rootManager->CreateNtupleDColumn("xPos");
   rootManager->CreateNtupleDColumn("yPos");
   rootManager->CreateNtupleDColumn("zPos");
   rootManager->FinishNtuple();
   scintTupleId = nrOfCreatedTuple++;
   
   int colId = 0;
   rootManager->FillNtupleIColumn(scintTupleId, colId, 0); 
   rootManager->FillNtupleIColumn(scintTupleId, ++colId, 0);
   rootManager->FillNtupleDColumn(scintTupleId, ++colId, 0.);
   rootManager->FillNtupleDColumn(scintTupleId, ++colId, 0.);
   rootManager->FillNtupleDColumn(scintTupleId, ++colId, 0.);		   
   rootManager->FillNtupleDColumn(scintTupleId, ++colId, 0.);
   rootManager->FillNtupleDColumn(scintTupleId, ++colId, 0.);		   
   rootManager->AddNtupleRow(scintTupleId);   
   
}


void AnalysisManager::AddHit(PMTHitsCollection* pmtHC, G4int eventId)
{
    G4int pmts = pmtHC->entries();
    for(G4int i=0; i!=pmts; i++)
    {
	   G4int moduleIndex = (*pmtHC)[i]->GetModuleIndex();
	   G4int pmtIndex = (*pmtHC)[i]->GetPMTIndex();
	   G4int photonsCounts = (*pmtHC)[i]->GetPhotonsCounts();
	   for(G4int photonNr=0; photonNr != photonsCounts; photonNr++)
       {
		   G4double energyDep = (*pmtHC)[i]->GetEnergyDeposit(photonNr);
		   G4double time = (*pmtHC)[i]->GetTime(photonNr);
		   
		   //ugly way to fill tuple forced by G4RootAnalysisManager
		   int colId = 0;
		   rootManager->FillNtupleIColumn(optPhTupleId, colId, eventId); 
		   rootManager->FillNtupleIColumn(optPhTupleId, ++colId, moduleIndex);
		   rootManager->FillNtupleIColumn(optPhTupleId, ++colId, pmtIndex);
		   rootManager->FillNtupleDColumn(optPhTupleId, ++colId, time);
		   //rootManager->FillNtupleDColumn(optPhTupleId, ++colId, energyDep/keV);
		   rootManager->AddNtupleRow(optPhTupleId);
	   }
    }	
}


void AnalysisManager::AddHit(ScintillatorHitsCollection* scintHC, G4int eventId)
{
    G4int modules = scintHC->entries();
    for(G4int i=0; i != modules; i++)
    {
	   G4int moduleIndex = (*scintHC)[i]->GetModuleIndex();
	   G4int nrOfInteractions = (*scintHC)[i]->GetNrOfInteractions();
	   for(G4int interact=0; interact != nrOfInteractions; interact++)
       {
		   G4double energyDep = (*scintHC)[i]->GetEnergyDeposit(interact);
		   G4double time = (*scintHC)[i]->GetTime(interact);
		   G4double xPos = (*scintHC)[i]->GetxPos(interact);
		   G4double yPos = (*scintHC)[i]->GetyPos(interact);
		   G4double zPos = (*scintHC)[i]->GetzPos(interact);
		   int colId = 0;
		   rootManager->FillNtupleIColumn(scintTupleId, colId, eventId); 
		   rootManager->FillNtupleIColumn(scintTupleId, ++colId, moduleIndex);
		   rootManager->FillNtupleDColumn(scintTupleId, ++colId, time);
		   rootManager->FillNtupleDColumn(scintTupleId, ++colId, energyDep/keV);
		   rootManager->FillNtupleDColumn(scintTupleId, ++colId, xPos);		   
		   rootManager->FillNtupleDColumn(scintTupleId, ++colId, yPos);
		   rootManager->FillNtupleDColumn(scintTupleId, ++colId, zPos);		   
		   rootManager->AddNtupleRow(scintTupleId);
	   }
    }	
}

	
	
AnalysisManager *AnalysisManager::s_instance = 0;
