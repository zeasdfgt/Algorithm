/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author Étudiant(e)
 * \version 0.1
 * \date mars 2020
 *
 *  Travail pratique numéro 2
 *
 */

#include "ReseauAerien.h"
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2 {

// Méthode fournie
/**
 * \fn void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
 * \param[in] le fichier contenant l'information sur le réseau
 */
    void ReseauAerien::chargerReseau(std::ifstream &fichierEntree) {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauAerien::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;
        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 15);

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while (buff != "Liste des trajets:") {
            std::string ville = buff;

            unReseau.nommer(i, ville);

            getline(fichierEntree, buff);
            i++;
        }

        while (!fichierEntree.eof()) {
            getline(fichierEntree, buff);
            std::string source = buff;

            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            int ns;
            iss >> ns;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout,
                                ns);
        }
    }

    ReseauAerien::ReseauAerien(std::string nomReseau, size_t nbVilles) : nomReseau(nomReseau), unReseau(nbVilles) {

    }

    ReseauAerien::~ReseauAerien() {
        //Je pense qu'il n'y a rien ici aussi

    }

    void ReseauAerien::resize(size_t nouvelleTaille) {
        this->unReseau.resize(nouvelleTaille);
    }

    Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &origine, const std::string &destination,
                                                 bool dureeCout) const {
        Chemin resultat;
        resultat.reussi = false;

        if (unReseau.getNumeroSommet(origine) > unReseau.getNombreSommets()
            || unReseau.getNumeroSommet(destination) > unReseau.getNombreSommets()) {
            throw std::logic_error("Origine et ou la destination est absente dans le reseau");
        }
        else {
            std::list<std::string> SommetAResoudre;
            std::vector<std::string> SommetsResolus;
            std::string VillePredecesseur[unReseau.getNombreSommets()];
            float Ponderation[unReseau.getNombreSommets()];

            for (size_t i = 0; i < unReseau.getNombreSommets(); i++) {
                if (unReseau.getNomSommet(i) == origine) {
                    Ponderation[i] = 0;
                    VillePredecesseur[i] = "";
                    SommetAResoudre.push_front(origine);
                }
                else {
                    Ponderation[i] = std::numeric_limits<float>::infinity();
                }
            }
            while (!SommetAResoudre.empty()) {
                std::string sommet = getMinListTraitement(SommetAResoudre, Ponderation);
                SommetAResoudre.remove(sommet);
                size_t numeroSommet = unReseau.getNumeroSommet(sommet);
                std::vector<size_t> adjacents = unReseau.listerSommetsAdjacents(numeroSommet);
                if(dureeCout) {
                    for (size_t i = 0; i < adjacents.size(); i++) {
                        if ((Ponderation[adjacents[i]] > (Ponderation[numeroSommet] +
                                                         unReseau.getPonderationsArc(numeroSommet, adjacents[i]).duree)
                                                         && !(std::find(SommetsResolus.begin(), SommetsResolus.end(), sommet) != SommetsResolus.end()))) {
                            Ponderation[adjacents[i]] = (Ponderation[numeroSommet] +
                                                         unReseau.getPonderationsArc(numeroSommet, adjacents[i]).duree);
                            VillePredecesseur[adjacents[i]] = sommet;
                            SommetAResoudre.push_front(unReseau.getNomSommet(adjacents[i]));

                        }

                    }
                    SommetsResolus.push_back(sommet);
                }
                else {
                    for (size_t i = 0; i < adjacents.size(); i++) {
                        if ((Ponderation[adjacents[i]] > (Ponderation[numeroSommet] +
                                                         unReseau.getPonderationsArc(numeroSommet,adjacents[i]).cout)
                                                         && !(std::find(SommetsResolus.begin(), SommetsResolus.end(), sommet) != SommetsResolus.end()))) {
                            Ponderation[adjacents[i]] = (Ponderation[numeroSommet] +
                                                         unReseau.getPonderationsArc(numeroSommet, adjacents[i]).cout);
                            VillePredecesseur[adjacents[i]] = sommet;
                            SommetAResoudre.push_front(unReseau.getNomSommet(adjacents[i]));

                        }

                    }
                    SommetsResolus.push_back(sommet);
                }
            }
            if(dureeCout) {
                resultat.dureeTotale = Ponderation[unReseau.getNumeroSommet(destination)];
                resultat.reussi = true;
            }
            else {
                resultat.coutTotal = Ponderation[unReseau.getNumeroSommet(destination)];
                resultat.reussi = true;
            }
            std::string ville = destination;
            resultat.listeVilles.push_back(destination);
            while (ville != origine) {
                resultat.listeVilles.push_back(VillePredecesseur[unReseau.getNumeroSommet(ville)]);
                ville = VillePredecesseur[unReseau.getNumeroSommet(ville)];
            }
            std::reverse(resultat.listeVilles.begin(), resultat.listeVilles.end());
        }
        return resultat;
    }


    Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination,
                                                    int dureeCoutNiveau) const {
        Chemin resultat;
        //if statement potentiellement pas utilise
        if (this->unReseau.getNumeroSommet(origine) > this->unReseau.getNombreSommets()
            || this->unReseau.getNumeroSommet(destination) > this->unReseau.getNombreSommets()) {
            throw std::logic_error("Origine et ou destination absente dans le reseau");  //potentiellement inutilisé
        } else {
            if (dureeCoutNiveau != 1 && dureeCoutNiveau != 2 && dureeCoutNiveau != 3) {
                throw std::logic_error("La ponderation doit etre entre 1 et 3 inclusivement");
            } else {
                std::map<std::string, std::string> VilleDePredecesseur;
                std::map<std::string, float> CoutOuDuree;
                std::map<std::string, int> CoutAvecSecurite;
                std::map<std::pair<std::string, std::string>, Ponderations> Arcs;
                float CoutDureeTemporaire;
                int CoutDureeTemporaireSecurite = 0;

                for (size_t i = 0; i < this->unReseau.getNombreSommets(); i++) {
                    //Si le sommet est le sommet d'origine on initialise les couts de la ville d'origine a 0 et les predecesseur a vide
                    //Selon algo de Bellman Ford, si c'est le sommet d'origine, on le met a 0
                    if (this->unReseau.getNomSommet(i) == origine) {
                        CoutOuDuree[origine] = 0.0;
                        CoutAvecSecurite[origine] = 0;
                        VilleDePredecesseur[origine] = "";
                        //Si un arc existe entre l'origine et un autre sommet, on l'ajoute a la paire d'Arcs
                        for (auto const &it : this->unReseau.listerSommetsAdjacents(i)) {
                            if (this->unReseau.arcExiste(i, it)) { //if statement pontentiellement pas utilise car si arc est dans la liste, il existe
                                std::pair<std::string, std::string> unePaire = std::make_pair(origine,this->unReseau.getNomSommet(it));
                                Arcs.insert(std::make_pair(unePaire, this->unReseau.getPonderationsArc(i, it)));
                            }
                        }
                    } else {
                        //On initialise tous les sommets sauf celui de depart a infini
                        //Selon algo de Bellman Ford, tous les sommets sauf celui de depart doivent etre a infini
                        CoutOuDuree[this->unReseau.getNomSommet(i)] = std::numeric_limits<float>::infinity();
                        CoutAvecSecurite[this->unReseau.getNomSommet(i)] = std::numeric_limits<int>::max();
                        VilleDePredecesseur[origine] = "";  //devrait etre VilleDePredecessur[unReseau.getNomSommet(i)] = ""; je pense
                        for (auto const &it: this->unReseau.listerSommetsAdjacents(i)) {
                            if (this->unReseau.arcExiste(i, it)) {  //if statement pontentiellement pas utilise car si arc est dans la liste, il existe
                                std::pair<std::string, std::string> unePaire = std::make_pair(this->unReseau.getNomSommet(i), this->unReseau.getNomSommet(it));
                                Arcs.insert(std::make_pair(unePaire, this->unReseau.getPonderationsArc(i, it)));
                            }
                        }
                    }
                }
                //On calcule la duree de vol comme ponderation
                if (dureeCoutNiveau == 1) {
                    //Loop des sommets
                    for (size_t i = 0; i < this->unReseau.getNombreSommets() - 1; i++) {
                        //Loop des arcs
                        for (auto const & it : Arcs) {
                            //Si la ponderation entre 2 sommets est plus petite que celle deja dans CoutOuDuree on change la ponderation
                            CoutDureeTemporaire = it.second.duree + CoutOuDuree[it.first.first];
                            if (CoutDureeTemporaire < CoutOuDuree[it.first.second]) {
                                CoutOuDuree[it.first.second] = CoutDureeTemporaire;
                                VilleDePredecesseur[it.first.second] = it.first.first;
                            }
                        }
                    }
                    //Selon algo de Bellman Ford, il ne devrait avoir aucun cycle plus petit que 0
                    //Alors il n'y a aucun chemin possible
                    for (auto const &it : Arcs) {
                        if (CoutOuDuree[it.first.second] > CoutOuDuree[it.first.first] + it.second.duree) {
                            resultat.reussi = false;
                            return resultat;
                        }
                    }
                    //Selon algo de Bellman Ford, il ne devrait pas avoir de chemin avec CoutOuDuree entre origine et destination = infini
                    //Alors il n'y a aucun chemin possible
                    if (CoutOuDuree[destination] == std::numeric_limits<float>::infinity()) {
                        resultat.reussi = false;
                        //Si CoutOuDuree n'est pas infini et est plus grand ou egale a 0
                        //On ajoute le sommet a la listeVilles
                    } else {
                        resultat.reussi = true;
                        resultat.dureeTotale = CoutOuDuree[destination];
                        std::string villes = destination;
                        resultat.listeVilles.push_back(destination);
                        while (villes != origine) {
                            resultat.listeVilles.push_back(VilleDePredecesseur[villes]);
                            villes = VilleDePredecesseur[villes];
                        }
                        //On reverse la liste pour les lister de la ville de depart vers la ville de destination
                        std::reverse(resultat.listeVilles.begin(), resultat.listeVilles.end());
                    }
                    //On utilise le cout du vol comme ponderation
                    //Meme chose que dureeCoutNiveau = 1 mais en utilisant la duree de vol au lieu du cout du vol
                } else if (dureeCoutNiveau == 2) {
                    for (size_t i = 0; i < this->unReseau.getNombreSommets() - 1; i++) {
                        for (auto const & it : Arcs) {
                            CoutDureeTemporaire = it.second.cout + CoutOuDuree[it.first.first];
                            if (CoutDureeTemporaire < CoutOuDuree[it.first.second]) {
                                CoutOuDuree[it.first.second] = CoutDureeTemporaire;
                                VilleDePredecesseur[it.first.second] = it.first.first;
                            }
                        }
                    }
                    for (auto const &it : Arcs) {
                        if (CoutOuDuree[it.first.second] > CoutOuDuree[it.first.first] + it.second.cout) {
                            resultat.reussi = false;
                            return resultat;
                        }
                    }
                    if (CoutOuDuree[destination] == std::numeric_limits<float>::infinity()) {
                        resultat.reussi = false;
                    } else {
                        resultat.reussi = true;
                        resultat.coutTotal = CoutOuDuree[destination];
                        std::string villes = destination;
                        resultat.listeVilles.push_back(destination);
                        while (villes != origine) {
                            resultat.listeVilles.push_back(VilleDePredecesseur[villes]);
                            villes = VilleDePredecesseur[villes];
                        }
                        //On reverse la liste pour les lister de la ville de depart vers la ville de destination
                        std::reverse(resultat.listeVilles.begin(), resultat.listeVilles.end());
                    }
                    //Meme chose que dureeCoutNiveau = 1 et 2 mais en utilisant la securite du vol
                } else if (dureeCoutNiveau == 3) {
                    for (size_t i = 0; i < this->unReseau.getNombreSommets() - 1; i++) {
                        for (auto const &it : Arcs) {
                            CoutDureeTemporaireSecurite = it.second.ns + CoutAvecSecurite[it.first.first];
                            if (CoutAvecSecurite[it.first.first] != std::numeric_limits<int>::max() &&
                                CoutDureeTemporaireSecurite < CoutAvecSecurite[it.first.second]) {
                                CoutAvecSecurite[it.first.second] = CoutDureeTemporaireSecurite;
                                VilleDePredecesseur[it.first.second] = it.first.first;
                            }
                        }
                    }
                    for (auto const &it : Arcs) {
                        if (CoutAvecSecurite[it.first.first] != std::numeric_limits<int>::max() &&
                            CoutAvecSecurite[it.first.second] > CoutAvecSecurite[it.first.first] + it.second.ns) {
                            resultat.reussi = false;
                            return resultat;
                        }
                    }
                    if (CoutAvecSecurite[destination] == std::numeric_limits<int>::max()) {
                        resultat.reussi = false;
                    } else {
                        resultat.reussi = true;
                        resultat.nsTotal = CoutAvecSecurite[destination];
                        std::string villes = destination;
                        resultat.listeVilles.push_back(destination);
                        while (villes != origine) {
                            resultat.listeVilles.push_back(VilleDePredecesseur[villes]);
                            villes = VilleDePredecesseur[villes];
                        }
                        //On reverse la liste pour les lister de la ville de depart vers la ville de destination
                        std::reverse(resultat.listeVilles.begin(), resultat.listeVilles.end());
                    }
                }
            }
        }
        return resultat;
    }

    std::string
    ReseauAerien::getMinListTraitement(std::list<std::string> list, const float *Ponderation) const {
        std::string min;
        float valeurMin = std::numeric_limits<float>::infinity();
        if(list.empty()) {
            throw std::logic_error("On ne peut trouver le minimum d'une liste vide");
        }
        for( const auto it : list) {
            if(valeurMin > Ponderation[unReseau.getNumeroSommet(it)]) {
                valeurMin = Ponderation[unReseau.getNumeroSommet(it)];
                min = it;
            }
        }
        return min;
    }






    //À compléter par l'implémentation des autres méthodes demandées

}//Fin du namespace
