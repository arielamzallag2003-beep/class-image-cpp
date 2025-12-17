#include "Image.h"
#include <iostream>

int main() {
    Image img1;
    std::cout << img1 << std::endl;

    Image img2(200, 100, 3, "RGB", 128);
    std::cout << img2 << std::endl;

    uint8_t buffer[12] = {255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255};
    Image img3(2, 2, 3, "RGB", buffer);
    std::cout << img3 << std::endl;

    Image img4 = img2;
    std::cout << img4 << std::endl;

    return 0;
}