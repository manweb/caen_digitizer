using namespace RooFit;

void PlotFitModelConv()
{
    RooRealVar e("e","electrons",0,30000);
    
    RooRealVar gausSigma("gausSigma","#sigma (gauss)",700);
    RooRealVar gausMean("gausMean","mean (gauss)",0);
    RooGaussian gauss("gauss","gaussian PDF",e,gausMean,gausSigma);
    
    RooRealVar landauSigma("landuaSigma","#sigma (landau)",800);
    RooRealVar landauMean("landauMean","mean (landau)",6000);
    RooLandau landau("landau","landau PDF",e,landauMean,landauSigma);
    
    RooGenericPdf constBkg("constBkg","0.5*TMath::Erfc((@0-@1)/@2)",RooArgList(e,landauMean,landauSigma));
    RooRealVar frac_signalBkg("frac_signalBkg","fraction of background events",0.09);
    RooAddPdf signal("signal","erfc+landau",RooArgList(constBkg,landau),frac_signalBkg);
    
    RooFFTConvPdf landauGauss("landauGauss","landau (x) gauss",e,landau,gauss);
    RooFFTConvPdf signalGauss("signalGauss","signal (x) gauss",e,signal,gauss);
    
    RooPlot *frame1 = e.frame();
    RooPlot *frame2 = e.frame();
    
    landau.plotOn(frame1, LineColor(kBlue+1), LineWidth(1));
    landauGauss.plotOn(frame1, LineColor(kRed+1), LineWidth(1));
    
    signal.plotOn(frame2, LineColor(kBlue+1), LineWidth(1));
    signalGauss.plotOn(frame2, LineColor(kRed+1), LineWidth(1));
    
    TCanvas *c1 = new TCanvas("c1","c1",800,400);
    c1->Divide(2,1);
    c1->cd(1);
    frame1->Draw();
    c1->cd(2);
    frame2->Draw();
}
