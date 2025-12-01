#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class Texture {
public:
    unsigned char *image;
    int width;
	int height;
	int compNum;

    Texture(unsigned char *img, int width, int height, int number);
};

#endif
