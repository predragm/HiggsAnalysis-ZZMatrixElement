///////////////////////////////
// Add Probabilities to tree //
///////////////////////////////

using namespace RooFit;
bool includePathIsSet = false;
//void addProbtoTree(char* inputFile= ,int flavor, int max, int LHCsqrts=8){
//void addProbtoTree(char* inputFile= "ggtoHtoZZ4l-PJ-125_6-8TeV-0+m_1_withDisc",int flavor=3, int max=5000, int LHCsqrts=8){
//void addProbtoTree(char* inputFile= "ggtoHtoZZ4l-PJ-125_6-8TeV-0-_1_withDisc",int flavor=3, int max=5000, int LHCsqrts=8){
//void addProbtoTree(char* inputFile= "ggtoHtoZZ4l-PJ-125_6-8TeV-0+m0-_1_withDisc",int flavor=3, int max=5000, int LHCsqrts=8){
  //flavor: 1:4e, 2:4mu, 3:2e2mu
//void addProbtoTree(char* inputFile= "ggtoHtoZZ4l_VariousKDs_0+h",int flavor=3, int max=5000, int LHCsqrts=8){

void addProbtoTree(char* inputFile= "4e/HZZ4lTree_powheg15jhuGenV3H126_withDisc_new",int flavor=1, int max=5000, int LHCsqrts=8){
//void addProbtoTree(char* inputFile= "ggtoHtoZZ4l_VariousKDs_0+m0+h",int flavor=3, int max=5000, int LHCsqrts=8){


  gSystem->Load("$CMSSW_BASE/src/ZZMatrixElement/MELA/data/$SCRAM_ARCH/libmcfm.so");
  gSystem->Load("$CMSSW_BASE/lib/slc5_amd64_gcc462/libZZMatrixElementMELA.so");
//  gROOT->LoadMacro("$CMSSW_BASE/src/ZZMatrixElement/MELA/interface/Mela.h+");
  // set up path for local cmssw includes
  // as well as roofit
  if (!includePathIsSet) {
    TString path = gSystem->GetIncludePath();
    path += "-I$CMSSW_BASE/src/ ";
    path += "-I$ROOFITSYS/include/ ";
    gSystem->SetIncludePath(path.Data());

    // this is awkward, but want to protect against 
    // multiple additions to the base include path
    // if this function is invoked multiple times per session
    includePathIsSet = true;
  }

  gROOT->LoadMacro("$CMSSW_BASE/src/ZZMatrixElement/MELA/interface/Mela.h+");
  gROOT->LoadMacro("../src/AngularPdfFactory.h+");
  Mela myMELA(8,126);
  
  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);

  char inputFileName[500];
  char outputFileName[500];
  sprintf(inputFileName,"%s.root",inputFile);
  sprintf(outputFileName,"%s_withMELA_new.root",inputFile);

  TFile* sigFile = new TFile(inputFileName);
  TTree* sigTree=0;
    if(sigFile)
        sigTree = (TTree*) sigFile->Get("SelectedTree");
    if(!sigTree){
      //2nd try with the name of data obs tree
      sigTree = (TTree*) sigFile->Get("data_obs");
      if(!sigTree){
	cout<<"ERROR could not find the tree!"<<endl;
	return;
      }
    }


  float m1,m2,mzz,h1,h2,hs,phi,phi1;                                    //angles
  float psig,pbkg,D;
  double oldD;                                                    //legacy probabilities
  float	p0plus_melaNorm,p0plus_mela,p0minus_mela,p0hplus_mela,p0plus_VAJHU,p0minus_VAJHU,p0plus_VAMCFM,p0plus_JHU_VAMCFM,p0hplus_VAJHU,p1_mela,p1_self_mela, p1_self_jhu,p1plus_mela,p1_VAJHU,p1plus_VAJHU,p2_mela,p2_self_mela,p2_self_jhu,p2qqb_mela,p2_VAJHU,p2qqb_VAJHU; // new signal probablities
  float bkg_mela, bkg_VAMCFM, bkg_VAMCFMNorm, ggzz_VAMCFM, bkg_decay_VAMCFM;    // background probabilities

float D_mela_CP, D_mela_0m, D_VAJHU_CP, D_VAJHU_0m; 
float p0plus_mela_check, p0minus_mela_check, p0minus_plus_mela, p0minus_plus_mela_check; 

