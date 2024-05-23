#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <random>


bool generation = true;
bool analyse = true;

void creationTableauOccurences(std::map<std::string, std::map<std::string, long>> &tabOccurence, int &profondeur, std::string &mot)
{
    std::string newMot;
    for (int i = 0; i < profondeur; i++)
    {
        newMot += "_";
    }
    newMot += mot + "#";

    for (int i = 0; i < newMot.size()-profondeur; i++)
    {
        std::string sequenceTemp;
        std::string nextLettre(1, newMot[i + profondeur]);
        for (int j = 0; j < profondeur; j++)
        {
            sequenceTemp += newMot[i + j];
        }

        if (tabOccurence.find(sequenceTemp) == tabOccurence.end())
        {
            tabOccurence[sequenceTemp][nextLettre] = 1;
        }
        else
        {
            if (tabOccurence[sequenceTemp].find(nextLettre) == tabOccurence[sequenceTemp].end())
            {
                tabOccurence[sequenceTemp][nextLettre] = 1;
            }
            else
            {
                tabOccurence[sequenceTemp][nextLettre] += 1;
            }
        }

    }

    
}

std::map<std::string, std::map<std::string, int>> creationTableauProbabilite(std::map<std::string, std::map<std::string, long>>& tabOccurence)
{
    std::map<std::string, std::map<std::string, int>> tabProbabilite;

    for (auto elem : tabOccurence)
    {
        float count = 0;
        for (auto chr : elem.second)
        {
            count += chr.second;
        }
        for (auto chr : elem.second)
        {
            tabProbabilite[elem.first][chr.first] = (tabOccurence[elem.first][chr.first] / count)* 100;
        }

    }

    return tabProbabilite;
}

std::string build(std::map<std::string, std::map<std::string, int>> &tabProbabilite, int profondeur)
{
    std::string mot;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0, 100);

    for (int i = 0; i < profondeur; i++)
    {
        mot += "_";
    }

    while (mot[mot.size()-1] != '#')
    {
        std::string tempMot = "";
        int random = distrib(gen);
        bool add = false;

        for (int i = mot.size()-profondeur; i < mot.size(); i++)
        {
            tempMot += mot[i];
            
        }

        for (auto elem : tabProbabilite)
        {
            if (tempMot == elem.first)
            {
                for (auto elem2 : elem.second)
                {
                    random -= elem2.second;
                    if (random <= 0 and not add)
                    {
                        mot += elem2.first;
                        add = true;
                    }
                }
                break;
            }
        }
    }
    
    std::string motReturn;
    for (int i = profondeur; i < mot.size()-1; i++)
    {
        motReturn += mot[i];
    }

    return motReturn;
}

std::vector<std::string> split(std::string line, char spliter)
{
    std::vector<std::string> vector;

    std::string tempMot;

    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == spliter)
        {
            if (tempMot != "")
            {
                vector.push_back(tempMot);
            }
            tempMot = "";
        }
        else
        {
            tempMot += line[i];
        }
    }
    vector.push_back(tempMot);

    return vector;
}

int main()
{

    int profondeur = 3;

    if (analyse)
    {
        std::ifstream fichier("gutenberg.txt");

        std::map<std::string, std::map<std::string, long>> tabOccurence;

        std::string ligne;
        int delay = 0;
        while (std::getline(fichier, ligne)) 
        {
            creationTableauOccurences(tabOccurence, profondeur, ligne);
            if (delay == 10)
            {

                delay = 0;
            }
            else
            {
                delay++;
            }
        }

        std::map<std::string, std::map<std::string, int>> tabProba = creationTableauProbabilite(tabOccurence);

        std::ofstream test("nom_du_fichier.txt");

        for (auto elem : tabProba)
        {
            for (auto elem2 : elem.second)
            {
                test << elem.first << "  " << elem2.first << "  " << elem2.second << std::endl;
            }
        }

        test.close();
        fichier.close();
    }

    if (generation)
    {
        std::ifstream test("nom_du_fichier.txt");

        std::map<std::string, std::map<std::string, int>> tabProba;

        std::string ligne;
        while (std::getline(test, ligne)) 
        {
            std::vector<std::string> vector = split(ligne, ' ');
            tabProba[vector[0]][vector[1]] = std::stoi(vector[2]);
        }

        std::ofstream result("result.txt");
        for (int i = 0; i < 100; i++)
        {
            result << build(tabProba, profondeur) << std::endl;
            std::cout << "etst";
        }

        result.close();
        test.close();
    }
    

    return 0;
}