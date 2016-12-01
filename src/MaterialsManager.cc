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
    

MaterialsManager::MaterialsManager()
{
  CleanUp();
  
  H  = new G4Element("Hydrogen", "H", z=1., atomicMass =1.008 *g/mole);
  C  = new G4Element("Carbon", "C",  z=6.,  atomicMass = 12.01*g/mole);
  N  = new G4Element("Nitrogen", "N",  z=7., atomicMass = 14.01*g/mole);
  O  = new G4Element("Oxygen", "O",  z=8.,  atomicMass = 16.00*g/mole);
  Na = new G4Element("Sodium",  "Na", z=11., atomicMass = 22.99*g/mole);
  Al = new G4Element("Aluminium","Al", z=13., atomicMass = 26.98*g/mole);
  Si = new G4Element("Silicon"  ,"Si", z=14., atomicMass = 28.09*g/mole);
  S = new G4Element("Sulfur", "S", z = 16., atomicMass = 32.06*g/mole);
  Fe = new G4Element("Iron", "Fe", z=26., atomicMass = 55.85*g/mole);
  Ni = new G4Element("Nickel", "Ni", z=28., atomicMass = 58.69*g/mole);
  Ag = new G4Element("Silver", "Ag", z = 47., atomicMass = 107.868*g/mole); 
  Sn  = new G4Element("Tin", "Sn",  z=50., atomicMass = 118.71*g/mole);
  I  = new G4Element("Iodine", "I",  z=53., atomicMass = 126.9*g/mole);
  Pb = new G4Element("Lead", "Pb",  z=82., atomicMass = 207.2*g/mole);
  
  
}

void MaterialsManager::CleanUp()
{
  vaccum = 0L;
  air = 0L;
  BC408 = 0L;
  tin = 0L;
  PMTGlass = 0L;
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
      
  return air;
}

G4Material* MaterialsManager::GetBC408()
{
  if(BC408)
    return BC408;
  BC408 = new G4Material("BC408", density= 1.032*g/cm3, numberElements=2);
  BC408->AddElement(H, 138);//H:C ratio = 1.104
  BC408->AddElement(C, 125);  
  
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
  BC408LightProperties->AddConstProperty("SCINTILLATIONYIELD",  
                                          ( 10000.0 * photonEmissionFraction ) / MeV );
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
  int energyPoints = 25;
  G4double particleEnergy[] = { 0.1*MeV, 0.13*MeV, 0.17*MeV, 0.2*MeV, 
                                0.24*MeV, 0.3*MeV, 0.34*MeV, 0.4*MeV, 
                                0.48*MeV, 0.6*MeV, 0.72*MeV, 0.84*MeV, 
                                1.*MeV, 1.3*MeV, 1.7*MeV, 2.*MeV, 2.4*MeV, 
                                3.*MeV, 3.4*MeV, 4.*MeV, 4.8*MeV, 6.*MeV, 
                                7.2*MeV, 8.4*MeV, 10.*MeV };

  G4double electronYield[] = { 1000, 1300, 1700, 2000, 2400, 3000, 3400, 
                               4000, 4800, 6000, 7200, 8400,10000, 13000, 
                               17000, 20000, 24000, 30000, 34000, 40000, 
                               48000, 60000, 72000, 84000, 100000 };
             
  BC408LightProperties->AddProperty("ELECTRONSCINTILLATIONYIELD",
                                     particleEnergy, electronYield, 
                                     energyPoints)->SetSpline(true);

   G4double protonYield[] = { 67.1, 88.6, 120.7, 146.5, 183.8, 246, 
                              290, 365, 483, 678, 910, 1175, 562, 
                              2385, 3660, 4725, 6250, 8660, 10420, 
                              13270, 17180, 23100, 29500, 36200, 45500 };

  BC408LightProperties->AddProperty("PROTONSCINTILLATIONYIELD",
                                    particleEnergy, protonYield, 
                                    energyPoints)->SetSpline(true);
  
   G4double ionYield[] = { 10.4, 12.7, 15.7, 17.9, 20.8, 25.1, 27.9, 
                           31.9, 36.8, 43.6, 50.2, 56.9, 65.7, 81.3, 
                           101.6, 116.5, 136.3, 166.15, 187.1, 218.6, 
                           260.54, 323.5, 387.5, 451.54, 539.9 };

  BC408LightProperties->AddProperty("IONSCINTILLATIONYIELD",
                                    particleEnergy, ionYield, 
                                    energyPoints)->SetSpline(true);
                                    
           
  BC408->SetMaterialPropertiesTable(BC408LightProperties);
    G4cout << "BC408 G4MaterialPropertiesTable" << G4endl;
  BC408LightProperties->DumpTable();
  return BC408;
}


G4Material* MaterialsManager::GetTin()
{
  tin = new G4Material("Tin", density= 7.31*g/cm3, numberElements=1);
  tin->AddElement(Sn, 1);
  return tin;
}

G4Material* MaterialsManager::GetPMTGlass()
{
  PMTGlass = new G4Material("PMTGlass", density= 2.6*g/cm3, 
                             numberElements=2);
  PMTGlass->AddElement(Si, 1);
  PMTGlass->AddElement(O, 2);
  return PMTGlass;
}

G4Material* MaterialsManager::GetAluminium()
{
  aluminium = new G4Material("Aluminium", density= 2.7*g/cm3, 
                              numberElements=1);
  aluminium->AddElement(Al, 1);
  return aluminium;
}

  
MaterialsManager *MaterialsManager::s_instance = 0;
