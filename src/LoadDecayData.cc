/* 
 * File:   LoadDecayData.cpp
 * Author: aleksandra
 * 
 * Created on 3 czerwiec 2012, 12:27
 */

#include "LoadDecayData.hh"
#include "MyTemplates.hh"
#include "PeriodicTable.hh"
#include "Exception.hh"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

LoadDecayData::LoadDecayData(const string filename)
{
	string atomicName;

	vector<Gamma*> gamma;
	ifstream in(filename.c_str());
	if (!in.is_open())
		throw IOException("Warning message: The file " + (string) filename + " is not open!");
	
	string data, buff;
	getline(in, buff);//first line - some information about decay
	getline(in, buff);

	data = buff.substr(0, 3);
	atomicMass_ = string2num <int> (data, std::dec);

	data = buff.substr(3, 2);
	atomicName = data;
	atomicNumber_ = PeriodicTable::GetAtomicNumber(atomicName);

	while (!in.eof())
	{
		getline(in, buff);
		if (buff.size() < 80)
			break;
			
		if (buff[6]!='c')
		{
			if(buff[7]=='P')//parent level
			{
				data=buff.substr(9,10);
				double energy = string2num <double>(data, std::dec);
				parentLevel_ = new Level(energy, atomicNumber_);
				//data=buff.substr(21,18); 
				//parentLevel_->SetSpinAndParity(data);
				
				data=buff.substr(39,9);
				string T12 = data;		
				parentLevel_->SetHalfLifeTime(T12);
				
				data=buff.substr(64,10);
				qVal_ = string2num <double>(data, std::dec);
				if(qVal_<10)
					qVal_ = qVal_*1000;
				qVal_ += energy;//total energy for decay takes parent level en
				std::cout<<"Q: "<<qVal_<<std::endl;
				//data=buff.substr(76,4);
				//parentLevel_->SetIonizationState(strtod(data.c_str(), NULL));
			}		
		
			if(buff[7]=='L' && buff[5]==' ' && buff[6]==' ') //each level
			{
				data = buff.substr(9, 10);
				double energyLevel = string2num <double>(data, std::dec);
				levels_.push_back(new Level(energyLevel, atomicNumber_));
				
				//data=buff.substr(21,14);
				//(*(levels_.end() - 1))->SetLevelSpin(data);
	
				data=buff.substr(39,9);
				string T12 = data;
				(*(levels_.end() - 1))->SetHalfLifeTime(T12);
				(*(levels_.end() - 1))->SetMaxBetaEnergy(qVal_ - energyLevel);
				//std::cout<<" level "<<energyLevel<<" beta window :"<<qVal_ - energyLevel<<std::endl;
				//data=buff.substr(55,9);
				//(*(levels_.end() - 1))->SetDeltaAngularMomentum(data);
				
				//there is something more here (in the *.ens file) but I don't need this information (at least so far)
				//data=buff.substr(77,2);
				//(*(levels_.end() - 1))->SetIsomerFlag(data);
			}
			
			if(buff[7]=='B' && buff[5]==' ' && buff[6]==' ' && levels_.size() > 0)
			{
				(*(levels_.end() - 1))->SetBetaMinus();
				data = buff.substr(21, 8);
				double betaIntensity = string2num <double>(data, std::dec);
				(*(levels_.end() - 1))->SetBetaFeedingFunction(betaIntensity);

				//data=buff.substr(41,8);
				//double betaLogft = string2num <double>(data, std::dec);
				//(*(levels_.end() - 1))->SetBetaLogft(betaLogft);

			}
			
			if(buff[7]=='E'&& buff[5]==' ' && buff[6]==' ' && levels_.size() > 0)
			{
				
				data = buff.substr(21, 8);
				double betaPlusIntensity = string2num <double>(data, std::dec);
				data=buff.substr(31,8);
				double ECIntensity= string2num <double>(data, std::dec);
				data=buff.substr(64,10);
				//double totalIntensity = string2num <double>(data, std::dec);
				(*(levels_.end() - 1))->SetBetaFeedingFunction(betaPlusIntensity+ECIntensity);
				(*(levels_.end() - 1))->SetBetaPlus(betaPlusIntensity, ECIntensity);
				//data=buff.substr(41,8);
				//double betaLogft = string2num <double>(data, std::dec);
				//(*(levels_.end() - 1))->SetBetaLogft(betaLogft);

		
			}
			
			if(buff[7]=='E'&& (buff[5]=='S'||buff[5]=='2') && buff[6]==' ')
			{
				data = buff.substr(9, 3);
				if (data == "CK=")
				{
					data = buff.substr(9, 74); //all line
					stringstream ss;
					ss << data;

					while (!ss.eof())
					{
						string singleCoef;
						getline(ss, singleCoef, '$');//"$" - separator
						stringstream sss; //data in file: CK=num or CK+=num
						sss << singleCoef;

						string type;
						double value;
						getline(sss, type, '=');
						sss >> value;
						(*(levels_.end() - 1))->GetBeta()->SetECCoef(type, value);
					}

				}
			}	
			
// The last check makes sure there are levels to attach the gamma.
			if(buff[7]=='G' && buff[5]==' ' && buff[6]==' ' && levels_.size() > 0)
			{
				data = buff.substr(9, 10);
				double gammaEnergy = string2num <double>(data, std::dec);
				data = buff.substr(21, 8);
				double gammaIntensity = string2num <double>(data, std::dec);
				data = buff.substr(29, 2);
				data = buff.substr(55, 7);
				double electronConversionCoefficient = string2num <double>(data, std::dec);
				gamma.push_back(new Gamma(atomicNumber_, gammaEnergy, gammaIntensity, electronConversionCoefficient));
				Level *finalLevel = FindFinalLevel((*(gamma.end() - 1)), (*(levels_.end() - 1)), levels_);
				(*(gamma.end() - 1))->SetFinalLevel(finalLevel);
				(*(levels_.end() - 1))->AddGamma(*(gamma.end() - 1));
				
				//data = buff.substr(31,10);			
				//(*(gamma.end() - 1))->SetGammaMultiplicity(data);

			}
			
			if (buff[7] == 'G' && (buff[5] == 'S' || buff[5] == '2') && buff[6] == ' ')//elektronowe współczynniki konwersji
			{
				data = buff.substr(9, 3);
				if (data == "KC=")
				{
					data = buff.substr(9, 74); //all line
					stringstream ss;
					ss << data;

					while (!ss.eof())
					{
						string singleCoef;
						getline(ss, singleCoef, '$');//"$" - separator
						stringstream sss; //data in file: KC=num or KC+=num
						sss << singleCoef;

						string type;
						double value;
						getline(sss, type, '=');
						sss >> value;
						(*(gamma.end() - 1))->SetShellElectronConvCoef(type, value);
					}

				}
			}			
		}
	}
	in.close();
}


Level* LoadDecayData::FindFinalLevel(Gamma* gamma, Level* parentLevel, vector<Level*> &levels)
{
    Level *finalLevel = parentLevel;
    double deltaEnergy = fabs(parentLevel->GetEnergy() - gamma-> GetEnergy());
    double minDeltaEnergy = deltaEnergy;
    for( vector<Level*>::const_iterator it=levels.begin(); it!=levels.end(); ++it)
    {
       if(minDeltaEnergy >= fabs(deltaEnergy - (*it)->GetEnergy()))
       {
	   minDeltaEnergy = fabs(deltaEnergy - (*it)->GetEnergy());
	   finalLevel = *it; 
       }
    }
    return finalLevel;
}


LoadDecayData::~LoadDecayData()
{

}

