//
//  main.cpp
//  Hello world
//
//  Created by Marco  on 27/02/14.
//  Copyright (c) 2014 Marco . All rights reserved.
//

#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;


const double r=0.01895, m=0.034, g=9.806, s_r=0.00001, s_g=0.001, s_m=0.0005;
double Momento(double al, double be);
long double Sigma(double alpha, double s_alpha, double beta, double s_beta);
void huhu();
void haha();

int main(int argc, const char * argv[])
{
    
    //cin alpha beta ed errori
    long double alpha, beta, s_alpha, s_beta,I_0,s_I_0;
    
    cout<<"---------------------Calcolare il momento d'inerzia----------------------------------------";
    cout<< endl;
    
    cout<< "Alpha = ";
    cin>> alpha;
    cout<< "Errore di alpha = ";
    cin>> s_alpha;
    cout<< "Beta = ";
    cin>> beta;
    cout<< "Errore di beta = ";
    cin>> s_beta;
    cout<<"Sto calcolando..."<< endl;
    cout<< endl;
    //huhu();
    //sleep(5);
    I_0= Momento(alpha, beta);
    s_I_0= Sigma(alpha, s_alpha, beta,s_beta);
    cout<<"Il momento è :  "<< I_0 << " ± " << s_I_0<< endl;
    
    
    
    return 0;
}

double Momento(double al, double be){
    return ((r*m*g-m*al*r*r)/(al-be));
}

long double Sigma(double alpha, double s_alpha, double beta, double s_beta){
    return (sqrt(       (((m*g-2*m*alpha*r)/(alpha-beta))   *   ((m*g-2*m*alpha*r)/(alpha-beta))    *s_r*s_r) +
                 (((r*g-r*alpha*r)/(alpha-beta))     *   ((r*g-r*alpha*r)/(alpha-beta))      *s_m*s_m) +
                 (((r*m-r*alpha*r*m)/(alpha-beta))   *   ((r*m-r*alpha*r*m)/(alpha-beta))    *s_g*s_g) +
                 (((r*r*m*beta-r*g*m)/((alpha-beta)*(alpha-beta)))   *   ((r*r*m*beta-r*g*m)/((alpha-beta)*(alpha-beta)))    *s_alpha*s_alpha) +
                 (((m*g*r-m*r*r*alpha)/((alpha-beta)*(alpha-beta)))  *   ((m*g*r-m*r*r*alpha)/((alpha-beta)*(alpha-beta)))   *s_beta*s_beta) ));
}
void huhu(){
    sleep(2);
    cout<<"segmentation fault"<< endl;
    cout<< endl;
    sleep(6);
    cout<<" o forse no?"<<endl;
    haha();
    return;
}
void haha(){
    sleep(2);
    cout<<"prova a dire : vai, programma!"<< endl;
    cout<< endl;
    return;
}
