#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

class Image {
private:
    int width;
    int height;
    int channels;
    std::string model;
    std::vector<uint8_t> data;

public:
    Image();
    Image(int w, int h, int c, const std::string& m, uint8_t fill_value = 0);
    Image(int w, int h, int c, const std::string& m, const uint8_t* buffer);

    ~Image();
    Image(const Image& other);
    Image& operator=(const Image& other);
    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    const std::string& getModel() const;

    void setWidth(int w);
    void setHeight(int h);
    void setChannels(int c);
    void setModel(const std::string& m);

    friend std::ostream& operator<<(std::ostream& os, const Image& img);
};

#endif