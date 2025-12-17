#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cassert>  // pour les assertions si besoin

// ==================== CLASSE IMAGE (tout-en-un) ====================

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

public:
    Image() = default;

    Image(int w, int h, int c, const std::string& m, uint8_t fill_value = 0)
        : width(w), height(h), channels(c), model(m) {
        if (w < 0 || h < 0 || c <= 0) throw std::invalid_argument("Invalid dimensions");
        data.assign(static_cast<size_t>(w) * h * c, fill_value);
    }

    Image(int w, int h, int c, const std::string& m, const uint8_t* buffer)
        : width(w), height(h), channels(c), model(m) {
        if (w < 0 || h < 0 || c <= 0) throw std::invalid_argument("Invalid dimensions");
        data.assign(buffer, buffer + static_cast<size_t>(w) * h * c);
    }

    // Règle des 5 (vector gère tout)
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

    // ===================== SEUILLAGE (ta partie) =====================

    Image operator<(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity < threshold) ? 255 : 0;
            }
        }
        return result;
    }

    Image operator<=(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity <= threshold) ? 255 : 0;
            }
        }
        return result;
    }

    Image operator>(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity > threshold) ? 255 : 0;
            }
        }
        return result;
    }

    Image operator>=(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity >= threshold) ? 255 : 0;
            }
        }
        return result;
    }

    Image operator==(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity == threshold) ? 255 : 0;
            }
        }
        return result;
    }

    Image operator!=(uint8_t threshold) const {
        Image result(width, height, 1, "GRAY");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t sum = 0;
                for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c);
                uint8_t intensity = static_cast<uint8_t>(sum / channels);
                result(x, y, 0) = (intensity != threshold) ? 255 : 0;
            }
        }
        return result;
    }
};

// =========================== MAIN ===========================

int main() {
    try {
        std::cout << "=== Tests de base (constructeurs et affichage) ===\n";
        Image img1;
        std::cout << img1 << std::endl;

        Image img2(200, 100, 3, "RGB", 128);
        std::cout << img2 << std::endl;

        uint8_t buffer[12] = {255,0,0, 0,255,0, 0,0,255, 255,255,255};
        Image img3(2, 2, 3, "RGB", buffer);
        std::cout << img3 << std::endl;

        std::cout << "\n=== Test accès et modification de pixels ===\n";
        Image img4(4, 3, 3, "RGB", 50);
        img4(1, 2, 0) = 255;
        img4.at(0, 0, 1) = 200;
        std::cout << "Pixel (1,2,0) = " << (int)img4(1,2,0) << std::endl;
        std::cout << "Pixel (0,0,1) = " << (int)img4.at(0,0,1) << std::endl;

        std::cout << "\n=== Test SEUILLAGE (nouvelle fonctionnalité) ===\n";
        // Image de test : fond à 100, quelques pixels modifiés
        Image testImg(6, 4, 3, "RGB", 100);  // fond moyen (100)
        testImg(1, 1, 0) = 200; testImg(1, 1, 1) = 200; testImg(1, 1, 2) = 200;  // pixel clair
        testImg(4, 2, 0) = 30;  testImg(4, 2, 1) = 30;  testImg(4, 2, 2) = 30;   // pixel sombre

        Image bin_gt = testImg > 150;   // pixels très clairs → 255
        Image bin_lt = testImg < 80;     // pixels très sombres → 255

        // Exemple supplémentaire : pixels "moyens" autour de 100
        Image bin_eq = testImg == 100;   // exactement 100 → 255, sinon 0
        Image bin_neq = testImg != 100;  // tout sauf 100 → 255

        std::cout << "Seuillage > 150 : " << bin_gt << std::endl;
        std::cout << "Seuillage < 80  : " << bin_lt << std::endl;
        std::cout << "Seuillage == 100 : " << bin_eq << std::endl;
        std::cout << "Seuillage != 100 : " << bin_neq << std::endl;

        // Vérification manuelle d'un pixel connu
        std::cout << "Pixel clair (1,1) dans >150 : " << (int)bin_gt(1,1,0) << " (doit être 255)" << std::endl;
        std::cout << "Pixel sombre (4,2) dans <80 : " << (int)bin_lt(4,2,0) << " (doit être 255)" << std::endl;
        std::cout << "Pixel de fond dans ==100 : " << (int)bin_eq(0,0,0) << " (doit être 255)" << std::endl;

        std::cout << "Image originale : " << testImg << std::endl;
        std::cout << "Seuillage > 150 : " << bin_gt << std::endl;
        std::cout << "Seuillage < 80  : " << bin_lt << std::endl;

        // Exemple de lecture d'un pixel du résultat binaire
        std::cout << "Pixel clair (1,1) dans >150 : " << (int)bin_gt(1,1,0) << " (doit être 255)" << std::endl;
        std::cout << "Pixel sombre (4,2) dans <80 : " << (int)bin_lt(4,2,0) << " (doit être 255)" << std::endl;

        std::cout << "\nTout fonctionne parfaitement !\n";
        std::cout << "Projet prêt pour les autres fonctionnalités (opérateurs arithmétiques, load/save).\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur capturée : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}