# Variables de compilation
CC = gcc  # Compilateur à utiliser
CFLAGS = `pkg-config --cflags gtk+-3.0` -lsqlite3  # Options de compilation pour GTK et SQLite
LDFLAGS = `pkg-config --libs gtk+-3.0` -lsqlite3  # Options de liaison pour GTK et SQLite
TARGET = gestion_inventaire_scanner  # Nom de l'exécutable final
OBJECTS = main.o gui.o database.o scanner.o  # Liste des fichiers objets à créer

# Règle principale pour construire l'exécutable
all: $(TARGET)

# Règle de création de l'exécutable
$(TARGET): $(OBJECTS)  # Dépendances de l'exécutable
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)  # Commande pour lier les objets et créer l'exécutable

# Règles de compilation des fichiers objets
main.o: main.c gui.h database.h scanner.h  # Dépendances pour main.o
	$(CC) $(CFLAGS) -c main.c  # Commande pour compiler main.c

gui.o: gui.c gui.h database.h scanner.h  # Dépendances pour gui.o
	$(CC) $(CFLAGS) -c gui.c  # Commande pour compiler gui.c

database.o: database.c database.h  # Dépendances pour database.o
	$(CC) $(CFLAGS) -c database.c  # Commande pour compiler database.c

scanner.o: scanner.c scanner.h  # Dépendances pour scanner.o
	$(CC) $(CFLAGS) -c scanner.c  # Commande pour compiler scanner.c

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f *.o $(TARGET)  # Supprimer les fichiers objets et l'exécutable

# Règle pour nettoyage complet, y compris la base de données
clean-all: clean
	rm -f inventaire.db  # Supprimer la base de données
