int k;
TH1F *h_trackLengths;
TH1F *h_energy;
TH1F *h_energyRotated;
TH1F *h_energyOriginal;

int GetEntryAndExit(TGraph *entryPoints,TGraph *exitPoints, int i, double startX, double startY, double startAngle, double width, double height, double angle);
double CalculateEnergyDeposit(double x1, double y1, double x2, double y2, double width, double height, double energy, double tau, double angle);

void SimulateMuons()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    double detWidth = 10;
    double detHeight = 10;
    double detOffsetX = 0;
    double detOffsetY = 0;
    double detAngle = 0.01;
    double pmt1Width = 50.8;
    double pmt2Width = 50.8;
    double pmt1Dist = 85;
    double pmt2Dist = 95;
    
    double frameX1 = -100.0;
    double frameX2 = 100.0;
    double frameY1 = -100.0;
    double frameY2 = 100.0;
    
    double eLifetime = 2e-3;
    
    k = 0;
    
    h_trackLengths = new TH1F("h_traceLengths","h_trackLengths",100,0,1.5*detHeight);
    
    h_energy = new TH1F("h_energy","h_energy",100,0,5000);
    
    h_energyRotated = new TH1F("h_energyRotated","h_energyRotated",100,0,5000);
    
    h_energyOriginal = new TH1F("h_energyOriginal","h_energyOriginal",100,0,5000);
    
    h_energy->SetLineColor(kBlack);
    h_energy->SetLineWidth(1);
    
    h_energyRotated->SetLineColor(kBlue);
    h_energyRotated->SetLineWidth(1);
    
    h_energyOriginal->SetLineColor(kRed);
    h_energyOriginal->SetLineWidth(1);
    
    TRandom *randPosition = new TRandom();
    TRandom *randAngle = new TRandom();
    TRandom *randEnergy = new TRandom();
    
    randPosition->SetSeed(0);
    randAngle->SetSeed(1);
    randEnergy->SetSeed(2);
    
    // Construct the detector
    double detDiagonal = sqrt(detWidth*detWidth+detHeight*detHeight)/2.0;
    double detX[5] = {detDiagonal*cos(TMath::Pi()/180.0*(225.0-detAngle))+detOffsetX,detDiagonal*cos(TMath::Pi()/180.0*(315.0-detAngle))+detOffsetX,detDiagonal*cos(TMath::Pi()/180.0*(45.0-detAngle))+detOffsetX,detDiagonal*cos(TMath::Pi()/180.0*(135.0-detAngle))+detOffsetX,detDiagonal*cos(TMath::Pi()/180.0*(225.0-detAngle))+detOffsetX};
    double detY[5] = {detDiagonal*sin(TMath::Pi()/180.0*(225.0-detAngle))+detOffsetY,detDiagonal*sin(TMath::Pi()/180.0*(315.0-detAngle))+detOffsetY,detDiagonal*sin(TMath::Pi()/180.0*(45.0-detAngle))+detOffsetY,detDiagonal*sin(TMath::Pi()/180.0*(135.0-detAngle))+detOffsetY,detDiagonal*sin(TMath::Pi()/180.0*(225.0-detAngle))+detOffsetY};
    
    TGraph *det = new TGraph(5,detX,detY);
    
    det->SetLineColor(kGreen+2);
    det->SetLineWidth(1);
    det->SetFillColor(kGreen+2);
    det->SetFillStyle(3002);
    
    // Construct the PMTs
    TLine *pmt1 = new TLine(-pmt1Width/2.0,pmt1Dist,pmt1Width/2.0,pmt1Dist);
    TLine *pmt2 = new TLine(-pmt2Width/2.0,-pmt2Dist,pmt2Width/2.0,-pmt2Dist);
    
    pmt1->SetLineColor(kRed);
    pmt1->SetLineWidth(2);
    
    pmt2->SetLineColor(kRed);
    pmt2->SetLineWidth(2);
    
    // Generate tracks
    const int n = 100000;
    
    TLine *tracks[n];
    
    TGraph *entryPoints = new TGraph();
    TGraph *exitPoints = new TGraph();
    
    for (int i = 0; i < n; i++) {
        // Get the start position
        double trackX = randPosition->Uniform(-pmt1Width/2.0,pmt1Width/2.0);
        double angleMin = -(pmt2Width/2.0+trackX)/(pmt1Dist+pmt2Dist);
        double angleMax = (pmt2Width/2.0-trackX)/(pmt1Dist+pmt2Dist);
        double trackAngle = randAngle->Uniform(angleMin,angleMax);
        
        cout << "Generated track at x = " << trackX << " and angle = " << trackAngle << " x2 = " << trackAngle*(pmt1Dist+pmt2Dist) << endl;
        
        tracks[i] = new TLine(trackX,pmt1Dist,trackX+trackAngle*(pmt1Dist+pmt2Dist),-pmt2Dist);
        
        tracks[i]->SetLineColor(kBlue);
        tracks[i]->SetLineWidth(1);
        
        int track = GetEntryAndExit(entryPoints,exitPoints,i,trackX,pmt1Dist,trackAngle,detWidth,detHeight,detAngle);
        
        double entryX;
        double entryY;
        double exitX;
        double exitY;
        
        if (track == 1) {
            entryPoints->GetPoint(k-1,entryX,entryY);
            exitPoints->GetPoint(k-1,exitX,exitY);
            
            double energy = randEnergy->Landau(1000,100);
            
            double E = CalculateEnergyDeposit(entryX,entryY,exitX,exitY,detWidth,detHeight,energy,eLifetime, detAngle);
            
            h_energy->Fill(E);
            h_energyOriginal->Fill(energy);
        }
    }
    
    entryPoints->SetMarkerStyle(20);
    entryPoints->SetMarkerSize(0.5);
    entryPoints->SetMarkerColor(kRed);
    
    exitPoints->SetMarkerStyle(20);
    exitPoints->SetMarkerSize(0.5);
    exitPoints->SetMarkerColor(kGreen);
    
    cout << "ratio = " << h_energy->GetEntries()/n << endl;
    
    TH2F *h_empty = new TH2F("h_empty","h_empty",100,frameX1,frameX2,100,frameY1,frameY2);
    
    h_empty->GetXaxis()->SetTitle("x (mm)");
    h_empty->GetYaxis()->SetTitle("z (mm)");
    
    h_empty->GetXaxis()->CenterTitle();
    h_empty->GetYaxis()->CenterTitle();
    
    TCanvas *c1 = new TCanvas("c1","c1",600,600);
    h_empty->Draw();
    pmt1->Draw("same");
    pmt2->Draw("same");
    
    for (int i = 0; i < n; i++) {
        tracks[i]->Draw("same");
    }
    
    det->Draw("Fsame");
    entryPoints->Draw("Psame");
    exitPoints->Draw("Psame");
    
    TCanvas *c2 = new TCanvas();
    h_trackLengths->Draw();
    
    TCanvas *c3 = new TCanvas();
    h_energy->Draw();
    h_energyRotated->Draw("same");
    h_energyOriginal->Draw("same");
}

