#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

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
};

int main() {
    try {
        Image img1;
        std::cout << img1 << std::endl;

        Image img2(200, 100, 3, "RGB", 128);
        std::cout << img2 << std::endl;

        uint8_t buffer[12] = {255,0,0, 0,255,0, 0,0,255, 255,255,255};
        Image img3(2, 2, 3, "RGB", buffer);
        std::cout << img3 << std::endl;

        Image img4(4, 3, 3, "RGB", 50);
        img4(1, 2, 0) = 255;
        img4.at(0, 0, 1) = 200;

        std::cout << "Pixel (1,2,0) = " << (int)img4(1,2,0) << std::endl;
        std::cout << "Pixel (0,0,1) = " << (int)img4.at(0,0,1) << std::endl;

        std::cout << "Tout fonctionne sans Image.cpp séparé !" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}