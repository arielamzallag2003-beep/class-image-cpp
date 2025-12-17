#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

using namespace std;

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

    Image& enlarge(int widthMax, int heightMax) {
        Image temp(widthMax, heightMax, channels, model);

        if (getWidth() == widthMax && getHeight() == heightMax) {
            return *this;
        }

        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    temp.data[temp.index(i, j, k)] = data[index(i, j, k)];
                }
            }

            if (getHeight() < heightMax) {
                for (int j = getHeight(); j < heightMax; j++) {
                    for (int k = 0; k < channels; k++) {
                        temp.data[temp.index(i, j, k)] = 0;
                    }
                }
            }
        }

        if (getWidth() < widthMax) {
            for (int i = getWidth(); i < widthMax; i++) {
                for (int j = 0; j < heightMax; j++) {
                    for (int k = 0; k < channels; k++) {
                        temp.data[temp.index(i, j, k)] = 0;
                    }
                }
            }
        }

        *this = temp;
        return *this;
    }

    //////  ADDITIONS    //////

    int add(int a, int b) {
        if (a + b > 255)
            return 255;
        else if (a + b < 0)
            return 0;
        else
            return a + b;
    }

    //Addition avec une image
    Image& operator+=(const Image& img) {
        if (channels != img.getChannels()) {
            throw std::invalid_argument("Image parameter has invalid number of channels");
        }
        if (model != img.getModel()) {
            throw std::invalid_argument("Image parameter has invalid model");
        }

        int heightMax = max(height, img.height);
        int widthMax = max(width, img.width);

        Image temp = img;
        temp.enlarge(widthMax, heightMax);
        enlarge(widthMax, heightMax);
        for (int i = 0; i < widthMax; i++) {
            for (int j = 0; j < heightMax; j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = add(data[index(i, j, k)], temp.data[temp.index(i, j, k)]);
                }
            }
        }

        return *this;
    }

    Image operator+(const Image& img) {
        Image I = *this;
        I += img;
        return I;
    }

    //Addition avec un entier
    Image& operator+=(int a) {
        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = add(data[index(i, j, k)], a);
                }
            }
        }
        return *this;
    }
    Image operator+(int a) {
        Image I = *this;
        I += a;
        return I;
    }

    //Addition avec un pixel
    Image& operator+=(vector<uint8_t> _data) {
        if (channels != _data.size()) {
            throw std::invalid_argument("Invalid number of channels");
        }

        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = add(data[index(i, j, k)], _data[k]);
                }
            }
        }
        return *this;
    }
    Image operator+(vector<uint8_t> _data) {
        Image I = *this;
        I += _data;
        return I;
    }


//////  SOUSTRACTIONS    //////

    int minus(int a, int b) {
        if (a - b > 255)
            return 255;
        else if (a - b < 0)
            return 0;
        else
            return a - b;
    }

    //soustraction avec une image
    Image& operator-=(const Image& img) {
        if (channels != img.getChannels()) {
            throw std::invalid_argument("Image parameter has invalid number of channels");
        }
        if (model != img.getModel()) {
            throw std::invalid_argument("Image parameter has invalid model");
        }

        int heightMax = max(height, img.height);
        int widthMax = max(width, img.width);

        Image temp = img;
        temp.enlarge(widthMax, heightMax);
        enlarge(widthMax, heightMax);
        for (int i = 0; i < widthMax; i++) {
            for (int j = 0; j < heightMax; j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = minus(data[index(i, j, k)], temp.data[temp.index(i, j, k)]);
                }
            }
        }

        return *this;
    }

    Image operator-(const Image& img) {
        Image I = *this;
        I -= img;
        return I;
    }

    //soustraction avec un entier
    Image& operator-=(int a) {
        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = minus(data[index(i, j, k)], a);
                }
            }
        }
        return *this;
    }
    Image operator-(int a) {
        Image I = *this;
        I -= a;
        return I;
    }

    //soustraction avec un pixel
    Image& operator-=(vector<uint8_t> _data) {
        if (channels != _data.size()) {
            throw std::invalid_argument("Invalid number of channels");
        }

        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = minus(data[index(i, j, k)], _data[k]);
                }
            }
        }
        return *this;
    }
    Image operator-(vector<uint8_t> _data) {
        Image I = *this;
        I -= _data;
        return I;
    }