int GetEntryAndExit(TGraph *entryPoints,TGraph *exitPoints, int i, double startX, double startY, double startAngle, double width, double height, double angle)
{
    double a = -1.0/startAngle;
    double b = startY -a*startX;
    
    double angleRad = TMath::Pi()/180.0*angle;
    
    double x0 = TMath::Cos(-angleRad)*(-width/2.0) - TMath::Sin(-angleRad)*height/2.0;
    double y0 = TMath::Sin(-angleRad)*(-width/2.0) + TMath::Cos(-angleRad)*height/2.0;
    
    double a1 = -TMath::Tan(TMath::Pi()/180.0*angle);
    double b1 = y0 - a1*x0;
    
    double x1 = (b1-b)/(a-a1);
    double y1 = a*x1 + b;
    
    double x1_0 = TMath::Cos(angleRad)*x1 - TMath::Sin(angleRad)*y1;
    double y1_0 = TMath::Sin(angleRad)*x1 + TMath::Cos(angleRad)*y1;
    
    double startAngle_0 = TMath::Tan(TMath::ATan(startAngle) + angleRad);
    
    double a_0 = -1.0/startAngle_0;
    double b_0 = y1_0 - a_0*x1_0;
    
    double entryPointX_0 = 0.0;
    double entryPointY_0 = 0.0;
    
    double exitPointX_0 = 0.0;
    double exitPointY_0 = 0.0;
    
    double entryPointX = 0.0;
    double entryPointY = 0.0;
    
    double exitPointX = 0.0;
    double exitPointY = 0.0;
    
    if (x1_0 < -width/2.0 && startAngle_0 < 0) {return 0;}
    if (x1_0 < -width/2.0) {
        double x2 = -width/2.0;
        double y2 = a_0*x2 + b_0;
        
        if (y2 < -height/2.0) {return 0;}
        
        entryPointX_0 = x2;
        entryPointY_0 = y2;
        
        exitPointX_0 = (-height/2.0-b_0)/a_0;
        exitPointY_0 = -height/2.0;
        
        if (exitPointX_0 > width/2.0) {
            exitPointX_0 = width/2.0;
            exitPointY_0 = a_0*exitPointX_0 + b_0;
        }
    }
    if (x1_0 >= -width/2.0 && x1_0 <= width/2.0) {
        entryPointX_0 = x1_0;
        entryPointY_0 = y1_0;
        
        exitPointX_0 = (-height/2.0-b_0)/a_0;
        exitPointY_0 = -height/2.0;
        
        if (exitPointX_0 < -width/2.0 || exitPointX_0 > width/2.0) {
            if (startAngle_0 < 0) {exitPointX_0 = -width/2.0;}
            else {exitPointX_0 = width/2.0;}
            
            exitPointY_0 = a_0*exitPointX_0 + b_0;
        }
    }
    if (x1_0 > width/2.0 && startAngle_0 > 0) {return 0;}
    if (x1_0 > width/2.0) {
        double x2 = width/2.0;
        double y2 = a_0*x2 + b_0;
        
        if (y2 < -height/2.0) {return 0;}
        
        entryPointX_0 = x2;
        entryPointY_0 = y2;
        
        exitPointX_0 = (-height/2.0-b_0)/a_0;
        exitPointY_0 = -height/2.0;
        
        if (exitPointX_0 < -width/2.0) {
            exitPointX_0 = -width/2.0;
            exitPointY_0 = a_0*exitPointX_0 + b_0;
        }
    }
    
    entryPointX = TMath::Cos(-angleRad)*entryPointX_0 - TMath::Sin(-angleRad)*entryPointY_0;
    entryPointY = TMath::Sin(-angleRad)*entryPointX_0 + TMath::Cos(-angleRad)*entryPointY_0;
    
    exitPointX = TMath::Cos(-angleRad)*exitPointX_0 - TMath::Sin(-angleRad)*exitPointY_0;
    exitPointY = TMath::Sin(-angleRad)*exitPointX_0 + TMath::Cos(-angleRad)*exitPointY_0;
    
    cout << entryPointX << " " << entryPointY << " " << exitPointX << " " << exitPointY << endl;
    
    entryPoints->SetPoint(k,entryPointX,entryPointY);
    exitPoints->SetPoint(k,exitPointX,exitPointY);
    
    h_trackLengths->Fill(TMath::Abs(TMath::Sqrt((entryPointX-exitPointX)*(entryPointX-exitPointX)+(entryPointY-exitPointY)*(entryPointY-exitPointY))));
    
    k++;
    
    return 1;
}

