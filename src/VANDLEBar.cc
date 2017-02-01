// $Id: VANDLEBar.cc 17.10.2016 A Fijalkowska $
//
/// \file VANDLEBar.cc
/// \brief Implementation of the VANDLEBar class based on 
///  optical/LXe/src/LXeMainVolume.cc
//
//
#include "VANDLEBar.hh"

#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

VANDLEBar::VANDLEBar(G4int size)                
{
	pmtSD = 0;
	scintillatorSD = 0;
	//TODO - enum?
	if(size == 0)
		SetSmallBarSizes();
	if(size == 1)
		SetMediumBarSizes();
	if(size == 2)
		SetLargeBarSizes();
	SetBasicSizes();
	
	materialsManager = MaterialsManager::GetInstance();
	
	MakeModuleLogic(barWidth, 
	                barHeight, 
	                barLength, 
	                airThickness + reflectiveFoilThickness, 
	                PMTRadius,
	                PMTGlassThickness, 
	                PMTLength);
	const G4ThreeVector zero( 0.0, 0.0, 0.0 ); 
	MakeWrapping(barWidth, barHeight, barLength, reflectiveFoilThickness);
	MakeWrapping(barWidth, 
	             barHeight,
	             barLength, 
	             airThickness, 
	             reflectiveFoilThickness);
    new G4PVPlacement( 0, 
                       zero, 
                       wrappingLogic, 
                       "wrappingPhysical", 
                       moduleLogic, 
                       0, 
                       0 );                         
	
	
	MakePlasticBar(barWidth, barHeight, barLength);
    //new G4PVPlacement( 0, zero, barLogic, "barPhysical", wrappingLogic, 0, 0 );             
    new G4PVPlacement( 0, zero, barLogic, "barPhysical", moduleLogic, 0, 0 );                 

	
	//MakeAirLayer(barWidth, barHeight, barLength, airThickness);
	/*new G4PVPlacement( 0, 
	                     zero, 
	                     airLayerLogic, 
	                     "airLayerPhysical", 
	                     wrappingLogic, 
	                     0, 
	                     0 );*/
	
	
	
	MakePMTsGlass(PMTRadius, PMTGlassThickness, barWidth, barHeight);
	MakePMTPhotocathode(PMTActiveRadius, 
	                    PMTPhotocathodeThickness, 
	                    barWidth, 
	                    barHeight);
	PlacePMTPhotocathode(PMTGlassThickness, PMTPhotocathodeThickness);
	PlacePMTGlass(barLength, PMTGlassThickness);
    		
	MakePMTShell(PMTShellThickness, PMTRadius, PMTLength);
    PlacePMTShell(barLength+reflectiveFoilThickness, 
                  PMTLength, 
                  reflectiveFoilThickness);
    SetOpticalSurfacesProperties();	
}

VANDLEBar::~VANDLEBar() {}

				
void VANDLEBar::SetBasicSizes()
{
	reflectiveFoilThickness = 0.1*mm;
	airThickness = 0.00*mm;
	//airThickness = 0.01*mm;
	PMTLength = 10.0*cm;
	PMTGlassThickness = 4.0*mm;	//1,5	
	PMTShellThickness = 1.0*mm;
	PMTPhotocathodeThickness = 1.0*mm;
	                             
}

void VANDLEBar::SetSmallBarSizes()
{
	barWidth = 3.*cm;
	barHeight = 3.*cm;
	barLength = 60.*cm;
	PMTRadius = 1.8*cm;
	PMTActiveRadius = 1.7*cm;
}

void VANDLEBar::SetMediumBarSizes()
{
	barWidth = 6.*cm;
	barHeight = 3.*cm;
	barLength = 120.*cm;
	PMTRadius = 2.5*cm;
	PMTActiveRadius = 2.3*cm;
}

void VANDLEBar::SetLargeBarSizes()
{
	barWidth = 5.*cm;
	barHeight = 5.*cm;
	barLength = 200.*cm;
	PMTRadius = 2.5*cm;
	PMTActiveRadius = 2.3*cm;
}

