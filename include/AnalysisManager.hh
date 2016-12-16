// $Id: AnalysisManager.hh 12.08.2016 A. Fijalkowska $
//
/// \file AnalysisManager.hh
/// \brief Definition of the AnalysisManager singleton class
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// ntuple are created in CreateTuple() for the following 
/// physics quantities:
/// - Edep in plasctic bar
/// - Optical photons in PMT
/// - Neutron en loss in plastic bar
///

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

#include "G4RootAnalysisManager.hh"
#include "G4String.hh"
#include "PMTHit.hh"


class AnalysisManager
{
	public:

		void CreateOutput(G4String filename);
		void SaveOutput();
		void AddHit(PMTHitsCollection* pmtHC, G4int eventId);//? just hit, or hit collection?
		
	private:
		AnalysisManager();
		virtual ~AnalysisManager() {}
		void CreateTuple();
		void CreatePMTTuple();
		void CreateScintilatorTuple();
		
		static AnalysisManager *s_instance;
		G4RootAnalysisManager* rootManager;
		G4int nrOfCreatedTuple;		
		G4int optPhTupleId;
		
	public:
	static AnalysisManager* GetInstance()
	{
		if (!s_instance)
			s_instance = new AnalysisManager();
		return s_instance;
	}
		
};

#endif
