# Variables de compilation
CC = gcc  # Compilateur à utiliser
CFLAGS = `pkg-config --cflags gtk+-3.0 gstreamer-1.0` -lsqlite3 -I. -I./includes  # Inclure le répertoire courant et includes
LDFLAGS = `pkg-config --libs gtk+-3.0 gstreamer-1.0` -lsqlite3 -lcurl  # Options de liaison
TARGET = bin/gestion_inventaire_scanner  # Chemin de l'exécutable final

# Chemin vers le dossier src
SRC_DIR = src
# Répertoire pour les fichiers objets
OBJ_DIR = obj

# Répertoire de la base de données
DB_DIR = db
DB_FILE = inventaire.db

# Liste des fichiers objets avec le chemin complet
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/gui.o $(OBJ_DIR)/database.o $(OBJ_DIR)/scanner.o $(OBJ_DIR)/gestionPdf.o

# Règle principale pour construire l'exécutable
all: $(DB_DIR) $(OBJ_DIR) $(TARGET)  # Créer les répertoires si nécessaires

# Règle pour créer le répertoire de la base de données
$(DB_DIR):
	mkdir -p $(DB_DIR)

# Règle pour créer le répertoire pour les objets s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règle de création de l'exécutable
$(TARGET): $(OBJECTS)  # Dépendances de l'exécutable
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)  # Commande pour lier les objets et créer l'exécutable

# Règles de compilation des fichiers objets
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c includes/gui.h includes/database.h includes/scanner.h includes/gestionPdf.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o  # Commande pour compiler main.c

$(OBJ_DIR)/gui.o: $(SRC_DIR)/gui.c includes/gui.h includes/database.h includes/scanner.h includes/gestionPdf.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/gui.c -o $(OBJ_DIR)/gui.o  # Commande pour compiler gui.c

$(OBJ_DIR)/database.o: $(SRC_DIR)/database.c includes/database.h includes/gestionPdf.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/database.c -o $(OBJ_DIR)/database.o  # Commande pour compiler database.c

$(OBJ_DIR)/scanner.o: $(SRC_DIR)/scanner.c includes/scanner.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/scanner.c -o $(OBJ_DIR)/scanner.o  # Commande pour compiler scanner.c

$(OBJ_DIR)/gestionPdf.o: $(SRC_DIR)/gestionPdf.c includes/gestionPdf.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/gestionPdf.c -o $(OBJ_DIR)/gestionPdf.o  # Commande pour compiler gestionPdf.c

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)  # Supprimer les fichiers objets et l'exécutable

# Règle pour nettoyage complet, y compris la base de données
clean-all: clean
	rm -f db/$(DB_FILE)  # Supprimer la base de données
