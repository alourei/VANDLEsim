// $Id: ArgonneGeArray.hh 10.05.2017 A Fijalkowska $
//
/// \file ArgonneGeArray.hh
/// \brief Definition of the ArgonneGeArray class
//
#ifndef ArgonneGeArray_H
#define ArgonneGeArray_H 1
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "HPGe.hh"

class ArgonneGeArray
{
  public:

    ArgonneGeArray(); 
    virtual ~ArgonneGeArray();     
    void Place(G4RotationMatrix *pRot, 
               G4ThreeVector &tlate, 
               const G4String &pName, 
               G4LogicalVolume *pMotherLogical,  
               G4int pCopyNo = 0);
    virtual void ConstructSDandField();           
  private:
     void SetBasicSizes();
     HPGe* HPGeDet;
     
     G4double HPGeXOffset;
};

#endif
