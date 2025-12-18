#include "Image.h"
#include <algorithm>
#include <cmath>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "_tparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "_tparty/stb_image_write.h"

size_t Image::index(int x, int y, int c) const {
    if (x < 0 || x >= width || y < 0 || y >= height || c < 0 || c >= channels)
        throw std::out_of_range("Pixel coordinates out of bounds");
    return static_cast<size_t>(y) * width * channels + x * channels + c;
}

void Image::enlargeTo(int newWidth, int newHeight) {
    if (newWidth <= width && newHeight <= height) return;

    Image temp(newWidth, newHeight, channels, model);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                temp(x, y, c) = (*this)(x, y, c);
            }
        }
    }
    // Le reste est déjà à 0 grâce au constructeur
    *this = std::move(temp);
}

// Clamping helpers
uint8_t Image::clampAdd(int a, int b) {
    int res = a + b;
    return static_cast<uint8_t>(res < 0 ? 0 : (res > 255 ? 255 : res));
}

uint8_t Image::clampSub(int a, int b) {
    int res = a - b;
    return static_cast<uint8_t>(res < 0 ? 0 : (res > 255 ? 255 : res));
}

uint8_t Image::clampDiff(int a, int b) {
    return clampSub(std::max(a, b), std::min(a, b)); // |a - b|
}

uint8_t Image::clampMul(int a, double b) {
    double res = a * b;
    return static_cast<uint8_t>(res < 0 ? 0 : (res > 255 ? 255 : std::lround(res)));
}

uint8_t Image::clampDiv(int a, double b) {
    if (b == 0.0) throw std::invalid_argument("Division by zero");
    double res = a / b;
    return static_cast<uint8_t>(res < 0 ? 0 : (res > 255 ? 255 : std::lround(res)));
}

// Constructeurs (déjà dans .h, corps ici si besoin)
Image::Image() = default;

Image::Image(int w, int h, int c, const std::string& m, uint8_t fill_value)
    : width(w), height(h), channels(c), model(m) {
    if (w < 0 || h < 0 || c <= 0) throw std::invalid_argument("Invalid dimensions");
    data.assign(static_cast<size_t>(w) * h * c, fill_value);
}

Image::Image(int w, int h, int c, const std::string& m, const uint8_t* buffer)
    : width(w), height(h), channels(c), model(m) {
    if (w < 0 || h < 0 || c <= 0) throw std::invalid_argument("Invalid dimensions");
    data.assign(buffer, buffer + static_cast<size_t>(w) * h * c);
}

// Accès
int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }
int Image::getChannels() const { return channels; }
const std::string& Image::getModel() const { return model; }

uint8_t& Image::at(int x, int y, int c) { return data[index(x, y, c)]; }
const uint8_t& Image::at(int x, int y, int c) const { return data[index(x, y, c)]; }
uint8_t& Image::operator()(int x, int y, int c) { return data[index(x, y, c)]; }
const uint8_t& Image::operator()(int x, int y, int c) const { return data[index(x, y, c)]; }

// === OPÉRATEURS ARITHMÉTIQUES ===

// + avec image
Image Image::operator+(const Image& other) const { Image res = *this; res += other; return res; }
Image& Image::operator+=(const Image& other) {
    if (channels != other.channels || model != other.model)
        throw std::invalid_argument("Incompatible channels or model");
    int nw = std::max(width, other.width);
    int nh = std::max(height, other.height);
    Image temp = other;
    temp.enlargeTo(nw, nh);
    enlargeTo(nw, nh);
    for (int y = 0; y < nh; ++y)
        for (int x = 0; x < nw; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampAdd((*this)(x, y, c), temp(x, y, c));
    return *this;
}

// + avec scalaire
Image Image::operator+(int value) const { Image res = *this; res += value; return res; }
Image& Image::operator+=(int value) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = clampAdd(data[i], value);
    return *this;
}

// + avec pixel (vector)
Image Image::operator+(const std::vector<uint8_t>& pixel) const { Image res = *this; res += pixel; return res; }
Image& Image::operator+=(const std::vector<uint8_t>& pixel) {
    if (pixel.size() != static_cast<size_t>(channels))
        throw std::invalid_argument("Pixel size mismatch");
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampAdd((*this)(x, y, c), pixel[c]);
    return *this;
}

// - avec image
Image Image::operator-(const Image& other) const { Image res = *this; res -= other; return res; }
Image& Image::operator-=(const Image& other) {
    if (channels != other.channels || model != other.model)
        throw std::invalid_argument("Incompatible channels or model");
    int nw = std::max(width, other.width);
    int nh = std::max(height, other.height);
    Image temp = other;
    temp.enlargeTo(nw, nh);
    enlargeTo(nw, nh);
    for (int y = 0; y < nh; ++y)
        for (int x = 0; x < nw; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampSub((*this)(x, y, c), temp(x, y, c));
    return *this;
}

// - avec scalaire
Image Image::operator-(int value) const { Image res = *this; res -= value; return res; }
Image& Image::operator-=(int value) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = clampSub(data[i], value);
    return *this;
}

