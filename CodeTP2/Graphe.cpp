/**
* \file Graphe.cpp
* \brief Implémentation d'un graphe orienté.
* \author Étudiant(e)
* \version 0.1
* \date mars 2020
*
*  Travail pratique numéro 2
*
*/

#include "Graphe.h"

//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{

	//Mettez l'implémentation de vos méthodes ici.

    Graphe::Graphe(size_t nbSommets) : nbSommets(nbSommets), nbArcs(0), sommets(), listesAdj() {
    }

    Graphe::~Graphe() {
        //pas sur s'il y a quelque chose a detruire
    }

    void Graphe::resize(size_t nouvelleTaille) {
        nbSommets = nouvelleTaille;
        sommets.resize(nouvelleTaille);
        listesAdj.resize(nouvelleTaille);
    }

    void Graphe::nommer(size_t sommet, const std::string &nom) {
        if(sommet > nbSommets || sommet < 0) {
            throw std::logic_error("Ce sommet n'existe pas");
        }
        else {
            sommets[sommet] = nom;
        }
    }

    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout, int ns) {
        if(arcExiste(source, destination)) {
            throw std::logic_error("Cet arc existe déja");
        }
        else {
            listesAdj[source].push_back(Arc(destination, Ponderations(duree, cout, ns)));
            nbArcs++;
        }
    }

    void Graphe::enleverArc(size_t source, size_t destination) {
        if(!arcExiste(source, destination)) {
            throw std::logic_error("Cet arc n'existe pas");
        }
        else {

            for(auto it = listesAdj[source].begin(); it != listesAdj[source].end(); it++) {
                if (it->destination == destination) {
                    listesAdj[source].erase(it--);
                    --nbArcs;
                }
            }
        }
    }

    bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source > nbSommets || source < 0) {
            throw std::logic_error("Le sommet source n'existe pas");
        }
        else if (destination > nbSommets || destination < 0) {
            throw std::logic_error("Le sommet destination n'existe pas");
        }
        else {
            for(const auto & it : listesAdj[source]) {
                if(it.destination == destination) {
                    return true;
                }
            }
            return false;
        }
    }

    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if(sommet > nbSommets || sommet < 0) {
            throw std::logic_error("Ce sommet n'existe pas");
        }
        else {
            std::vector<size_t> sommetsAdjacents;
            for(const auto & it : listesAdj[sommet]) {
                sommetsAdjacents.push_back(it.destination);
            }
            return sommetsAdjacents;
        }
    }

    std::string Graphe::getNomSommet(size_t sommet) const {
        if(sommet > nbSommets || sommet < 0) {
            throw std::logic_error("Ce sommet n'existe pas");
        }
        else {
            return sommets[sommet];
        }
    }

    size_t Graphe::getNumeroSommet(const std::string &nom) const {
        for(size_t i = 0; i < sommets.size(); i++) {
            if (sommets[i] == nom) {
                return i;
            }
        }
        throw std::logic_error("Aucun sommet ne porte ce nom");
    }

    int Graphe::getNombreSommets() const {
        return nbSommets;
    }

    int Graphe::getNombreArcs() const {
        return nbArcs;
    }

    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        for(const auto & it : listesAdj[source]) {
            if(it.destination == destination) {
                return it.poids;
            }
        }
        throw std::logic_error("Cet arc n'existe pas");
    }
}//Fin du namespace
