#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <stdexcept>

class Image {
private:
    int width = 0;
    int height = 0;
    int channels = 0;
    std::string model = "NONE";
    std::vector<uint8_t> data;

    size_t index(int x, int y, int c) const;

    // Fonction helper pour agrandir l'image (padding à 0)
    void enlargeTo(int newWidth, int newHeight);

    // Fonctions de clamping
    static uint8_t clampAdd(int a, int b);
    static uint8_t clampSub(int a, int b);
    static uint8_t clampDiff(int a, int b);
    static uint8_t clampMul(int a, double b);
    static uint8_t clampDiv(int a, double b);

public:
    Image();
    Image(int w, int h, int c, const std::string& m, uint8_t fill_value = 0);
    Image(int w, int h, int c, const std::string& m, const uint8_t* buffer);

    // Règle des 5
    ~Image() = default;
    Image(const Image&) = default;
    Image& operator=(const Image&) = default;
    Image(Image&&) noexcept = default;
    Image& operator=(Image&&) noexcept = default;

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    const std::string& getModel() const;

    uint8_t& at(int x, int y, int c);
    const uint8_t& at(int x, int y, int c) const;
    uint8_t& operator()(int x, int y, int c);
    const uint8_t& operator()(int x, int y, int c) const;

    // Opérateurs arithmétiques
    Image operator+(const Image& other) const;
    Image& operator+=(const Image& other);
    Image operator+(int value) const;
    Image& operator+=(int value);
    Image operator+(const std::vector<uint8_t>& pixel) const;
    Image& operator+=(const std::vector<uint8_t>& pixel);

    Image operator-(const Image& other) const;
    Image& operator-=(const Image& other);
    Image operator-(int value) const;
    Image& operator-=(int value);
    Image operator-(const std::vector<uint8_t>& pixel) const;
    Image& operator-=(const std::vector<uint8_t>& pixel);

    Image operator^(const Image& other) const;  // différence
    Image& operator^=(const Image& other);
    Image operator^(int value) const;
    Image& operator^=(int value);
    Image operator^(const std::vector<uint8_t>& pixel) const;
    Image& operator^=(const std::vector<uint8_t>& pixel);

    Image operator*(double value) const;
    Image& operator*=(double value);
    Image operator/(double value) const;
    Image& operator/=(double value);

    Image operator~() const;  // inversion

    // Seuillage
    Image operator<(uint8_t threshold) const;
    Image operator<=(uint8_t threshold) const;
    Image operator>(uint8_t threshold) const;
    Image operator>=(uint8_t threshold) const;
    Image operator==(uint8_t threshold) const;
    Image operator!=(uint8_t threshold) const;

    // Load / Save
    bool save(const char* filename) const;
    static Image load(const char* filename, int desired_channels = 0);

    friend std::ostream& operator<<(std::ostream& os, const Image& img);
};

#endif