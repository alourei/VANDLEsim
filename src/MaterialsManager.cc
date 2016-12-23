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
// $Id: MaterialsManager.cc 17.10.2016 A Fijalkowska $
//
/// \file MaterialsManager.cc
/// \brief Implementation of the MaterialsManager class
//
//
#include "globals.hh"
#include "MaterialsManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"    

MaterialsManager::MaterialsManager()
{
  CleanUp();
  useOptical = true;
  G4NistManager* man = G4NistManager::Instance();
  
  H  = man->FindOrBuildElement("H");
  C  = man->FindOrBuildElement("C");
  N  = man->FindOrBuildElement("N");
  O  = man->FindOrBuildElement("O");
  Na = man->FindOrBuildElement("Na");
  Al = man->FindOrBuildElement("Al");
  Si = man->FindOrBuildElement("Si");
  S = man->FindOrBuildElement("S");
  K = man->FindOrBuildElement("K");
  Fe = man->FindOrBuildElement("Fe");
  Ni = man->FindOrBuildElement("Ni");
  Ag = man->FindOrBuildElement("Ag");
  Sn = man->FindOrBuildElement("Sn");
  Sb = man->FindOrBuildElement("Sb");
  I = man->FindOrBuildElement("I");
  Cs = man->FindOrBuildElement("Cs");
  Pb = man->FindOrBuildElement("Pb");
  B = man->FindOrBuildElement("B"); 

  
}

void MaterialsManager::CleanUp()
{
  vaccum = 0L;
  air = 0L;
  BC408 = 0L;
  tin = 0L;
  borosilicate = 0L;
  bialkali = 0L;
  aluminium = 0L;
  NaI = 0L;
  
}