float p0minus_plus_VAJHU;
  // probabilities for production independent calculations
  float p1_decay_VAJHU, p1plus_decay_VAJHU, p2_decay_VAJHU; 
  // probabilities for exotic spin-2 models
  float p2hminus_VAJHU, p2hplus_VAJHU, p2bplus_VAJHU;
  float p2hminus_mela, p2hplus_mela, p2bplus_mela;
  float p1_decay_mela, p1plus_decay_mela, p2_decay_mela;

float D_CP, D_JHUGen_CP, D_0m, D_JHUGen_0m;
float D_JHUGen_CP, D_JHUGen_CP_T, D_JHUGen_int, D_JHUGen_int_T, D_int;
float D_lambda1_VAJHU,D_lambda1_mela, D_int_lambda1, D_int_lambda1_VAJHU, D_int_lambda1_mela;
float p0plus_m4l, bkg_m4l;
  // -------- CJLST TREES ---------------
  sigTree->SetBranchAddress("Z2Mass",&m2);
  sigTree->SetBranchAddress("Z1Mass",&m1);
  sigTree->SetBranchAddress("ZZMass",&mzz);
  sigTree->SetBranchAddress("costhetastar",&hs);
  sigTree->SetBranchAddress("helcosthetaZ1",&h1);
  sigTree->SetBranchAddress("helcosthetaZ2",&h2);
  sigTree->SetBranchAddress("helphi",&phi);
  sigTree->SetBranchAddress("phistarZ1",&phi1);
  //sigTree->SetBranchAddress("ZZLD",&oldD);
  Y4l=0.0;
  pt4l=0.0;

  float weight;
  sigTree->SetBranchAddress("MC_weight_noxsec",&weight);
  //---------------------------------------*/

  TFile* newFile = new TFile(outputFileName,"RECREATE");
  TTree* newTree = sigTree->CloneTree(0);//new TTree("newTree","SelectedTree"); 

  newTree->Branch("p0plus_mela_NEW",&p0plus_mela,"p0plus_mela_NEW/F");  // higgs, vector algebra, JHUgen
  newTree->Branch("p0minus_mela_NEW",&p0minus_mela,"p0minus_mela_NEW/F");  // pseudoscalar, vector algebra, JHUgen
  newTree->Branch("p0hplus_mela_NEW",&p0hplus_mela,"p0hplus_mela_NEW/F");  // 0h+, analytic distribution 
  newTree->Branch("p0plus_VAJHU_NEW",&p0plus_VAJHU,"p0plus_VAJHU_NEW/F");  // higgs, vector algebra, JHUgen
  newTree->Branch("p0minus_VAJHU_NEW",&p0minus_VAJHU,"p0minus_VAJHU_NEW/F");  // pseudoscalar, vector algebra, JHUgen
  newTree->Branch("p0plus_VAMCFM",&p0plus_VAMCFM,"p0plus_VAMCFM/F");  // higgs, vector algebra, MCFM
  newTree->Branch("p0plus_JHU_VAMCFM",&p0plus_JHU_VAMCFM,"p0plus_JHU_VAMCFM/F");  // higgs, vector algebra, MCFM
  newTree->Branch("p0hplus_VAJHU_NEW",&p0hplus_VAJHU,"p0hplus_VAJHU_NEW/F");  // 0h+(high order dimension operator) , vector algebra, JHUgen


  newTree->Branch("D_mela_CP",&D_mela_CP,"D_mela_CP/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) MELA 
  newTree->Branch("D_mela_0m",&D_mela_0m,"D_mela_0m/F");  // p(0+)/(p(0+)+p(0-)) MELA 

  newTree->Branch("D_JHUGen_CP",&D_JHUGen_CP,"D_JHUGen_CP/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) MELA
  newTree->Branch("D_JHUGen_0m",&D_JHUGen_0m,"D_JHUGen_0m/F");  // p(0+)/(p(0+)+p(0-)) MELA

  newTree->Branch("D_CP",&D_CP,"D_CP/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) MELA
  newTree->Branch("D_0m",&D_0m,"D_0m/F");  // p(0+)/(p(0+)+p(0-)) MELA

   newTree->Branch("p0plus_mela_check",&p0plus_mela_check,"p0plus_mela_check/F");  // p(0+)/(p(0+)+p(0-)) MELA
   newTree->Branch("p0minus_mela_check",&p0minus_mela_check,"p0minus_mela_check/F");  // p(0+)/(p(0+)+p(0-)) MELA
   newTree->Branch("p0minus_plus_mela",&p0minus_plus_mela,"p0minus_plus_mela/F");  // p(0+)/(p(0+)+p(0-)) MELA
   newTree->Branch("p0minus_plus_mela_check",&p0minus_plus_mela_check,"p0minus_plus_mela_check/F");  // p(0+)/(p(0+)+p(0-)) MELA

  newTree->Branch("D_VAJHU_CP",&D_VAJHU_CP,"D_VAJHU_CP/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU 
  newTree->Branch("D_VAJHU_0m",&D_VAJHU_0m,"D_VAJHU_0m/F");  // p(0+)/(p(0+)+p(0-)) JHU

  newTree->Branch("D_JHUGen_CP",&D_JHUGen_CP,"D_JHUGen_CP/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU
  newTree->Branch("D_JHUGen_CP_T",&D_JHUGen_CP_T,"D_JHUGen_CP_T/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU
  newTree->Branch("D_JHUGen_int",&D_JHUGen_int,"D_JHUGen_int/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU
  newTree->Branch("D_JHUGen_int_T",&D_JHUGen_int_T,"D_JHUGen_int_T/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU
  newTree->Branch("D_int",&D_int,"D_int/F");  // p(0+,0-)-p(0-)-p(0+)/(p(0+)+p(0-)) JHU

  newTree->Branch("D_lambda1_VAJHU_NEW",&D_lambda1_VAJHU,"D_lambda1_VAJHU_NEW/F");  // p(0+)/p(0+)+p(Q2) 

  newTree->Branch("D_int_lambda1_NEW",&D_int_lambda1,"D_int_lambda1_NEW/F");  // p(0+)/p(0+)+p(Q2) 
  newTree->Branch("D_int_lambda1_VAJHU_NEW",&D_int_lambda1_VAJHU,"D_int_lambda1_VAJHU_NEW/F");  // p(0+)/p(0+)+p(Q2) 
  newTree->Branch("D_int_lambda1_mela_NEW",&D_int_lambda1_mela,"D_int_lambda1_mela_NEW/F");  // p(0+)/p(0+)+p(Q2) 

float pq2_VAJHU, pq2_mela;
float pq2sm;

  newTree->Branch("pq2_VAJHU_NEW",&pq2_VAJHU,"pq2_VAJHU_NEW/F");  // p(0+)/p(0+)+p(Q2) 
  newTree->Branch("pq2_mela_NEW",&pq2_mela,"pq2_mela_NEW/F");  // p(0+)/p(0+)+p(Q2) 
  newTree->Branch("pq2sm_NEW",&pq2sm,"pq2sm_NEW/F");  // p(0+)/p(0+)+p(Q2) 

  newTree->Branch("p1_mela_NEW",&p1_mela,"p1_mela_NEW/F");  // zprime, analytic distribution 
  newTree->Branch("p1plus_mela_NEW",&p1plus_mela,"p1plus_mela_NEW/F");  // 1+, analytic distribution 
  newTree->Branch("p1_VAJHU_NEW",&p1_VAJHU,"p1_VAJHU_NEW/F");  // zprime, vector algebra, JHUgen,
  newTree->Branch("p1plus_VAJHU_NEW",&p1plus_VAJHU,"p1plus_VAJHU_NEW/F");  // 1+ (axial-vector), vector algebra, JHUgen,
  newTree->Branch("p2_mela_NEW",&p2_mela ,"p2_mela_NEW/F");  // graviton, analytic distribution 
  newTree->Branch("p2qqb_mela_NEW",&p2qqb_mela,"p2qqb_mela_NEW/F"); // graviton produced by qqbar vector algebra, analytical,
  newTree->Branch("p2_VAJHU_NEW",&p2_VAJHU,"p2_VAJHU_NEW/F");  // graviton produced by gg, vector algebra, JHUgen,
  newTree->Branch("p2_self_mela",&p2_self_mela,"p2_self_mela/F");  // graviton produced by gg, vector algebra, JHUgen,
  newTree->Branch("p2_self_jhu",&p2_self_jhu,"p2_self_jhu/F");  // graviton produced by gg, vector algebra, JHUgen,
  newTree->Branch("p1_self_jhu",&p1_self_jhu,"p1_self_jhu/F");  // graviton produced by gg, vector algebra, JHUgen,
  newTree->Branch("p1_self_mela",&p1_self_mela,"p1_self_mela/F");  // graviton produced by gg, vector algebra, JHUgen,

  newTree->Branch("p2qqb_VAJHU_NEW",&p2qqb_VAJHU,"p2qqb_VAJHU_NEW/F");  // graviton produced by qqbar, vector algebra, JHUgen,
  newTree->Branch("p2qqb_mela_NEW",&p2qqb_mela,"p2qqb_mela_NEW/F");  // graviton produced by qqbar, analytical,
  newTree->Branch("p1_decay_VAJHU_NEW",&p1_decay_VAJHU,"p1_decay_VAJHU_NEW/F");  // 1-, vector algebra, production indpendent JHUgen
  newTree->Branch("p1_decay_mela_NEW",&p1_decay_mela,"p1_decay_mela_NEW/F");  // 1-, analytical, production indpendent
  newTree->Branch("p1plus_decay_VAJHU_NEW",&p1plus_decay_VAJHU,"p1plus_decay_VAJHU_NEW/F");  // 1+, vector algebra, production indpendent JHUgen
  newTree->Branch("p1plus_decay_mela_NEW",&p1plus_decay_mela,"p1plus_decay_mela_NEW/F");  // 1+, analytical, production indpendent
  newTree->Branch("p2_decay_VAJHU_NEW",&p2_decay_VAJHU,"p2_decay_VAJHU_NEW/F");  // 2m+, vector algebra, production indpendent JHUgen
  newTree->Branch("p2_decay_mela_NEW",&p2_decay_mela,"p2_decay_mela_NEW/F");  // 2m+, analytical, production indpendent JHUgen
  newTree->Branch("p2hminus_VAJHU_NEW",&p2hminus_VAJHU,"p2hminus_VAJHU_NEW/F");  // 2h-, vector algebra, JHUgen,
  newTree->Branch("p2hplus_VAJHU_NEW",&p2hplus_VAJHU,"p2hplus_VAJHU_NEW/F");     // 2h+, vector algebra, JHUgen,
  newTree->Branch("p2hminus_mela_NEW",&p2hminus_mela,"p2hminus_mela_NEW/F");  // 2h-, vector algebra, JHUgen,
  newTree->Branch("p2hplus_mela_NEW",&p2hplus_mela,"p2hplus_mela_NEW/F");     // 2h+, vector algebra, analytical,
  newTree->Branch("p2bplus_VAJHU_NEW",&p2bplus_VAJHU,"p2bplus_VAJHU_NEW/F");     // 2b+, vector algebra, JHUgen,
  newTree->Branch("p2bplus_mela_NEW",&p2bplus_mela,"p2bplus_mela_NEW/F");     // 2b+, analytical

  //backgrounds
  newTree->Branch("bkg_mela_NEW",&bkg_mela,"bkg_mela_NEW/F");  // background,  analytic distribution 
  newTree->Branch("bkg_VAMCFM_NEW",&bkg_VAMCFM,"bkg_VAMCFM_NEW/F");  // background, vector algebra, MCFM
  newTree->Branch("ggzz_VAMCFM_NEW",&ggzz_VAMCFM,"ggzz_VAMCFM_NEW/F");  // background, vector algebra, MCFM for ggzz
  newTree->Branch("bkg_VAMCFMNorm_NEW",&bkg_VAMCFMNorm,"bkg_VAMCFMNorm_NEW/F");  // background, vector algebra, MCFM Normalized
  newTree->Branch("bkg_decay_VAMCFM_NEW",&bkg_decay_VAMCFM,"bkg_decay_VAMCFM_NEW/F");  // background, vector algebra, MCFM integrating out the production angles
  newTree->Branch("p0plus_m4l_NEW",&p0plus_m4l,"p0plus_m4l_NEW/F");  // background, vector algebra, MCFM integrating out the production angles
  newTree->Branch("bkg_m4l_NEW",&bkg_m4l,"bkg_m4l_NEW/F");  // background, vector algebra, MCFM integrating out the production angles
  
  //interference weight
  float interfWeight;
  newTree->Branch("interfWeight",&interfWeight,"interfWeight/F"); // weight to be used for interference reweighting

  for(int iEvt=0; iEvt<(max<0?sigTree->GetEntries():max); iEvt++){
    
    if(iEvt>=sigTree->GetEntries()) break;
    
    // if ( iEvt != 2 ) continue;
    if(iEvt%1000==1) {
      cout<<"---------\n event: "<<iEvt<<endl;
    }
    sigTree->GetEntry(iEvt);

    // 
    // ANALYTICAL calcualtions
    // 

    // qqZZ0+
//    myMELA.setProcess(TVar::ZZ_2e2m, TVar::ANALYTICAL, TVar::ZZQQB);
//    myMELA.computeP(mzz, m1, m2, 
//		    hs,h1,h2,phi,phi1,flavor, 
//		    bkg_mela);


    // 0+
    myMELA.setProcess(TVar::HSMHiggs, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p0plus_mela);
    

//     myMELA.spin0Model->makeCustom(1.,0,0,0);
//   myMELA.computeP(mzz, m1, m2,
//        hs,h1,h2,phi,phi1,flavor,
//        p0plus_mela_check);
// 
//  myMELA.spin0Model->makeCustom(0,0,0,2.521);
//   myMELA.computeP(mzz, m1, m2,
//        hs,h1,h2,phi,phi1,flavor,
//        p0minus_mela_check);
//
//  myMELA.spin0Model->makeCustom(1,0,0,2.521);
//   myMELA.computeP(mzz, m1, m2,
//        hs,h1,h2,phi,phi1,flavor,
//        p0minus_plus_mela_check);
//
//   myMELA.setProcess(TVar::CPMixHZZ_4l, TVar::ANALYTICAL, TVar::ZZGG);
//    myMELA.computeP(mzz, m1, m2,
//        hs,h1,h2,phi,phi1,flavor,
//        p0minus_plus_mela);


//  D_mela_CP = (p0minus_plus_mela - p0minus_mela_check - p0plus_mela_check)/ (p0plus_mela_check+ p0minus_mela_check);
// D_mela_0m = p0plus_mela_check/ (p0plus_mela_check++p0minus_mela_check); 
     // 0-
    myMELA.setProcess(TVar::H0minus, TVar::ANALYTICAL, TVar::ZZGG);

    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p0minus_mela);
	float d_g1g4, d_g1g4_pi2_jhu, d_g1g2, d_g1g2_pi2_jhu, d_g1g4_jhu, d_g1g2_jhu;
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::ANALYTICAL, TVar::D_g1g4, d_g1g4);
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::ANALYTICAL, TVar::D_g1g2, d_g1g2);
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::JHUGen, TVar::D_g1g4_pi_2, d_g1g4_pi2_jhu);
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::JHUGen, TVar::D_g1g2_pi_2, d_g1g2_pi2_jhu);
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::JHUGen, TVar::D_g1g4, d_g1g4_jhu);
  myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::JHUGen, TVar::D_g1g2, d_g1g2_jhu); 

//       myMELA.computeD_0m(mzz, m1, m2,
 //       hs,h1,h2,phi,phi1,flavor, TVar::JHUGen,D_JHUGen_0m);

    myMELA.setProcess(TVar::HSMHiggs, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p0plus_VAJHU);
   
    // 0-
    myMELA.setProcess(TVar::H0minus, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p0minus_VAJHU);

    myMELA.setProcess(TVar::H0hplus, TVar::JHUGen, TVar::ZZGG);
 myMELA.computeP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, p0hplus_VAJHU);
// mixed
//   myMELA.setProcess(TVar::CPMixHZZ_4l, TVar::JHUGen, TVar::ZZGG);
//    myMELA.computeP(mzz, m1, m2,
//        hs,h1,h2,phi,phi1,flavor, p0minus_plus_VAJHU);    
//
//D_VAJHU_0m= p0plus_VAJHU/(p0plus_VAJHU+ p0minus_VAJHU);
//D_VAJHU_CP = (p0minus_plus_VAJHU- p0minus_VAJHU - p0plus_VAJHU)/ (p0plus_VAJHU+ p0minus_VAJHU);

    myMELA.setProcess(TVar::H0hplus, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p0hplus_mela);
   

	D_CP= d_g1g4/ ( p0minus_mela+p0plus_mela);
	D_JHUGen_CP= d_g1g4_jhu / (p0plus_VAJHU+p0minus_VAJHU);
  D_JHUGen_CP_T= d_g1g4_pi2_jhu / (p0plus_VAJHU+p0minus_VAJHU);
  D_int = d_g1g2/ ( p0plus_mela+p0hplus_mela);
  D_JHUGen_int= d_g1g2_jhu / (p0plus_VAJHU+p0hplus_VAJHU);
  D_JHUGen_int_T= d_g1g2_pi2_jhu / (p0plus_VAJHU+p0hplus_VAJHU);
    // 0h+



   myMELA.setProcess(TVar::H0_g1prime2, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,
        pq2_VAJHU);

 D_lambda1_VAJHU = p0plus_VAJHU/ (p0plus_VAJHU+pq2_VAJHU); 

double coupl[20][2] ;
for(int iidx =0 ; iidx <20; iidx ++){
	for(int jidx =0 ; jidx <2; jidx ++){
		coupl[iidx][jidx] = 0.;
	}
}

coupl[0][0] =1.;
coupl[5][0] =-12046.01;

   myMELA.setProcess(TVar::SelfDefine_spin0, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,coupl,
        pq2sm);
 D_int_lambda1 = (pq2sm - p0plus_VAJHU - pq2_VAJHU )/ (pq2_VAJHU + p0plus_VAJHU);

float d_g1g1prime2_jhu;
 myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::JHUGen, TVar::D_g1g1prime2, d_g1g1prime2_jhu);
 D_int_lambda1_VAJHU = d_g1g1prime2_jhu/(pq2_VAJHU+p0plus_VAJHU); 

   myMELA.setProcess(TVar::H0_g1prime2, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,
        pq2_mela);

 D_lambda1_mela = p0plus_mela/ (p0plus_mela+pq2_mela); 
