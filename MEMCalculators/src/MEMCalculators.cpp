/*************************************************************************
 *  Authors:   MEKD & MELA fans
 *  Contact:   ...
 *  Created:   11.01.2013.
 *************************************************************************/
#ifndef MEMCalc_MEMCalc_cpp
#define MEMCalc_MEMCalc_cpp

/// MEMs header
#include "ZZMatrixElement/MELA/interface/Mela.h"
#include "ZZMatrixElement/MEKD/interface/MEKD.h"
#include "../interface/MEMCalculators.h"
#include "ZZMatrixElement/MELA/src/computeAngles.h"

using namespace std;
using namespace MEMNames;

//////////////////////////////////////////////////////////////////////////
///  MEMs interface class to MELA & MEKD packages.
///
///  Provides interface to the MEKD & MELA packages to
///  computes MEs and KDs for the process specified by the user.
///
//////////////////////////////////////////////////////////////////////////



/// MEKD process name translation - initialisation
const TString MEMs::m_processNameMEKD[MEMNames::NUM_PROCESSES] = {
	"ggSpin0Pm", "Spin0Pm", "ggSpin0h", "Spin0h", "ggSpin0M", "Spin0M",
	"qqSpin1P", "Spin1P", "qqSpin1M", "Spin1M",
	"ggSpin2Pm", "qqSpin2Pm", "Spin2Pm", "ggSpin2Ph", "qqSpin2Ph", "Spin2Ph", "ggSpin2Mh", "qqSpin2Mh", "Spin2Mh", "ggSpin2Pb", "qqSpin2Pb", "Spin2Pb",
	"ZZ", "", "","ggZZ_Higgs","ggSpin0Lambda1",
	"ggSpin0", "Spin0",
	"qqSpin1", "Spin1",
	"ggSpin2", "qqSpin2", "Spin2",
	"VBFSpin0Pm_jj", "VBFSpin0M_jj", "ggSpin0Pm_jj", "ggSpin0M_jj","VHSpin0Pm", "VHSpin0M_jj"
};



