void AnalyzeEu154()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    //string filename1 = "061716_2600VG2_2600VG1_n2000V_10mm_2um_TEA_Eu154_ch1_001.root";
    //string filename2 = "061716_2600VG2_2600VG1_n2000V_10mm_2um_TEA_bg_ch1_001.root";
    
    string filename1 = "061716_2600VNG_n2000V_10mm_2um_TEA_Eu154_001.root";
    string filename2 = "TMS\ Muons/042116_2000V_n2000V_10mm_Eu154_ch2_5m_001.root";
    
    TFile *f = new TFile(filename1.c_str(),"READ");
    TFile *f2 = new TFile(filename2.c_str(),"READ");
    
    TTree *t = (TTree*)f->Get("dataTree");
    TTree *t2 = (TTree*)f2->Get("dataTree");
    
    TH1F *h = new TH1F("h","h",120,0,24000);
    TH1F *h2 = new TH1F("h2","h2",120,0,24000);
    
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
    
    // **** New preamp, channel 2, shaper1
    double calib_p0_ch2 = 0;
    double calib_p1_ch2 = 2.235;
    
    double yMax = 100;
    
    double threshold = 9000;
    
    cout << "baseline 1 = " << baseline_1 << "  RMS 1 = " << rms_1 << " (" << rms_1*calib_p1_ch1 << " electrons)" << endl;
    cout << "baseline 2 = " << baseline_2 << "  RMS 2 = " << rms_2 << " (" << rms_2*calib_p1_ch2 << " electrons)" << endl;
    
    t->Draw(Form("(peak-baseline)*%f+%f>>h",calib_p1_ch1,calib_p0_ch1),"peak_position > 1000 && isNoise == 0","goff");
    t2->Draw(Form("(peak-baseline)*%f+%f>>h2",calib_p1_ch1,calib_p0_ch1),"peak_position > 1000 && isNoise == 0","goff");
    
    h->SetLineColor(kBlack);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.5);
    h->SetMarkerColor(kBlack);
    
    h2->SetLineColor(kRed);
    h2->SetMarkerStyle(20);
    h2->SetMarkerSize(0.5);
    h2->SetMarkerColor(kRed);
    
    h->GetXaxis()->SetTitle("number of electrons");
    h->GetYaxis()->SetTitle("counts");
    
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    
    h->GetYaxis()->SetRangeUser(0,yMax);
    
    TH1F *h_diff = h->Clone();
    
    h_diff->Add(h2,-1);
    
    TGraph *gr = new TGraph(5);
    
    gr->SetPoint(0,0,0);
    gr->SetPoint(1,3*rms_1*calib_p1_ch1,0);
    gr->SetPoint(2,3*rms_1*calib_p1_ch1,yMax);
    gr->SetPoint(3,0,yMax);
    gr->SetPoint(4,0,0);
    
    gr->SetFillColor(kGreen);
    gr->SetFillStyle(3003);
    
    TLine *l2 = new TLine((threshold-baseline_1)*calib_p1_ch1,0,(threshold-baseline_1)*calib_p1_ch1,yMax);
    TLine *l3 = new TLine((threshold-baseline_2)*calib_p1_ch2,0,(threshold-baseline_2)*calib_p1_ch2,yMax);
    
    l2->SetLineColor(kBlack);
    l2->SetLineWidth(2);
    l2->SetLineStyle(2);
    
    l3->SetLineColor(kRed);
    l3->SetLineWidth(2);
    l3->SetLineStyle(2);
    
    TLegend *l = new TLegend(0.8,0.8,0.9,0.9);
    
    l->AddEntry(h,"Eu-154","lp");
    l->AddEntry(h2,"Background","lp");
    l->AddEntry(l2,"threshold Eu-154","l");
    l->AddEntry(l3,"threshold background","l");
    l->AddEntry(gr,"3 #sigma noise","f");
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    
    h->Draw("EZP");
    h2->Draw("EZP same");
    gr->Draw("Fsame");
    l->Draw("same");
    l2->Draw("same");
    l3->Draw("same");
    
    TCanvas *c2 = new TCanvas();
    h_diff->Draw("EZP");
}