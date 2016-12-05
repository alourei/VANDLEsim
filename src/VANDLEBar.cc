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
// $Id: VANDLEBar.cc 17.10.2016 A Fijalkowska $
//
/// \file VANDLEBar.cc
/// \brief Implementation of the VANDLEBar class based on optical/LXe/src/LXeMainVolume.cc
//
//
#include "VANDLEBar.hh"

#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

VANDLEBar::VANDLEBar(G4RotationMatrix *pRot,
                             const G4ThreeVector &tlate,
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
                             G4int size)
  :G4PVPlacement(pRot,tlate,
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "housing",pMotherLogical,pMany,pCopyNo)
                 
                 
{
	//TODO - enum?
	if(size == 0)
		SetSmallBarSizes();
	if(size == 1)
		SetMediumBarSizes();
	if(size == 2)
		SetLargeBarSizes();
	SetBasicSizes();
	
	materialsManager = MaterialsManager::GetInstance();
	
	MakeModuleLogic(barWidth, barHeight, barLength, 
	                airThickness + reflectiveFoilThickness, PMTLength);
	
	MakeWrapping(barWidth, barHeight, barLength, airThickness,
                 reflectiveFoilThickness);
                 
    const G4ThreeVector zero( 0.0, 0.0, 0.0 );             
    new G4PVPlacement( 0, zero, wrappingLogic, "wrappingPhysical", moduleLogic, 0, 0 );
	
	MakeAirLayer(barWidth, barHeight, barLength, airThickness);
	new G4PVPlacement( 0, zero, airLayerLogic, "airLayerPhysical", wrappingLogic, 0, 0 );
	
	MakePlasticBar(barWidth, barHeight, barLength);
	new G4PVPlacement( 0, zero, barLogic, "bairPhysical", airLayerLogic, 0, 0 );

	
	MakePMTsGlass(PMTRadius, PMTGlassThickness);
	MakePMTPhotocathode(PMTRadius, PMTPhotocathodeThickness);
	PlacePMTPhotocathode(PMTGlassThickness, PMTPhotocathodeThickness);
	PlacePMTGlass(barLength, PMTGlassThickness);
	
	MakePMTShell(PMTShellThickness, PMTRadius, PMTLength);
    PlacePMTShell(barLength, PMTLength);	
	SetLogicalVolume(moduleLogic);
}
				
void VANDLEBar::SetBasicSizes()
{
	reflectiveFoilThickness = 0.01*mm;
	airThickness = 0.01*mm;
	PMTLength = 10.0*cm;
	PMTGlassThickness = 2.0*mm;		
	PMTShellThickness = 1.0*mm;
	PMTPhotocathodeThickness = 1.0*mm;
	
	//fD_mtl=0.0635*cm;
}

void VANDLEBar::SetSmallBarSizes()
{
	barWidth = 3.*cm;
	barHeight = 3.*cm;
	barLength = 60.*cm;
	PMTRadius = 1.5*cm;
}

void VANDLEBar::SetMediumBarSizes()
{
	barWidth = 6.*cm;
	barHeight = 3.*cm;
	barLength = 120.*cm;
	PMTRadius = 1.5*cm;
}

void VANDLEBar::SetLargeBarSizes()
{
	barWidth = 5.*cm;
	barHeight = 5.*cm;
	barLength = 200.*cm;
	PMTRadius = 2.5*cm;
}

void VANDLEBar::MakeModuleLogic(G4double barX, G4double barY, G4double barZ, 
                                G4double wrappThickness, G4double PMTLen)
{
	G4double halfModuleX = barX/2. + wrappThickness;
	G4double halfModuleY = barY/2. + wrappThickness;
	G4double halfModuleZ = barZ/2. + PMTLen;
	G4Box* moduleSolid = new G4Box("moduleSolid", halfModuleX, halfModuleY, halfModuleZ);
	
	G4Material* airMaterial = materialsManager->GetAir();
	moduleLogic = new G4LogicalVolume(moduleSolid, airMaterial,"moduleLogic");
	
	moduleLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}	

