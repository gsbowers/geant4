#include "myParticleInput.hh"
#include "myParticle.hh"
#include <iostream>
#include <fstream>
#include <math.h>
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

using namespace std;

myParticleInput::myParticleInput(string filename)
{
	ifstream f;	

  // get number of lines in file
  f.open(filename.c_str(), ios::in);
  string line;
  for (fNinput=0l; getline(f, line); ++fNinput);
  // reset file pointer
  f.clear();
  f.seekg(0, ios::beg);

  // allocate memory
  fWeight      = new double[fNinput]();
  fArrivalTime = new double[fNinput](); 
  fEnergy      = new double[fNinput](); 
  fRadialDist  = new double[fNinput](); 
  fzcos        = new double[fNinput](); 
  fNCompton    = new int[fNinput](); 

  //fECD         = new double[fNinput]();
  fECD         = new double[fNinput]();
 
  // read input file 
  long i = 0l;
  while ( f >> fArrivalTime[i]
            >> fEnergy[i]
            >> fRadialDist[i]
            >> fzcos[i]
            >> fNCompton[i]){
 
    // calculate weighting factor, max out at 100
    /*a = fabs(fzcos[i]);
    if (a < 0.05)
      fWeight[i] = 20.0;
    else
      fWeight[i] = 1.0/a;
		*/
    fWeight[i] = 1.0/fabs(fzcos[i]);
    if (fWeight[i] > 20.0) fWeight[i] = 20.0;

    // build the weighted emperical cumulative distribution to 
    // use to select indices from particle array
    if (i == 0)
      fECD[i] = fWeight[i];
    else 
      fECD[i] = fECD[i-1] + fWeight[i];
    
    i++;
	}
  f.close();

  // normalize the ECD to max out at 1
  for (i=0l; i<fNinput; i++)
    fECD[i] /= fECD[fNinput-1];

  cout << fNinput << " particles input from " << filename << endl;

  return;
}

myParticleInput::~myParticleInput()
{
  delete[] fWeight;
  delete[] fArrivalTime; 
  delete[] fEnergy;
  delete[] fRadialDist;
  delete[] fzcos;
  delete[] fNCompton;
  delete[] fECD;
}

myParticle myParticleInput::getParticle()
{
  // This routine will do 3 things:
  // 1.  Pick a particle from the particle array according 
  // to the specified fECD
  // 2.  Specify it's direction
  // 3.  Specify it's location on the input sphere
 
  myParticle particle;

//c Pick one of the input photons randomly and set up its initial
//c parameters:

//c Select a random # from 0-1 and walk up the integral probability
//c distribution until you exceed it.  Wishing for IDL's "where" here.
//c "indx" is the photon you've picked.

  long indx = -1l;
  double randval = G4UniformRand();
  for (long i=0l; i < fNinput; i++){
    if (fECD[i] > randval){
			indx = i;
      break;
		}  
	}

  particle.set_Energy(fEnergy[indx]);

//c CHOOSE A DIRECTION: the third direction cosine is given, pick
//c the horizontal ones using a random azimuthal angle phi.

  double zd = fzcos[indx];
  double hor = sqrt(1.0 - zd*zd);
  double random_phi = G4UniformRand(); 
  double phi = 2.0 * pi * random_phi;
  double dir[3] = {hor*cos(phi), hor*sin(phi), zd};
  //cout << "dir*dir " << dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2] << endl;
  //
  particle.set_StartDir(dir[0],dir[1],dir[2]);

//c CHOOSE A STARTING POSITION randomly on a disk (which will be  the
//c input sphere's shadow cast by a plane wave coming in the input
//c direction generated above for the current photon:

  double random_radius = G4UniformRand();
  double random_phi2 = G4UniformRand(); 
  double r = sqrt(random_radius);
  double phi2 = 2.0 * pi * random_phi2;
  double x = r * sin(phi2);
  double y = r * cos(phi2);
  double z = -sqrt(1.0 - x*x - y*y); 

//c Transform the initial position by rotating it into the
//c reference frame defined by the incoming photon direction
//c for this photon (direction cosines are the variable dir;
//c coordinates to start the photon are cxyz; rr is the radius
//c of the sphere for photon inputs:

  double vecA[3] = {x, y, z};
  double vecB[3] = {0, 0, 0};
  double zaxis[3] = {dir[0], dir[1], dir[2]};
  double xaxis[3] = {-dir[2]/sqrt(dir[2]*dir[2]+dir[0]*dir[0]), 0.0,
                      dir[0]/sqrt(dir[2]*dir[2]+dir[0]*dir[0])};

//c SUBROUTINE TO PERFORM A ROTATION OF THREE MUTUALLY
//c ORTHOGONAL AXES.  vecA is the vector in frame A, vecB is
//c the vector in frame B (desired result) and zaxis and
//c xaxis are the coordinates of those A axes in the B frame.
//c T is a rotation matrix.

  double T[3][3];
  int i, j, k;
  for (i=0; i<3; i++){
    T[0][i] = xaxis[i];
    T[2][i] = zaxis[i];
    j = ((i+1) % 3);
    k = ((j+1) % 3);
    T[1][i] = xaxis[k]*zaxis[j] - xaxis[j]*zaxis[k];
  } 

  /*
  // check that T*transpose(T) = 1
  double I[3][3];
  double sum = 0;
  for (i=0; i<3; i++)
    for (j=0; j<3; j++){
      for (k=0; k<3; k++)
        sum = sum + T[i][k]*T[j][k];
    	I[i][j] = sum; 
    	sum = 0;
		}
 
  for (i=0; i<3; i++){
   for (j=0; j<3; j++)
     cout << I[i][j] << '\t';
   cout << endl;
  }
  */
 
  /*
  //check that det(T) = 1
  cout << T[0][0]*(T[1][1]*T[2][2] -
T[2][1]*T[1][2]) - T[0][1]*(T[1][0]*T[2][2] - T[2][0]*T[1][2]) +
          T[0][2]*(T[1][0]*T[2][1] - T[2][0]*T[1][1]) << endl;
	*/
  
  // rotate vecA
  for (i=0; i<3; i++)
    for (j=0; j<3; j++)
      vecB[i] = vecB[i] + T[j][i]*vecA[j];

  particle.set_StartXYZ(vecB[0],vecB[1],vecB[2]);
	return particle;

}