void VANDLEBar::MakeModuleLogic(G4double barX, 
                                G4double barY, 
                                G4double barZ, 
                                G4double wrappThickness, 
                                G4double PMTRadiusVal,
                                G4double PMTGlassThicknessVal, 
                                G4double PMTLen)
{
	
	G4double halfModuleX = barX/2. + wrappThickness;
	if(halfModuleX < PMTRadiusVal)
	    halfModuleX = PMTRadiusVal;
	
	G4double halfModuleY = barY/2. + wrappThickness;
	if(halfModuleY < PMTRadiusVal)
	    halfModuleY = PMTRadiusVal;
	G4double halfModuleZ = barZ/2. + PMTLen + PMTGlassThicknessVal;
	G4Box* moduleSolid = new G4Box("moduleSolid", 
	                               halfModuleX, 
	                               halfModuleY, 
	                               halfModuleZ);
	
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
	barLogic->SetVisAttributes(visAtt);
	//barLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
} 


			

void VANDLEBar::MakeWrapping(G4double barX, 
                             G4double barY, 
                             G4double barZ, 
                             G4double airThick,
                             G4double wrappThick)
{
	G4double halfExtWrappX = barX/2. + airThick + wrappThick;
	G4double halfExtWrappY = barY/2. + airThick + wrappThick;
	G4double halfExtWrappZ = barZ/2. + wrappThick;
	G4Box* wrappingExteriorSolid = new G4Box( "wrappingExtSolid", 
	                                         halfExtWrappX, 
	                                         halfExtWrappY, 
	                                         halfExtWrappZ);	
	G4double halfIntWrappX = barX/2. + airThick;
	G4double halfIntWrappY = barY/2. + airThick;
	G4double halfIntWrappZ = barZ/2. ;
	G4Box* wrappingInteriorSolid = new G4Box( "wrappingIntSolid", 
	                                         halfIntWrappX, 
	                                         halfIntWrappY, 
	                                         halfIntWrappZ);
	const G4ThreeVector position(0., 0., 0.);                                        
	G4SubtractionSolid* wrappingSolid = new G4SubtractionSolid("wrappingSolid",
	                                    wrappingExteriorSolid,
	                                    wrappingInteriorSolid);
	                                    
	G4Material* wrappingMaterial = materialsManager->GetAluminium();
	wrappingLogic = new G4LogicalVolume(wrappingSolid, 
	                                    wrappingMaterial, 
	                                    "wrappingLogic");
	
	//G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.,1.,0.) );
	//visAtt->SetLineWidth(0.1);
	//visAtt->SetForceAuxEdgeVisible(true);
	//visAtt->SetForceSolid(true);
	//wrappingLogic->SetVisAttributes(visAtt);
	wrappingLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}

void VANDLEBar::MakeWrapping(G4double barX, 
                             G4double barY, 
                             G4double barZ, 
                             G4double wrappThick)
{
	G4double halfExtWrappX = barX/2. + wrappThick;
	G4double halfExtWrappY = barY/2. + wrappThick;
	G4double halfExtWrappZ = barZ/2. + wrappThick;
	G4Box* wrappingSolid = new G4Box( "wrappingExtSolid", 
	                                         halfExtWrappX, 
	                                         halfExtWrappY, 
	                                         halfExtWrappZ);	
	const G4ThreeVector position(0., 0., 0.);                                        	                                    
	G4Material* wrappingMaterial = materialsManager->GetAluminium();
	wrappingLogic = new G4LogicalVolume(wrappingSolid, 
	                                    wrappingMaterial, 
	                                    "wrappingLogic");
	
	//G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.,1.,0.) );
	//visAtt->SetLineWidth(0.1);
	//visAtt->SetForceAuxEdgeVisible(true);
	//visAtt->SetForceSolid(true);
	//wrappingLogic->SetVisAttributes(visAtt);
	wrappingLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}


