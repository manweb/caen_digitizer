using namespace RooFit;

TRandom *random;

Double_t fitFunction(Double_t *x, Double_t* par)
{
    //Double_t value = par[0]*TMath::Landau(x[0],par[1],par[2]);
    
    Double_t value = 0;
    Double_t xStart = x[0]-5*par[3];
    Double_t xEnd = x[0]+5*par[3];
    int nSteps = 1000;
    Double_t stepSize = (xEnd-xStart)/double(nSteps);
    for (int i=0; i < nSteps; i++) {
        Double_t x2 = xStart + i*stepSize;
        value += TMath::Landau(x2,par[1],par[2])*TMath::Gaus(x2,x[0],par[3]);
    }
    
    value *= par[0];
    
    return value;
}

void AnalyzeMuons()
{
    gStyle->SetOptStat(0);
    //gStyle->SetOptTitle(0);
    
    random = new TRandom();
    
    random->SetSeed(0);
    
    //string filename = "040216_2000V_1mm_muon_ch1_*.root";
    //string filename = "040716_450V_1mm_muon_ch1_001.root";
    //string filename = "040516_4500V_10mm_muon_ch2_001.root";
    //string filename = "040816_2000V_1mm_muon_ch1_CC_20p3hrs_001.root";
    //string filename = "041016_4500V_10mm_muon_ch1_CC_42p4hrs_001.root";
    //string filename = "041116_2000V_1mm_muon_ch1_CC_12p3hrs_back_001.root";
    //string filename = "041016_4500_10mm_muon_ch1_CC_8p5hrs_back_001.root";
    //string filename = "042216_2500V_n2000V_10mm_muon_ch2_001.root";
    //string filename = "04222016_2500V_n2000V_10mm_muon_CC_nPMT_*.root";
    //string filename = "042716_2000V_n1500V_10mm_muon_CC_nPMT_*.root";
    //string filename = "043016_3000VG2_2500VG1_n3000VC_10mm_muon_CC_nPMT_ch1_001.root";
    //string filename = "043016_2500VG2_2500VG1_n2000VC_10mm_muon_CC_nPMT_ch1_001.root";
    //string filename = "043016_3000VNG_n3000VC_10mm_muon_CC_nPMT_ch1_*.root";
    //string filename = "043016_2500VNG_2000VC_10mm_muon_CC_nPMT_ch1_001.root";
    //string filename = "050416_3500VG2_2800VG1_n3000C_10mm_muon_CC_nPMT_ch1_*.root";
    //string filename = "050516_4000VG2_3300VG1_n3000C_10mm_muon_CC_nPMT_ch1_*.root";
    //string filename = "050516_3600VNG_n3000C_10mm_muon_CC_nPMT_ch1_*.root";
    //string filename = "051216_3600VNG_n3000C_10mm_muon_CC_nPMT_90deg_ch1_*.root";
    //string filename = "051816_2500VG2_2500VG1_n2000V_10mm_2um_muon_CC_nPMT_ch1_001.root";
    //string filename = "052016_1300VG2_2000VG1_n3700V_10mm_2um_muon_CC_nPMT_Cube1_ch1_001.root";
    //string filename = "052216_600VG2_1500VG1_n3000V_10mm_2um_muon_CC_nPMT_Cube1_ch1_*.root";
    //string filename = "052216_1000VNG_n3000V_10mm_2um_muon_CC_nPMT_Cube1_ch1_*.root";
    //string filename = "muon_TMS_n100VC_4000VNG_011117_001.root";
    //string filename = "muon_TMS_n500VC_5000VNG_011217_*.root";
    //string filename = "TMS_Muons_Cont_Purification/EFP/052717/TMS_muon_n100VC_5000VNG_FLON_50V_06052017_*.root";
    //string filename = "TMS_Muons_Cont_Purification/EFP/TMS_muon_n100VC_5000VNG_FLON_CH1_07182017_*.root";
    //string filename = "Diamondoids/Diamondoid_muon_n700VC_500VNG_CH1_060817_*.root";
    //string filename = "Diamondoids/Diamondoid_muon_n700VC_300VG1_700VG2_CH1_061217_*.root";
    
    // Filenames for the analysis 072817
    string basepath = "TMS_Muons_Cont_Purification/EFP/Analysis_072817/data/";
    string filename;
    
    // 1. FLOFF
    //filename = basepath+"1._FLOFF/TMS_muon_n100VC_2500VNG_FLOFF_CH1_06202017_*.root";
    //filename = basepath+"1._FLOFF/TMS_muon_n100VC_3200VNG_FLOFF_CH1_06272017_*.root";
    //filename = basepath+"1._FLOFF/TMS_muon_n100VC_3800VNG_FLOFF_CH1_06262017_*.root";
    //filename = basepath+"1._FLOFF/TMS_muon_n100VC_5000VNG_FLOFF_CH1_06192017_*.root";
    //filename = basepath+"1._FLOFF/TMS_muon_n3000VC_5000VNG_FLOFF_CH1_06222017_*.root";
    
    // 2. FLON
    //filename = basepath+"2._FLON/TMS_muon_n100VC_2500VNG_FLON_CH1_07102017_*.root";
    //filename = basepath+"2._FLON/TMS_muon_n100VC_3200VNG_FLON_CH1_07122017_*.root";
    //filename = basepath+"2._FLON/TMS_muon_n100VC_3800VNG_FLON_CH1_07172017_*.root";
    //filename = basepath+"2._FLON/TMS_muon_n100VC_5000VNG_FLON_CH1_07182017_*.root";
    
    // 3. FLOFF
    //filename = basepath+"3._FLOFF/TMS_muon_n100VC_2500VNG_FLOFF_CH1_07242017_*.root";
    
    // Forced trigger
    //filename = "TMS_Muons_Cont_Purification/EFP/TMS_muon_n100VC_2500VNG_FLOFF_CH1_07282017_ForcedTrigger_001.root";
    
    // PMT offset
    //filename = "TMS_Muons_Cont_Purification/EFP/TMS_muon_n100VC_2500VNG_FLOFF_CH1_07312017_PMT_offset_*.root";
    
    // TMS general
    //filename = "TMS_Muons_Cont_Purification/PM/TMS_muon_n100VC_3800VNG_PMTest_CH1_092217_*.root";
    filename = "TMS_Muons_Cont_Purification/PM/TMS_NewCell_muon_0VA_n3300VC_CH1_103017_*.root";
    
    TChain *t = new TChain("dataTree","dataTree");
    
    t->Add(filename.c_str());
    
    //double calib_p0 = 0;
    //double calib_p1 = 1;
    
    // **** box2, shaper1
    //double calib_p0 = -17683.4;
    //double calib_p1 = 2.036;
    
    // **** box1, shaper1
    //double calib_p0 = -20541.8;
    //double calib_p1 = 2.36612;
    
    // **** box2, shaper2
    //double calib_p0 = -16053.4;
    //double calib_p1 = 2.09036;
    
    // **** New preamp, channel 1, shaper1
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.392;
    
    // **** New preamp, channel 2, shaper1
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.235;
    
    // **** New preamp, channel 1, shaper 2
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.253;
    
    // **** New preamp, channel 2, shaper 2
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.209;
    
    // **** New preamp, channel 1, shaper 2 (2µs)
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.379;
    
    // **** New preamp, channel 2, shaper 2 (2µs)
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 2.316;
    
    // **** LXe preamp, channel 1, shaper 2 (2µs)
    //double calib_p0_ch1 = -877.7;
    //double calib_p1_ch1 = 6.1905;
    
    // **** LXe preamp, channel 1, shaper 2 (2µs) G200
    double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 5.875;
    //double calib_p1_ch1 = 5.942; // updated for TMS
    double calib_p1_ch1 = 12.457; // TMS with G200 * 0.5
    
    // **** LXe preamp, channel 1, shaper 2 (2µs) G100
    //double calib_p0_ch1 = 0;
    //double calib_p1_ch1 = 12.0907;
    
    double ROI_low_init = 1000;
    double ROI_up_init = 1300;
    
    // Binning
    int nBins = 80;
    double binMin = 0;
    double binMax = 54000;
    
    // Default binning (120,0,15000)
    TH1F *h = new TH1F("h","h",nBins,binMin,binMax);
    
    TH1F *h_signal = new TH1F("h_signal","h_signal",nBins,binMin,binMax);
    
    TH1F *h_bg = new TH1F("h_bg","h_bg",nBins,binMin,binMax);
    
    TH1F *h2 = new TH1F("h2","h2",nBins,binMin,binMax);
    
    TH1F *h_chi2 = new TH1F("h_chi2","h_chi2",200,0,50);
    
    TH1F *h_fit = new TH1F("h_fit","h_fit",200,0,50);
    
    TH1F *h_baseline = new TH1F("h_baseline","h_baseline",200,6500,9000);
    
    TH1F *h_rms = new TH1F("h_rms","h_rms",200,100,600);
    
    TH2F *h_rms_peakPosition = new TH2F("h_rms_peakPosition","h_rms_peakPosition",100,ROI_low_init,ROI_up_init,50,0,300);
    
    TH2F *h_fit_peakPosition = new TH2F("h_fit_peakPosition","h_fit_peakPosition",100,ROI_low_init,ROI_up_init,100,0,50);
    
    TH2F *h_chi2_peakPosition = new TH2F("h_chi2_peakPosition","h_chi2_peakPosition",100,ROI_low_init,ROI_up_init,100,0,50);
    
    TH1F *h_timeDiff = new TH1F("h_timeDiff","h_timeDiff",1000,-1000,1000);
    
    t->Draw("baseline>>h_baseline","","goff");
    t->Draw("rms>>h_rms","","goff");
    
    double baseline = h_baseline->GetMean()*calib_p1_ch1 + calib_p0_ch1;
    double rms = h_rms->GetMean()*calib_p1_ch1;
    double rms_sigma = h_rms->GetRMS()*calib_p1_ch1;
    double sigma = 3;
    double xMax = 200;
    
    double rms_cut_up = 300.0; //400.0
    double rms_cut_low = 50.0;
    double fit_cut = 50.0; //8.0
    double chi2_cut = 50.0;
    double ROI_low = 1120.0; //1120.0
    double ROI_up = 1250.0; //1175.0
    
    double fitMin = 4000;
    double fitMax = (16384 - h_baseline->GetMean())*calib_p1_ch1;
    
    cout << "baseline = " << h_baseline->GetMean() << "+-" << h_baseline->GetRMS() << endl;
    
    cout << "rms = " << h_rms->GetMean() << "+-" << h_rms->GetRMS() << " (" << h_rms->GetMean()*calib_p1_ch1 + calib_p0_ch1 << " e- +- " << h_rms->GetRMS()*calib_p1_ch1 << ")" << endl;
    
    double peak_t;
    double peak_position_t;
    double baseline_t;
    double rms_t;
    double fit_t;
    double chi2_t;
    int timestamp_t;
    
    t->SetBranchAddress("peak",&peak_t);
    t->SetBranchAddress("peak_position",&peak_position_t);
    t->SetBranchAddress("baseline",&baseline_t);
    t->SetBranchAddress("rms",&rms_t);
    t->SetBranchAddress("fit",&fit_t);
    t->SetBranchAddress("chi2",chi2_t);
    t->SetBranchAddress("timestamp",&timestamp_t);
    
    /*for (int i = 0; i < t->GetEntries()-1; i++) {
        t->GetEntry(i);
        
        int t1 = timestamp_t;
        double val1 = (peak_t-baseline_t)*calib_p1_ch1;
        int peak_position1 = peak_position_t;
        double rms1 = rms_t;
        double fit1 = fit_t;
        double chi21 = chi2_t;
        
        t->GetEntry(i+1);
        
        int t2 = timestamp_t;
        double val2 = (peak_t-baseline_t)*calib_p1_ch1;
        int peak_position2 = peak_position_t;
        double rms2 = rms_t;
        double fit2 = fit_t;
        double chi22 = chi2_t;
        
        h_timeDiff->Fill((t2-t1));
        
        if (!((((t2-t1) > 490) && ((t2-t1) < 510)))) {continue;}
        
        if (fit1 < fit_cut && peak_position1 > ROI_low && peak_position1 < ROI_up && rms1 < rms_cut && chi21 < chi2_cut) {h->Fill(val1); h_signal->Fill(val1);}
        
        if (fit2 < fit_cut && peak_position2 > ROI_low && peak_position2 < ROI_up && rms2 < rms_cut && chi22 < chi2_cut) {h->Fill(val2); h_bg->Fill(val2);}
    }*/
    
    //t->Draw(Form("(peak-baseline)*%f+%f>>h",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && rms > %f && chi2 < %f && isNoise == 0",fit_cut,ROI_low,ROI_up,rms_cut_up,rms_cut_low,chi2_cut),"goff");
    t->Draw(Form("(peak-baseline)*%f+%f>>h",calib_p1_ch1,calib_p0_ch1),Form("peak_position > %f && peak_position < %f && rms < %f && rms > %f && isNoise == 0",ROI_low,ROI_up,rms_cut_up,rms_cut_low),"goff");
    //t->Draw(Form("(peak-baseline)*%f+%f>>h_signal",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f && (2*TMath::Ceil(event/2)-event != 0)",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    //t->Draw(Form("(peak-baseline)*%f+%f>>h_bg",calib_p1_ch1,calib_p0_ch1),Form("fit < %f && peak_position > %f && peak_position < %f && rms < %f && chi2 < %f && (2*TMath::Ceil(event/2)-event == 0)",fit_cut,ROI_low,ROI_up,rms_cut,chi2_cut),"goff");
    t->Draw(Form("(amplitude-baseline)*%f+%f>>h2",calib_p1_ch1,calib_p0_ch1),Form("peak_position > %f && peak_position < %f && rms < %f && rms > %f",ROI_low,ROI_up,rms_cut_up,rms_cut_low),"goff");
    t->Draw("chi2>>h_chi2","","goff");
    t->Draw("fit>>h_fit","","goff");
    t->Draw("rms:peak_position>>h_rms_peakPosition","","goff");
    t->Draw("fit:peak_position>>h_fit_peakPosition","","goff");
    t->Draw("chi2:peak_position>>h_chi2_peakPosition","","goff");
    
    TGraph2D *gr_rms_peakPosition = new TGraph2D(h_rms_peakPosition);
    TGraph2D *gr_fit_peakPosition = new TGraph2D(h_fit_peakPosition);
    
    h_rms_peakPosition->GetXaxis()->SetTitle("peak position");
    h_rms_peakPosition->GetYaxis()->SetTitle("RMS");
    
    h_fit_peakPosition->GetXaxis()->SetTitle("peak position");
    h_fit_peakPosition->GetYaxis()->SetTitle("fit");
    
    h_chi2_peakPosition->GetXaxis()->SetTitle("peak_position");
    h_chi2_peakPosition->GetYaxis()->SetTitle("chi2");
    
    h_rms_peakPosition->GetXaxis()->CenterTitle();
    h_rms_peakPosition->GetYaxis()->CenterTitle();
    
    h_fit_peakPosition->GetXaxis()->CenterTitle();
    h_fit_peakPosition->GetYaxis()->CenterTitle();
    
    h_chi2_peakPosition->GetXaxis()->CenterTitle();
    h_chi2_peakPosition->GetYaxis()->CenterTitle();
    
    TPolyLine *rms_peakPosition_cut = new TPolyLine(5);
    
    rms_peakPosition_cut->SetPoint(0,ROI_low,rms_cut_low);
    rms_peakPosition_cut->SetPoint(1,ROI_up,rms_cut_low);
    rms_peakPosition_cut->SetPoint(2,ROI_up,rms_cut_up);
    rms_peakPosition_cut->SetPoint(3,ROI_low,rms_cut_up);
    rms_peakPosition_cut->SetPoint(4,ROI_low,rms_cut_low);
    
    TPolyLine *fit_peakPosition_cut = new TPolyLine(5);
    
    fit_peakPosition_cut->SetPoint(0,ROI_low,0);
    fit_peakPosition_cut->SetPoint(1,ROI_up,0);
    fit_peakPosition_cut->SetPoint(2,ROI_up,fit_cut);
    fit_peakPosition_cut->SetPoint(3,ROI_low,fit_cut);
    fit_peakPosition_cut->SetPoint(4,ROI_low,0);
    
    rms_peakPosition_cut->SetLineColor(kRed);
    rms_peakPosition_cut->SetLineWidth(2);
    rms_peakPosition_cut->SetLineStyle(2);
    
    fit_peakPosition_cut->SetLineColor(kRed);
    fit_peakPosition_cut->SetLineWidth(2);
    fit_peakPosition_cut->SetLineStyle(2);
    
    TPolyLine *chi2_peakPosition_cut = new TPolyLine(5);
    
    chi2_peakPosition_cut->SetPoint(0,ROI_low,0);
    chi2_peakPosition_cut->SetPoint(1,ROI_up,0);
    chi2_peakPosition_cut->SetPoint(2,ROI_up,chi2_cut);
    chi2_peakPosition_cut->SetPoint(3,ROI_low,chi2_cut);
    chi2_peakPosition_cut->SetPoint(4,ROI_low,0);
    
    chi2_peakPosition_cut->SetLineColor(kRed);
    chi2_peakPosition_cut->SetLineWidth(2);
    chi2_peakPosition_cut->SetLineStyle(2);
    
    h->SetLineColor(kBlack);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.5);
    
    h_signal->SetLineColor(kBlue);
    h_signal->SetMarkerStyle(20);
    h_signal->SetMarkerSize(0.5);
    
    h_bg->SetLineColor(kRed);
    h_bg->SetMarkerStyle(20);
    h_bg->SetMarkerSize(0.5);
    
    h2->SetLineColor(kBlack);
    h2->SetMarkerStyle(20);
    h2->SetMarkerSize(0.5);
    
    int overflowBin = h->GetXaxis()->FindBin((16383-h_baseline->GetMean())*calib_p1_ch1 + calib_p0_ch1);
    
    for (int i = overflowBin+1; i < nBins+1; i++) {
        h->SetBinContent(overflowBin,h->GetBinContent(overflowBin)+h->GetBinContent(i));
        h->SetBinContent(i,0);
    }
    
    //TF1 *fit = new TF1("fit",fitFunction,4,1400,8000);
    TF1 *fit = new TF1("fit","[0]*TMath::Landau(x,[1],[2])",fitMin,fitMax);
    
    fit->SetParameters(20,6000,800);
    //fit->FixParameter(3,h_rms->GetMean()*calib_p1);
    
    fit->SetLineColor(kBlue);
    fit->SetLineWidth(2);
    
    // RooFit
    RooRealVar e("e","electrons",binMin,binMax);
    
    // #### Gauss for noise and signal smearing ############################################################
    //RooRealVar gausSigma("gausSigma","#sigma (gauss)",800,500,1500);
    //RooRealVar gausSigma("gausSigma","#sigma (gauss)",1.16925e+03);
    RooRealVar gausSigma("gausSigma","#sigma (gauss)",rms,0.5*rms,3*rms);
    RooRealVar gausMean("gausMean","mean (gauss)",0);
    RooGaussian gauss("gauss","gaussian PDF",e,gausMean,gausSigma);
    
    // #### Landau for muon distribution ###################################################################
    RooRealVar landauSigma("landuaSigma","#sigma (landau)",800,200,1500);
    RooRealVar landauMean("landauMean","mean (landau)",6000,2000,12000);
    RooLandau landau("landau","landau PDF",e,landauMean,landauSigma);
    
    // #### Uniform distribution below Landau peak to account for partial energy deposits #################
    RooGenericPdf constBkg("constBkg","0.5*TMath::Erfc((@0-@1)/@2)",RooArgList(e,landauMean,landauSigma));
    RooRealVar frac_signalBkg("frac_signalBkg","fraction of background events",0.01);
    RooAddPdf signal("signal","erfc+landau",RooArgList(constBkg,landau),frac_signalBkg);
    
    // #### Smearing the signal with Gauss to account for noise ###########################################
    RooFFTConvPdf signalGauss("signalGauss","signal (x) gauss",e,signal,gauss);
    
    // #### Efficiency function that cuts off at RMS of Gaussian noise ####################################
    RooRealVar effCut("effCut","efficiency cutoff",rms);
    //RooRealVar effCutSlope("effCutSlope","efficiency cutoff slope",300,100,1000);
    RooRealVar effCutSlope("effCutSlope","efficiency cutoff slope",3.04209e+02);
    RooFormulaVar eff("eff","0.5*(TMath::Erf((@0-@1)/@2)+1)",RooArgList(e,gausSigma,effCutSlope));
    
    // #### Applying efficiency function ##################################################################
    RooEffProd gaussEff("gaussEff","gauss with efficiency function",gauss,eff);
    RooEffProd signalGaussEff("signalGaussEff","signal with efficiency function",signalGauss,eff);
    
    // #### Adding signal to background ###################################################################
    RooRealVar nsig("nsig","#signal events",500,0,10000);
    RooRealVar nbkg("nbkg","#background events",200,0,10000);
    RooAddPdf sum("sum","gauss+erfc+landau",RooArgList(gaussEff,signalGaussEff),RooArgList(nbkg,nsig));
    
    /*RooGenericPdf constBkg("constBkg","0.5*TMath::Erfc((@0-@1)/@2)",RooArgList(e,landauMean,landauSigma));
    
    RooEffProd gaussEff("gaussEff","gauss with efficiency function",gauss,eff);
    RooEffProd landauEff("landauEff","landua with efficiency function",landau,eff);
    RooEffProd erfcEff("erfcEff","erfc with effciency funciton",constBkg,eff);
    
    RooRealVar nsig("nsig","#signal events",500,0,10000);
    RooRealVar nbkg("nbkg","#background events",200,0,10000);
    RooRealVar frac_signalBkg("frac_signalBkg","fraction of background events",0.09);
    //RooRealVar frac_signalEmpty("frac_signalEmpty","fraction of emtpy events",0.6,0,1);
    RooAddPdf signal("signal","erfc+landau",RooArgList(erfcEff,landauEff),frac_signalBkg);
    
    RooFFTConvPdf signalConv("signalConv","signal x gaus",e,signal,gauss);
    
    RooAddPdf sum("sum","gauss+erfc+landau",RooArgList(gaussEff,signalConv),RooArgList(nbkg,nsig));*/
    
    //RooAddPdf sum("sum","erfc+landau",RooArgList(constBkg,landau),frac_signalBkg);
    
    //RooEffProd sumEff("sumEff","model with efficiency function",sum,eff);
    
    RooDataHist data("data","dataset",e,h2);
    
    sum.fitTo(data, Extended());
    
    //landauGauss.fitTo(data,Range(2500,15000));
    
    RooPlot *frame = e.frame();
    frame->GetYaxis()->SetRangeUser(0,xMax);
    data.plotOn(frame);
    //sum.plotOn(frame,Components(gauss),LineStyle(kDashed));
    //sum.plotOn(frame,Components(landau),LineStyle(kDotted));
    //landauGauss.plotOn(frame);
    //gauss.plotOn(frame,LineWidth(2),LineColor(kGreen+1));
    //sum.plotOn(frame,Components(gauss),LineStyle(kDashed));
    //sum.plotOn(frame,Components(landau),LineWidth(2),LineColor(kRed+1));
    //sum.plotOn(frame,Components(gauss),LineWidth(2),LineColor(kGreen+1));
    //sum.plotOn(frame,Components(constBkg),LineWidth(2),LineColor(kYellow+1));
    eff.plotOn(frame,LineStyle(kDashed),LineWidth(2),LineColor(kGray+1));
    
    landau.plotOn(frame,LineWidth(2),LineColor(kRed+1));
    gauss.plotOn(frame,LineWidth(2),LineColor(kGreen+1));
    signalGauss.plotOn(frame,LineWidth(2),LineColor(kYellow+1));
    sum.plotOn(frame,LineWidth(2),LineColor(kBlue));
    
    cout << "RooFit:    gausSigma = " << gausSigma.getValV() << " +- " << gausSigma.getError() << endl;
    cout << "           landauMean = " << landauMean.getValV() << " +- " << landauMean.getError() << endl;
    cout << "           landauSigma = " << landauSigma.getValV() << " +- " << landauSigma.getError() << endl;
    
    TLine *l = new TLine((16384-h_baseline->GetMean())*calib_p1_ch1 + calib_p0_ch1,0,(16384-h_baseline->GetMean())*calib_p1_ch1 + calib_p0_ch1,xMax);
    
    l->SetLineColor(kRed);
    l->SetLineWidth(1);
    
    double x[5] = {0,sigma*rms,sigma*rms,0,0};
    double y[5] = {0,0,xMax,xMax,0};
    
    TGraph *l2 = new TGraph(5,x,y);
    
    l2->SetFillStyle(3003);
    l2->SetFillColor(kGreen);
    
    TLine *l3 = new TLine(rms,0,rms,xMax);
    
    l3->SetLineColor(kRed);
    l3->SetLineWidth(1);
    l3->SetLineStyle(2);
    
    TLegend *legend = new TLegend(0.5,0.65,0.72,0.84);
    
    legend->AddEntry(h,"data","lp");
    legend->AddEntry(l,"ADC full scale","l");
    //legend->AddEntry(h_signal,"signal","l");
    //legend->AddEntry(h_bg,"background","l");
    legend->AddEntry(l3,"RMS","l");
    legend->AddEntry(l2,"3#sigma noise","F");
    
    legend->SetFillStyle(0);
    
    TLegend *legend2 = new TLegend(0.25,0.65,0.48,0.84);
    
    TGraph *grPDFFitModel = new TGraph();
    TGraph *grPDFLandau = new TGraph();
    TGraph *grPDFLandauErrfc = new TGraph();
    TGraph *grPDFGauss = new TGraph();
    
    grPDFFitModel->SetLineColor(kBlue+1);
    grPDFLandau->SetLineColor(kRed+1);
    grPDFLandauErrfc->SetLineColor(kYellow+1);
    grPDFGauss->SetLineColor(kGreen+1);
    
    legend2->AddEntry(grPDFFitModel,"Fit","l");
    legend2->AddEntry(grPDFLandau,"Landau (muon signal)","l");
    legend2->AddEntry(grPDFLandauErrfc,"Landau+Errfc (x) Gaus","l");
    legend2->AddEntry(grPDFGauss,"Electronics noise","l");
    
    legend2->SetFillStyle(0);
    
    h->SetTitle(Form("Unity gain cell - FLON 50V - 5.1 kV/cm - 10mm - %.0f events",h->GetEntries()));
    
    h->GetYaxis()->SetRangeUser(0,xMax);
    
    h2->GetYaxis()->SetRangeUser(0,xMax);
    
    h->GetXaxis()->SetTitle("number of electrons");
    h->GetYaxis()->SetTitle("counts");
    
    h2->GetXaxis()->SetTitle("number of electrons");
    h2->GetYaxis()->SetTitle("counts");
    
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    
    h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->CenterTitle();
    
    h->Fit(fit,"rn","goff");
    
    fit->SetRange(0,fitMax);
    
    double fit_mean = fit->Mean(0,8000,fit->GetParameters());
    
    TLine *l_fit_mean = new TLine(fit_mean,0,fit_mean,xMax);
    
    l_fit_mean->SetLineColor(kBlack);
    l_fit_mean->SetLineWidth(1);
    l_fit_mean->SetLineStyle(2);
    
    TLatex *fit_result_1 = new TLatex(28000,xMax*0.5,"Fit result:");
    TLatex *fit_result_2 = new TLatex(28000,xMax*0.43,Form("MPV = %.1f #pm %.1f",fit->GetParameter(1),fit->GetParError(1)));
    TLatex *fit_result_3 = new TLatex(28000,xMax*0.36,Form("#Chi^{2}/ndf = %.2f",fit->GetChisquare()/fit->GetNDF()));
    
    TLatex *fit2_result_1 = new TLatex(28000,xMax*0.5,"Fit result:");
    TLatex *fit2_result_2 = new TLatex(28000,xMax*0.43,Form("MPV = %.1f #pm %.1f",landauMean.getValV(),landauMean.getError()));
    TLatex *fit2_result_3 = new TLatex(28000,xMax*0.36,Form("#Chi^{2}/ndf = %.2f",frame->chiSquare()));
    
    fit_result_1->SetTextSize(0.03);
    fit_result_2->SetTextSize(0.03);
    fit_result_3->SetTextSize(0.03);
    
    fit2_result_1->SetTextSize(0.03);
    fit2_result_2->SetTextSize(0.03);
    fit2_result_3->SetTextSize(0.03);
    
    TCanvas *c1 = new TCanvas();
    frame->Draw();
    h->Draw("EZP");
    //h_signal->Draw("same");
    //h_bg->Draw("same");
    fit->Draw("same");
    l2->Draw("Fsame");
    l->Draw("same");
    l3->Draw("same");
    //l_fit_mean->Draw("same");
    legend->Draw("same");
    fit_result_1->Draw("same");
    fit_result_2->Draw("same");
    fit_result_3->Draw("same");
    
    TCanvas *c2 = new TCanvas();
    frame->Draw();
    //h2->Draw("EZP");
    l2->Draw("Fsame");
    l->Draw("same");
    l3->Draw("same");
    legend->Draw("same");
    legend2->Draw("same");
    fit2_result_1->Draw("same");
    fit2_result_2->Draw("same");
    fit2_result_3->Draw("same");
    
    TCanvas *c3 = new TCanvas();
    h_chi2_peakPosition->Draw("cont1 z");
    chi2_peakPosition_cut->Draw("same");
    
    TCanvas *c4 = new TCanvas();
    h_fit_peakPosition->Draw("cont1 z");
    fit_peakPosition_cut->Draw("same");
    
    TCanvas *c5 = new TCanvas();
    //h_baseline->Draw();
    h_timeDiff->Draw();
    
    TCanvas *c6 = new TCanvas();
    h_rms_peakPosition->Draw("cont1 z");
    rms_peakPosition_cut->Draw("same");
}
