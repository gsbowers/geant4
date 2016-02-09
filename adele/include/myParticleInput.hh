#ifndef myParticleInput_h
#define myParticleInput_h 1

#include <string>
#include "myParticle.hh"

using namespace std;

class myParticleInput
{

  public: 
    myParticleInput(string filename);
    ~myParticleInput();
    
    myParticle getParticle();
	
  private:
    // input file variables
    
    long    fNinput;
    double* fWeight;
    double* fArrivalTime; // ms
    double* fEnergy;      // keV
    double* fRadialDist;  // km
    double* fzcos;        // cos(theta) 
    int*    fNCompton;    // number of compton scatters

    // statistics related variables
    double* fECD;         // empirical cumulative distribution 
};

#endif
