
//
// $Id: PrimaryGeneratorAction.cc 12.10.2016 A Fijalkowska $
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "Decay.hh"
#include "Exception.hh"

#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), 
   particleGun(0)
{
	decay = 0L;
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);
  	runDecay = false;
	SetUpDefault();

}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete decay;
}

void PrimaryGeneratorAction::SetUpDefault()
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("neutron");

	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleTime(0.0*ns);
	particleGun->SetParticlePosition(G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
	particleGun->SetParticleEnergy(500.0*keV);
	
	if(runDecay)
		LoadDecay("Isotope.ens");
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{	
	if(runDecay)
	{
		GenerateDecay(anEvent);
	}

	else
	{
		GenerateSingleParticle(anEvent);
	}
}	

void PrimaryGeneratorAction::GenerateSingleParticle(G4Event* anEvent)
{

	//G4ThreeVector aim(0.0,0.0,1.0);
	//GenerateIsotropicDirectionDistribution(&aim,0.0);
	//G4ThreeVector startPos(0.0*cm,0.0*cm,0.0*cm);	
	//particleGun->SetParticlePosition(startPos);
	//particleGun->SetParticleMomentumDirection(aim);
	//particleGun->GeneratePrimaryVertex(anEvent);
	G4double barWidth = 3*cm;
	G4double barLength = 200*cm;
	RadiateBarByParallelBeam(anEvent, barWidth, barLength);	
	return;
}
	
void PrimaryGeneratorAction::GenerateDecay(G4Event* anEvent)
{

	
	G4ThreeVector startPos( 0.*mm, 0.0*cm, 0.0*cm );
	
	//Direction
	G4ThreeVector direction( 0.0, 1.0, 0.0 );
	GenerateIsotropicDirectionDistribution(&direction,0.0);
	std::vector<Event> allDecay;
	try{
		allDecay = decay -> Execute();
	}
	catch (Exception& except)
	{
		std::cout << "PrimaryGeneratorAction exception: " 
		<< except.GetMessage() << std::endl;
		throw except;
	}
	
	for(unsigned int i=0; i<allDecay.size();i++)
	{
			particleGun->SetParticlePosition( startPos );
			GenerateIsotropicDirectionDistribution(&direction,0.0);
			particleGun->SetParticleMomentumDirection( direction );
			particleGun->SetParticleEnergy(allDecay.at(i).energy *keV);
			particleGun->SetParticleDefinition(allDecay.at(i).type);
			particleGun->GeneratePrimaryVertex(anEvent);
	}
}

void PrimaryGeneratorAction::LoadDecay(std::string filename)
{  
	try
	{
		decay=new Decay(filename);
	}
	catch (Exception& except)
	{
		std::cout << "PrimaryGeneratorAction::LoadDecay exception: " 
		<< except.GetMessage() << std::endl;
		throw except;
	}
}


void PrimaryGeneratorAction::RadiateIsotropicallyMediumBar(G4Event* anEvent)
{

	G4ThreeVector aim(0.0,0.0,1.0);
	//GenerateIsotropicDirectionDistribution(&aim,0.0);
	G4double pi = 4.*atan(1.0);
	G4double thetaMin = 0.48*pi;
	G4double thetaMax = 0.51*pi;
	G4double phiMin = 0.8*pi;
	G4double phiMax = 1.2*pi;
	GenerateIsotropicDirectionDistribution( &aim,thetaMin, thetaMax, 
	                                        phiMin, phiMax);
	//m_ParticleGun->SetParticleEnergy(1*MeV);
	G4ThreeVector startPos(100.0*cm,0.0*cm,0.0*cm);
	
	particleGun->SetParticlePosition(startPos);
	particleGun->SetParticleMomentumDirection(aim);
	particleGun->GeneratePrimaryVertex(anEvent);	
	return;
}

	
void PrimaryGeneratorAction::RadiateBarByParallelBeam(G4Event* anEvent, G4double barWidth, G4double barLength)
{

	G4ThreeVector aim(0.0,-1.0,0.0);
	G4double xPos = G4UniformRand()*barWidth - barWidth/2.;
	G4double yPos = 1*m;
	G4double zPos = G4UniformRand()*barLength - barLength/2.;
	
	G4ThreeVector startPos(xPos, yPos, zPos);
	particleGun->SetParticlePosition(startPos);
	particleGun->SetParticleMomentumDirection(aim);
	particleGun->GeneratePrimaryVertex(anEvent);	
	return;
}


void PrimaryGeneratorAction::GenerateIsotropicDirectionDistribution
                             (G4ThreeVector* direction,
                             G4double thetaMin, 
                             G4double thetaMax, 
                             G4double phiMin, 
                             G4double phiMax)
{
	G4double cosThetaMin = cos(thetaMin);
	G4double cosThetaMax = cos(thetaMax);
	
	//assumption: cosThetaMin > cosThetaMax 
	G4double randomCosTheta = G4UniformRand()*(cosThetaMin-cosThetaMax) + cosThetaMax;
	G4double randomSinTheta = sqrt(1.0 - randomCosTheta * randomCosTheta);
	G4double randomPhi = G4UniformRand()*(phiMax-phiMin) + phiMin;

	*direction = G4ThreeVector(cos(randomPhi)*randomSinTheta, sin(randomPhi)*randomSinTheta, randomCosTheta);	 
}


void PrimaryGeneratorAction::GenerateIsotropicDirectionDistribution
                             (G4ThreeVector* direction, G4double theta0)
{
	
	//GenerateIsotropicDirectionDistribution( &direction,thetaMin, thetaMax, 
	//                                        phiMin, phiMax);
	//TODO!!!!!                                       
	                                        
	                                        
	// -cos(Theta0) < cosTheta < cos(Theta0)
	G4double cosTheta = ( G4UniformRand() - 0.5 ) * 2.0*cos(theta0);
	G4double sinTheta = sqrt( 1.0 - cosTheta * cosTheta );
	// 0. < phi < 2*pi
	G4double phi = G4UniformRand() * 8.0 * atan(1.0);
	*direction = G4ThreeVector (cos(phi)*sinTheta, sin(phi)*sinTheta, cosTheta);
}
