//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DetectorConstruction.hh 18.10.2016 A. Fijalkowska $
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "VANDLEBar.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "MaterialsManager.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    //virtual void ConstructSDandField();

  private:
    G4VPhysicalVolume* ConstructDetector();
    G4LogicalVolume* experimentalHallLogic;
    G4VPhysicalVolume* experimentalHallPhys;
    MaterialsManager* materialsManager;
    
	VANDLEBar* vandleBar;

    //Sensitive Detectors
    //G4Cache<ScintSD*> fScint_SD;
    //G4Cache<PMTSD*> fPmt_SD;

};

#endif
