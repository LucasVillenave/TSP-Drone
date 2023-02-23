# TSP-Drone

Ce projet est un projet de M2, groupe B pour le projet professionnel de La Poste.

Il a pour but d'optimiser l'intégration de drones de livraison dans la flotte de La Poste.

## Installation

- You need a GUROBI license
- You need python packages : numpy, pandas, folium, networkx
```shell
pip install numpy
```

## Affichage des solutions
```shell
python3 draw_solution.py <instance_file> <solution_file> (road_graph_display) (truck_tour_display) (drone0_tour_display) (drone1_tour_display)
```
instance_file = le chemin de l'instance concernée, exemple : "/Data/init/"
solution_file = le chemin du fichier solution à afficher, exemple : "/result/solution.txt"
road_graph_display = 1, pour afficher le graphe routier (par défaut)
                     0, sinon
truck_tour_display = 1, pour afficher le trajet du véhicule (par défaut)
                     0, sinon
drone0_tour_display = 1, pour afficher le trajet du drone 0 (par défaut)
                      0, sinon
drone1_tour_display = 1, pour afficher le trajet du drone 1 (par défaut)
                      0, sinon

Légende de la carte dynamique créée :
  BLEU = ROUTES
   cercle bleu          Sommet du graphe routier
   droite bleu          Arête du graphe routier
   cercle cyan          Sommet de dépôt

  ROUGE = DEMANDES
   cercle rouge         Demande sur le sommet (nombre de cercle = nombre de demande)

  VIOLET = VÉHICULE
   cercle violet        Sommet de passage du véhicule
   droite violet        Trajet du véhicule
   cercle plein violet  Livraison par le véhicule sur le sommet

  VERT = DRONE 0
   cercle vert          Sommet de passage du drone 0
   droite vert          Trajet du drone 0 dans le véhicule
   pointillé vert       Trajet du drone 0 en vol
   cercle plein vert    Livraison par le drone 0 sur le sommet

  GRIS = DRONE 1
   cercle gris          Sommet de passage du drone 1
   droite gris          Trajet du drone 1 dans le véhicule
   pointillé gris       Trajet du drone 1 en vol
   cercle plein gris    Livraison par le drone 1 sur le sommet

## Checker de solution
```shell
./toCheck.sh <instance_file> <solution_file>
```
