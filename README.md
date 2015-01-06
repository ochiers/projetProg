projetProg
==========

Projet de S5. PROG - ALM

==========
Listes des commandes d'installation

Dans le dossier cible :
git init \n
git clone <url>

Pour récupérer la version en ligne :
git pull

Pour pousser la version locale en ligne :
git add <nomFichier1> <nomFichier2>
git commit -m "Commentaire"
git push

S'il y a un conflit lors du push, faire un pull
! Attention à vos modifications, elles peuvent disparaître !

Concernant la configuration :
git config --list //Permet de voir la config. courante
git config --global user.name "VotreNomGitHub"
git config --global user.email <@-mail>

Lors du premier push, il se peut qu'il y ait une erreur demandant une option de configuration concernant le "push.default"
Et il propose une ligne de commande. Il faut l'executer avec l'option : push.default=simple
