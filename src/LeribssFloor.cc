// $Id: LeribssFloor.cc 26.01.2016 A Fijalkowska $
//
/// \file LeribssFloor.cc
/// \brief Implementation of the LeribssFloor class, mostly Sergey's Ilyushkin code
//
//

#include "LeribssFloor.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

LeribssFloor::LeribssFloor()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
} 

LeribssFloor::~LeribssFloor()
{
	
}

void LeribssFloor::SetBasicSizes()
{
	inch = 2.54*cm;	

	
}


void LeribssFloor::MakeCeilFloor(G4RotationMatrix *pRot, 
                                 G4ThreeVector &tlate, 
                                 const G4String &pName, 
                                 G4LogicalVolume *pMotherLogical,  
                                 G4int pCopyNo)
{


	G4Material* concreteMaterial = materialsManager->GetConcrete();
	G4Material* hdpeMaterial = materialsManager->GetHDPE();
	
	const G4double offsetCenter = 0.0; 

	G4double boxWidth = 5.0*m;
	G4double boxThickness = 10.0*inch;
	G4double boxLength = boxWidth;
	
     //const G4double ceilYOffset = 4.0*m + boxThickness; //MSU: 97.087 //LeRIBSS: 50.874 //MSU:148.27
     //const G4double floorYOffset = (91.5 - 5.08/4.)*cm + 5.0*inch + 37.973/2.0*cm; //154.305*cm //MSU: 51.18 //LeRIBSS: 91.5
	
	 const G4double ceilYOffset =  50.874*inch + boxThickness;
     //const G4double floorYOffset = (91.5 - 5.08/4.)*cm + 5.0*inch + 37.973/2.0*cm;
     const G4double floorYOffset = 2*m;
     
	G4double wallXOffset = boxWidth - boxThickness;
	G4double wallYOffset = ((ceilYOffset + floorYOffset)/2. - floorYOffset);
	G4double wallZOffset = boxLength - boxThickness;

	
	G4double mxWallX = 7.*m;
	G4double pxWallX = 4.*m;
	G4double mzWallZ = 6.*m;
	G4double pzWallZ = 4.*m;
	
	G4double wallHeight = (ceilYOffset + floorYOffset)/2. - boxThickness;
	
	G4double xWallL = wallHeight;
	G4double xWallW = boxThickness;
	G4double xWallH = (mzWallZ + pzWallZ)/2. - xWallW;
	
	G4double zWallL = wallHeight;
	G4double zWallW = boxThickness;
	G4double zWallH = (mxWallX + pxWallX)/2. + xWallW;
	
	G4double FCL = zWallH;
	G4double FCW = boxThickness;
	G4double FCH = xWallH + 2.*boxThickness;
	
	G4double xWallZOffset = (mzWallZ + pzWallZ)/2. - pzWallZ;
	G4double zWallXOffset = (mxWallX + pxWallX)/2. - pxWallX;
	G4double CFZOffset = xWallZOffset;
	G4double CFXOffset = zWallXOffset;
	
	//G4Box* boxCeil = new G4Box("Ceil", boxWidth, boxThickness, boxLength);
	G4Box* boxCeil = new G4Box("Ceil", FCL, FCW, FCH);
	//G4Box* boxFloor = new G4Box("Floor", boxWidth, boxThickness, boxLength); 
	G4Box* boxFloor = new G4Box("Floor", FCL, FCW, FCH); 
	//G4Box* boxWallx = new G4Box("Wallx", wallHeight, boxThickness, boxLength - 2.*boxThickness); 
	G4Box* boxWallx = new G4Box("Wallx", xWallL, xWallW, xWallH); 
	//G4Box* boxWallz = new G4Box("Wallz", wallYOffset, boxThickness, boxLength); 
	G4Box* boxWallz = new G4Box("Wallz", zWallL, zWallW, zWallH); 
   

     G4LogicalVolume* ceil_LogicalVolume = new G4LogicalVolume(boxCeil, concreteMaterial, "ceil_Logical");
     G4LogicalVolume* floor_LogicalVolume = new G4LogicalVolume(boxFloor, concreteMaterial, "floor_Logical");
     G4LogicalVolume* wallmx_LogicalVolume = new G4LogicalVolume(boxWallx, concreteMaterial, "wallmx_Logical");
     G4LogicalVolume* wallpx_LogicalVolume = new G4LogicalVolume(boxWallx, concreteMaterial, "wallpx_Logical");
     G4LogicalVolume* wallmz_LogicalVolume = new G4LogicalVolume(boxWallz, concreteMaterial, "wallmz_Logical");
     G4LogicalVolume* wallpz_LogicalVolume = new G4LogicalVolume(boxWallz, concreteMaterial, "wallpz_Logical");
     
    G4VisAttributes* m_CeilFloorVisAtt = new G4VisAttributes( G4Colour(0x00,0x00,0xFF));
	m_CeilFloorVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	m_CeilFloorVisAtt->SetForceSolid(true);
	ceil_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);
	floor_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);
	//m_CeilFloorVisAtt = new G4VisAttributes(G4Colour(0./255.,255./255.,0./255.));
	//m_CeilFloorVisAtt->SetForceSolid(true);
	wallmx_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);
	wallpx_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);
	//m_CeilFloorVisAtt = new G4VisAttributes(G4Colour(139./255.,0./255.,0./255.));
	//m_CeilFloorVisAtt->SetForceSolid(true);
	wallmz_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);
	wallpz_LogicalVolume->SetVisAttributes(m_CeilFloorVisAtt);

	//if(true){
	if(false){
		ceil_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
		floor_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
		wallmx_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());	
		wallpx_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
		wallmz_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
		wallpz_LogicalVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
	}
	
	G4RotationMatrix rotCeil;
     rotCeil.rotateY(0.0*deg);
     const G4ThreeVector posCeil(-CFXOffset,ceilYOffset,-CFZOffset);
     G4Transform3D transformCeil(rotCeil,posCeil);     
	new G4PVPlacement(transformCeil, ceil_LogicalVolume, "ceil", pMotherLogical, 0, 0);
	
	G4RotationMatrix rotFloor;
     rotFloor.rotateY(0.0*deg);
     const G4ThreeVector posFloor(-CFXOffset,-floorYOffset,-CFZOffset);
     G4Transform3D transformFloor(rotFloor,posFloor);     
	new G4PVPlacement(transformFloor, floor_LogicalVolume, "floor", pMotherLogical, 0, 0);
	
	G4RotationMatrix rotWallmx;
     rotWallmx.rotateZ(90.0*deg);
     //const G4ThreeVector posWallmx(-wallXOffset,wallYOffset,0.0);
     const G4ThreeVector posWallmx(-mxWallX,wallYOffset,-xWallZOffset);
     G4Transform3D transformWallmx(rotWallmx,posWallmx);     
	//new G4PVPlacement(transformWallmx, wallmx_LogicalVolume, "wallmx", pMotherLogical, 0, 0);
	
	G4RotationMatrix rotWallpx;
     rotWallpx.rotateZ(90.0*deg);
     //const G4ThreeVector posWallpx(wallXOffset,wallYOffset,0.0);
     const G4ThreeVector posWallpx(pxWallX,wallYOffset,-xWallZOffset);
     G4Transform3D transformWallpx(rotWallpx,posWallpx);     
	//new G4PVPlacement(transformWallpx, wallpx_LogicalVolume, "wallpx", pMotherLogical, 0, 0);
	
	G4RotationMatrix rotWallmz;
     rotWallmz.rotateY(90.0*deg);
     rotWallmz.rotateX(90.0*deg);
     //const G4ThreeVector posWallmz(0.0,wallYOffset,-wallZOffset);
     //const G4ThreeVector posWallmz(0.0,wallYOffset,-mzWallZ);
     const G4ThreeVector posWallmz(-zWallXOffset,wallYOffset,-mzWallZ);
     G4Transform3D transformWallmz(rotWallmz,posWallmz);     
	//new G4PVPlacement(transformWallmz, wallmz_LogicalVolume, "wallmz", pMotherLogical, 0, 0);
	
	G4RotationMatrix rotWallpz;
     rotWallpz.rotateY(90.0*deg);
     rotWallpz.rotateX(90.0*deg);
     //const G4ThreeVector posWallpz(0.0,wallYOffset,wallZOffset);
     //const G4ThreeVector posWallpz(0.0,wallYOffset,pzWallZ);
     const G4ThreeVector posWallpz(-zWallXOffset,wallYOffset,pzWallZ);
     G4Transform3D transformWallpz(rotWallpz,posWallpz);     
	//new G4PVPlacement(transformWallpz, wallpz_LogicalVolume, "wallmz", pMotherLogical, 0, 0);

    	
}

void LeribssFloor::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{

    MakeCeilFloor(pRot, tlate, pName+"IBeam",  pMotherLogical, pCopyNo);
}

