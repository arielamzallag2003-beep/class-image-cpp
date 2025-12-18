#include "Image.h"
#include <iostream>
#include <vector>

int main() {
    try {
        std::cout << "=============================================================\n";
        std::cout << "     DÉMONSTRATION FINALE - CLASSE IMAGE (Groupe TALVET / MAILLARD / AMZALLAG)\n";
        std::cout << "=============================================================\n\n";

        // ===============================================================
        std::cout << "1. CHARGEMENT DES DEUX IMAGES RÉELLES (forcé en RGB 3 canaux)\n\n";
        Image lulu = Image::load("new-lulu-fit.png", 3); 
        Image pip  = Image::load("pip-secret.png", 3); 

        std::cout << "Image 1 (Lulu)     : " << lulu << "\n";
        std::cout << "Image 2 (Pip)      : " << pip  << "\n\n";

        if (lulu.getChannels() != 3 || pip.getChannels() != 3) {
            std::cout << "Attention : une image avait de la transparence, convertie en RGB.\n\n";
        }

        // ===============================================================
        std::cout << "2. OPÉRATIONS DIRECTES ENTRE LES DEUX IMAGES\n\n";
        Image addition       = lulu + pip;
        Image soustraction   = lulu - pip;
        Image difference     = lulu ^ pip; 

        addition.save("addition_lulu_pip.png");
        soustraction.save("soustraction_lulu_pip.png");
        difference.save("difference_lulu_pip.png");

        std::cout << "Addition (lulu + pip)      : sauvegardée dans addition_lulu_pip.png\n";
        std::cout << "Soustraction (lulu - pip)  : sauvegardée dans soustraction_lulu_pip.png\n";
        std::cout << "Différence (|lulu - pip|)  : sauvegardée dans difference_lulu_pip.png\n\n";

        // ===============================================================
        std::cout << "3. TRAITEMENTS INDIVIDUELS SUR CHAQUE IMAGE\n\n";

        // Sur Lulu
        Image lulu_inversee  = ~lulu;
        Image lulu_bright    = lulu + 60;
        Image lulu_dark      = lulu - 40;
        Image lulu_seuil     = lulu > 120;
        Image lulu_contraste = lulu * 1.5;

        lulu_inversee.save("lulu_inversee.png");
        lulu_bright.save("lulu_plus_lumineuse.png");
        lulu_dark.save("lulu_plus_sombre.png");
        lulu_seuil.save("lulu_seuillage.png");
        lulu_contraste.save("lulu_contraste.png");

        std::cout << "Lulu - Inversion           : lulu_inversee.png\n";
        std::cout << "Lulu - +60 luminosité      : lulu_plus_lumineuse.png\n";
        std::cout << "Lulu - -40 luminosité      : lulu_plus_sombre.png\n";
        std::cout << "Lulu - Seuillage >120      : lulu_seuillage.png\n";
        std::cout << "Lulu - Contraste x1.5       : lulu_contraste.png\n\n";

        Image pip_inversee   = ~pip;
        Image pip_seuil      = pip > 100;
        Image pip_bright     = pip + 80;

        pip_inversee.save("pip_inversee.png");
        pip_seuil.save("pip_seuillage.png");
        pip_bright.save("pip_plus_lumineuse.png");

        std::cout << "Pip - Inversion            : pip_inversee.png\n";
        std::cout << "Pip - Seuillage >100       : pip_seuillage.png\n";
        std::cout << "Pip - +80 luminosité       : pip_plus_lumineuse.png\n\n";

        // ===============================================================
        std::cout << "4. RÉSUMÉ DES FICHIERS GÉNÉRÉS (à ouvrir pour la démo !)\n\n";
        std::cout << "- addition_lulu_pip.png\n";
        std::cout << "- soustraction_lulu_pip.png\n";
        std::cout << "- difference_lulu_pip.png\n";
        std::cout << "- lulu_inversee.png\n";
        std::cout << "- lulu_plus_lumineuse.png\n";
        std::cout << "- lulu_plus_sombre.png\n";
        std::cout << "- lulu_seuillage.png\n";
        std::cout << "- lulu_contraste.png\n";
        std::cout << "- pip_inversee.png\n";
        std::cout << "- pip_seuillage.png\n";
        std::cout << "- pip_plus_lumineuse.png\n\n";

        std::cout << "=============================================================\n";
        std::cout << "           TOUTES LES FONCTIONNALITÉS TESTÉES AVEC SUCCÈS\n";
        std::cout << "     Projet prêt pour la soutenance - Effets visuels garantis !\n";
        std::cout << "=============================================================\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nERREUR lors du chargement ou traitement : " << e.what() << "\n";
        std::cerr << "Vérifiez que new-lulu-fit.png et pip-secret.png sont bien dans le dossier.\n";
        return 1;
    }

    return 0;
}