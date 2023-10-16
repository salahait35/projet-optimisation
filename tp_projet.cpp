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
    int nb_saut;
    vector<sommet> mes_sommets;
    vector <vector <int> > cycle_dispo;
    vector <double> gain_de_mes_cycle;
    Graphe(){
        nombre_sommet = 4;

        for(int i=1;i<=nombre_sommet;i++)
        {
            mes_sommets.push_back(sommet(i));
        }

        cout<<"Veuillez entrez un nombre d'échange:"<<endl;
        cin>>nb_saut;
        cout<<endl;

    }

    void affiche_mon_cycle(int indice)
    {
        if (indice >= 0 && indice < cycle_dispo.size()) {
        const std::vector<int>& cycle = cycle_dispo[indice];
        cout << "Cycle " << indice << ": [ ";
        for (int element : cycle) {
            cout << devise(element) << " ";
        }
        cout << "]"<<endl;
        }
    }

    void affiche_mes_cycles()
    {
        for(int j = 0 ; j< cycle_dispo.size();j++)
        {
            affiche_mon_cycle(j);
        }
    }

    void calcule_mes_cycles()
    {
        affiche_mes_cycles();
        for (const std::vector<int>& cycle : cycle_dispo) {
            double gain = 1;
            for(int i = 0; i< cycle.size()-1;i++)
            {
                gain = gain * matriceDeChange[cycle[i]-1][cycle[i+1]-1];
            }
            gain_de_mes_cycle.push_back(gain);    
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

    void trouver_les_cycles(int p) //p nombre de cout 
    {
        int lenght = p - 1;
        for(int i = 0; i< mes_sommets[EURO-1].connexions.size();i++)
        {
            vector <int> init; 
            init.push_back(EURO);
            trouve_le_cycle_de_mon_voisin_en_recusrsive(init,mes_sommets[EURO-1].connexions[i].end,lenght);
        }
    }

    void trouve_le_cycle_de_mon_voisin_en_recusrsive(vector <int> cycle, int sommet_depart, int p)
    {

        if(sommet_depart == EURO || p==0 )
        {
            cycle.push_back(sommet_depart);
            if(sommet_depart == EURO)cycle_dispo.push_back(cycle);
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
                p = p-1;
                for(int i = 0 ; i < mes_sommets[sommet_depart-1].connexions.size();i++)
                {
                
                    vector <int> temp = cycle;
                    trouve_le_cycle_de_mon_voisin_en_recusrsive(temp,mes_sommets[sommet_depart-1].connexions[i].end,p);
                }

                break;
        }
        }
    }

    pair<double,double> mon_meilleur_cycle()
    {

        double max_valeur = gain_de_mes_cycle[0];
        int max_indice = 0;
        for (int i = 1; i < gain_de_mes_cycle.size(); i++) {
        if (gain_de_mes_cycle[i] > max_valeur) {
            max_valeur = gain_de_mes_cycle[i];
            max_indice = i;
        }
    }
    cout<<endl<<"Mon meilleur cycle est ";
    affiche_mon_cycle(max_indice);
    cout<<"Avec une valeur de "<<max_valeur<<" et mon total de cycle est de "<<cycle_dispo.size()<<endl;
    return std::make_pair(max_valeur, max_indice);
    }

    void mon_gain_calculer(double max_valeur)
    {
        double mise_de_depart = 0;
        cout<<endl<<"Veuillez rentrer votre mise de depart"<<endl;
        cin>>mise_de_depart;
        cout<<endl<<"somme a la fin des echanges : "<<(mise_de_depart*max_valeur)<<" avec un gain de : "<<(mise_de_depart*max_valeur) - mise_de_depart;
        cout<<endl<<"ce gain reprensente : "<<(1 -(1 - (((mise_de_depart*max_valeur) - mise_de_depart)/mise_de_depart)))*100<<"%"<<endl;


    }

    


};
          


int main()
{
    Graphe g;
    g.trouve_les_aretes_possible();
    g.trouve_le_meilleur_retour();
    g.trouver_les_cycles(g.nb_saut);
    g.calcule_mes_cycles();
    g.mon_gain_calculer(g.mon_meilleur_cycle().first);
 
    return 0;
}