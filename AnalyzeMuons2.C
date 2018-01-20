void AnalyzeMuons2()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    string filename1 = "040516_2000V_1mm_muon_ch1_001.root";
    string filename2 = "040516_4500V_10mm_muon_ch2_001.root";
    
    /*TFile *f = new TFile(filename1.c_str(),"READ");
    TFile *f2 = new TFile(filename2.c_str(),"READ");
    
    TTree *t = (TTree*)f->Get("dataTree");
    TTree *t2 = (TTree*)f2->Get("dataTree");*/
    
    TChain *t = new TChain("dataTree","dataTree");
    TChain *t2 = new TChain("dataTree","dataTree");
    
    t->Add(filename1.c_str());
    t2->Add(filename2.c_str());
    
    TH1F *h_basline_1 = new TH1F("h_baseline_1","h_baseline_1",100,7000,9000);
    TH1F *h_basline_2 = new TH1F("h_baseline_2","h_baseline_2",100,7000,9000);
    
    TH1F *h_rms_1 = new TH1F("h_rms_1","h_rms_1",100,100,600);
    TH1F *h_rms_2 = new TH1F("h_rms_2","h_rms_2",100,100,600);
    
    t->Draw("baseline>>h_baseline_1","","goff");
    t2->Draw("baseline>>h_baseline_2","","goff");
    
    t->Draw("rms>>h_rms_1","","goff");
    t2->Draw("rms>>h_rms_2","","goff");
    
    double baseline_1 = h_baseline_1->GetMean();
    double baseline_2 = h_baseline_2->GetMean();
    
    double rms_1 = h_rms_1->GetMean();
    double rms_2 = h_rms_2->GetMean();
    
    // **** New preamp, channel 1, shaper1
    double calib_p0_ch1 = 0;
    double calib_p1_ch1 = 2.392;
    
    // **** New preamp, channel 2, shaper2
    //double calib_p0_ch2 = 0;
    //double calib_p1_ch2 = 2.209;
    
    // **** New preamp, channel 2, shaper1
    double calib_p0_ch2 = 0;
    double calib_p1_ch2 = 2.235;
    
    double yMax = 250;
    
    cout << "RMS 1 = " << rms_1 << " (" << rms_1*calib_p1_ch1 << " electrons)" << endl;
    cout << "RMS 2 = " << rms_2 << " (" << rms_2*calib_p1_ch2 << " electrons)" << endl;
    
    TH1F *h_spectrum_1 = new TH1F("h_spectrum_1","h_spectrum_1",200,0,8000);
    TH1F *h_spectrum_2 = new TH1F("h_spectrum_2","h_spectrum_2",200,0,8000);
    
    t->Draw(Form("(peak-baseline)*%f+%f>>h_spectrum_1",calib_p1_ch1,calib_p0_ch1),"peak_position > 1100 && peak_position < 1250","goff");
    t2->Draw(Form("(peak-baseline)*%f+%f>>h_spectrum_2",calib_p1_ch2,calib_p0_ch2),"peak_position > 1100 && peak_position < 1250","goff");
    
    h_spectrum_1->SetLineColor(kBlack);
    h_spectrum_1->SetMarkerStyle(20);
    h_spectrum_1->SetMarkerSize(0.5);
    h_spectrum_1->SetMarkerColor(kBlack);
    
    h_spectrum_2->SetLineColor(kRed);
    h_spectrum_2->SetMarkerStyle(20);
    h_spectrum_2->SetMarkerSize(0.5);
    h_spectrum_2->SetMarkerColor(kRed);
    
    h_spectrum_1->GetYaxis()->SetRangeUser(0,yMax);
    
    TLegend *l = new TLegend(0.8,0.8,0.9,0.9);
    
    l->AddEntry(h_spectrum_1,"Muons 1mm, V_{bias}=2000V","lp");
    l->AddEntry(h_spectrum_2,"Muons 10mm, V_{bias}=4500V","lp");
    
    l->SetFillStyle(0);
    
    TGraph *gr = new TGraph(5);
    
    gr->SetPoint(0,0,0);
    gr->SetPoint(1,3*rms_1*calib_p1_ch1,0);
    gr->SetPoint(2,3*rms_1*calib_p1_ch1,yMax);
    gr->SetPoint(3,0,yMax);
    gr->SetPoint(4,0,0);
    
    gr->SetFillColor(kGreen);
    gr->SetFillStyle(3003);
    
    TLine *l2 = new TLine(rms_1*calib_p1_ch1,0,rms_1*calib_p1_ch1,yMax);
    TLine *l3 = new TLine(rms_2*calib_p1_ch2,0,rms_2*calib_p1_ch2,yMax);
    
    l2->SetLineColor(kBlack);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    
    l3->SetLineColor(kRed);
    l3->SetLineWidth(2);
    l3->SetLineStyle(2);
    
    TCanvas *c1 = new TCanvas();
    
    h_spectrum_1->Draw("EZP");
    h_spectrum_2->Draw("EZP same");
    l->Draw("same");
    gr->Draw("Fsame");
    l2->Draw("same");
    l3->Draw("same");
}
