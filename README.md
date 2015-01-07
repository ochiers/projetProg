projetProg
==========

Projet de S5. PROG - ALM

==========

Pour récupérer la version en ligne :
  - git pull

Pour pousser la version locale en ligne :
  - git status // Pour voir les fichiers modifiés sur votre dossier local par rapport à la version en ligne
  - git add nomFichier1 nomFichier2  // que vous avez modifiés et pas les autres
  - git pull
  - git commit -m "Votre Commentaire"
  - git push

==========

Listes des commandes d'installation
  
  Dans le dossier cible :
  - git init
  - git clone url

  Concernant la configuration :
  - git config --list //Permet de voir la config. courante
  - git config --global user.name "VotreNomGitHub"
  - git config --global user.email @-mail

Lors du premier push, il se peut qu'il y ait une erreur demandant une option de configuration concernant le "push.default"
Et il propose une ligne de commande. Il faut l'executer avec l'option : push.default=simple
