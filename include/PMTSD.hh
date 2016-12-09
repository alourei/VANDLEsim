//
// $Id: PMTSD.hh 12.08.2016 A. Fijalkowska $
//
/// \file PMTSD.hh
/// \brief Definition of the pmt sensitive detector class
//
//
#ifndef PMTSD_h
#define PMTSD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4LogicalVolume.hh"
#include "PMTHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class PMTSD : public G4VSensitiveDetector
{

  public:

    PMTSD(G4String name, G4int moduleDeph, G4int pmtDeph);
    virtual ~PMTSD();
 
    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();

  private:
    G4int GetIndex(G4Step* aStep, int deph);
    G4double GetEnergyDeposit(G4Step* aStep);
    G4double GetHitTime(G4Step* aStep);
    G4LogicalVolume* GetHitLogVol(G4Step* aStep);
    PMTHitsCollection* pmtHitCollection;
    G4int pmtDeph;
    G4int moduleDeph;

};

#endif