void VANDLEBar::MakeAirLayer(G4double barX, 
                             G4double barY, 
                             G4double barZ, 
                             G4double airThick)
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

    							 
void VANDLEBar::MakePMTsGlass(G4double radius, 
                              G4double thickness, 
                              G4double barX, 
                              G4double barY)
{
	G4double innerRadius = 0.0*mm;
	G4double outerRadius = radius;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4Tubs* tubeSolid = new G4Tubs( "tubeSolid", innerRadius, outerRadius, 
	                                 thickness, startPhi, deltaPhi);
	G4Box* boxSolid = new G4Box( "boxSolid", barX/2., barY/2., thickness/2.);
	
	G4IntersectionSolid* glassSolid = new G4IntersectionSolid("glassSolid",
	                                                          boxSolid,
	                                                          tubeSolid);                       
	G4Material* PMTGlassMaterial = materialsManager->GetBorosilicate();
	glassLogic = new G4LogicalVolume(glassSolid, 
	                                 PMTGlassMaterial, 
	                                 "glassLogical");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.5,0.5,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	glassLogic->SetVisAttributes(visAtt);
	//glassLogic->SetVisAttributes(G4VisAttributes::GetInvisible());
}


void VANDLEBar::MakePMTPhotocathode(G4double radius, 
                                    G4double thickness, 
                                    G4double barX, 
                                    G4double barY)
{
	G4double innerRadius = 0.0*mm;
	G4double outerRadius = radius;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4Tubs* tubeSolid = new G4Tubs( "tubeSolid", 
	                                 innerRadius, 
	                                 outerRadius, 
	                                 thickness, 
	                                 startPhi, 
	                                 deltaPhi);
	G4Box* boxSolid = new G4Box( "boxSolid", barX/2., barY/2., thickness/2.);
	
	G4IntersectionSolid* photocathSolid = new G4IntersectionSolid("photocathSolid",
	                                                          boxSolid,
	                                                          tubeSolid); 
	                                       
	G4Material* photocathMaterial = materialsManager->GetBialkali();
	photocathLogic = new G4LogicalVolume(photocathSolid, 
	                                     photocathMaterial,
	                                     "photocathLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.0,0.0,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	photocathLogic->SetVisAttributes(visAtt);
	//glassLogic->SetVisAttributes(G4VisAttributes::GetInvisible());	
	
	
}
    
void VANDLEBar::PlacePMTPhotocathode(G4double glassThickness, 
                                     G4double photocathThickness)
{
	const G4double zPosition = (glassThickness - photocathThickness) * 0.5;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, zPosition);
	new G4PVPlacement( 0, 
	                   placement, 
	                   photocathLogic, 
	                   "PMTPhotocathPhysical", 
	                   glassLogic, 
	                   0, 
	                   0 );
}
   
    
void VANDLEBar::PlacePMTGlass(G4double barZ, G4double thickness)
{
	const G4double PMTGlassPosition = (barZ - thickness) * 0.5;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, PMTGlassPosition);
	new G4PVPlacement( 0, 
	                   placement, 
	                   glassLogic, 
	                   "PMTGlassPhysical", 
	                   barLogic, 
	                   0, 
	                   0 );
	
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX( 180 * degree );
	placement = G4ThreeVector(0.0, 0.0, -PMTGlassPosition);
	new G4PVPlacement( rot, 
	                   placement, 
	                   glassLogic, 
	                   "PMTGlassPhysical", 
	                   barLogic, 
	                   0, 
	                   1 );

}



void VANDLEBar::MakePMTShell(G4double thickness, 
                             G4double outerRad, 
                             G4double length)
{
	G4double innerRadius = outerRad - thickness;
	G4double outerRadius = outerRad;
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4double halfLength = length/2.;
	

	G4Tubs* PMTSolid = new G4Tubs("PMTSolid", 
	                               innerRadius, 
	                               outerRadius, 
	                               halfLength, 
	                               startPhi, 
	                               deltaPhi);
	G4Material* PMTMaterial = materialsManager->GetBorosilicate();
	PMTLogic = new G4LogicalVolume( PMTSolid, PMTMaterial, "PMTLogic");
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.5,0.5,1.) );
	visAtt->SetLineWidth(0.1);
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	PMTLogic->SetVisAttributes(visAtt);
}


