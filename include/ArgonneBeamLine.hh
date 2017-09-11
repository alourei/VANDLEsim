// $Id: ArgonneBeamLine.hh 10.05.2017 A Fijalkowska $
//
/// \file ArgonneBeamLine.hh
/// \brief Definition of the ArgonneBeamLine class
//
#ifndef ArgonneBeamLine_H
#define ArgonneBeamLine_H1
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "MaterialsManager.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"

class ArgonneBeamLine
{
  public:

    ArgonneBeamLine(); 
    virtual ~ArgonneBeamLine(); 
    void Place(G4RotationMatrix *pRot, 
               G4ThreeVector &tlate, 
               const G4String &pName, 
               G4LogicalVolume *pMotherLogical,  
               G4int pCopyNo = 0);
               
  private:
     void SetBasicSizes();                                                                                      
     MaterialsManager* materialsManager;
     G4double inch;

};



#endif
