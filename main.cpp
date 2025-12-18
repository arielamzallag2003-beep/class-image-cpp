#include "Image.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "_tparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "_tparty/stb_image_write.h"

class Image {
private:
    int width = 0;
    int height = 0;
    int channels = 0;
    std::string model = "NONE";
    std::vector<uint8_t> data;

    size_t index(int x, int y, int c) const {
        if (x < 0 || x >= width || y < 0 || y >= height || c < 0 || c >= channels) {
            throw std::out_of_range("Pixel coordinates out of bounds");
        }
        return static_cast<size_t>(y) * width * channels + x * channels + c;
    }

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

    // Règle des 5 simplifiée (le vector gère tout)
    ~Image() = default;
    Image(const Image&) = default;
    Image& operator=(const Image&) = default;
    Image(Image&&) noexcept = default;
    Image& operator=(Image&&) noexcept = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    const std::string& getModel() const { return model; }

    uint8_t& at(int x, int y, int c) { return data[index(x, y, c)]; }
    const uint8_t& at(int x, int y, int c) const { return data[index(x, y, c)]; }
    uint8_t& operator()(int x, int y, int c) { return data[index(x, y, c)]; }
    const uint8_t& operator()(int x, int y, int c) const { return data[index(x, y, c)]; }

    friend std::ostream& operator<<(std::ostream& os, const Image& img) {
        os << img.width << "x" << img.height << "x" << img.channels << " (" << img.model << ")";
        return os;
    }

    bool save(char const *filename) const
    {
        assert(channels <5 && "4 canaux max.");
        const int wrote = stbi_write_png(filename, width, height, channels, data.data(), channels * width);

        return wrote;
    }

    static Image Load(char const *filename, int channels) {
        Image im;
        FILE *f = fopen(filename, "rb");

        if(f == NULL)
            goto no_file;
        
        stbi_uc * data;
        data = stbi_load_from_file(f, &im.width, &im.height, &im.channels, channels);
        
        if(data == NULL)
            goto no_data;
        
        for(int i = 0; i < im.width * im.height * im.channels; i++) {
            im.data.push_back(data[i]);
        }
        
        if(false) {
            no_data:
            std::cerr << "Couldnt parse file with stbi_load_from_file()\n";
        }
        free(data);
        if(false) {
            no_file:
            std::cerr << "Couldnt open file" << filename << "\n";
        }
        fclose(f);

        return im;
    }
};

void test_save_load();

int main() {
    try {
        Image img1;
        std::cout << img1 << std::endl;

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

        test_save_load();

        std::cout << "Tout fonctionne sans Image.cpp séparé !" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "\nERREUR : " << e.what() << "\n";
        return 1;
    }

    return 0;
}

void test_save_load()
{
    Image img4(4, 3, 3, "RGB", 50);
    img4(1, 2, 0) = 255;
    img4.at(0, 0, 1) = 200;

    const bool success = img4.save("toto.png");
    assert(success && "Probleme pour sauvegarder l'image");

    Image toto = Image::Load("toto.png", 3);
    assert(toto.at(0,0, 1) == 200 && "chargement ok");
}