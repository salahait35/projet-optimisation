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


class arete {
    public:
    int start;
    int end;
    double cost;
    arete(int s,int e, float c){
        start = s;
        end = e;
        cost = c;
    }
    void affiche_arrete(){cout<<"Arete : "<<devise(start)<<" --------- "<<cost<<" ---------> "<<devise(end)<<endl;}
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
        cout<<" SOMMET ID : "<<devise(id)<<endl;
        for(int i=0;i<connexions.size();i++)
        {
            connexions[i].affiche_arrete();
        }
        cout<<"***************************************"<<endl;
        }
    }
    
    bool trouve_cette_arete(int change)
    {
        for(int o = 0; o<connexions.size();o++)
        {
            if(connexions[o].end == change)
            {
                return true;
            }
        }
        return false;
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
    vector <vector <int> > cycle_dispo;
    Graphe(){
        nombre_sommet = 4;

        for(int i=1;i<=nombre_sommet;i++)
        {
             mes_sommets.push_back(sommet(i));
        //     for(int j = 0;j<nombre_sommet;j++)
        //     {
        //         if(j != i-1)
        //         {
        //             mes_sommets[i-1].connexions.push_back(arete(i,j+1,matriceDeChange[i-1][j]));
        //         }
        //     }
        }
    }

    void affich_mes_cycle()
    {
        for (const std::vector<int>& cycle : cycle_dispo) {
            cout << "[ ";
            for (int element : cycle) {
            cout << devise(element) << " ";
            }
        cout << "]" << std::endl;
        }
    }

    void affiche_mes_sommets()
    {
        for(int j = 0; j< mes_sommets.size();j++)
        {
            mes_sommets[j].affiche_sommet();
        }
    }

    int trouve_le_meilleur_retour()
    {
        double max = 0;
        int change = 0;
        for(int i=1; i< nombre_sommet ;i++)
        {
            double temp = matriceDeChange[EURO-1][i] * matriceDeChange[i][EURO-1]; 

            if(temp>max)
            {
                max = temp;
                change = i+1;
            }
        }
        //cout<<"max "<<max<<" pour le sommet : "<<devise(change);
        return change;
    }

    void trouve_les_aretes_possible()
    {
        for(int i = 0;i<nombre_sommet;i++)
        {
            double temp;
            for(int j = i;j<nombre_sommet;j++)
            {
                if(i != j)
                {
                   temp = matriceDeChange[i][j] * matriceDeChange[j][i];
                   if(temp > 1)
                   {
                        mes_sommets[i].connexions.push_back(arete(i+1,j+1,matriceDeChange[i][j]));
                        mes_sommets[j].connexions.push_back(arete(j+1,i+1,matriceDeChange[j][i]));
                   }
                   else{
                    if(i == 0)
                    {
                       mes_sommets[i].connexions.push_back(arete(i+1,j+1,matriceDeChange[i][j])); 
                    }
                   } 
                }
            }
        }
    }

    void trouver_les_cycle(int p) //p nombre de cout 
    {
        int lenght = p - 1;
        for(int i = 0; i< mes_sommets[EURO-1].connexions.size();i++)
        {
            vector <int> init; 
            init.push_back(EURO);
            if(i == 2) // a enelevr uniquepment pour teste
            {trouve_le_cycle_de_mon_voisin_en_recusrsive(init,mes_sommets[EURO-1].connexions[i].end,lenght);}
        }
        cout<<"fin"<<endl;
    }

    void trouve_le_cycle_de_mon_voisin_en_recusrsive(vector <int> cycle, int sommet_depart, int p)
    {

        if(sommet_depart == EURO || p==0 )
        {
            cycle.push_back(sommet_depart);
            cycle_dispo.push_back(cycle);
        }
        else{
        switch (p)
        {
            case 1: //allez a EURO
                p = p-1;
                cycle.push_back(sommet_depart);
                trouve_le_cycle_de_mon_voisin_en_recusrsive(cycle,EURO,p);
                break;

            case 2: //allez a CHF
                if(mes_sommets[sommet_depart-1].trouve_cette_arete(trouve_le_meilleur_retour()))
                {   
                    cycle.push_back(sommet_depart);
                    cycle.push_back(trouve_le_meilleur_retour());
                    p = p-1;
                    trouve_le_cycle_de_mon_voisin_en_recusrsive(cycle,EURO,p);
                }   
                else if(sommet_depart == trouve_le_meilleur_retour()){
                    cycle.push_back(sommet_depart);
                    trouve_le_cycle_de_mon_voisin_en_recusrsive(cycle,EURO,p);
                }
                break;

            default:
                cycle.push_back(sommet_depart);
                for(int i = 0 ; i < mes_sommets[sommet_depart-1].connexions.size();i++)
                {
                    //if(mes_sommets[sommet_depart-1].connexions[i].end != EURO)
                    {
                        vector <int> temp = cycle;
                        p = p-1;
                        trouve_le_cycle_de_mon_voisin_en_recusrsive(temp,mes_sommets[sommet_depart-1].connexions[i].end,p);
                    }
                }

                break;
        }
        }
    }


};
          


int main()
{
    Graphe g;
    g.trouve_les_aretes_possible();
    g.affiche_mes_sommets();
    g.trouve_le_meilleur_retour();
    g.trouver_les_cycle(4);
    g.affich_mes_cycle();
    return 0;
}