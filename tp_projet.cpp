#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <string>
using namespace std;


#define DEPART EURO;

enum change{
    EURO = 1,
    USD  = 2,
    JP   = 3,
    CHF  = 4,
};



class arete{
    public:
    int start;
    int end;
    double cost;
    arete(int s,int e, float c){
        start = s;
        end = e;
        cost = c;
    }
    void affiche_arrete(){cout<<"Arete : "<<change(start)<<" --------- "<<cost<<" ---------> "<<end<<endl;}
    float renvoi_le_cout_pour_un_sommet(int sommet){return 0;}
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
                     EURO |  USD  |  JP  |  CHF
            -----------------------------------        
            EURO |     1  |  1.19 | 1.33 | 1.62
            USD  |   0.84 |   1   | 1.12 | 1.37
            JP   |   0.75 |  0.89 |  1   | 1.22
            CHF  |   0.62 |  0.73 | 0.82 |  1 
*/
    vector<vector<double> > matriceDeChange = {
        {1.0, 1.19, 1.33, 1.62},
        {0.84, 1.0, 1.12, 1.37},
        {0.75, 0.89, 1.0, 1.22},
        {0.62, 0.73, 0.82, 1.0}
    };


public:
    int nombre_sommet;
    vector<sommet> mes_sommets;
    Graphe(){
        nombre_sommet = 4;
        for(int i=1;i<=nombre_sommet;i++)
        {
            mes_sommets.push_back(sommet(i));
            for(int j = 0;j<nombre_sommet;j++)
            {
                if(j != i-1)
                {
                    mes_sommets[i-1].connexions.push_back(arete(i,j+1,matriceDeChange[i-1][j]));
                }
            }
        }

        for(int j = 0; j< mes_sommets.size();j++)
        {
            mes_sommets[j].affiche_sommet();
        }

    }


string devise(int c) {
    switch (c) {
        case EURO:
            return "EURO";
        case USD:
            return "USD";
        case JP:
            return "JP";
        case CHF:
            return "CHF";
        default:
            return "Valeur d'énumération non trouvée";
    }
}

    void trouve_le_meilleur_allez_retour()
    {
        double max = 0;
        int change = 0;
        for(int i=0; i< mes_sommets[EURO-1].connexions.size() ;i++)
        {
            double temp = mes_sommets[EURO-1].connexions[i].cost * mes_sommets[mes_sommets[EURO-1].connexions[i].end-1].connexions[0].cost;

            if(temp>max)
            {
                max = temp;
                change = mes_sommets[EURO-1].connexions[i].end;
            }
        }
        cout<<"max "<<max<<" pour le sommet : "<<devise(change);
    }



};
          


int main()
{
    Graphe g;
    g.trouve_le_meilleur_allez_retour();
    return 0;
}