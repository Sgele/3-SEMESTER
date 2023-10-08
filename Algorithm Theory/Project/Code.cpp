#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <ctype.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <istream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdio>
using namespace std;

long double atsitiktinis_skaicius(vector<long double>& A, long double& sk)
{
    random_device atsitiktinis_sk; // gaunamas atsitikinis skaicius
    mt19937 gen(atsitiktinis_sk()); // skaiciu generatorius
    uniform_real_distribution<> distr(-sk * 10, sk * 10); //intervalas is kurio bus generuojami atsitiktiniai skaiciai

    long double atsitiktinis = distr(gen);
    //if (find(A.begin(), A.end(), atsitiktinis) != A.end()) { //tikrinama ar nera tokiu paciu sugeneruotu elementu
    //    long long pagalba = distr(gen);
    //    return atsitiktinis_skaicius(A, pagalba);
    //}
    //else
        return atsitiktinis;
}

void skaitymas(vector<long double>& A, long double& k, long double& skaitymo_laikas)
{
    int x; //nuspresime ar naudosime jau turimus duomenis ar generuosime
    int size = 1;
    cout << "Iveskite '0', jei duomenys bus skaitomi is jau sukurto .txt failo; " << endl;
    cout << "Iveskite '1', jei norite, kad duomenys butu generuojami atsitiktinai: "; cin >> x; cout << endl;
    cout << "Iveskite sveikaji skaiciu, kuris norodys kelinta pagal dydi norite rasti aibes elementa: "; cin >> k; cout << endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    if (x != 0 && x != 1)
    {
        cout << "Netinkamas ivestas skaicius" << endl;
        exit(EXIT_FAILURE);
    }
    else if (x == 0)
    {
        // atidarome faila   
        ifstream Failas;
        Failas.open("duomenys.txt");
        if (Failas) {
            long double elementas;

            // nuskaitome elementus is failo i vektoriu  
            while (Failas >> elementas)
            {
                A.push_back(elementas);
                A.resize(size++);
            }
            Failas.close();
        }
        else {
            cout << "Failo nepavyko rasti, patikrinkite pavadinima!" << endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (x == 1)
    {
        long double sk;
        cout << "Iveskite sveikaji skaiciu, kuris nurodo kokio dydzio atsitiktinai sugeneruoto duomenu failo norite: "; cin >> sk;

        string pavadinimas = "duomenys" + to_string(sk) + ".txt";
        ofstream out_data(pavadinimas);

        for (long double i = 0; i < sk; i++)
        {
            long double skaicius = atsitiktinis_skaicius(A, sk);
            A.push_back(skaicius);
            A.resize(size++);
            out_data << skaicius << endl;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    skaitymo_laikas = time / std::chrono::milliseconds(1) / 1000.00;
    return;
}

long double mediana(vector<long double> mediana)
{
    size_t size = mediana.size();

    sort(mediana.begin(), mediana.end());
    if (mediana.size() % 2 == 0)
    {
        return (mediana[size / 2 - 1] + mediana[size / 2]) / 2;
    }
    else
    {
        return mediana[size / 2];
    }
}

long double Elemento_paieska(long double k, vector<long double>& A)
{
    if (A.size() < 50)
    {
        sort(A.begin(), A.end());
        return A.at(k - 1);
    }
    else
    {
        int n = 5; //skaicius, nurodantis is kiek elementu bus daryti aibes poaibiai
        vector<vector<long double>> poaibis;
        long double poaibiu_skaicius = ceil(A.size() / 5.0); //suzinome kiek poaibiu bus
        poaibis.resize(poaibiu_skaicius);
        vector<long double> M; //poabiu medianos

        for (long double i = 0; i < poaibiu_skaicius; i++)
        {
            poaibis[i].resize(n);
            int start_itr = i * n;
            int end_itr = i * n + n;

            if (end_itr > A.size())
            {
                end_itr = A.size();
                poaibis[i].resize(A.size() - start_itr);
            }

            copy(A.begin() + start_itr, A.begin() + end_itr, poaibis[i].begin());
        }

        for (long double i = 0; i < poaibiu_skaicius; i++)
        {
            sort(poaibis[i].begin(), poaibis[i].end());
            long double med = mediana(poaibis[i]);
            M.push_back(med);
        }

        long double m = Elemento_paieska(ceil(M.size() / 2.0), M);

        vector<long double> A1; //vektorius, kuriame bus irasomi elementai, kurie yra mazesni uz m (t.y. bendra visu poaibiu mediana)
        vector<long double> A2; //vektorius, kuriame bus irasomas elementas, lygus m (t.y. bendra visu poaibiu mediana)
        vector<long double> A3; //vektorius, kuriame bus irasomi elementai, kurie yra didesni uz m (t.y. bendra visu poaibiu mediana)

        for (long double i = 0; i < A.size(); i++)
        {
            if (A.at(i) < m)
                A1.push_back(A.at(i));
            else if (A.at(i) == m)
                A2.push_back(A.at(i));
            else
                A3.push_back(A.at(i));
        }

        if (A1.size() >= k)
            return Elemento_paieska(k, A1);
        else if (A1.size() + A2.size() >= k)
            return m;
        else
            return Elemento_paieska(k - A1.size() - A2.size(), A3);
    }

}

int main()
{
    vector<long double> A; //Aibe
    long double k; //skaicius pagal kuri rasime k-aji pagal dydi aibes elementa
    long double skaitymo_laikas, skaiciavimo_laikas;

    skaitymas(A, k, skaitymo_laikas);

    auto start_time = std::chrono::high_resolution_clock::now();
    long double ats = Elemento_paieska(k, A);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    cout << fixed << setprecision(0) << k << "-asis pagal dydi elementas: " << endl;
    cout << setprecision(std::numeric_limits<long double>::digits10 + 1) << ats << endl;

    skaiciavimo_laikas = time / std::chrono::milliseconds(1) / 1000.00;
    long double bendras_laikas = skaitymo_laikas + skaiciavimo_laikas;
    string pavadinimas = "Rezultatas.txt";
    ofstream out_data(pavadinimas);
    out_data << fixed << setprecision(0) << k << "-asis pagal dydi elementas: " << endl;
    out_data << setprecision(std::numeric_limits<long double>::digits10 + 1) << ats << endl;
    out_data << "Programos vykdymo laikas : " << setprecision(1) << skaiciavimo_laikas << " sec" << endl;
    out_data.close();
    cout << "Programos vykdymo laikas : " << setprecision(1) << skaiciavimo_laikas << " sec" << endl;
    system("pause");
    return 0;
}