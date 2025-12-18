# Projet Classe Image - C++ Avancé

## Description rapide
Projet pour le cours M1 – T1 – C++ avancé et design pattern.  
Implémentation d'une classe `Image` gérant des images matricielles couleur (uint8_t par canal) avec de nombreuses opérations surchargées.

Rendu final : **mercredi 17 décembre 2025 à 23h59**

## Structure du projet (core actuel)
- `Image.h`        → Déclaration de la classe
- `Image.cpp`      → Implémentation de la classe
- `main.cpp`       → Fichier de tests/démonstration (à enrichir)

## Comment compiler et tester

### Prérequis
- Compilateur C++ supportant au minimum C++17 (g++, clang++, MSVC...)

### Commandes (Linux / macOS / WSL)

```bash
# Compilation
g++ -std=c++17 -Wall -Wextra -O2 Image.cpp main.cpp -o image_test

# Exécution
./image_test
```