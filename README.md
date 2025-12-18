# Projet Classe Image - C++ Avancé

**Cours** : M1 – T1 – C++ avancé et design pattern  
**Groupe** : Ivo Talvet, Enzo Maillard, Ariel Amzallag  

## Structure du projet

- `Image.h`       → Déclaration de la classe
- `Image.cpp`     → Implémentation complète
- `main.cpp`      → Démonstration de toutes les fonctionnalités
- `_tparty/`      → stb_image.h + stb_image_write.h (load/save PNG)

## Compilation et exécution

```bash
g++ -std=c++17 -Wall -Wextra Image.cpp main.cpp -o projet
./projet
```

## Fonctionnalités implémentées

- Constructeurs (défaut, remplissage, buffer)
- Règle des 5
- Accès pixels sécurisé (`at()`, `operator()`) avec exceptions
- Opérations arithmétiques (+, -, ^, *, /, ~) avec scalaire, pixel et image
- Gestion des tailles différentes (padding 0)
- Clamping systématique [0–255]
- Exceptions pour incompatibilité (canaux, modèle)
- Seuillage complet (<, <=, >, >=, ==, !=) → image GRAY binaire
- Affichage `<<` au format demandé
- Chargement/sauvegarde PNG (via stb_image)

## Démonstration

Le programme :
- Teste toutes les fonctionnalités
- Charge une image réelle (si présente dans le dossier)
- Applique inversion, seuillage, ajustement luminosité
- Sauvegarde les résultats : `inverted.png`, `threshold.png`, `bright.png`

Ouvrez ces images pour voir les effets.

Projet terminé et prêt pour la soutenance.