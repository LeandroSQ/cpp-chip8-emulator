#pragma once

struct Size {
	int width;
	int height;

	// Size(int width, int height) : width(width), height(height) { }
	Size(int width, int height) : width{width}, height{height} { }

    Size operator * (const int &scalar) {
        return Size(width * scalar, height * scalar);
    }

	Size clone() {
		return Size(width, height);
	}
};