///----------------------------------------------------------------------------------------------
/// MEMs::MEMs - constructor
///----------------------------------------------------------------------------------------------
MEMs::MEMs(double collisionEnergy, double sKD_mass, string PDFName, bool debug_)
{
    /// Mapping between MEMs process enums and MELA process enums 
    /// - initialisation (to be updated)
    MELAprocMap[kSMHiggs]         =TVar::HSMHiggs;
    MELAprocMap[k0hplus]          =TVar::H0hplus;
    MELAprocMap[k0minus]          =TVar::H0minus;
    MELAprocMap[k1plus]           =TVar::H1plus;
    MELAprocMap[k1plus_prodIndep] =TVar::H1plus;
    MELAprocMap[k1minus]          =TVar::H1minus;
    MELAprocMap[k1minus_prodIndep]=TVar::H1minus;
    MELAprocMap[k2mplus_gg]       =TVar::H2_g1g5;
    MELAprocMap[k2mplus_qqbar]    =TVar::H2_g1g5;
    MELAprocMap[k2mplus_prodIndep]=TVar::H2_g1g5;
    MELAprocMap[k2hplus]          =TVar::H2_g4;
    MELAprocMap[k2hminus]         =TVar::H2_g8;
    MELAprocMap[k2bplus]          =TVar::H2_g5;
    MELAprocMap[kqqZZ]            =TVar::bkgZZ;
    MELAprocMap[kqqZZ_prodIndep]  =TVar::bkgZZ;
    MELAprocMap[kggZZ]            =TVar::bkgZZ;
    MELAprocMap[kggZZ_SMHiggs]		=TVar::bkgZZ_SMHiggs;
    MELAprocMap[k0_g1prime2]		=TVar::H0_g1prime2;
	
	MELAprocMap[kSpin0_gg]			=TVar::SelfDefine_spin0;
	MELAprocMap[kSpin0_prodIndep]	=TVar::SelfDefine_spin0;
	MELAprocMap[kSpin1_qqbar]		=TVar::SelfDefine_spin1;
	MELAprocMap[kSpin1_prodIndep]	=TVar::SelfDefine_spin1;
	MELAprocMap[kSpin2_gg]			=TVar::SelfDefine_spin2;
	MELAprocMap[kSpin2_qqbar]		=TVar::SelfDefine_spin2;
	MELAprocMap[kSpin2_prodIndep]	=TVar::SelfDefine_spin2;
    
    MELAprocMap[kJJ_SMHiggs_VBF]	=TVar::HSMHiggs;
    MELAprocMap[kJJ_0minus_VBF]		=TVar::H0minus;
    MELAprocMap[kJJ_SMHiggs_GG]		=TVar::HSMHiggs;
    MELAprocMap[kJJ_0minus_GG]		=TVar::H0minus;
    MELAprocMap[kJJ_SMHiggs_VH]		=TVar::HSMHiggs;
    MELAprocMap[kJJ_0minus_VH]		=TVar::H0minus;


	MELAprocIntMap[kg1g4]			=TVar::D_g1g4;
	MELAprocIntMap[kg1g2]			=TVar::D_g1g2;
	MELAprocIntMap[kg1g4_pi_2]		=TVar::D_g1g4_pi_2;
	MELAprocIntMap[kg1g2_pi_2]		=TVar::D_g1g2_pi_2;
	MELAprocIntMap[k_g1g1prime2]	=TVar::D_g1g1prime2;
    
    /// Mapping between MEMs process enums and MELA production enums 
    /// - initialisation (to be updated)
    MELAprodMap[kSMHiggs]         =TVar::ZZGG;
    MELAprodMap[k0hplus]          =TVar::ZZGG;
    MELAprodMap[k0minus]          =TVar::ZZGG;
    MELAprodMap[k1plus]           =TVar::ZZQQB;
    MELAprodMap[k1plus_prodIndep] =TVar::ZZINDEPENDENT;
    MELAprodMap[k1minus]          =TVar::ZZQQB;
    MELAprodMap[k1minus_prodIndep]=TVar::ZZINDEPENDENT;
    MELAprodMap[k2mplus_gg]       =TVar::ZZGG;
    MELAprodMap[k2mplus_qqbar]    =TVar::ZZQQB;
    MELAprodMap[k2mplus_prodIndep]=TVar::ZZINDEPENDENT;
    MELAprodMap[k2hplus]          =TVar::ZZGG;
    MELAprodMap[k2hminus]         =TVar::ZZGG;
    MELAprodMap[k2bplus]          =TVar::ZZGG;
    MELAprodMap[kqqZZ]            =TVar::ZZQQB;
    MELAprodMap[kqqZZ_prodIndep]  =TVar::ZZINDEPENDENT;
    MELAprodMap[kggZZ]            =TVar::ZZGG;
    MELAprodMap[kggZZ_SMHiggs]    =TVar::ZZGG;
    MELAprodMap[k0_g1prime2]      =TVar::ZZGG;
	
	MELAprodMap[kSpin0_gg]			=TVar::ZZGG;
	MELAprodMap[kSpin0_prodIndep]	=TVar::ZZINDEPENDENT;
	MELAprodMap[kSpin1_qqbar]		=TVar::ZZQQB;
	MELAprodMap[kSpin1_prodIndep]	=TVar::ZZINDEPENDENT;
	MELAprodMap[kSpin2_gg]			=TVar::ZZGG;
	MELAprodMap[kSpin2_qqbar]		=TVar::ZZQQB;
	MELAprodMap[kSpin2_prodIndep]	=TVar::ZZINDEPENDENT;

    MELAprodMap[kJJ_SMHiggs_VBF]	=TVar::JJVBF;
    MELAprodMap[kJJ_0minus_VBF]		=TVar::JJVBF;
    MELAprodMap[kJJ_SMHiggs_GG]		=TVar::JJGG;
    MELAprodMap[kJJ_0minus_GG]		=TVar::JJGG;
    MELAprodMap[kJJ_SMHiggs_VH]		=TVar::JJVH;
    MELAprodMap[kJJ_0minus_VH]		=TVar::JJVH;

	
    MELAprodIntMap[kg1g4]			=TVar::ZZGG;
    MELAprodIntMap[kg1g2]			=TVar::ZZGG;
    MELAprodIntMap[kg1g4_pi_2]		=TVar::ZZGG;
    MELAprodIntMap[kg1g2_pi_2]		=TVar::ZZGG;
    MELAprodIntMap[k_g1g1prime2]	=TVar::ZZGG;
	
    /// Mapping between MEMs calculator enums and MELA MatrixElement enums 
    /// - initialisation (to be updated)
    MELAcalcMap[kMCFM]      =TVar::MCFM;
    MELAcalcMap[kJHUGen]    =TVar::JHUGen;
    MELAcalcMap[kAnalytical]=TVar::ANALYTICAL;
    MELAcalcMap[kMELA_HCP]  =TVar::ANALYTICAL; 
//    MELAcalcMap[kMEKD]      =TVar::MadGraph; 

    debug=debug_;
	if( debug ) cout << "MEMs::MEMs. The debug flag is ON\n";
	
	/// Number of processes for precalculation
	NUM_PROCESSES_PRECALC = 27;

    /// Initialise MEKD
    m_MEKD = new MEKD(collisionEnergy, PDFName);
    /// Initialise MELA
    m_MELA = new Mela(collisionEnergy, sKD_mass); //sMELA_mass for SuperMELA calculation
    
    /// Set some non-physical values for MEs initially
    for(int iMemCalc = 0; iMemCalc < NUM_MEMCALCS; iMemCalc++ )
        for(int iProcess = 0; iProcess < NUM_PROCESSES; iProcess++ )
            m_computedME[iProcess][iMemCalc] = -999.;

    m_weight = 0.0;
	
	m_VCbuffer = new vector<complex<double> >;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeME - Compute ME for the specified process.
///----------------------------------------------------------------------------------------------
int MEMs::computeME(Processes process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, double& me2process)
{
	return computeME(process, calculator, partP, partId, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, me2process);
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeME - Compute ME for the specified process. A generic case
///----------------------------------------------------------------------------------------------
int MEMs::computeME(Processes process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, vector<complex<double> > *ProdCouplings, vector<complex<double> > *DecayCouplings, double& me2process)
{
	if( debug ) cout << "MEMs::computeME started.\n";
	if( debug ) cout << "MEMs::computeME. Selected calculator: " << calculator << "\n";
	
	/// check if process is supported
	if( !isProcSupported[process][calculator] ) return ERR_PROCESS;
	
	/// Check the couplings for consistency
	if( process==kSpin0_gg || process==kSpin0_prodIndep ||
		process==kSpin1_qqbar || process==kSpin1_prodIndep ||
		process==kSpin2_gg || process==kSpin2_qqbar || process==kSpin2_prodIndep
	)
		if( (Check_Couplings( process, ProdCouplings, DecayCouplings)) != 0 ) return ERR_PROCESS;
	
	
	if( debug ) cout << "MEMs::computeME. Process is supported!\n";
  
	/// perform computation according to the specified process and MEM package
	switch ( calculator )
	{
		case kMEKD:			/// compute ME with MEKD
			if( ProdCouplings!=(vector<complex<double> >*) NULL || DecayCouplings!=(vector<complex<double> >*) NULL )
				if( (m_err=MEKD_Mixed_State( m_processNameMEKD[process], ProdCouplings, DecayCouplings )) != 0 ) return ERR_COMPUTE;
			if( (m_MEKD->computeME(m_processNameMEKD[process], partP, partId, me2process)) != 0 ) return ERR_COMPUTE;
			break;
			
		case kAnalytical:	/// compute ME with MELA
			if( debug )
				cout << "MEMs::computeME. Analytical -> process: " << process << endl;
			if (ProdCouplings!=(vector<complex<double> >*) NULL || DecayCouplings!=(vector<complex<double> >*) NULL){
				if ( cacheMELAcalculation(process,calculator,partP,partId,ProdCouplings, DecayCouplings,me2process) != 0) return ERR_COMPUTE;
			}
			else	return cacheMELAcalculation(process,calculator,partP,partId,me2process); 
			break;
			
		case kJHUGen:       /// compute ME with JHUGen
			if( debug )
				cout << "MEMs::computeME. JHUGen -> process: " << process << endl;
			if (ProdCouplings!=(vector<complex<double> >*) NULL || DecayCouplings!=(vector<complex<double> >*) NULL){
				if ( cacheMELAcalculation(process,calculator,partP,partId,ProdCouplings, DecayCouplings,me2process) != 0) return ERR_COMPUTE;
			}
			else
				return cacheMELAcalculation(process,calculator,partP,partId,me2process);
			break;

    case kMCFM:       /// compute ME with JHUGen
      if( debug )
        cout << "MEMs::computeME. kMCFM-> process: " << process << endl;
      if (ProdCouplings!=(vector<complex<double> >*) NULL || DecayCouplings!=(vector<complex<double> >*) NULL){
        if ( cacheMELAcalculation(process,calculator,partP,partId,ProdCouplings, DecayCouplings,me2process) != 0) return ERR_COMPUTE;
      }
      else
        return cacheMELAcalculation(process,calculator,partP,partId,me2process);
      break;
			
			
		case kMELA_HCP:     /// compute ME with MELA_HCP
			if( debug )
				cout << "MEMs::computeME. MELA_HCP -> process: " << process << endl;
			return ERR_PROCESS;
			break;
			
		default:
			if( debug )
				cout << "MEMs::computeME. default case hit... don't recognize calculator" << endl;
			return ERR_PROCESS;
			break;
	}
	
	return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeME_Interference - Compute individual mixed MEs interference term for the specified "model". Intended for the production-independent models PLUS gg -> Spin 0
///----------------------------------------------------------------------------------------------
int MEMs::computeME_Interference(Processes process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, vector<complex<double> > *DecayCouplings, double& me2process)
{
	if( debug ) cout << "MEMs::computeME_Interference started.\n";
	
	/// Calculate the full term
	if( (m_err=computeME( process, calculator, partP, partId, (vector<complex<double> >*) NULL, DecayCouplings, me2process))!=0 ) return m_err;
	
	/// Prepare a buffer vector (pointer)
	(*m_VCbuffer).resize( (*DecayCouplings).size(), complex<double>( 0, 0 ) );	// should be a vector (pointer) of zeroes
	
	/// Subtract individual pure term(s)
	for( m_uIbuffer=0; m_uIbuffer<(*DecayCouplings).size(); m_uIbuffer++ )
	{
		if( norm((*DecayCouplings)[m_uIbuffer]) > 0 ) 
		{
			(*m_VCbuffer)[m_uIbuffer] = (*DecayCouplings)[m_uIbuffer];	// load a current coupling
			if( (m_err=computeME( process, calculator, partP, partId, (vector<complex<double> >*) NULL, m_VCbuffer, m_Dbuffer))!=0 ) return m_err;	// pure term(s)
			me2process -= m_Dbuffer;	// subtracting a pure term
			
			(*m_VCbuffer)[m_uIbuffer] = complex<double>( 0, 0 );	// reverting back to 0 coupling
		}
	}
	
	return NO_ERR;
}



/// Work in progress method. Currently serves as a bridge to MELA package implementations.
int MEMs::computeME_Interference(Processes_int process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId,  double& me2process)
{
	if( calculator == kJHUGen || calculator ==kAnalytical )
		cacheMELAcalculation( static_cast<int>(process), calculator, partP, partId, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, me2process);
	else 
		return ERR_COMPUTE;
	return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeKD - Compute KD and MEs for the specified processes and MEM calculator.
///----------------------------------------------------------------------------------------------
int MEMs::computeKD(Processes processA, Processes processB, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, double& kd, double& me2processA, double& me2processB )
{    
	return computeKD( processA, processB, calculator, partP, partId, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, kd, me2processA, me2processB );
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeKD - Compute KD and MEs for the specified processes and MEM calculator. A generic case
///----------------------------------------------------------------------------------------------
int MEMs::computeKD(Processes processA, Processes processB, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, vector<complex<double> > *ProdCouplingsA, vector<complex<double> > *DecayCouplingsA, vector<complex<double> > *ProdCouplingsB, vector<complex<double> > *DecayCouplingsB, double& kd, double& me2processA, double& me2processB )
{    
	/// check if processes are supported
	if (!isProcSupported[processA][calculator]) return ERR_PROCESS;
	if (!isProcSupported[processB][calculator]) return ERR_PROCESS;
	
	/// Check the couplings for consistency; A
	if( processA==kSpin0_gg || processA==kSpin0_prodIndep ||
		processA==kSpin1_qqbar || processA==kSpin1_prodIndep ||
		processA==kSpin2_gg || processA==kSpin2_qqbar || processA==kSpin2_prodIndep
	)
		if( (Check_Couplings( processA, ProdCouplingsA, DecayCouplingsA)) != 0 ) return ERR_PROCESS;
		
	/// Check the couplings for consistency; B
	if( processB==kSpin0_gg || processB==kSpin0_prodIndep ||
		processB==kSpin1_qqbar || processB==kSpin1_prodIndep ||
		processB==kSpin2_gg || processB==kSpin2_qqbar || processB==kSpin2_prodIndep
	)
		if( (Check_Couplings( processB, ProdCouplingsB, DecayCouplingsB)) != 0 ) return ERR_PROCESS;
	
	/// perform computation according to the specified process and MEM package
	switch ( calculator )
	{
		case kMEKD:			/// compute KD with MEKD
			if( ProdCouplingsA!=(vector<complex<double> >*) NULL || DecayCouplingsA!=(vector<complex<double> >*) NULL )
				if( (m_err=MEKD_Mixed_State( m_processNameMEKD[processA], ProdCouplingsA, DecayCouplingsA )) != 0 ) return ERR_COMPUTE;
			if( (m_MEKD->computeME(m_processNameMEKD[processA], partP, partId, me2processA)) != 0 ) return ERR_COMPUTE;
			
			if( ProdCouplingsB!=(vector<complex<double> >*) NULL || DecayCouplingsB!=(vector<complex<double> >*) NULL )
				if( (m_err=MEKD_Mixed_State( m_processNameMEKD[processB], ProdCouplingsB, DecayCouplingsB )) != 0 ) return ERR_COMPUTE;
			if( (m_MEKD->computeME(m_processNameMEKD[processB], partP, partId, me2processB)) != 0 ) return ERR_COMPUTE;
			
			kd = log( me2processA/me2processB );
			break;
			
		case kAnalytical:	/// compute KD with MELA
			if(cacheMELAcalculation(processA,calculator,partP,partId,me2processA) || 
				cacheMELAcalculation(processB,calculator,partP,partId,me2processB) )
				return ERR_COMPUTE;
			else{
				kd=me2processA/(me2processA+me2processB);
				return NO_ERR;
			}
			break;
			
		case kJHUGen:		/// compute KD with JHUGen
			if(cacheMELAcalculation(processA,calculator,partP,partId,me2processA) || 
				cacheMELAcalculation(processB,calculator,partP,partId,me2processB) )
				return ERR_COMPUTE;
			else{
				kd=me2processA/(me2processA+me2processB);
				return NO_ERR;
			}
			break;
			
		case kMCFM:			/// compute KD with MCFM
			if(cacheMELAcalculation(processA,calculator,partP,partId,me2processA) || 
				cacheMELAcalculation(processB,calculator,partP,partId,me2processB) )
				return ERR_COMPUTE;
			else{
				kd=me2processA/(me2processA+me2processB);
				return NO_ERR;
			}
			break;
			
		case kMELA_HCP:		/// compute KD with HCP MELA
			return ERR_PROCESS;
			break;
			
		default:
			return ERR_PROCESS;
			break;
	}
  
	return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeMEs - Compute MEs for the supported set of processes.
///----------------------------------------------------------------------------------------------
int MEMs::computeMEs(vector<TLorentzVector> partP, vector<int> partId)
{
	double me2process;
	
	//loop over MEMCalcs and loop over Processes
	for(int iMemCalc = 0; iMemCalc < NUM_MEMCALCS; iMemCalc++ )
	{
		for(int iProcess = 0; iProcess < NUM_PROCESSES_PRECALC; iProcess++ )
		{
			if (!isProcSupported[iProcess][iMemCalc]) continue;
			if( (computeME(static_cast<Processes>(iProcess), static_cast<MEMCalcs>(iMemCalc), partP, partId, me2process)) != 0 ) return ERR_COMPUTE;
			m_computedME[iProcess][iMemCalc] = me2process;
		}
	}
	
	// compute and store the sig. and bkg. pdf(m4l) values for all systs
	computePm4ls(partP, partId);
	
	//return NO_ERR only if all ME computations were successful
	return 0;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computePm4ls - Compute pdf(m4l) with computePm4l(...) for all systs
///----------------------------------------------------------------------------------------------
void MEMs::computePm4ls(vector<TLorentzVector> partP, vector<int> partId)
{
    for(int iSuperKDsyst = 0; iSuperKDsyst < NUM_SuperKDsyst; iSuperKDsyst++ ) {
        double sigProb, bkgProb;
        computePm4l(partP, partId, static_cast<SuperKDsyst>(iSuperKDsyst), sigProb, bkgProb);
        m_computedPm4lSig[iSuperKDsyst] = sigProb;
        m_computedPm4lBkg[iSuperKDsyst] = bkgProb;
    }
}



///----------------------------------------------------------------------------------------------
/// MEMs::retrieveME - Retrieve ME for specified process and specified calculator tool.
///----------------------------------------------------------------------------------------------
int MEMs::retrieveME(Processes process, MEMCalcs calculator, double& me2process)
{
    /// check if process is supported
    if (!isProcSupported[process][calculator]) return ERR_PROCESS;
    
    /// retrieve ME
    me2process = m_computedME[process][calculator];
    
    return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeKD - Compute KD for process A and process B, for specified calculator.
///----------------------------------------------------------------------------------------------
int MEMs::computeKD(Processes processA, MEMCalcs calculatorA, Processes processB, MEMCalcs calculatorB, double (MEMs::*funcKD)(double, double), double& kd, double& me2processA, double& me2processB )
{
    /// check if processes are supported
    if (!isProcSupported[processA][calculatorA]) return ERR_PROCESS;
    if (!isProcSupported[processB][calculatorB]) return ERR_PROCESS;

	/// retrieve already computed MEs
    me2processA = m_computedME[processA][calculatorA];
    me2processB = m_computedME[processB][calculatorB];
	/// compute KD
    kd = (*this.*funcKD)(me2processA, me2processB);
    
    return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeKD - Compute KD for process A and process B, for specified calculator.
///----------------------------------------------------------------------------------------------
int MEMs::computeKD(Processes processA, MEMCalcs calculatorA, Processes processB, MEMCalcs calculatorB, double (MEMs::*funcKD)(Processes, MEMCalcs, Processes, MEMCalcs), double& kd, double& me2processA, double& me2processB )
{
    /// check if processes are supported
    if (!isProcSupported[processA][calculatorA]) return ERR_PROCESS;
    if (!isProcSupported[processB][calculatorB]) return ERR_PROCESS;
    
    /// retrieve already computed MEs
    me2processA = m_computedME[processA][calculatorA];
    me2processB = m_computedME[processB][calculatorB];
    /// compute KD
    kd = (*this.*funcKD)(processA, calculatorA, processB, calculatorB);
    
    return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::computeKD - Compute KD for process A and process B with pdf(m4l) folded in.
///----------------------------------------------------------------------------------------------
int MEMs::computeKD(Processes processA, MEMCalcs calculatorA, Processes processB, MEMCalcs calculatorB, double (MEMs::*funcKD)(double, double, SuperKDsyst), double& kd, double& me2processA, double& me2processB, SuperKDsyst syst )
{
    /// check if processes are supported
    if( !isProcSupported[processA][calculatorA] ) return ERR_PROCESS;
    if( !isProcSupported[processB][calculatorB] ) return ERR_PROCESS;
    /// check if processB is kqqZZ or kqqZZ_prodIndep
    if( processB != kqqZZ && processB != kqqZZ_prodIndep ) return ERR_PROCESS;

	/// retrieve already computed MEs
    me2processA = m_computedME[processA][calculatorA];
    me2processB = m_computedME[processB][calculatorB];
	/// compute KD with pdf(m4l) folded in
    kd = (*this.*funcKD)(me2processA, me2processB, syst);

    return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// MEMs::logRatio - KD function which returns ln( me2processA / me2processB )
///----------------------------------------------------------------------------------------------
double MEMs::logRatio(double me2processA, double me2processB){
	if (me2processB == 0) return -999.;
	return log( me2processA / me2processB );
}



///----------------------------------------------------------------------------------------------
/// MEMs::probRatio - KD function which returns me2processA / ( me2processA + c * me2processB )
///----------------------------------------------------------------------------------------------
double MEMs::probRatio(Processes processA, MEMCalcs calculatorA, Processes processB, MEMCalcs calculatorB)
{
    /// check if processes are supported
    if (!isProcSupported[processA][calculatorA]) return ERR_PROCESS;
    if (!isProcSupported[processB][calculatorB]) return ERR_PROCESS;

    /// retrieve already computed MEs
    double me2processA = m_computedME[processA][calculatorA];
    double me2processB = m_computedME[processB][calculatorB];

    // compute KD per case basis (for the time being, find more elegant solution later)
    double c;
    // determine c
    // keep c = 1. for all MELA_HCP calculations
    // if case is not known, use c = 1. - this is not necessarily wrong though.
    if( (calculatorA==kJHUGen || calculatorA==kMEKD) && (calculatorB==kJHUGen || calculatorB==kMEKD) ){ // (JHUGen or MEKD)
        if( processA==kSMHiggs && processB==k0minus ){
            c = 1.; // for JHUGen or MEKD when 0+ vs 0-
        }else if( processA==kSMHiggs && processB==k2mplus_gg ){
            c = 1.; // for JHUGen or MEKD when 0+ vs 2m+
        }else if ( processB==kqqZZ ){
            c = 1.;
//            me2processB = qqZZ_MCFMNorm; // qqZZ_MCFMNorm/qqZZ_MCFM should be used for (JHUGen or MEKD) signal vs MEKD bkg
        }else{
            c = 1.; // default for all "non-known" cases
        }
    }else if( (calculatorA==kJHUGen || calculatorA==kMEKD) && (calculatorB==kMCFM) ){ // (JHUGen or MEKD) vs. MCFM
        if( processB==kqqZZ ){
            c = 1.;
//            me2processB = qqZZ_MCFMNorm; // qqZZ_MCFMNorm/qqZZ_MCFM should be used for (JHUGen or MEKD) signal vs MCFM bkg
        }else{
            c = 1.; // default for all "non-known" cases
        }
    }else{
        c = 1.; // default for all "non-known" cases
    }
    
    if (me2processA + c * me2processB == 0) return -999.;
    return me2processA/( me2processA + c * me2processB );
}



///----------------------------------------------------------------------------------------------
/// MEMs::PDFm4lRatio - KD function: Pm4lSig * me2sig / ( Pm4lSig * me2sig + Pm4lBkg * me2bkg )
///----------------------------------------------------------------------------------------------
double MEMs::PDFm4lRatio(double me2processA, double me2processB, SuperKDsyst syst)
{
    if (m_computedPm4lSig[syst] * me2processA + m_computedPm4lBkg[syst] * me2processB == 0) return -999.;
    return m_computedPm4lSig[syst]*me2processA/( m_computedPm4lSig[syst] * me2processA + m_computedPm4lBkg[syst] * me2processB );
}



int MEMs::Check_Couplings( Processes process, vector<complex<double> > *ProdCouplings, vector<complex<double> > *DecayCouplings )
{
	if( process==kSpin0_gg || process==kSpin0_prodIndep )	// checking Spin-0 case
	{
		if( ProdCouplings!=(vector<complex<double> >*) NULL && (*DecayCouplings).size() != 4 && (*DecayCouplings).size() != 20)
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided couplings decay. Expected size: 4 or 20 (form-factor case), provided: "  << (*DecayCouplings).size() << endl;
			return 1;
		}
		
		return 0;
	}
	else if( process==kSpin1_qqbar || process==kSpin1_prodIndep )	// checking Spin-1 case
	{
		if( process==kSpin1_qqbar && (*ProdCouplings).size() != 2 )
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided prod. couplings. Expected size: 4, provided: "  << (*ProdCouplings).size() << endl;
			return 1;
		}
		if( (*DecayCouplings).size() != 2 )
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided decay couplings. Expected size: 2, provided: "  << (*DecayCouplings).size() << endl;
			return 1;
		}
		
		return 0;
	}
	else if( process==kSpin2_gg || process==kSpin2_qqbar || process==kSpin2_prodIndep )	// checking Spin-2 case
	{
		if( process==kSpin2_gg && (*ProdCouplings).size() != 10 && (*ProdCouplings).size() != 5)
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided prod. couplings. Expected size: 10 for MEKD, 5 for MELA, provided: "  << (*ProdCouplings).size() << endl;
			return 1;
		}
		if( process==kSpin2_qqbar && (*ProdCouplings).size() != 4 )
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided prod. couplings. Expected size: 4, provided: "  << (*ProdCouplings).size() << endl;
			return 1;
		}
		if( (*DecayCouplings).size() != 10 )
		{
			if( debug ) cout << "MEMs::Check_Couplings. Error in provided decay couplings. Expected size: 10, provided: "  << (*DecayCouplings).size() << endl;
			return 1;
		}
		
		return 0;
	}
	
	
	return 1;
}



///----------------------------------------------------------------------------------------------
/// MEMs::MEKD_Mixed_State - Sets up a mixed state in the MEKD
///----------------------------------------------------------------------------------------------
int MEMs::MEKD_Mixed_State( TString Model, vector<complex<double> > *ProdCouplings, vector<complex<double> > *DecayCouplings )
{
	/// Setting couplings inside of MEKD
	if( Model=="ggSpin0" || Model=="Spin0" )
	{
		m_MEKD->m_Mixing_Coefficients_Spin0[0] = (*DecayCouplings)[0];
		m_MEKD->m_Mixing_Coefficients_Spin0[1] = (*DecayCouplings)[1];
		m_MEKD->m_Mixing_Coefficients_Spin0[2] = (*DecayCouplings)[2];
		m_MEKD->m_Mixing_Coefficients_Spin0[3] = (*DecayCouplings)[3];
		
		return 0;
	}
	else if( Model=="qqSpin1" || Model=="Spin1" )
	{
		if( Model=="qqSpin1" ) m_MEKD->m_Mixing_Coefficients_Spin1[0] = (*ProdCouplings)[0];
		if( Model=="qqSpin1" ) m_MEKD->m_Mixing_Coefficients_Spin1[1] = (*ProdCouplings)[1];
		if( Model=="qqSpin1" ) m_MEKD->m_Mixing_Coefficients_Spin1[2] = (*ProdCouplings)[2];
		if( Model=="qqSpin1" ) m_MEKD->m_Mixing_Coefficients_Spin1[3] = (*ProdCouplings)[3];
		m_MEKD->m_Mixing_Coefficients_Spin1[4] = (*DecayCouplings)[0];
		m_MEKD->m_Mixing_Coefficients_Spin1[5] = (*DecayCouplings)[1];
		
		return 0;
	}
	else if( Model=="ggSpin2" || Model=="qqSpin2" || Model=="Spin2" )
	{
		if( Model=="ggSpin2" || Model=="qqSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[0] = (*ProdCouplings)[0];
		if( Model=="ggSpin2" || Model=="qqSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[1] = (*ProdCouplings)[1];
		if( Model=="ggSpin2" || Model=="qqSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[2] = (*ProdCouplings)[2];
		if( Model=="ggSpin2" || Model=="qqSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[3] = (*ProdCouplings)[3];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[4] = (*ProdCouplings)[4];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[5] = (*ProdCouplings)[5];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[6] = (*ProdCouplings)[6];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[7] = (*ProdCouplings)[7];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[8] = (*ProdCouplings)[8];
		if( Model=="ggSpin2" ) m_MEKD->m_Mixing_Coefficients_Spin2[9] = (*ProdCouplings)[9];
		m_MEKD->m_Mixing_Coefficients_Spin2[10] = (*DecayCouplings)[0];
		m_MEKD->m_Mixing_Coefficients_Spin2[11] = (*DecayCouplings)[1];
		m_MEKD->m_Mixing_Coefficients_Spin2[12] = (*DecayCouplings)[2];
		m_MEKD->m_Mixing_Coefficients_Spin2[13] = (*DecayCouplings)[3];
		m_MEKD->m_Mixing_Coefficients_Spin2[14] = (*DecayCouplings)[4];
		m_MEKD->m_Mixing_Coefficients_Spin2[15] = (*DecayCouplings)[5];
		m_MEKD->m_Mixing_Coefficients_Spin2[16] = (*DecayCouplings)[6];
		m_MEKD->m_Mixing_Coefficients_Spin2[17] = (*DecayCouplings)[7];
		m_MEKD->m_Mixing_Coefficients_Spin2[18] = (*DecayCouplings)[8];
		m_MEKD->m_Mixing_Coefficients_Spin2[19] = (*DecayCouplings)[9];
		
		return 0;
	}
	
	return 1;
}



//////////////////////////////////////////////////////////////////////////
///----------------------------------------------------------------------------------------------
/// MEMCalculators::cacheMELAcalculation - method to interface with Mela::computeP and cache results
///----------------------------------------------------------------------------------------------
int MEMs::cacheMELAcalculation(Processes process, MEMCalcs calculator,vector<TLorentzVector> partP, vector<int> partId, double& me2process)
{
	return MEMs::cacheMELAcalculation( static_cast<int>(process), calculator, partP, partId, (vector<complex<double> >*) NULL, (vector<complex<double> >*) NULL, me2process);
}



///----------------------------------------------------------------------------------------------
/// MEMCalculators::cacheMELAcalculation - method to interface with Mela::computeP and cache results. A transfer function
///----------------------------------------------------------------------------------------------
int MEMs::cacheMELAcalculation(Processes process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, vector<complex<double> > *ProdCouplings, vector<complex<double> > *DecayCouplings, double& me2process)
{
	return cacheMELAcalculation( static_cast<int>(process), calculator, partP, partId, ProdCouplings, DecayCouplings, me2process);
}



///----------------------------------------------------------------------------------------------
/// MEMCalculators::cacheMELAcalculation - method to interface with Mela::computeP and cache results. A generic case
///----------------------------------------------------------------------------------------------
int MEMs::cacheMELAcalculation(int process, MEMCalcs calculator, vector<TLorentzVector> partP, vector<int> partId, vector<complex<double> > *ProdCouplings, vector<complex<double> > *DecayCouplings, double& me2process){

  if( debug ){
    std::cout << "MEMs::cacheMELAcalculation started." << std::endl;
    std::cout << "MEMs::cacheMELAcalculation. Process: " << process << std::endl;
    std::cout << "MEMs::cacheMELAcalculation. Calculator: " << calculator << std::endl;
  }

  partPCache = partP;
  partIdCache = partId;

  // NEED TO INCLUDE SOME PROTECTION SO THAT USER CANT 
  // PASS FOUR-VECTORS IN WRONG ORDER.  FOR NOW ASSUMING
  // THEY ARE PASSED AS e-,e+,mu-,mu+
  // ------------------ channel ------------------------
  int flavor;
  
  if(abs(partId[0])==abs(partId[1])&&
     abs(partId[0])==abs(partId[2])&&
     abs(partId[0])==abs(partId[3])){
    
    if(abs(partId[0])==11) flavor=1;
    else flavor=2;
    
  }else flavor=3;
  
  if( debug )
    std::cout << "MEMs::cacheMELAcalculation. Flavor: "  << flavor << std::endl;
  
  // ---------------------------------------------------
  // ---------- COMPUTE ANGLES and MASSES --------------
  
  if( debug )
    std::cout << "MEMs::cacheMELAcalculation. Computing angles" << std::endl;
  
  float costheta1, costheta2, costhetastar;
  float phi, phi1;
  
  mela::computeAngles(partP[0], partId[0], partP[1], partId[1], 
		      partP[2], partId[2], partP[3], partId[3],
		      costhetastar,costheta1,costheta2,phi,phi1);
  
  // protections against NaNs, return ERR_COMPUTE and skip ME calc
  if(TMath::IsNaN(costhetastar)||
     TMath::IsNaN(costheta1)||
     TMath::IsNaN(costheta2)||
     TMath::IsNaN(phi)||
     TMath::IsNaN(phi1) ) return ERR_COMPUTE; 
  
	float m1=(partP[0] + partP[1]).M();
	float m2=(partP[2] + partP[3]).M();
	
	TLorentzVector ZZ = (partP[0] + partP[1] + partP[2] + partP[3]);
	
	//if(MELAprocMap[static_cast<Processes>(process)] == TVar::HJJVBF || MELAprocMap[static_cast<Processes>(process)] == TVar::PSHJJVBF || MELAprocMap[static_cast<Processes>(process)] == TVar::HJJNONVBF || MELAprocMap[static_cast<Processes>(process)] == TVar::PSHJJNONVBF )
	if(MELAprodMap[static_cast<Processes>(process)] == TVar::JJVBF|| MELAprodMap[static_cast<Processes>(process)] == TVar::JJGG || MELAprodMap[static_cast<Processes>(process)] == TVar::JJVH)
	{
		float me2process_float;
		m_MELA->setProcess(MELAprocMap[static_cast<Processes>(process)],MELAcalcMap[calculator],MELAprodMap[static_cast<Processes>(process)]);
		m_MELA->computeProdP(partP[4],2,partP[5],2,ZZ,25,0.,0,me2process_float);
		me2process = (double) me2process_float;
	}
	else
	{
		float mzz = ZZ.M();
		
		float pt4l  = ZZ.Pt();
		float Y4l   = ZZ.Rapidity(); // Fixme: should probably protect against NaN?
		
		if( debug )
		{
			cout << "MEMs::cacheMELAcalculation. mzz: " << mzz << endl;
			cout << "MEMs::cacheMELAcalculation. m1: "  << m1 << endl;
			cout << "MEMs::cacheMELAcalculation. m2: " << m2 << endl;
			cout << "MEMs::cacheMELAcalculation. costheta1: " << costheta1 << endl;
			cout << "MEMs::cacheMELAcalculation. costheta2: " << costheta2 << endl;
			cout << "MEMs::cacheMELAcalculation. costhetastar: " << costhetastar << endl;
			cout << "MEMs::cacheMELAcalculation. phi: " << phi << endl;
			cout << "MEMs::cacheMELAcalculation. phi1: " << phi1 << endl;
			cout << "MEMs::cacheMELAcalculation. pT4l: " << pt4l << endl;
			cout << "MEMs::cacheMELAcalculation. Y4l: " << Y4l << endl;
		}
		
		// retrieve ME calculations
		// ---------------------------------------------------
		
		float me2process_float;
		
		
		if( process==kSpin0_gg || process==kSpin0_prodIndep ||
			process==kSpin1_qqbar || process==kSpin1_prodIndep ||
			process==kSpin2_gg || process==kSpin2_qqbar || process==kSpin2_prodIndep || process==kggZZ_SMHiggs )
		{
			if( process==kSpin0_gg || process==kSpin0_prodIndep )
			{
				double translation[20][2];
				if( (*DecayCouplings).size() == 4 )
				{
					for(int i=0;i<4;i++)
					{
						translation[i][0] = (*DecayCouplings)[i].real();
						translation[i][1] = (*DecayCouplings)[i].imag();
					}
					for(int i=4;i<20;i++)
					{
						translation[i][0] = 0.;
						translation[i][1] = 0.; 
					}
				}
				else if( (*DecayCouplings).size() == 20 )
				{
					for(int i=0;i<20;i++)
					{
						translation[i][0] = (*DecayCouplings)[i].real();
						translation[i][1] = (*DecayCouplings)[i].imag();
					}
				}
				else
				{
					cout<< "expect 4/20 decay couplings for mela"<<endl;
				}
				
				//                 TVar::Process        TVar::MatrixElement     TVar::Production
				m_MELA->setProcess( MELAprocMap[static_cast<Processes>(process)], MELAcalcMap[calculator], MELAprodMap[static_cast<Processes>(process)] );
				m_MELA->computeP( mzz, m1, m2,
						costhetastar,costheta1,costheta2,phi,phi1,
						flavor,
						translation,
						me2process_float );
			}
			else if (process == kSpin1_qqbar || process ==kSpin1_prodIndep)
			{
				 double translation[2][2];
				 if((*DecayCouplings).size() == 2)
				 {
					for(int i=0;i<2;i++)
					{
						 translation[i][0] = (*DecayCouplings)[i].real();
						 translation[i][1] = (*DecayCouplings)[i].imag();
					}
					
					m_MELA->setProcess( MELAprocMap[static_cast<Processes>(process)], MELAcalcMap[calculator], MELAprodMap[static_cast<Processes>(process)] );
					m_MELA->computeP_selfDspin1( mzz, m1, m2,
												costhetastar,costheta1,costheta2,phi,phi1,
												flavor,
												translation,
												me2process_float );
				}
			}
			else if(process==kSpin2_gg || process==kSpin2_qqbar || process==kSpin2_prodIndep )
			{
				double translation[10][2];
				double translationProd[5][2];
				if((*DecayCouplings).size() == 10 && (*ProdCouplings).size()==5 )
				{
					for(int i=0;i<10;i++)
					{
						translation[i][0] = (*DecayCouplings)[i].real();
						translation[i][1] = (*DecayCouplings)[i].imag();
					}
					for(int i=0;i<5;i++)
					{
						translationProd[i][0] = (*ProdCouplings)[i].real();
						translationProd[i][1] = (*ProdCouplings)[i].imag();
					}
					
					m_MELA->setProcess( MELAprocMap[static_cast<Processes>(process)], MELAcalcMap[calculator], MELAprodMap[static_cast<Processes>(process)] );
					m_MELA->computeP_selfDspin2( mzz, m1, m2,
												costhetastar,costheta1,costheta2,phi,phi1,
												flavor,
												translationProd,translation,
												me2process_float );
				}
			} 
		else if (process==kggZZ_SMHiggs)
		{
          m_MELA->setProcess( MELAprocMap[static_cast<Processes>(process)], MELAcalcMap[calculator], MELAprodMap[static_cast<Processes>(process)] );
			double translation[2];
       if(DecayCouplings!= (vector<complex<double> >*) NULL  )
        {
					translation[0] = (*DecayCouplings)[0].real();
					translation[1] = (*DecayCouplings)[0].imag();
				
          m_MELA->computeP( mzz, m1, m2,
                        costhetastar,costheta1,costheta2,phi,phi1,
                        flavor,
                        translation,
                        me2process_float );
			}
			else
			{
				          m_MELA->computeP( mzz, m1, m2,
                        costhetastar,costheta1,costheta2,phi,phi1,
                        flavor,
                        me2process_float );
			}
		}
			
			else return ERR_PROCESS;	// not yet implemented
		}
		else if( process==kg1g4 || process==kg1g2 || process==kg1g4_pi_2 ||
			process==kg1g2_pi_2 || process==k_g1g1prime2 )
		{
			m_MELA->computeD_CP( mzz, m1, m2,
								costhetastar,costheta1,costheta2,phi,phi1,
								flavor,MELAcalcMap[calculator],MELAprocIntMap[static_cast<Processes_int>(process)],
								me2process_float );
		}
		else
		{
			//                 TVar::Process        TVar::MatrixElement     TVar::Production
			m_MELA->setProcess(MELAprocMap[static_cast<Processes>(process)],MELAcalcMap[calculator],MELAprodMap[static_cast<Processes>(process)]);
			
			// check if ZZ_4e is configured and event is 2e2mu event
	//		if(MELAprocMap[static_cast<Processes>(process)]==TVar::ZZ_4e && MELAcalcMap[calculator]==TVar::MCFM && flavor==3)
	//			m_MELA->setProcess(TVar::ZZ_2e2m,MELAcalcMap[calculator],MELAprodMap[static_cast<Processes>(process)]);
			
			m_MELA->computeP(mzz, m1, m2,
				costhetastar,costheta1,costheta2,phi,phi1,
				flavor,
				me2process_float);
		}
		
		
		me2process = (double) me2process_float;
		
		if( debug ) cout << "MEMs::cacheMELAcalculation. me2process: " << me2process << endl;
		
		/// Add weight calculation
		m_MELA->computeWeight(mzz, m1,  m2,
				costhetastar,
				costheta1,
				costheta2,
				phi,
				phi1,
				// return variables:
				m_weight);
	}
	
	if( debug )
		cout << "MEMs::cacheMELAcalculation. Done!" << endl;
	
	return NO_ERR;
}



///----------------------------------------------------------------------------------------------
/// interface for calculating P(m4l) for superKD
///----------------------------------------------------------------------------------------------
/// Possible syst values:
///   kNone: nominal shape is used
///   kScaleUp/kScaleDown: mean mass shifted up/down appropriate scale error
///   kResolUp/kResolDown: width is varied by appropriate resolution error
///----------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
void MEMs::computePm4l(vector<TLorentzVector> partP,
		  vector<int> partId,
		  SuperKDsyst syst,
		  double& sigProb,
		  double& bkgProb){
  
  float prob_float;

  m_MELA->setProcess(TVar::bkgZZ,TVar::JHUGen,TVar::ZZGG);

  m_MELA->computePM4l(partP[0],partId[0],
		      partP[1],partId[1],
		      partP[2],partId[2],
		      partP[3],partId[3],
		      (TVar::SuperMelaSyst)syst,prob_float);

  bkgProb = (double) prob_float;

  m_MELA->setProcess(TVar::HSMHiggs,TVar::JHUGen,TVar::ZZGG);

  m_MELA->computePM4l(partP[0],partId[0],
		      partP[1],partId[1],
		      partP[2],partId[2],
		      partP[3],partId[3],
		      (TVar::SuperMelaSyst)syst,prob_float);

  sigProb = (double) prob_float;

}

#endif
