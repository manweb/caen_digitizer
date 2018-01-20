void PlotSpectrum()
{
    gStyle->SetOptStat(0);
    
    TFile *f = new TFile("040416_2000V_1mm_10100_Eu154_60m_ch1_001.root","READ");
    //TFile *f = new TFile("032416_calibration_ch1.root","READ");
    
    TTree *t = (TTree*)f->Get("dataTree");
    
    TH1F *h = new TH1F("h","h",500,8000,16384);
    
    TH1F *h_1 = new TH1F("h_1","h_1",500,8000,16384);
    
    TH1F *h_2 = new TH1F("h_2","h_2",500,8000,16384);
    
    TH1F *h2 = new TH1F("h2","h2",500,8000,16384);
    
    TH1F *h_chi2 = new TH1F("h_chi2","h_chi2",200,0,50);
    
    TH2F *h_chi2_signal = new TH2F("h_chi2_signal","h_chi2_signal",250,0,16384,200,0,50);
    
    TH2F *h_corr = new TH2F("h_corr","h_corr",100,0.001,0.1,100,0,40);
    
    TH1F *h_chi2_2 = new TH1F("h_chi2_2","h_chi2_2",200,0,50);
    
    TH2F *h_chi2_chi2_2 = new TH2F("h_chi2_chi2_2","h_chi2_chi2_2",200,0,50,200,0,50);
    
    dataTree->Draw("amplitude>>h","","goff");
    dataTree->Draw("amplitude>>h_1","chi2 < 2","goff");
    dataTree->Draw("peak>>h_2","fit < 4","goff");
    dataTree->Draw("peak>>h2","","goff");
    dataTree->Draw("chi2>>h_chi2","","goff");
    dataTree->Draw("chi2:amplitude>>h_chi2_signal","","goff");
    dataTree->Draw("chi2:abs((peak-amplitude)/peak)>>h_corr","","goff");
    dataTree->Draw("fit>>h_chi2_2","","goff");
    dataTree->Draw("chi2:fit>>h_chi2_chi2_2","","goff");
    
    TH1F *h_res = h->Clone();
    h_res->Add(h2,-1);
    h_res->Divide(h2);
    
    cout << h_res->Integral() << endl;
    
    h->SetLineColor(kBlack);
    h_1->SetLineColor(kBlue);
    h_2->SetLineColor(kGreen);
    
    h2->SetLineColor(kRed);
    
    TCanvas *c1 = new TCanvas();
    
    c1->Divide(2,3);
    c1->cd(1);
    h->Draw();
    h_1->Draw("same");
    h_2->Draw("same");
    h2->Draw("same");
    c1->cd(2);
    h_chi2->Draw();
    c1->cd(3);
    h_chi2_signal->Draw("colz");
    c1->cd(4);
    h_res->Draw("EPZ");
    c1->cd(5);
    h_chi2_2->Draw();
    c1->cd(6);
    h_chi2_chi2_2->Draw("colz");
}