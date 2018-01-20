TGraph *GetErrorBand(TF1 *f1, TF1 *f2);
TGraph *GetCrossSectionBand(int n, TGraph *gr);
TGraph *GetCrossSectionBand(TGraph *gr1, TGraph *gr2);

double range_low = 0.1e-6;
double range_up = 5e-6;
double yMin = 0;
double yMax = 200;

void PlotWvsTau()
{
    /*string function1 = "112500000000*exp(-1/(450000*x))*(exp(1/(450000*x))-1)*x/1021";
    string function1_errP = "1.08449e8*(1-exp(-2.22222e-6/x))*x";
    string function1_errN = "1.11979e8*(1-exp(-2.22222e-6/x))*x";
    
    string function2 = "35e10*exp(-1/(350000*x))*(exp(1/(350000*x))-1)*x/3557";
    string function2_errP = "9.43854e7*(1-exp(-2.85714e-6/x))*x";
    string function2_errN = "1.02766e8*(1-exp(-2.85714e-6/x))*x";
    
    string function3 = "1.16682e8*(1-exp(-1.66667e-6/x))*x";
    string function3_errP = "1.11019e8*(1-exp(-1.66667e-6/x))*x";
    string function3_errN = "1.22953e8*(1-exp(-1.66667e-6/x))*x";
    
    string function3 = "1.21246e8*(1-exp(-1.66667e-6/x))*x";
    string function3_errP = "1.14879e8*(1-exp(-1.66667e-6/x))*x";
    string function3_errN = "1.28361e8*(1-exp(-1.66667e-6/x))*x";*/
    
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    double peak_err_sys = 0.012;
    
    const int nMeasurements = 5;
    
    double peaks[nMeasurements] = {5892.1, 6197.7, 5056.7, 4471.2, 3166.3}; // 8 kV/cm, 5.1 kV/cm, 3.9 kV/cm, 3.3 kV/cm, 2.6 kV/cm
    double peaks_err_stat[nMeasurements] = {195.6, 154.3, 133.5, 149.0, 211.8};
    double peaks_err[nMeasurements];
    
    string labels[nMeasurements] = {"8.0 kV/cm", "5.1 kV/cm", "3.9 kV/cm", "3.3 kV/cm", "2.6 kV/cm"};
    
    for (int i = 0; i < nMeasurements; i++) {
        peaks_err[i] = sqrt(peaks_err_stat[i]*peaks_err_stat[i] + peak_err_sys*peak_err_sys*peaks[i]*peaks[i]);
    }
    
    double eFields[nMeasurements] = {8.0, 5.1, 3.9, 3.3, 2.6};
    double eFields_err[nMeasurements] = {0, 0, 0, 0, 0};
    
    TGraphErrors *grPeakvsField = new TGraphErrors(nMeasurements,eFields,peaks,eFields_err,peaks_err);
    
    grPeakvsField->SetMarkerStyle(20);
    grPeakvsField->SetMarkerSize(0.5);
    
    string function = "[0]*1e8*(1-exp(-[1]*1e-6/x))*x";
    
    TF1 *f[nMeasurements];
    TF1 *f_errP[nMeasurements];
    TF1 *f_errN[nMeasurements];
    
    for (int i = 0; i < nMeasurements; i++) {
        f[i] = new TF1(Form("f%i",i),function.c_str(),range_low,range_up);
        f_errP[i] = new TF1(Form("f%i_errP",i),function.c_str(),range_low,range_up);
        f_errN[i] = new TF1(Form("f%i_errN",i),function.c_str(),range_low,range_up);
    }
    
    TF1 *f1 = new TF1("f1",function.c_str(),range_low,range_up);
    TF1 *f1_errP = new TF1("f1_errP",function.c_str(),range_low,range_up);
    TF1 *f1_errN = new TF1("f1_errN",function.c_str(),range_low,range_up);
    
    TF1 *f2 = new TF1("f2",function.c_str(),range_low,range_up);
    TF1 *f2_errP = new TF1("f2_errP",function.c_str(),range_low,range_up);
    TF1 *f2_errN = new TF1("f2_errN",function.c_str(),range_low,range_up);
    
    TF1 *f3 = new TF1("f3",function.c_str(),range_low,range_up);
    TF1 *f3_errP = new TF1("f3_errP",function.c_str(),range_low,range_up);
    TF1 *f3_errN = new TF1("f3_errN",function.c_str(),range_low,range_up);
    
    // Cube 1 4.5kV/cm
    /*f1->SetParameters(1.10448,2.22222);
    f1_errP->SetParameters(1.07766,2.22222);
    f1_errN->SetParameters(1.13267,2.22222);
    
    // Cube 1 3.5kV/cm
    f2->SetParameters(0.89452,2.85714);
    f2_errP->SetParameters(0.86430,2.85714);
    f2_errN->SetParameters(0.92693,2.85714);*/
    
    // Cube 2 6kV/cm
    /*f3->SetParameters(1.08909,1.66667);
    f3_errP->SetParameters(1.04934,1.66667);
    f3_errN->SetParameters(1.13197,1.66667);
    
    // Cube 2 4.5kV/cm
    f1->SetParameters(0.96591,2.22222);
    f1_errP->SetParameters(0.91680,2.22222);
    f1_errN->SetParameters(1.02059,2.22222);
    
    // Cube 2 6.6kV/cm
    f2->SetParameters(1.15264,1.51515);
    f2_errP->SetParameters(1.13217,1.51515);
    f2_errN->SetParameters(1.17386,1.51515);*/
    
    // TMS contineous purification 8 kV/cm
    f[0]->SetParameters(1.357752,1.25);
    f_errP[0]->SetParameters(1.311456,1.25);
    f_errN[0]->SetParameters(1.407432,1.25);
    
    // TMS contineous purification 5.1 kV/cm
    f[1]->SetParameters(0.822885,1.960784);
    f_errP[1]->SetParameters(0.7997514,1.960784);
    f_errN[1]->SetParameters(0.8474007,1.960784);
    
    // TMS contineous purification 3.3 kV/cm
    f[2]->SetParameters(0.7380582,3.030303);
    f_errP[2]->SetParameters(0.7128099,3.030303);
    f_errN[2]->SetParameters(0.7652238,3.030303);
    
    // TMS contineous purification 3.9 kV/cm
    f[3]->SetParameters(0.7514766,2.6315789);
    f_errP[3]->SetParameters(0.7302992,2.6315789);
    f_errN[3]->SetParameters(0.7739194,2.6315789);
    
    // TMS contineous purification 2.6 kV/cm
    f[4]->SetParameters(0.8211476,3.846154);
    f_errP[4]->SetParameters(0.7688928,3.846154);
    f_errN[4]->SetParameters(0.8810204,3.846154);
    
    int colors[10] = {601, 633, 417, 401, 617, 433, 801, 821, 801};
    
    TGraph *errBands[nMeasurements];
    
    for (int i = 0; i < nMeasurements; i++) {
        f[i]->SetLineColor(colors[i]);
        f[i]->SetLineWidth(1);
        
        errBands[i] = GetErrorBand(f_errP[i], f_errN[i]);
        
        errBands[i]->SetLineWidth(1);
        errBands[i]->SetLineColor(colors[i]);
        errBands[i]->SetFillStyle(3001);
        errBands[i]->SetFillColor(colors[i]-1);
    }
    
    TH2F *h_empty = new TH2F("h_empty","h_empty",100,range_low,range_up,100,yMin,yMax);
    
    h_empty->GetXaxis()->SetTitle("#tau (s)");
    h_empty->GetYaxis()->SetTitle("W (eV)");
    
    h_empty->GetXaxis()->CenterTitle();
    h_empty->GetYaxis()->CenterTitle();
    
    TGraph *errBands_CrossSection[3] = {errBands[1], errBands[2], errBands[3]};
    
    TGraph *gr = GetCrossSectionBand(3, errBands_CrossSection);
    
    gr->SetLineWidth(1);
    gr->SetLineColor(kBlack);
    //gr->SetFillStyle(3001);
    gr->SetFillColor(kBlack);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.5);
    
    TGraphErrors *gr2 = GetCenterPoint(gr);
    
    TGraph *gr3 = gr->Clone();
    
    gr3->SetFillColor(kBlue+2);
    gr3->SetFillStyle(3001);
    
    cout << "Best value for W = " << gr2->GetY()[0] << " +- " << gr2->GetErrorY(0) << endl;
    cout << "               tau = " << gr2->GetX()[0] << " +- " << gr2->GetErrorX(0) << endl;
    
    TLine *l1 = new TLine(1.8e-6,0,1.8e-6,150);
    
    TLine *l2 = new TLine(range_low,140,range_up,140);
    
    l1->SetLineColor(kBlue);
    l1->SetLineWidth(1);
    l1->SetLineStyle(2);
    
    l2->SetLineColor(kBlue);
    l2->SetLineWidth(1);
    l2->SetLineStyle(2);
    
    TLegend *l = new TLegend(0.6,0.2,0.85,0.5);
    
    for (int i = 0; i < nMeasurements; i++) {
        if (i < 1 || i > 3) {continue;}
        l->AddEntry(f[i],labels[i].c_str(),"l");
    }
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    
    h_empty->Draw();
    
    for (int i = 0; i < nMeasurements; i++) {
        if (i < 1 || i > 3) {continue;}
        errBands[i]->Draw("Fsame");
        f[i]->Draw("same");
    }
    
    l->Draw("same");
    
    TCanvas *c2 = new TCanvas();
    grPeakvsField->Draw("AP");
    
    /*TCanvas *c3 = new TCanvas();
    gr3->Draw("AF");
    gr2->Draw("Psame");*/
}

