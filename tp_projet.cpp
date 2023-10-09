#include <iostream>
#include <vector>
#include <stack>
#include <limits>

using namespace std;
class arete{
    public:
    int start;
    int end;
    float cost;
    arete(int s,int e, float c){
        start = s;
        end = e;
        cost = c;
    }
    void affiche_arrete(){cout<<"Arete : V"<<start<<" --------- +"<<cost<<" ---------> "<<end<<endl;}
}; 

 class sommet{
    public:
    int id; 
    vector <arete> connexions;
    sommet(int iD){
        id = iD;
    }

    void affiche_sommet()
    {
        if(connexions.size()>0)
        {
        cout<<"***************************************"<<endl;
        cout<<" SOMMET ID : "<<id<<endl;
        for(int i=0;i<connexions.size();i++)
        {
            connexions[i].affiche_arrete();
        }
        cout<<"***************************************"<<endl;
        }
    }

};

class Graphe {
/*
MATRICE DE CHANGE : 
                 |   EURO |  USD  |  JP  |  CHF
            -----------------------------------        
            EURO |     1  |  1.19 | 1.33 | 1.62
            USD  |   0.84 |   1   | 1.12 | 1.37
            JP   |   0.75 |  0.89 |  1   | 1.22
            CHF  |   0.62 |  0.73 | 0.82 |  1 

*/


public:
    int nombre_sommet;
    vector<sommet> mes_sommets;
    Graphe(){}
};
          