void PlotGainVsNoGain()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    string input1 = "/Volumes/Time\ Machine\ Backups/Stanford/result_LXe_Bi207_Shaper1_G50_CAEN_650VG1_1350VG2.root"; // Gain file
    string input2 = "/Volumes/Time\ Machine\ Backups/Stanford/result_LXe_Bi207_Shaper2_G50_CAEN_1000VNG.root"; // Non-gain file
    
    string NEST_table = "NEST_table_1kVcm.txt";
    string MC = "Bi207_MCHist_10mm_2.root";
    
    double calib_p0_gain = 0;
    double calib_p1_gain = 24.86;
    double calib_p0_nonGain = 0;
    double calib_p1_nonGain = 23.95;
    
    double xmin = 0;
    double xmax = 200000;
    double ymin = 1;
    double ymax = 1e5;
    int numberOfBins = 1000;
    
    string cut = "";
    
    cut += "isNoise==0";
    //cut += "&&rms<150";
    //cut += "&&peakPosition>1040";
    //cut += "&&peakPosition<1160";
    
    TFile *f1 = new TFile(input1.c_str(),"READ");
    TFile *f2 = new TFile(input2.c_str(),"READ");
    
    TTree *t1 = (TTree*)f1->Get("dataTree");
    TTree *t2 = (TTree*)f2->Get("dataTree");
    
    TH1F *h1 = new TH1F("h1","h1",numberOfBins,xmin,xmax);
    TH1F *h2 = new TH1F("h2","h2",numberOfBins,xmin,xmax);
    
    TH1F *h1_noise = new TH1F("h1_noise","h1_noise",numberOfBins,xmin,xmax);
    TH1F *h2_noise = new TH1F("h2_noise","h2_noise",numberOfBins,xmin,xmax);
    
    t1->Draw(Form("(peak-baseline)*%f+%f>>h1",calib_p1_gain,calib_p0_gain),cut.c_str(),"goff");
    t2->Draw(Form("(peak-baseline)*%f+%f>>h2",calib_p1_nonGain,calib_p0_nonGain),cut.c_str(),"goff");
    
    t1->Draw(Form("(peak-baseline)*%f+%f>>h1_noise",calib_p1_gain,calib_p0_gain),"isNoise==1","goff");
    t2->Draw(Form("(peak-baseline)*%f+%f>>h2_noise",calib_p1_nonGain,calib_p0_nonGain),"isNoise==1","goff");
    
    TTree *t = new TTree();
    
    t->ReadFile(NEST_table.c_str());
    
    t->Draw("energy:ionization*energy","","goff");
    
    TGraph *gr1 = new TGraph(t->GetSelectedRows(),t->GetV2(),t->GetV1());
    TGraph *gr2 = new TGraph(t->GetSelectedRows(),t->GetV1(),t->GetV2());
    
    TFile *f3 = new TFile(MC.c_str(),"READ");
    
    TH1F *h3 = (TH1F*)f3->Get("h");
    
    double nBins = h3->GetNbinsX();
    double hXmin = h3->GetXaxis()->GetXmin();
    double hXmax = h3->GetXaxis()->GetXmax();
    double binSize = h3->GetXaxis()->GetBinWidth(0);
    
    TH1F *h4 = new TH1F("h4","h4",nBins,0,gr2->Eval(hXmax*1000));
    
    for (int i = 0; i < nBins; i++) {
        double binContent = h3->GetBinContent(i);
        double energy = hXmin + i*binSize + binSize/2;
        double electrons = gr2->Eval(energy*1000);
        
        h4->SetBinContent(h4->FindBin(electrons),binContent);
    }
    
    double MCscale = 1.0;
    
    h4->Scale(MCscale);
    
    h4->SetLineColor(kGreen+1);
    
    h4->Rebin(5);
    
    double scale = h1->Integral(h1->FindBin(25000),h1->FindBin(35000)) / h2->Integral(h2->FindBin(25000),h2->FindBin(35000));
    
    h2->Scale(scale);
    h2_noise->Scale(scale);
    
    h1->SetLineColor(kBlack);
    h2->SetLineColor(kRed+1);
    
    h1_noise->SetLineColor(kGray+1);
    h2_noise->SetLineColor(kGray+2);
    
    h1_noise->SetFillStyle(1001);
    h2_noise->SetFillStyle(1001);
    
    h1_noise->SetFillColor(kGray+1);
    h2_noise->SetFillColor(kGray+2);
    
    TGaxis *axis = new TGaxis(xmin,ymax,xmax,ymax,0,gr1->Eval(xmax),510,"-",0);
    
    axis->SetTitle("energy (keV)");
    axis->CenterTitle();
    axis->SetLabelFont(42);
    axis->SetTitleFont(42);
    axis->SetLabelSize(0.035);
    axis->SetTitleSize(0.035);
    axis->SetTitleOffset(1.2);
    
    TH2D *h_empty = new TH2D("h_empty","h_empty",100,xmin,xmax,100,ymin,ymax);
    
    h_empty->GetXaxis()->SetTitle("number of electrons");
    h_empty->GetYaxis()->SetTitle("counts");
    
    h_empty->GetXaxis()->CenterTitle();
    h_empty->GetYaxis()->CenterTitle();
    
    TLegend *l = new TLegend(0.6,0.65,0.85,0.85);
    
    l->AddEntry(h1,"Gain","l");
    l->AddEntry(h2,"Non-Gain","l");
    l->AddEntry(h4,"MC","l");
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    
    c1->SetLogy();
    c1->SetGridy();
    c1->SetGridx();
    
    h_empty->Draw();
    h1_noise->Draw("same");
    h2_noise->Draw("same");
    h1->Draw("same");
    h2->Draw("same");
    h4->Draw("same");
    axis->Draw("same");
    l->Draw("same");
}