void VANDLEBar::PlacePMTShell(G4double barZ, 
                              G4double length, 
                              G4double foilThickness)
{
	const G4double PMTPosition = (barZ + length) * 0.5 + foilThickness;
	
	G4ThreeVector placement = G4ThreeVector(0.0, 0.0, PMTPosition);
	new G4PVPlacement( 0, 
	                   placement, 
	                   PMTLogic, 
	                   "PMTShellPhysical", 
	                   moduleLogic, 
	                   0, 
	                   0 );
	
	placement = G4ThreeVector(0.0, 0.0, -PMTPosition);
	new G4PVPlacement( 0, 
	                   placement, 
	                   PMTLogic, 
	                   "PMTShellPhysical", 
	                   moduleLogic, 
	                   0, 
	                   1 );

}


void VANDLEBar::SetOpticalSurfacesProperties()
{
	/*TODO make optical photons verification
	* move photons energies to class fields
	* move creations of each optical surface to separate method
	*/ 
	G4double photonsEn [] = {2.38*eV, 2.48*eV, 2.58*eV, 2.69*eV,
                             2.75*eV, 2.82*eV, 2.92*eV, 2.95*eV, 
                             3.02*eV, 3.10*eV, 3.26*eV, 3.44*eV};
    int scintEntries = 12;
                             
    //**Photocathode surface properties                         
    G4double photocathEfficiency[]={1., 1., 1., 1., 1., 1.,
		                            1., 1., 1., 1., 1., 1.};
    assert(sizeof(photocathEfficiency) == sizeof(photonsEn));    
    G4double photocathReflectivity[]={0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
		                              0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
    assert(sizeof(photocathReflectivity) == sizeof(photonsEn));
       
    G4MaterialPropertiesTable* photocathMatTable = new G4MaterialPropertiesTable();
    photocathMatTable->AddProperty("EFFICIENCY", 
                                    photonsEn,
                                    photocathEfficiency, 
                                    scintEntries);
    photocathMatTable->AddProperty("REFLECTIVITY", 
                                    photonsEn,
                                    photocathReflectivity, 
                                    scintEntries);
    
    G4OpticalSurface* photocathOpSurf = new G4OpticalSurface("photocathOpSurf");
    photocathOpSurf->SetModel( unified );
    photocathOpSurf->SetFinish( polished );
    photocathOpSurf->SetType( dielectric_metal );		   		
    photocathOpSurf->SetMaterialPropertiesTable(photocathMatTable);
    new G4LogicalSkinSurface( "photocathOptSkin", photocathLogic, photocathOpSurf);


     
     //** reflective foil properties		                            
	G4double foilEfficiency[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
		                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    G4double foilReflectivity[] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
		                            1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
		                            
    // rIndex of Al taken from 
    // http://refractiveindex.info/?shelf=main&book=Al&page=Rakic                            
    // G4double foilRIndex[] = { 0.86, 0.79, 0.72, 0.65, 0.62, 0.58, 
	//	                      0.54, 0.52, 0.50, 0.47, 0.41, 0.37 };
		                      
	G4double plasticWallSpecularLobe[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		                                   0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double plasticWallSecularSpike[] = { 0.99, 0.99, 0.99, 0.99,
		                                   0.99, 0.99, 0.99, 0.99, 
		                                   0.99, 0.99, 0.99, 0.99};
		                                   
	G4double plasticWallBackscatter[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
		                                  0.0, 0.0, 0.0, 0.0, 0.0, 0.0};	                          
    G4MaterialPropertiesTable* reflectFoilMatTable = new G4MaterialPropertiesTable();
	reflectFoilMatTable->AddProperty( "EFFICIENCY", 
	                                   photonsEn, 
	                                   foilEfficiency, 
	                                   scintEntries );
	reflectFoilMatTable->AddProperty( "REFLECTIVITY", 
	                                   photonsEn, 
	                                   foilReflectivity, 
	                                   scintEntries );
	reflectFoilMatTable->AddProperty( "SPECULARLOBECONSTANT", 
	                                   photonsEn, 
	                                   plasticWallSpecularLobe, 
	                                   scintEntries );
	reflectFoilMatTable->AddProperty( "SPECULARSPIKECONSTANT", 
	                                   photonsEn, 
	                                   plasticWallSecularSpike, 
	                                   scintEntries );
	reflectFoilMatTable->AddProperty( "BACKSCATTERCONSTANT", 
	                                   photonsEn, 
	                                   plasticWallBackscatter, 
	                                   scintEntries );	
	G4OpticalSurface* reflectFoilOpSurf = new G4OpticalSurface("reflectFoilOpSurf");
	reflectFoilOpSurf->SetType( dielectric_metal );
	reflectFoilOpSurf->SetFinish( polished );
	reflectFoilOpSurf->SetModel( unified );
	reflectFoilOpSurf->SetMaterialPropertiesTable(reflectFoilMatTable);
	new G4LogicalSkinSurface( "reflectFoilSkin", wrappingLogic, reflectFoilOpSurf);


    //** plastic wall properties
/*    //this fragment of code was taken from the Charlie's version
	G4double angleStandardDev = 0.5;
	// standard deviation, in radians?, of surface normals. .5 ~28.5 degrees
	G4double plasticWallrIndex[] = { 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 
		                             1.58, 1.58, 1.58, 1.58, 1.58, 1.58};
	G4double plasticWallSpecularLobe[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		                                   0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double plasticWallSecularSpike[] = { 0.99, 0.99, 0.99, 0.99,
		                                   0.99, 0.99, 0.99, 0.99, 
		                                   0.99, 0.99, 0.99, 0.99};
		                                   
	G4double plasticWallBackscatter[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
		                                  0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //the rest is diffuse lobe
	G4MaterialPropertiesTable* plasticWallMatTable = new G4MaterialPropertiesTable();
	//plasticWallMatTable->AddProperty( "RINDEX", photonsEn, 
	//                                  plasticWallrIndex, scintEntries );
	plasticWallMatTable->AddProperty( "SPECULARLOBECONSTANT", photonsEn, 
	                                  plasticWallSpecularLobe, scintEntries );
	plasticWallMatTable->AddProperty( "SPECULARSPIKECONSTANT", photonsEn, 
	                                  plasticWallSecularSpike, scintEntries );
	plasticWallMatTable->AddProperty( "BACKSCATTERCONSTANT", photonsEn, 
	                                   plasticWallBackscatter, scintEntries );

	G4OpticalSurface* plasticWallOpSurf = new G4OpticalSurface("plasticWallOpSurf");
	plasticWallOpSurf->SetType( dielectric_metal );
	//plasticWallOpSurf->SetType( dielectric_dielectric );
	plasticWallOpSurf->SetFinish( polished );
	plasticWallOpSurf->SetModel( unified );
	plasticWallOpSurf->SetSigmaAlpha( angleStandardDev );
	plasticWallOpSurf->SetMaterialPropertiesTable(plasticWallMatTable);
		
	new G4LogicalSkinSurface("plasticWallSkin", barLogic, plasticWallOpSurf);
 */  
}


void VANDLEBar::ConstructSDandField()
{
  // PMT SD
  if (!pmtSD) 
  {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /VANDLEDet/pmtSD" << G4endl;
    pmtSD = new PMTSD("/VANDLEDet/pmtSD");
    pmtSD->SetModuleDeph(3); //TODO - get deph from VANDLE bar
    pmtSD->SetPMTDeph(1);
  }

  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events   
   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   SDman->AddNewDetector(pmtSD);
   photocathLogic->SetSensitiveDetector(pmtSD);
    
    
    
     
  // Scintillator SD
   if (!scintillatorSD) 
   {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /VANDLEDet/scintillatorSD" << G4endl;
    scintillatorSD = new ScintillatorSD("/VANDLEDet/scintillatorSD");
    scintillatorSD->SetModuleDeph(1);
  }
  SDman->AddNewDetector(scintillatorSD);
  barLogic->SetSensitiveDetector(scintillatorSD);	
	
}

void VANDLEBar::Place(G4RotationMatrix *pRot, 
                      const G4ThreeVector &tlate, 
                      const G4String &pName, 
                      G4LogicalVolume *pMotherLogical, 
                      G4int pCopyNo)
{
   new G4PVPlacement(pRot, 
                     tlate, 
                     moduleLogic, 
                     pName, 
	                 pMotherLogical,
	                 0,
	                 pCopyNo);					  
						  
						  
}