G4Material* MaterialsManager::GetVaccum()
{
  if(vaccum)
    return vaccum;
  vaccum = new G4Material("GalacticVacuum", z=1., atomicMass= 1.01*g/mole,
                           density = CLHEP::universe_mean_density, 
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  return vaccum;
}

G4Material* MaterialsManager::GetAir()
{
  if(air)
    return air;
  air = new G4Material("Air", density= 1.29*mg/cm3, numberElements=2);
  air->AddElement(N, 70*perCent);
  air->AddElement(O, 30*perCent);
  
  if(useOptical)
  {
     G4MaterialPropertiesTable* airLightProperties = new G4MaterialPropertiesTable();
     int scintEntriesNum = 12;
     G4double photonEnergies[] = {2.38*eV, 2.48*eV, 2.58*eV, 2.69*eV, 
                                  2.75*eV, 2.82*eV, 2.92*eV, 2.95*eV, 
                                  3.02*eV, 3.10*eV, 3.26*eV, 3.44*eV};
                                              
     G4double rIndex[] = {1.00029, 1.00029, 1.00029, 1.00029, 
                          1.00029, 1.00029, 1.00029, 1.00029, 
                          1.00029, 1.00029, 1.00029, 1.00029};
                                      
     G4double absLength[] = {100.0*m, 100.0*m, 100.0*m, 
                             100.0*m, 100.0*m, 100.0*m, 
                             100.0*m, 100.0*m, 100.0*m, 
                             100.0*m, 100.0*m, 100.0*m};
  
     airLightProperties->AddProperty("RINDEX", photonEnergies, rIndex, scintEntriesNum);
     airLightProperties->AddProperty("ABSLENGTH", photonEnergies, absLength, scintEntriesNum);

     air->SetMaterialPropertiesTable(airLightProperties);
  }    
  return air;
}

G4Material* MaterialsManager::GetBC408()
{
  if(BC408)
    return BC408;
  BC408 = new G4Material("BC408", density= 1.032*g/cm3, numberElements=2);
  BC408->AddElement(H, 138);//H:C ratio = 1.104
  BC408->AddElement(C, 125);  
  
  if(useOptical)
  {
     //add light properties (is it a good place?), should I read it from file?
     G4MaterialPropertiesTable* BC408LightProperties = new G4MaterialPropertiesTable();
  
     //general properties
     int scintEntries = 12;
     G4double photonEnergy[] = {2.38*eV, 2.48*eV, 2.58*eV, 2.69*eV,
                                2.75*eV, 2.82*eV, 2.92*eV, 2.95*eV, 
                                3.02*eV, 3.10*eV, 3.26*eV, 3.44*eV};
            
     G4double rIndex[] = { 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 
                           1.58, 1.58, 1.58, 1.58, 1.58, 1.58 };
             
     BC408LightProperties->AddProperty("RINDEX", photonEnergy, 
                                       rIndex, scintEntries);
  
     G4double absLength[] = { 3.8*m, 3.8*m, 3.8*m, 3.8*m, 3.8*m, 3.8*m, 
                              3.8*m, 3.8*m, 3.8*m, 3.8*m, 3.8*m, 3.8*m};
              
     BC408LightProperties->AddProperty("ABSLENGTH", photonEnergy, 
                                       absLength, scintEntries);
  
     G4double photonEmissionFraction = 0.2667; //PMT efficiency (TODO - discuss it)
     G4double pEF = photonEmissionFraction;
    // BC408LightProperties->AddConstProperty("SCINTILLATIONYIELD",  
     //                                       ( 10000.0 * photonEmissionFraction ) / MeV );
                                           //I guess i don't need it
                                            
     BC408LightProperties->AddConstProperty("RESOLUTIONSCALE",
                                             1.0 / sqrt(photonEmissionFraction));
    
      //fast component
     G4double scintilFast[] = { 0.02, 0.09, 0.20, 0.40, 0.55, 0.80, 
                                 1.00, 0.80, 0.50, 0.20, 0.08, 0.02 }; 
            
     G4double* scintilSlow = scintilFast;//for now
     BC408LightProperties->AddProperty("FASTCOMPONENT",photonEnergy, 
                                        scintilFast, scintEntries)->SetSpline(true);
     BC408LightProperties->AddConstProperty("FASTTIMECONSTANT", 2.1*ns);
    
     //slow component - do we have any?    
     BC408LightProperties->AddProperty("SLOWCOMPONENT",photonEnergy, 
                                       scintilSlow,scintEntries)->SetSpline(true);
                                    
     BC408LightProperties->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
     BC408LightProperties->AddConstProperty("YIELDRATIO",1.0); //TODO!!!!! find the value

     //light yield - data taken form V.V. Verbinski et al, Nucl. Instrum. & Meth. 65 (1968) 8-25
     int energyPoints = 26;
     G4double particleEnergy[] = { 0.001*MeV, 0.1*MeV, 0.13*MeV, 0.17*MeV, 0.2*MeV, 
                                   0.24*MeV, 0.3*MeV, 0.34*MeV, 0.4*MeV, 
                                   0.48*MeV, 0.6*MeV, 0.72*MeV, 0.84*MeV, 
                                   1.*MeV, 1.3*MeV, 1.7*MeV, 2.*MeV, 2.4*MeV, 
                                   3.*MeV, 3.4*MeV, 4.*MeV, 4.8*MeV, 6.*MeV, 
                                   7.2*MeV, 8.4*MeV, 10.*MeV };

     G4double electronYield[] = {0*pEF, 1000*pEF, 1300*pEF, 1700*pEF, 
		                         2000*pEF, 2400*pEF, 3000*pEF, 3400*pEF, 
                                 4000*pEF, 4800*pEF, 6000*pEF, 7200*pEF,
                                 8400*pEF,10000*pEF, 13000*pEF, 17000*pEF,
                                 20000*pEF, 24000*pEF, 30000*pEF, 34000*pEF, 
                                 40000*pEF, 48000*pEF, 60000*pEF, 72000*pEF, 
                                 84000*pEF, 100000*pEF };
             
     BC408LightProperties->AddProperty("ELECTRONSCINTILLATIONYIELD",
                                       particleEnergy, electronYield, 
                                       energyPoints)->SetSpline(true);

     G4double protonYield[] = { 0.6*pEF, 67.1*pEF, 88.6*pEF, 120.7*pEF, 
		                        146.5*pEF, 183.8*pEF, 246*pEF, 290*pEF, 
		                        365*pEF, 483*pEF, 678*pEF, 910*pEF, 
		                        1175*pEF, 562*pEF, 2385*pEF, 3660*pEF, 
		                        4725*pEF,6250*pEF, 8660*pEF, 10420*pEF, 
		                        13270*pEF,17180*pEF, 23100*pEF, 
		                        29500*pEF, 36200*pEF, 45500*pEF};

     BC408LightProperties->AddProperty("PROTONSCINTILLATIONYIELD",
                                       particleEnergy, protonYield, 
                                       energyPoints)->SetSpline(true);
  
     G4double ionYield[] = { 0.2*pEF, 10.4*pEF, 12.7*pEF, 15.7*pEF, 
		                     17.9*pEF, 20.8*pEF, 25.1*pEF, 27.9*pEF, 
                             31.9*pEF, 36.8*pEF, 43.6*pEF, 50.2*pEF, 
                             56.9*pEF, 65.7*pEF, 81.3*pEF, 101.6*pEF, 
                             116.5*pEF, 136.3*pEF, 166.15*pEF, 187.1*pEF, 
                             218.6*pEF, 260.54*pEF, 323.5*pEF, 387.5*pEF, 
                             451.54*pEF, 539.9*pEF };

     BC408LightProperties->AddProperty("IONSCINTILLATIONYIELD",
                                       particleEnergy, ionYield, 
                                       energyPoints)->SetSpline(true);
                                    
           
     BC408->SetMaterialPropertiesTable(BC408LightProperties);
       G4cout << "BC408 G4MaterialPropertiesTable" << G4endl;
     BC408LightProperties->DumpTable();
  }
  return BC408;
}


G4Material* MaterialsManager::GetTin()
{
  if(tin)
    return tin;
  tin = new G4Material("Tin", density= 7.31*g/cm3, numberElements=1);
  tin->AddElement(Sn, 1);
  return tin;
}

G4Material* MaterialsManager::GetBorosilicate()
{
  if(borosilicate)
    return borosilicate;
  borosilicate = new G4Material("Borosilicate", density= 2.23*g/cm3, 
                             numberElements=6); 
  borosilicate->AddElement(B, massFraction=0.040064);
  borosilicate->AddElement(O, massFraction=0.539562); 
  borosilicate->AddElement(Na, massFraction=0.028191);
  borosilicate->AddElement(Al, massFraction=0.011644);
  borosilicate->AddElement(Si, massFraction=0.377220);
  borosilicate->AddElement(K, massFraction=0.003321);
  
  
  if(useOptical)
  {  
     //optical parameters of Si02 taken from NEXT sim code
     const G4int num = 6;
     G4double photonEn[] = {2.3*eV, 2.757*eV, 3.102*eV, 3.312*eV, 3.545*eV, 4.136*eV };
     G4double rIndex[] = {1.54, 1.54, 1.54, 1.54, 1.54, 1.54 };
     G4double absLenght[] =  {127*cm, 125*cm, 123.5*cm, 122*cm, 121*cm, 120*cm};
     G4MaterialPropertiesTable* optProperty = new G4MaterialPropertiesTable();
     optProperty->AddProperty("RINDEX", photonEn, rIndex, num);
     optProperty->AddProperty("ABSLENGTH", photonEn, absLenght, num);
     borosilicate->SetMaterialPropertiesTable(optProperty);
  }
  return borosilicate;
}
 


  
  
G4Material* MaterialsManager::GetAluminium()
{
  if(aluminium)
    return aluminium;
  aluminium = new G4Material("Aluminium", density= 2.7*g/cm3, 
                              numberElements=1);
  aluminium->AddElement(Al, 1);
  
   //if(useOptical)
   if(false)
   {  
     //rIndex of Al taken from http://refractiveindex.info/?shelf=main&book=Al&page=Rakic
     const G4int scintEntries = 6;
     G4double photonEn[] = {2.38*eV, 2.58*eV, 2.75*eV, 
		                    2.92*eV, 3.02*eV, 3.26*eV};
                                
     G4double rIndex[] = { 0.86, 0.72, 0.62, 0.54, 0.50, 0.41 };
     
     //find it!!!
     G4double absLength[] = { 1*mm, 1*mm, 1*mm, 1*mm, 1*mm, 1*mm};
                              
     G4MaterialPropertiesTable* optProperty = new G4MaterialPropertiesTable();
     optProperty->AddProperty("RINDEX", photonEn, rIndex, scintEntries);
     optProperty->AddProperty("ABSLENGTH", photonEn, absLength, scintEntries);
     aluminium->SetMaterialPropertiesTable(optProperty);                                      
   }
  
  
  return aluminium;
}

G4Material* MaterialsManager::GetBialkali()
{

  if(bialkali)
    return bialkali;
  bialkali = new G4Material("Bialkali", density= 4.28*g/cm3, 
                              numberElements=3);
  bialkali->AddElement(K, massFraction=0.133);
  bialkali->AddElement(Cs, massFraction=0.452);
  bialkali->AddElement(Sb, massFraction=0.415); 
  return bialkali;
}


MaterialsManager *MaterialsManager::s_instance = 0;