// - avec pixel
Image Image::operator-(const std::vector<uint8_t>& pixel) const { Image res = *this; res -= pixel; return res; }
Image& Image::operator-=(const std::vector<uint8_t>& pixel) {
    if (pixel.size() != static_cast<size_t>(channels))
        throw std::invalid_argument("Pixel size mismatch");
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampSub((*this)(x, y, c), pixel[c]);
    return *this;
}

// ^ (différence) avec image
Image Image::operator^(const Image& other) const { Image res = *this; res ^= other; return res; }
Image& Image::operator^=(const Image& other) {
    if (channels != other.channels || model != other.model)
        throw std::invalid_argument("Incompatible channels or model");
    int nw = std::max(width, other.width);
    int nh = std::max(height, other.height);
    Image temp = other;
    temp.enlargeTo(nw, nh);
    enlargeTo(nw, nh);
    for (int y = 0; y < nh; ++y)
        for (int x = 0; x < nw; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampDiff((*this)(x, y, c), temp(x, y, c));
    return *this;
}

// ^ avec scalaire
Image Image::operator^(int value) const { Image res = *this; res ^= value; return res; }
Image& Image::operator^=(int value) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = clampDiff(data[i], value);
    return *this;
}

// ^ avec pixel
Image Image::operator^(const std::vector<uint8_t>& pixel) const { Image res = *this; res ^= pixel; return res; }
Image& Image::operator^=(const std::vector<uint8_t>& pixel) {
    if (pixel.size() != static_cast<size_t>(channels))
        throw std::invalid_argument("Pixel size mismatch");
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            for (int c = 0; c < channels; ++c)
                (*this)(x, y, c) = clampDiff((*this)(x, y, c), pixel[c]);
    return *this;
}

// * et / avec double
Image Image::operator*(double value) const { Image res = *this; res *= value; return res; }
Image& Image::operator*=(double value) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = clampMul(data[i], value);
    return *this;
}

Image Image::operator/(double value) const { Image res = *this; res /= value; return res; }
Image& Image::operator/=(double value) {
    for (size_t i = 0; i < data.size(); ++i)
        data[i] = clampDiv(data[i], value);
    return *this;
}

// Inversion
Image Image::operator~() const {
    Image res(width, height, channels, model);
    for (size_t i = 0; i < data.size(); ++i)
        res.data[i] = 255 - data[i];
    return res;
}

// === SEUILLAGE COMPLET ===
#define THRESHOLD_OP(op) \
    Image result(width, height, 1, "GRAY"); \
    for (int y = 0; y < height; ++y) { \
        for (int x = 0; x < width; ++x) { \
            uint32_t sum = 0; \
            for (int c = 0; c < channels; ++c) sum += (*this)(x, y, c); \
            uint8_t intensity = static_cast<uint8_t>(sum / channels); \
            result(x, y, 0) = (intensity op threshold) ? 255 : 0; \
        } \
    } \
    return result;

Image Image::operator<(uint8_t threshold) const { THRESHOLD_OP(<) }
Image Image::operator<=(uint8_t threshold) const { THRESHOLD_OP(<=) }
Image Image::operator>(uint8_t threshold) const { THRESHOLD_OP(>) }
Image Image::operator>=(uint8_t threshold) const { THRESHOLD_OP(>=) }
Image Image::operator==(uint8_t threshold) const { THRESHOLD_OP(==) }
Image Image::operator!=(uint8_t threshold) const { THRESHOLD_OP(!=) }

// === LOAD / SAVE ===
bool Image::save(const char* filename) const {
    if (channels > 4) return false;
    int stride = width * channels;
    return stbi_write_png(filename, width, height, channels, data.data(), stride) != 0;
}

Image Image::load(const char* filename, int desired_channels) {
    Image img;
    int loaded_channels;
    unsigned char* ptr = stbi_load(filename, &img.width, &img.height, &loaded_channels, desired_channels);
    if (!ptr) throw std::runtime_error("Failed to load image: " + std::string(filename));
    img.channels = desired_channels != 0 ? desired_channels : loaded_channels;
    img.model = (img.channels == 1) ? "GRAY" : "RGB";
    size_t size = static_cast<size_t>(img.width) * img.height * img.channels;
    img.data.assign(ptr, ptr + size);
    stbi_image_free(ptr);
    return img;
}

// Affichage
std::ostream& operator<<(std::ostream& os, const Image& img) {
    os << img.width << "x" << img.height << "x" << img.channels << " (" << img.model << ")";
    return os;
}