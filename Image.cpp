#include "Image.h"
#include <algorithm>
#include <stdexcept>

Image::Image()
    : width(0), height(0), channels(0), model("NONE") {}

Image::Image(int w, int h, int c, const std::string& m, uint8_t fill_value)
    : width(w), height(h), channels(c), model(m) {
    if (w < 0 || h < 0 || c <= 0)
        throw std::invalid_argument("Invalid dimensions or channels");
    data.assign(static_cast<size_t>(w) * h * c, fill_value);
}

Image::Image(int w, int h, int c, const std::string& m, const uint8_t* buffer)
    : width(w), height(h), channels(c), model(m) {
    if (w < 0 || h < 0 || c <= 0)
        throw std::invalid_argument("Invalid dimensions or channels");
    data.assign(buffer, buffer + static_cast<size_t>(w) * h * c);
}

Image::~Image() = default;

Image::Image(const Image& other)
    : width(other.width), height(other.height), channels(other.channels),
      model(other.model), data(other.data) {}

Image& Image::operator=(const Image& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        channels = other.channels;
        model = other.model;
        data = other.data;
    }
    return *this;
}

Image::Image(Image&& other) noexcept
    : width(other.width), height(other.height), channels(other.channels),
      model(std::move(other.model)), data(std::move(other.data)) {
    other.width = 0;
    other.height = 0;
    other.channels = 0;
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        channels = other.channels;
        model = std::move(other.model);
        data = std::move(other.data);

        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }
    return *this;
}

int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }
int Image::getChannels() const { return channels; }
const std::string& Image::getModel() const { return model; }

void Image::setWidth(int w) { width = w; }
void Image::setHeight(int h) { height = h; }
void Image::setChannels(int c) { channels = c; }
void Image::setModel(const std::string& m) { model = m; }

std::ostream& operator<<(std::ostream& os, const Image& img) {
    os << img.width << "x" << img.height << "x" << img.channels << " (" << img.model << ")";
    return os;
}