TGraph *GetErrorBand(TF1 *f1, TF1 *f2) {
    TGraph *gr = new TGraph(20000);
    
    int nSteps = 10000;
    double step = (range_up-range_low)/9999.0;
    
    for (int i = 0; i < nSteps; i++) {
        gr->SetPoint(i,step*i+range_low,f1->Eval(step*i+range_low));
        gr->SetPoint(2*nSteps-1-i,step*i+range_low,f2->Eval(step*i+range_low));
    }
    
    return gr;
}

TGraph *GetCrossSectionBand(int n, TGraph *gr) {
    TGraph *grTMP;
    TGraph *crossSectionBand = gr[0];
    
    for (int i = 1; i < n; i++) {
        grTMP = GetCrossSectionBand(crossSectionBand,gr[i]);
        
        crossSectionBand = grTMP;
    }
    
    return crossSectionBand;
}

TGraph *GetCrossSectionBand(TGraph *gr1, TGraph *gr2) {
    TGraph *gr = new TGraph(20000);
    
    int nSteps = 10000;
    
    double *gr1_x = gr1->GetX();
    double *gr1_y = gr1->GetY();
    double *gr2_y = gr2->GetY();
    
    double *x = new double();
    double *y = new double();
    
    int k = 0;
    
    for (int i = 0; i < nSteps; i++) {
        double gr1_up = gr1_y[2*nSteps-1-i];
        double gr1_low = gr1_y[i];
        double gr2_up = gr2_y[2*nSteps-1-i];
        double gr2_low = gr2_y[i];
        
        if (gr1_low > gr1_up) {gr1_low = gr1_up; gr1_up = gr1_y[i];}
        if (gr2_low > gr2_up) {gr2_low = gr2_up; gr2_up = gr2_y[i];}
        
        if (gr2_low < gr1_up) {
            if (gr1_low > gr2_up) {continue;}
            if (gr2_low < gr1_low) {gr->SetPoint(i,gr1_x[i],gr1_low);}
            else {gr->SetPoint(i,gr1_x[i],gr2_low);}
            
            if (gr2_up > gr1_up) {gr->SetPoint(2*nSteps-1-i,gr1_x[i],gr1_up);}
            else {gr->SetPoint(2*nSteps-1-i,gr1_x[i],gr2_up);}
        }
        else if (gr1_low < gr2_up) {
            if (gr2_low > gr1_up) {continue;}
            if (gr2_low < gr1_low) {gr->SetPoint(i,gr1_x[i],gr2_low);}
            else {gr->SetPoint(i,gr1_x[i],gr1_low);}
            
            if (gr2_up > gr1_up) {gr->SetPoint(2*nSteps-1-i,gr1_x[i],gr2_up);}
            else {gr->SetPoint(2*nSteps-1-i,gr1_x[i],gr1_up);}
        }
    }
    
    return gr;
}

TGraph *GetCenterPoint(TGraph *gr) {
    int n = gr->GetN();
    double *x = gr->GetX();
    double *y = gr->GetY();
    
    double xMean = 0;
    double yMean = 0;
    
    double xMin = 1e6;
    double xMax = -1e6;
    double yMin = 1e6;
    double yMax = -1e6;
    
    int k = 0;
    
    for (int i = 0; i < n; i++) {
        if (x[i] <= 0 || y[i] <= 0) {continue;}
        
        xMean += x[i];
        yMean += y[i];
        
        if (x[i] > xMax) {xMax = x[i];}
        if (x[i] < xMin) {xMin = x[i];}
        if (y[i] > yMax) {yMax = y[i];}
        if (y[i] < yMin) {yMin = y[i];}
        
        k++;
    }
    
    xMean /= k;
    yMean /= k;
    
    double xErr = (xMax-xMin)/2;
    double yErr = (yMax-yMin)/2;
    
    TGraphErrors *grCenter = new TGraphErrors(1);
    
    grCenter->SetPoint(0,xMean,yMean);
    grCenter->SetPointError(0,xErr,yErr);
    
    grCenter->SetMarkerStyle(3);
    grCenter->SetMarkerColor(kRed+1);
    
    return grCenter;
}
