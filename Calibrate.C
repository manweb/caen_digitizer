void Calibrate()
{
    gStyle->SetOptStat(0);
    //gStyle->SetOptTitle(0);
    gStyle->SetOptFit(1);
    
    //string baseFilename = "043016_calibration_newPA_ch1_sh2_2us_[wildcard].root";
    //string baseFilename = "Xe_Bi207/032117/calib/LXe_Bi207_calib_1V_[wildcard]_Shaper1_G50_CAEN_1000VG1_1000VG2_032117_001.root";
    //string baseFilename = "Xe_Bi207/032117/calib/LXe_Bi207_calib_1V_[wildcard]_Shaper2_G50_CAEN_1000VNG_032117_001.root";
    //string baseFilename = "TMS_calibration_n100VC_5000VNG_shaper2_G200_1V_[wildcard]_001.root";
    string baseFilename = "TMS_Muons_Cont_purification/PM/calibration/TMS_calibration_n100VC_3800VNG_PMTest_shaper2_G200_1V_[wildcard]_092517_001.root";
    
    string outName1 = "TMS_Muons_Cont_Purification/PM/calibration/Plots/TMS_calibration_n100VC_3800VNG_PMTest_shaper2_G200_092517.root";
    string outName2 = outName1;
    
    outName2.replace(outName2.find(".root"),5,"_spectra.root");
    
    string title = "Non gain, channel1, shaper2";
    
    const int nFiles = 5;
    
    double att[nFiles] = {50, 53, 56, 60, 63};
    double parInit[nFiles] = {3000,2200,1600,1000,800};
    
    double C = 2e-12; // 2pF
    double q = 1.60217662e-19;
    
    TH1F *h[nFiles];
    TF1 *fit[nFiles];
    
    TGraphErrors *gr = new TGraphErrors(nFiles);
    
    gr->SetTitle(title.c_str());
    
    for (int i = 0; i < nFiles; i++) {
        string filename = baseFilename;
        
        filename.replace(filename.find("[wildcard]"),10,Form("%.0fdB",att[i]));
        
        cout << "Reading file " << filename.c_str() << endl;
        
        TFile *f = new TFile(filename.c_str(),"READ");
        
        TTree *t = (TTree*)f->Get("dataTree");
        
        h[i] = new TH1F(Form("h_%i",i),Form("h_%i",i),800,0,8000);
        
        t->Draw(Form("peak-baseline>>h_%i",i),"isNoise==0","goff");
        
        fit[i] = new TF1(Form("fit_%i",i),"[0]*TMath::Gaus(x,[1],[2])",500,8000);
        
        fit[i]->SetParameters(100,parInit[i],50);
        
        h[i]->Fit(fit[i],"rn","goff");
        
        h[i]->SetLineColor(kBlack);
        h[i]->SetMarkerStyle(20);
        h[i]->SetMarkerSize(0.5);
        
        fit[i]->SetLineColor(kBlue);
        fit[i]->SetLineWidth(2);
        
        double voltage = TMath::Power(10,-att[i]/20);
        
        double N = C*voltage/q;
        
        gr->SetPoint(i,fit[i]->GetParameter(1),N);
        gr->SetPointError(i,fit[i]->GetParError(1),N*0.03);
    }
    
    TCanvas *c2 = new TCanvas();
    
    c2->Divide(2,ceil(double(nFiles)/2));
    
    for (int i = 1; i < nFiles+1; i++) {
        c2->cd(i);
        
        h[i-1]->Draw("EZP");
        fit[i-1]->Draw("same");
    }
    
    TF1 *fit2 = new TF1("fit2","[0]+[1]*x",0,8000);
    
    fit2->FixParameter(0,0);
    
    fit2->SetLineColor(kBlack);
    fit2->SetLineWidth(2);
    fit2->SetLineStyle(2);
    
    gr->Fit(fit2,"r","goff");
    
    gr->SetLineColor(kBlack);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.5);
    
    gr->GetXaxis()->SetTitle("ADC channel");
    gr->GetYaxis()->SetTitle("number of electrons");
    
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();
    
    gr->GetYaxis()->SetTitleOffset(1.4);
    
    /*fit2->SetLineColor(kBlack);
    fit2->SetLineWidth(1);
    fit2->SetLineStyle(2);*/
    
    TCanvas *c3 = new TCanvas();
    
    gr->Draw("AZP");
    fit2->Draw("same");
    
    c2->SaveAs(outName2.c_str());
    
    c3->SaveAs(outName1.c_str());
}
