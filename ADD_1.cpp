#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

double getf1(double d);
double getf2(double d);
double getf3(double d);
double get1x52(double d);
double sawtooth(double d);
double grand();
double frand();


int main()
{
    srand((unsigned int)time(NULL));
    double w[1001];
    double f1w[1001];
    double f2w[1001];
    double f3w[1001];

    for(int i = 0;i < 1001;i++){
        w[i] = (i*0.01) - 5;
        f1w[i] = getf1(w[i]);
        f2w[i] = getf2(w[i]);
        f3w[i] = getf3(w[i]);
    }

    double tt[301];
    double pos = 0;
    double sig[301];
    for(int i = 0;i < 301;i++){
        tt[i] = (i*0.1) - 15;
        sig[i] = getf2(2*tt[i]-pos);
        //sig2[i] += 0.5*grand();
        sig[i] += 5*abs(sawtooth(tt[i]/1.5-2));
        //sig2[i] += -0.5*tt[i]

    }

    double scaled = 0.001;
    double fsig1[301];
    double fsig2[301];
    double fsig3[301];
    double fsig[301];
    double f1f,f2f,f3f;
    double targ_j;
    double interval_precal[601];
    double targ_j_precal1[601];
    double targ_j_precal2[601];
    double targ_j_precal3[601];


    double interval = tt[1]-tt[0];

    for(int i = 0;i < 601;i++){
        interval_precal[i] = interval*(i-300)/scaled;
        targ_j_precal1[i] = getf1(interval_precal[i]);
        targ_j_precal2[i] = getf2(interval_precal[i]);
        targ_j_precal3[i] = getf3(interval_precal[i]);
    }

   

    for(int i = 0;i < 301;i++){
        pos = tt[i];
        fsig1[i] = 0;
        fsig2[i] = 0;
        fsig3[i] = 0;
        for(int j = 0;j < 301;j++){
            f1f = targ_j_precal1[j-i+300]*sig[j];
            f2f = targ_j_precal2[j-i+300]*sig[j];
            f3f = targ_j_precal3[j-i+300]*sig[j];
            //fsig1[i] += f1f;
            fsig2[i] += f2f;
            //fsig3[i] += f3f;
        }
        fsig[i] = sqrt(fsig1[i]*fsig1[i]+fsig2[i]*fsig2[i]+fsig3[i]*fsig3[i]);
    }
    cout << sig[150] << endl;
    cout << fsig[150] << endl;
    return 0;
}


double getf1(double d){
    double c1,c2,c3;
    c1 = sqrt(128.0/M_PI/3.0);
    c2 = d*d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double getf2(double d){
    double c1,c2,c3;
    c1 = sqrt(128.0/M_PI/5.0);
    c2 = d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double getf3(double d){
    double c1,c2,c3;
    c1 = sqrt(24.0/M_PI/5.0);
    c2 = 1-(5.0/3.0)*d*d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double get1x52(double d){
    return pow(1+d*d,-2.5);
}

double sawtooth(double d){
    double temp;
    temp = floor(d/M_PI/2)*2;
    return d/M_PI-temp-1;
}

double grand(){
    double v1,v2,S;
    do{
        v1 = 2*frand()-1;
        v2 = 2*frand()-1;
        S = v1*v1+v2*v2;
    }while(S>=1||S==0);
    S = sqrt(-2*log(S)/S);
    return v1*S;
    
}

double frand(){
    return rand()/((double)RAND_MAX);
}