//
// $Id: RunAction.cc 12.10.2016 A Fijalkowska $
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "AnalysisManager.hh"
#include "time.h"
#include "G4Run.hh"
//#include "G4RunManager.hh"
//#include "G4UnitsTable.hh"
//#include "G4SystemOfUnits.hh"


RunAction::RunAction(): G4UserRunAction()
{ 
  // set printing event number per each event
  //G4RunManager::GetRunManager()->SetPrintProgress(1);     
  timer = new G4Timer;
   AnalysisManager* analysisManager = AnalysisManager::GetInstance();
   G4String fileName = MakeOutputFileName();
   std::cout<<fileName<<std::endl; 
   analysisManager->CreateOutput(fileName);
}


RunAction::~RunAction()
{
   delete timer;  
}

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
   G4cout << "### Run " << run->GetRunID() << " start." << G4endl; 
   timer->Start();	

}


void RunAction::EndOfRunAction(const G4Run* run)
{
   timer->Stop();
   G4cout << "number of event = " << run->GetNumberOfEvent() 
          << " " << *timer << G4endl;
         
   AnalysisManager* analysisManager = AnalysisManager::GetInstance();
   analysisManager->SaveOutput();

}


G4String RunAction::MakeOutputFileName()
{
	//time marked file, code taken form NEXT sim
   time_t rawtime;
   struct tm* timeinfo;
   char buffer[180];

   time ( &rawtime );
   timeinfo = localtime ( &rawtime );

   strftime (buffer,180,"_%H:%M:%S_%m_%d_%Y",timeinfo);

   // create a root file for the current run
   char fileName[300];
   sprintf(fileName, "./VANDLE%s.root",buffer);
   G4String name(fileName);
   return name;	
}