double CalculateEnergyDeposit(double x1, double y1, double x2, double y2, double width, double height, double energy, double tau, double angle)
{
    double trackLength = TMath::Abs(TMath::Sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    double e = energy/10*trackLength;
    double xStep = (x2-x1)/1000;
    double yStep = (y2-y1)/1000;
    double dE = e/1000;
    
    double angleRad = TMath::Pi()/180.0*angle;
    
    double x0 = TMath::Cos(-angleRad)*(width/2.0) - TMath::Sin(-angleRad)*height/2.0;
    double y0 = TMath::Sin(-angleRad)*(width/2.0) + TMath::Cos(-angleRad)*height/2.0;
    
    double a1 = -TMath::Tan(TMath::Pi()/180.0*angle);
    double b1 = y0 - a1*x0;
    double b1_2;
    
    double a2 = -1/a1;
    double b2;
    
    double E = 0.0;
    double E2 = 0.0;
    
    for (int i = 0; i < 1000; i++) {
        double x = x1 + i*xStep;
        double y = y1 + i*yStep;
        
        b2 = y - a2*x;
        
        double x2 = (b2-b1)/(a1-a2);
        double y2 = a2*x2 + b2;
        
        double dist = TMath::Sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));
        
        E += dE*TMath::Exp(-dist/(4500000*tau));
        
        b1_2 = y - a1*x;
        b2 = y0 - a2*x0;
        
        x2 = (b2-b1_2)/(a1-a2);
        y2 = a2*x2 + b2;
        
        dist = TMath::Sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));
        
        E2 += dE*TMath::Exp(-dist/(4500000*tau));
    }
    
    cout << "Energy original = " << energy << " Energy = " << E << " for track length = " << trackLength << endl;
    
    h_energyRotated->Fill(E2);
    
    return E;
}