float d_g1g1prime2_mela;
 myMELA.computeD_CP(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor, TVar::ANALYTICAL, TVar::D_g1g1prime2, d_g1g1prime2_mela);
 D_int_lambda1_mela = d_g1g1prime2_mela/(pq2_mela+p0plus_mela); 

//    // 1-
    myMELA.setProcess(TVar::H1minus, TVar::ANALYTICAL, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p1_mela);
    
    // production independent
    myMELA.setProcess(TVar::H1minus, TVar::ANALYTICAL, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
    hs,h1,h2,phi,phi1,flavor, 
    p1_decay_mela);
    
    // 1+
    myMELA.setProcess(TVar::H1plus, TVar::ANALYTICAL, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p1plus_mela);
    
    // production independent
    myMELA.setProcess(TVar::H1plus, TVar::ANALYTICAL, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
    hs,h1,h2,phi,phi1,flavor, 
    p1plus_decay_mela);

		double g1coupl[2][2]={{0.}};
		g1coupl[1][0]=1.;
    myMELA.setProcess(TVar::SelfDefine_spin1, TVar::ANALYTICAL, TVar::ZZQQB);
    myMELA.computeP_selfDspin1(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,g1coupl,p1_self_mela
        );
    myMELA.setProcess(TVar::SelfDefine_spin1, TVar::JHUGen, TVar::ZZQQB);
    myMELA.computeP_selfDspin1(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,g1coupl,p1_self_jhu
        );
    // gg->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2_mela);

    // qqb->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::ANALYTICAL, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2qqb_mela);
    
    // qqb->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::ANALYTICAL, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2_decay_mela);
    
    // 2h-
    myMELA.setProcess(TVar::H2_g8, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2hminus_mela);
    
    // 2h+
    myMELA.setProcess(TVar::H2_g4, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2hplus_mela);
    
    // 2b+
    myMELA.setProcess(TVar::H2_g5, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, 
		    p2bplus_mela);

    double g2coupl[10][2]={{0.}};
    double g2pcoupl[5][2]={{0.}};
		g2pcoupl[0][0]=1.;
    g2coupl[0][0]=1.;
    g2coupl[4][0]=1.;
    myMELA.setProcess(TVar::SelfDefine_spin2, TVar::ANALYTICAL, TVar::ZZGG);
    myMELA.computeP_selfDspin2(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,g2pcoupl,g2coupl,p2_self_mela
        );
    myMELA.setProcess(TVar::SelfDefine_spin2, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP_selfDspin2(mzz, m1, m2,
        hs,h1,h2,phi,phi1,flavor,g2pcoupl,g2coupl,p2_self_jhu
        );

    // 
    // MCFM calcualtionsA
    // 

    // qqZZ 

//    if ( flavor == 3 ) {
      myMELA.setProcess(TVar::bkgZZ, TVar::MCFM, TVar::ZZQQB);
      myMELA.computeP(mzz, m1, m2, 
		      hs,h1,h2,phi,phi1,flavor, bkg_VAMCFM);
//    } else {
//      myMELA.setProcess(TVar::ZZ_4e, TVar::MCFM, TVar::ZZQQB);
//      myMELA.computeP(mzz, m1, m2, 
//		      hs,h1,h2,phi,phi1,flavor, bkg_VAMCFM);
//    }

    // ggZZ
    myMELA.setProcess(TVar::bkgZZ, TVar::MCFM, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, ggzz_VAMCFM);

    // ******  qqZZ-> production independent 
    
   
  //  if ( flavor == 3 ) {
      myMELA.setProcess(TVar::bkgZZ, TVar::MCFM, TVar::ZZINDEPENDENT);
      myMELA.computeP(mzz, m1, m2, 
		      hs,h1,h2,phi,phi1,flavor, bkg_decay_VAMCFM);
//    } else {
//      myMELA.setProcess(TVar::ZZ_4e, TVar::MCFM, TVar::ZZINDEPENDENT);
//      myMELA.computeP(mzz, m1, m2, 
//		      hs,h1,h2,phi,phi1,flavor, bkg_decay_VAMCFM);
//    }
//    */

  // 0+
	    double coupling[2]={0.0,0.0};
	    coupling[0]=1.0;
  myMELA.setProcess(TVar::bkgZZ_SMHiggs, TVar::JHUGen, TVar::ZZGG);
  myMELA.computeP(mzz, m1, m2, 
	    hs,h1,h2,phi,phi1,flavor, coupling,p0plus_JHU_VAMCFM);

  myMELA.setProcess(TVar::bkgZZ_SMHiggs, TVar::MCFM, TVar::ZZGG);
  myMELA.computeP(mzz, m1, m2, 
	    hs,h1,h2,phi,phi1,flavor, coupling,p0plus_VAMCFM);

  // 
  // JHU Gen based signal calculations 
  // 
  
  // 0+
  
  // 0h+
    myMELA.setProcess(TVar::H0hplus, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p0hplus_VAJHU);
    
    // 1-
    myMELA.setProcess(TVar::H1minus, TVar::JHUGen, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p1_VAJHU);
    
    // production independent
    myMELA.setProcess(TVar::H1minus, TVar::JHUGen, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p1_decay_VAJHU);
    
    // 1+
    myMELA.setProcess(TVar::H1plus, TVar::JHUGen, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p1plus_VAJHU);
    
    // production independent
    myMELA.setProcess(TVar::H1plus, TVar::JHUGen, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p1plus_decay_VAJHU);
    
    // gg->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2_VAJHU);
    
    // qqb->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::JHUGen, TVar::ZZQQB);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2qqb_VAJHU);
    
    // qqb->2m+
    myMELA.setProcess(TVar::H2_g1g5, TVar::JHUGen, TVar::ZZINDEPENDENT);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2_decay_VAJHU);
    
    // 2h-
    myMELA.setProcess(TVar::H2_g8, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2hminus_VAJHU);
    
    // 2h+
    myMELA.setProcess(TVar::H2_g4, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2hplus_VAJHU);
    
    // 2b+
    myMELA.setProcess(TVar::H2_g5, TVar::JHUGen, TVar::ZZGG);
    myMELA.computeP(mzz, m1, m2, 
		    hs,h1,h2,phi,phi1,flavor, p2bplus_VAJHU);

//		myMELA->setProcess(HJJNONVBF,TVar::JHUGen,MELAprodMap[process]);
//    myMELA->computeProdP(partP[4],2,partP[5],2,ZZ,25,0.,0,me2process_float);
	    myMELA.setProcess(TVar::HSMHiggs, TVar::JHUGen, TVar::ZZGG);
		myMELA.computePM4l(mzz,flavor,TVar::SMSyst_None,p0plus_m4l);
	    myMELA.setProcess(TVar::bkgZZ, TVar::JHUGen, TVar::ZZGG);
		myMELA.computePM4l(mzz,flavor,TVar::SMSyst_None,bkg_m4l);
    newTree->Fill();
    
  }
  
  newFile->cd();
  newTree->Write("SelectedTree"); 
  newFile->Close();
  
}

