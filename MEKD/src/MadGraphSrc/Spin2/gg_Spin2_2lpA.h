//==========================================================================
// This file has been automatically generated for C++ Standalone by
// MadGraph 5 v. 1.5.7, 2013-01-15
// By the MadGraph Development Team
// Please visit us at https://launchpad.net/madgraph5
//==========================================================================

#ifndef MG5_Sigma_HEF_MEKD2_gg_spin2_mummupa_H
#define MG5_Sigma_HEF_MEKD2_gg_spin2_mummupa_H

#include <complex> 
#include <vector> 

#include "../Parameters_HEF_MEKD.h"
#include "../read_slha.h"

using namespace std; 

//==========================================================================
// A class for calculating the matrix elements for
// Process: g g > xg > mu- mu+ a / h zp WEIGHTED=4
//--------------------------------------------------------------------------

class gg_Spin2_2lpA
{
  public:

    // Constructor.
    gg_Spin2_2lpA() {}

    // Initialize process.
    virtual void initProc(string param_card_name);
	
	// Update process.
	virtual void updateProc(SLHAReader slha);

    // Calculate flavour-independent parts of cross section.
    virtual void sigmaKin(); 

    // Evaluate sigmaHat(sHat).
    virtual double sigmaHat(); 

    // Info on the subprocess.
    virtual string name() const {return "g g > mu- mu+ a (HEF_MEKD2)";}

    virtual int code() const {return 0;}

    const vector<double> & getMasses() const {return mME;}

    // Get and set momenta for matrix element evaluation
    vector < double * > getMomenta(){return p;}
    void setMomenta(vector < double * > & momenta){p = momenta;}
    void setInitial(int inid1, int inid2){id1 = inid1; id2 = inid2;}

    // Get matrix element vector
    const double * getMatrixElements() const {return matrix_element;}

    // Constants for array limits
    static const int ninitial = 2; 
    static const int nexternal = 5; 
    static const int nprocesses = 1; 

  private:

    // Private functions to calculate the matrix element for all subprocesses
    // Calculate wavefunctions
    void calculate_wavefunctions(const int perm[], const int hel[]); 
    static const int nwavefuncs = 9; 
    std::complex<double> w[nwavefuncs][18]; 
    static const int namplitudes = 4; 
    std::complex<double> amp[namplitudes];
	int ntry, sum_hel, ngood;	// moved here by Ghost remover v. 0.1 
    double matrix_gg_xg_mummupa_no_hzp(); 

    // Store the matrix element value from sigmaKin
    double matrix_element[nprocesses]; 

    // Color flows, used when selecting color
    double * jamp2[nprocesses]; 

    // Pointer to the model parameters
    Parameters_HEF_MEKD * pars;

    // vector with external particle masses
    vector<double> mME; 

    // vector with momenta (to be changed each event)
    vector < double * > p; 
    // Initial particle ids
    int id1, id2; 

}; 


#endif  // MG5_Sigma_HEF_MEKD2_gg_mummupa_H
