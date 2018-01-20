void SumWaveforms()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    string filename = "Diamondoid_muon_n100VC_5000VNG_CH1_101117_[wildcard].root";
    //string filename = "Diamondoid_muon_n700VC_300VG1_700VG2_CH1_061217_[wildcard].root";
    //string filename = "TMS_Muons_Cont_Purification/EFP/TMS_muon_n100VC_5000VG1_5000VG2_FLOFF_CH1_06292017_[wildcard].root";
    
    const int nFiles = 5;
    string fileNumbers[nFiles] = {"001","002","003","004","005"}
    
    int nSamples = 2048;
    
    TH2I *h2D = new TH2I("h_waveforms","h_waveforms",nSamples/4,1,nSamples,2048,1,16384);
    
    int nEvents = 0;
    for (int i = 0; i < nFiles; i++) {
        string fName = filename;
        fName.replace(fName.find("[wildcard]"),10,fileNumbers[i]);
        
        TFile *f = new TFile(fName.c_str(),"READ");
        
        TCanvas *c1 = new TCanvas();
        
        for (int k = 0; k < 10000; k++) {
            if (c1) {delete c1;}
            
            c1 = (TCanvas*)f->Get(Form("h_evt%i_ch0",k));
            
            if (!c1) {nEvents += k; break;}
            
            TPad *c1_1 = c1->GetPrimitive(Form("h_evt%i_ch0_1",k));
            
            TH1I *h1D = c1_1->GetPrimitive(Form("h_evt%i_ch0",k));
            
            TList *functions = h1D->GetListOfFunctions();
            TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
            
            Double_t *peaks_xTMP = pm->GetX();
            Double_t *peaks_yTMP = pm->GetY();
            
            bool saturated = false;
            for (int m = 0; m < pm->GetN(); m++) {
                if (peaks_yTMP[m] > 16380 && (peaks_xTMP[m] < 1024 || peaks_xTMP[m] > 1400)) {saturated = true;}
            }
            
            if (saturated) {cout << "Event " << k << " is saturated" << endl; continue;}
            
            for (int m = 1; m < nSamples; m++) {
                int binValue = h1D->GetBinContent(m);
                
                int binX = h2D->GetXaxis()->FindBin(m);
                int binY = h2D->GetYaxis()->FindBin(binValue);
                
                h2D->SetBinContent(binX,binY,h2D->GetBinContent(binX,binY)+1);
            }
        }
    }
    
    cout << "Number of events: " << nEvents << endl;
    
    h2D->GetXaxis()->SetTitle("time (#mus)");
    h2D->GetYaxis()->SetTitle("ADC channel");
    
    h2D->GetXaxis()->CenterTitle();
    h2D->GetYaxis()->CenterTitle();
    
    TLine *l = new TLine(1024,0,1024,16384);
    
    l->SetLineColor(kRed+1);
    l->SetLineWidth(2);
    l->SetLineStyle(2);
    
    TText *text = new TText(1000,16000,"Muon trigger");
    
    text->SetTextSize(0.03);
    text->SetTextAlign(32);
    
    TCanvas *c2 = new TCanvas();
    
    h2D->Draw("colz");
    l->Draw("same");
    text->Draw("same");
}