void VANDLEBar::MakePlasticBar(G4double barX, G4double barY, G4double barZ)
{
	G4Box* barSolid = new G4Box( "scintillatorSolid", barX/2., barY/2., barZ/2.);
	G4Material* barMaterial = materialsManager->GetBC408();
	barLogic = new G4LogicalVolume(barSolid, barMaterial, "barLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0xFF,0x00,0x00) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	//barLogic->SetVisAttributes(visAtt);
	barLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
} 


			

void VANDLEBar::MakeWrapping(G4double barX, G4double barY, 
                             G4double barZ, G4double airThick,
                             G4double wrappThick)
{
	G4double halfWrappX = barX/2. + airThick + wrappThick;
	G4double halfWrappY = barY/2. + airThick + wrappThick;
	G4double halfWrappZ = barZ/2. ;
	G4Box* wrappingSolid = new G4Box( "scintillatorSolid", halfWrappX, 
	                                   halfWrappY, halfWrappZ);
	G4Material* wrappingMaterial = materialsManager->GetAluminium();
	wrappingLogic = new G4LogicalVolume(wrappingSolid, 
	                                    wrappingMaterial, 
	                                    "wrappingLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.,1.,0.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	//visAtt->SetForceSolid(true);
	wrappingLogic->SetVisAttributes(visAtt);
}



void VANDLEBar::MakeAirLayer(G4double barX, G4double barY, 
                             G4double barZ, G4double airThick)
{
	G4double halfWrappX = barX/2. + airThick;
	G4double halfWrappY = barY/2. + airThick;
	G4double halfWrappZ = barZ/2. ;
	G4Box* airLayerSolid = new G4Box( "airLayerSolid", 
	                                   halfWrappX, 
	                                   halfWrappY, 
	                                   halfWrappZ);
	                                   
	G4Material* airLayerMaterial = materialsManager->GetAir();
	airLayerLogic = new G4LogicalVolume(airLayerSolid, 
	                                    airLayerMaterial, 
	                                    "airLayerLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0x00,0x00,0xFF) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	//airLayerLogic->SetVisAttributes(visAtt);
	airLayerLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}							 

    							 
void VANDLEBar::MakePMTsGlass(G4double radius, G4double thickness = 0.001*mm)
{
	G4double innerRadius = 0.0*mm;
	G4double outerRadius = radius;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4Tubs* glassSolid = new G4Tubs( "glassSolid", innerRadius, outerRadius, 
	                                 thickness, startPhi, deltaPhi);
	G4Material* PMTGlassMaterial = materialsManager->GetPMTGlass();
	glassLogic = new G4LogicalVolume(glassSolid, PMTGlassMaterial, "glassLogical");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.5,0.5,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	glassLogic->SetVisAttributes(visAtt);
	//glassLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}


void VANDLEBar::MakePMTPhotocathode(G4double radius, G4double thickness)
{
	G4double innerRadius = 0.0*mm;
	G4double outerRadius = radius;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4Tubs* photocathSolid = new G4Tubs( "photocathSolid", innerRadius, 
	                                       outerRadius, thickness, startPhi, deltaPhi);
	                                       
	G4Material* photocathMaterial = materialsManager->GetTin();
	photocathLogic = new G4LogicalVolume(photocathSolid, photocathMaterial,
	                                     "photocathLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.0,0.0,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	photocathLogic->SetVisAttributes(visAtt);
	//glassLogic->SetVisAttributes(G4VisAttributes::GetInvisible());	
	
	
}
    
void VANDLEBar::PlacePMTPhotocathode(G4double glassThickness, G4double photocathThickness)
{
	const G4double zPosition = (glassThickness - photocathThickness) * 0.5;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, zPosition);
	new G4PVPlacement( 0, placement, photocathLogic, 
	                   "PMTPhotocathPhysical", glassLogic, 0, 0 );
}
   
    
void VANDLEBar::PlacePMTGlass(G4double barZ, G4double thickness)
{
	const G4double PMTGlassPosition = (barZ - thickness) * 0.5;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, PMTGlassPosition);
	new G4PVPlacement( 0, placement, glassLogic, 
	                   "PMTGlassPhysical", barLogic, 0, 0 );
	
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX( 180 * degree );
	placement = G4ThreeVector(0.0, 0.0, -PMTGlassPosition);
	new G4PVPlacement( rot, placement, glassLogic, 
	                   "PMTGlassPhysical", barLogic, 0, 1 );

}



void VANDLEBar::MakePMTShell(G4double thickness, G4double outerRad, G4double length)
{
	G4double innerRadius = outerRad - thickness;
	G4double outerRadius = outerRad;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4double halfLength = length/2.;
	

	G4Tubs* PMTSolid = new G4Tubs("PMTSolid", innerRadius, outerRadius, 
	                               halfLength, startPhi, deltaPhi);
	G4Material* PMTMaterial = materialsManager->GetTin();
	PMTLogic = new G4LogicalVolume( PMTSolid, PMTMaterial, "PMTLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.5,0.5,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	PMTLogic->SetVisAttributes(visAtt);
}


void VANDLEBar::PlacePMTShell(G4double barZ, G4double length)
{
	const G4double PMTPosition = (barZ + length) * 0.5;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, PMTPosition);
	new G4PVPlacement( 0, placement, PMTLogic, 
	                   "PMTShellPhysical", moduleLogic, 0, 0 );
	
	placement = G4ThreeVector(0.0, 0.0, -PMTPosition);
	new G4PVPlacement( 0, placement, PMTLogic, 
	                   "PMTShellPhysical", moduleLogic, 0, 1 );

}
   
/*
void VANDLEBar::SurfaceProperties()
{
	

  G4double photonEnergy[] = {2.38*eV, 2.48*eV, 2.58*eV, 2.69*eV,
                             2.75*eV, 2.82*eV, 2.92*eV, 2.95*eV, 
                             3.02*eV, 3.10*eV, 3.26*eV, 3.44*eV};
   //TODO - get this array from materials properties                          
                             
  const G4int scintEntries = sizeof(photonEnergy)/sizeof(G4double);

  //**Scintillator housing properties
  G4double reflectivity[] = {fRefl, fRefl};
  assert(sizeof(reflectivity) == sizeof(ephoton));
  G4double efficiency[] = {0.0, 0.0};
  assert(sizeof(efficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  scintHsngPT->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  G4OpticalSurface* OpScintHousingSurface =
    new G4OpticalSurface("HousingSurface",unified,polished,dielectric_metal);
  OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);
 
 
  //**Photocathode surface properties
  G4double photocath_EFF[]={1.,1.}; //Enables 'detection' of photons
  assert(sizeof(photocath_EFF) == sizeof(ephoton));
  G4double photocath_ReR[]={1.92,1.92};
  assert(sizeof(photocath_ReR) == sizeof(ephoton));
  G4double photocath_ImR[]={1.69,1.69};
  assert(sizeof(photocath_ImR) == sizeof(ephoton));
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,
                         dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

  //**Create logical skin surfaces
  new G4LogicalSkinSurface("photocath_surf",fHousing_log,
                           OpScintHousingSurface);
  new G4LogicalSkinSurface("sphere_surface",fSphere_log,OpSphereSurface);
  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);
}
*/

/*

// START ADDING Optical BOUNNDRIES
	// START REFLECTING FOIL SURFACE
		const G4int numEntriesFoil = 12;
// 100% REFLECTIVE
		G4double rFoil[numEntriesFoil] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};// For dielectric_metalSurfaces ONLY
		G4double efficiencyFoil[numEntriesFoil] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};// For dielectric_metalSurfaces ONLY

		G4OpticalSurface* reflectiveFoilOpticalSurface = new G4OpticalSurface("ReflectiveFoilSurface");
	//	new G4LogicalSkinSurface( "WallPerfectReflectorSkin", m_CellWall_logVol, plasticWallOpticalSurface );

		G4MaterialPropertiesTable* reflectiveFoilBoundry_mt = new G4MaterialPropertiesTable();
		reflectiveFoilBoundry_mt->AddProperty( "REFLECTIVITY", photonEnergies, rFoil, numEntriesFoil );
		reflectiveFoilBoundry_mt->AddProperty( "EFFICIENCY", photonEnergies, efficiencyFoil, numEntriesFoil );

		reflectiveFoilOpticalSurface->SetType( dielectric_dielectric );
		reflectiveFoilOpticalSurface->SetFinish( polished );
		reflectiveFoilOpticalSurface->SetModel( unified );
		reflectiveFoilOpticalSurface->SetMaterialPropertiesTable( reflectiveFoilBoundry_mt );

	for( int i = 0; i < numberPMTSizes; ++i )
	{
		new G4LogicalSkinSurface( "ReflectiveFoilSkin", m_ModuleTotal_LogicalVolumes[i], reflectiveFoilOpticalSurface );
	}

	// PMT SURFACE
		const G4int numEntriesPMT = 12;
	// These may or may not be well defined numbers...
		G4double reflectivityPMT[numEntriesPMT] = { 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};// 80% absorbed and then...
//		G4double efficiencyPMT[numEntriesPMT] = { 0.25, 0.25 };// 25% of those that are absorbed are detected
		G4double efficiencyPMT[numEntriesPMT] = { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};// 100% of those that are absorbed are detected

		G4MaterialPropertiesTable* PMTBoundry_mt = new G4MaterialPropertiesTable();
		PMTBoundry_mt->AddProperty( "REFLECTIVITY", photonEnergies, reflectivityPMT, numEntriesPMT );
		PMTBoundry_mt->AddProperty( "EFFICIENCY", photonEnergies, efficiencyPMT, numEntriesPMT );

		G4OpticalSurface* PMTOpticalSurface = new G4OpticalSurface("PMTSurface");

		PMTOpticalSurface->SetType( dielectric_metal );
		PMTOpticalSurface->SetFinish( polished );
		PMTOpticalSurface->SetModel( unified );
		PMTOpticalSurface->SetMaterialPropertiesTable( PMTBoundry_mt );
		
//		PMTOpticalSurface->DumpInfo();

	//	new G4LogicalSkinSurface( "WallPerfectReflectorSkin", m_CellWall_logVol, plasticWallOpticalSurface );
	for( unsigned int i = 0; i < m_PMT_LogicalVolumes.size(); ++i )
	{
		new G4LogicalSkinSurface( "PMTSkin", m_PMT_LogicalVolumes[i], PMTOpticalSurface );
	}
	// END PMT SURFACE
// Roughness of plasticwall surface, if not used wall surfaces are perfect.
	if( m_UseRoughSurfaces && ! m_PerfectPlasticWalls )
	{

////////// PLASTIC WALL AND SCINTILLATOR SURFACE///////////////////

		const G4int numEntriesPlasticWallScintillator = 12;
		
		G4double angleStandardDev = 0.5;// standard deviation, in radians?, of surface normals. .5 ~28.5 degrees
		G4double nPlasticWall[numEntriesPlasticWallScintillator] = { 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58};
		G4double specularLobePlasticWall[numEntriesPlasticWallScintillator] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		G4double specularSpikePlasticWall[numEntriesPlasticWallScintillator] = { 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99};
		G4double backscatterPlasticWall[numEntriesPlasticWallScintillator] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	//	G4double diffuseLobePlasticWall[numEntriesPlasticWallScintillator] = { 0.05, 0.05 };// This is set by subtracting the others from 1.0

		G4MaterialPropertiesTable* plasticWallScintillatorBoundry_mt = new G4MaterialPropertiesTable();
		plasticWallScintillatorBoundry_mt->AddProperty( "RINDEX", photonEnergies, nPlasticWall, numEntriesPlasticWallScintillator );
		plasticWallScintillatorBoundry_mt->AddProperty( "SPECULARLOBECONSTANT", photonEnergies, specularLobePlasticWall, numEntriesPlasticWallScintillator );
		plasticWallScintillatorBoundry_mt->AddProperty( "SPECULARSPIKECONSTANT", photonEnergies, specularSpikePlasticWall, numEntriesPlasticWallScintillator );
		plasticWallScintillatorBoundry_mt->AddProperty( "BACKSCATTERCONSTANT", photonEnergies, backscatterPlasticWall, numEntriesPlasticWallScintillator );

		G4OpticalSurface* plasticWallScintillatorOpticalSurface = new G4OpticalSurface("PlasticWallSurface");
		plasticWallScintillatorOpticalSurface->SetType( dielectric_dielectric );
		plasticWallScintillatorOpticalSurface->SetFinish( polished );
		plasticWallScintillatorOpticalSurface->SetModel( unified );
		plasticWallScintillatorOpticalSurface->SetSigmaAlpha( angleStandardDev );
		
		plasticWallScintillatorOpticalSurface->SetMaterialPropertiesTable( plasticWallScintillatorBoundry_mt );
		
//		plasticWallScintillatorOpticalSurface->DumpInfo();

		for( int i = 0; i < numberPMTSizes; ++i )
		{
			new G4LogicalSkinSurface( "PlasticWallSkin", m_BC408Cell_LogicalVolumes[i], plasticWallScintillatorOpticalSurface );
		}

//////// END PLASTIC WALL AND SCINTILLATOR SURFACE///////////
	}

// END Optical BOUNNDRIES

	}
}



*/















/*

void LXeMainVolume::SurfaceProperties(){
  G4double ephoton[] = {7.0*eV, 7.14*eV};
  const G4int num = sizeof(ephoton)/sizeof(G4double);

  //**Scintillator housing properties
  G4double reflectivity[] = {fRefl, fRefl};
  assert(sizeof(reflectivity) == sizeof(ephoton));
  G4double efficiency[] = {0.0, 0.0};
  assert(sizeof(efficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  scintHsngPT->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  G4OpticalSurface* OpScintHousingSurface =
    new G4OpticalSurface("HousingSurface",unified,polished,dielectric_metal);
  OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);
 
  //**Sphere surface properties
  G4double sphereReflectivity[] = {1.0, 1.0};
  assert(sizeof(sphereReflectivity) == sizeof(ephoton));
  G4double sphereEfficiency[] = {0.0, 0.0};
  assert(sizeof(sphereEfficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* spherePT = new G4MaterialPropertiesTable();
  spherePT->AddProperty("REFLECTIVITY", ephoton, sphereReflectivity, num);
  spherePT->AddProperty("EFFICIENCY", ephoton, sphereEfficiency, num);
  G4OpticalSurface* OpSphereSurface =
    new G4OpticalSurface("SphereSurface",unified,polished,dielectric_metal);
  OpSphereSurface->SetMaterialPropertiesTable(spherePT);
 
  //**Photocathode surface properties
  G4double photocath_EFF[]={1.,1.}; //Enables 'detection' of photons
  assert(sizeof(photocath_EFF) == sizeof(ephoton));
  G4double photocath_ReR[]={1.92,1.92};
  assert(sizeof(photocath_ReR) == sizeof(ephoton));
  G4double photocath_ImR[]={1.69,1.69};
  assert(sizeof(photocath_ImR) == sizeof(ephoton));
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,
                         dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

  //**Create logical skin surfaces
  new G4LogicalSkinSurface("photocath_surf",fHousing_log,
                           OpScintHousingSurface);
  new G4LogicalSkinSurface("sphere_surface",fSphere_log,OpSphereSurface);
  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);
}
*/