//////  DIFFERENCES    //////

    int difference(int a, int b) {

        int temp;
        if (a - b < 0)
            temp = (a - b)*-1;

        if (a - b > 255 || temp > 255)
            return 255;
        else
            return a - b;
    }

    //différence avec une image
    Image& operator^=(const Image& img) {
        if (channels != img.getChannels()) {
            throw std::invalid_argument("Image parameter has invalid number of channels");
        }
        if (model != img.getModel()) {
            throw std::invalid_argument("Image parameter has invalid model");
        }

        int heightMax = max(height, img.height);
        int widthMax = max(width, img.width);

        Image temp = img;
        temp.enlarge(widthMax, heightMax);
        enlarge(widthMax, heightMax);
        for (int i = 0; i < widthMax; i++) {
            for (int j = 0; j < heightMax; j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = difference(data[index(i, j, k)], temp.data[temp.index(i, j, k)]);
                }
            }
        }

        return *this;
    }

    Image operator^(const Image& img) {
        Image I = *this;
        I ^= img;
        return I;
    }

    //différence avec un entier
    Image& operator^=(int a) {
        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = difference(data[index(i, j, k)], a);
                }
            }
        }
        return *this;
    }
    Image operator^(int a) {
        Image I = *this;
        I ^= a;
        return I;
    }

    //différence avec un pixel
    Image& operator^=(vector<uint8_t> _data) {
        if (channels != _data.size()) {
            throw std::invalid_argument("Invalid number of channels");
        }

        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = difference(data[index(i, j, k)], _data[k]);
                }
            }
        }
        return *this;
    }
    Image operator^(vector<uint8_t> _data) {
        Image I = *this;
        I ^= _data;
        return I;
    }


    //////  MULTIPLICATIONS    //////

    int multiplication(int a, int b) {
        if (a * b > 255)
            return 255;
        else if (a * b < 0)
            return 0;
        else
            return a * b;
    }

    //multiplication avec un entier
    Image& operator*=(int a) {
        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = multiplication(data[index(i, j, k)], a);
                }
            }
        }
        return *this;
    }
    Image operator*(int a) {
        Image I = *this;
        I *= a;
        return I;
    }


    //////  DIVISIONS    //////

    int division(int a, int b) {
        if (a / b > 255)
            return 255;
        else if (a / b < 0)
            return 0;
        else
            return a / b;
    }

    //division avec un entier
    Image& operator/=(int a) {
        for (int i = 0; i < getWidth(); i++) {
            for (int j = 0; j < getHeight(); j++) {
                for (int k = 0; k < channels; k++) {
                    data[index(i, j, k)] = division(data[index(i, j, k)], a);
                }
            }
        }
        return *this;
    }
    Image operator/(int a) {
        Image I = *this;
        I /= a;
        return I;
    }


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

        uint8_t buffer[12] = { 0,100,0, 100,100,100};
        Image img3(1, 2, 3, "RGB", buffer);
        std::cout << img3 << std::endl;

        uint8_t buffer2[12] = {100,0,0, 100,100,100};
        Image img5(2, 1, 3, "RGB", buffer2);
        std::cout << img3 << std::endl;

        std::cout << img3 << std::endl;
        for (int i = 0; i < img3.getWidth() * img3.getHeight() * img3.getChannels(); i++) {
            std::cout << "Pixel (1,2,0) = " << (int)img3.data[i] << std::endl;
        }

        img3 = img3 + img5;

        std::cout << std::endl;
        for (int i = 0; i < img3.getWidth() * img3.getHeight() * img3.getChannels(); i++) {
            std::cout << "Pixel (1,2,0) = " << (int)img3.data[i] << std::endl;
        }
        /*Image img4(4, 3, 3, "RGB", 50);
        img4(1, 2, 0) = 255;
        img4.at(0, 0, 1) = 200;*/

        //std::cout << "Pixel (1,2,0) = " << (int)img4(1,2,0) << std::endl;
        //std::cout << "Pixel (0,0,1) = " << (int)img4.at(0,0,1) << std::endl;

        std::cout << "Tout fonctionne sans Image.cpp séparé !" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}