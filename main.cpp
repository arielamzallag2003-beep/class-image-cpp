#include "Image.h"
#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    try {
        std::cout << "=============================================================\n";
        std::cout << "          DÉMONSTRATION CLASSE IMAGE - M1 C++ Avancé         \n";
        std::cout << "                  Groupe : [Vos Noms Ici]                    \n";
        std::cout << "=============================================================\n\n";

        // ===============================================================
        std::cout << "1. CONSTRUCTEURS ET AFFICHAGE\n\n";
        Image vide;
        std::cout << "Image vide (défaut)          : " << vide << "\n";

        Image remplie(120, 80, 3, "RGB", 100);
        std::cout << "Image remplie de 100         : " << remplie << "\n";

        uint8_t buffer[] = {255,0,0, 0,255,0, 0,0,255, 255,255,255};
        Image petite(2, 2, 3, "RGB", buffer);
        std::cout << "Image 2x2 personnalisée      : " << petite << "\n\n";

        // ===============================================================
        std::cout << "2. ACCÈS ET MODIFICATION DE PIXELS\n\n";
        remplie(10, 20, 0) = 255;   // Rouge max
        remplie(10, 20, 1) = 200;   // Vert fort
        remplie(10, 20, 2) = 50;    // Bleu faible
        std::cout << "Pixel modifié à (10,20) : R=" << (int)remplie(10,20,0)
                  << " G=" << (int)remplie(10,20,1)
                  << " B=" << (int)remplie(10,20,2) << "\n\n";

        // ===============================================================
        std::cout << "3. OPÉRATIONS ARITHMÉTIQUES\n\n";
        Image plus_lumineuse = remplie + 80;
        Image plus_sombre    = remplie - 60;
        Image inversion      = ~remplie;

        std::cout << "Image originale              : " << remplie << "\n";
        std::cout << "+80 (plus lumineuse)         : " << plus_lumineuse << "\n";
        std::cout << "-60 (plus sombre)            : " << plus_sombre << "\n";
        std::cout << "Inversion (~)                : " << inversion << "\n";

        // Test multiplication et division
        Image contraste_haut = remplie * 1.8;
        Image contraste_bas  = remplie / 1.5;
        std::cout << "*1.8 (contraste augmenté)    : " << contraste_haut << "\n";
        std::cout << "/1.5 (contraste réduit)      : " << contraste_bas << "\n\n";

        // ===============================================================
        std::cout << "4. SEUILLAGE (Binarisation)\n\n";
        Image seuil_haut  = remplie > 140;
        Image seuil_bas   = remplie < 80;
        // Image seuil_moyen = remplie >= 90 && remplie <= 110;

        std::cout << "Seuillage > 140              : " << seuil_haut << "\n";
        std::cout << "Seuillage < 80               : " << seuil_bas << "\n";

        // Vérification d'un pixel connu
        std::cout << "Pixel modifié (très clair) dans >140 : "
                  << (int)seuil_haut(10,20,0) << " (doit être 255)\n\n";

        // ===============================================================
        std::cout << "5. CHARGEMENT D'UNE IMAGE RÉELLE\n\n";
        // Remplace "votre_image.png" par le nom exact de ton image dans le dossier
        Image photo = Image::load("new-lulu-fit.png");  // ou .jpg si tu as un JPG
        std::cout << "Image chargée                : " << photo << "\n\n";

        // ===============================================================
        std::cout << "6. TRAITEMENTS SUR IMAGE RÉELLE ET SAUVEGARDE\n\n";
        Image photo_inversee = ~photo;
        Image photo_seuillee = photo > 128;
        Image photo_bright   = photo + 50;

        std::cout << "Inversion sauvegardée dans   : inverted.png\n";
        photo_inversee.save("inverted.png");

        std::cout << "Seuillage moyen (>128) dans  : threshold.png\n";
        photo_seuillee.save("threshold.png");

        std::cout << "Image plus lumineuse dans    : bright.png\n";
        photo_bright.save("bright.png");

        std::cout << "\nFichiers générés : inverted.png, threshold.png, bright.png\n";
        std::cout << "Ouvrez-les pour voir les résultats !\n\n";

        // ===============================================================
        std::cout << "=============================================================\n";
        std::cout << "        TOUTES LES FONCTIONNALITÉS DU SUJET TESTÉES         \n";
        std::cout << "                   AVEC SUCCÈS !                            \n";
        std::cout << "=============================================================\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nERREUR : " << e.what() << "\n";
        return 1;
    }

    return 0;
}