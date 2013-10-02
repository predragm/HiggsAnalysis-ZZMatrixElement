/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOSPINONE_7D
#define ROOSPINONE_7D

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
using namespace TMath;

class RooSpinOne_7D : public RooAbsPdf {
public:
  RooSpinOne_7D() {} ; 
  RooSpinOne_7D(const char *name, const char *title,
		RooAbsReal& _mzz,
		RooAbsReal& _m1,
		RooAbsReal& _m2,
		RooAbsReal& _h1,
		RooAbsReal& _h2,
		RooAbsReal& _hs,
		RooAbsReal& _Phi,
		RooAbsReal& _Phi1,
		RooAbsReal& _g1Val,
		RooAbsReal& _g2Val,
		RooAbsReal& _R1Val,
		RooAbsReal& _R2Val,
		RooAbsReal& _aParam,
                RooAbsReal& _mZ,
                RooAbsReal& _gamZ);
  RooSpinOne_7D(const RooSpinOne_7D& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooSpinOne_7D(*this,newname); }
  inline virtual ~RooSpinOne_7D() { }
  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;
  
 protected:

	RooRealProxy mzz ;
	RooRealProxy m1 ;
	RooRealProxy m2 ;
	RooRealProxy h1 ;
	RooRealProxy h2 ;
	RooRealProxy hs ;
	RooRealProxy Phi ;
	RooRealProxy Phi1 ;
	RooRealProxy g1Val ;
	RooRealProxy g2Val ;
	RooRealProxy R1Val ;
	RooRealProxy R2Val ;
	RooRealProxy aParam ;
	RooRealProxy mZ ;
	RooRealProxy gamZ ;
	Double_t evaluate() const ;
	
 private:
	
	//	ClassDef(RooSpinOne_7D,1) // Your description goes here...
};

#endif
