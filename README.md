# Mini Gestion d'Inventaire avec Scanner de Codes-Barres

## Description

Ce projet est une application de gestion d'inventaire qui utilise un scanner de codes-barres pour ajouter rapidement et efficacement des articles dans une base de données. L'application s'appuie sur une interface graphique développée avec GTK, et les données sont gérées à l'aide de SQLite, permettant ainsi une solution simple et efficace pour le suivi des stocks.

## Fonctionnalités

- **Interface graphique intuitive** : L'application propose une interface utilisateur simple et conviviale, facilitant l'interaction.
- **Utilisation d'un scanner de codes-barres** : Après avoir scanné un article, son nom est automatiquement affiché dans un champ de saisie, réduisant le risque d'erreurs de saisie.
- **Ajout dans la base de données** : Une fois l'article validé, il est enregistré dans la base de données, et un message de confirmation de succès s'affiche, assurant une expérience utilisateur fluide.

## Prérequis

Avant de commencer, assurez-vous d'avoir installé les éléments suivants :

- **Compilateur C** : GCC ou équivalent.
- **GTK** : Pour l'interface graphique.
- **SQLite** : Pour la gestion des données.
- **Scanner de codes-barres** : Pour lire les codes-barres.

### Installation des dépendances

Pour installer les bibliothèques nécessaires sous Ubuntu, exécutez la commande suivante :

```bash
sudo apt-get install libgtk-3-dev sqlite3 libsqlite3-dev




