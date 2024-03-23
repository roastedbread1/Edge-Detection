#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename) {
	if (read(filename)) {
		printf("Read %s\n", filename);
		size = w * h * channels;
	}
	else {
		printf("Failed to read %s\n", filename);

	};
};

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels){
	size = w * h * channels;
	data = new uint8_t[size];
};


Image::Image(const Image& img) : Image(img.w, img.h, img.channels){
	memcpy(data, img.data, size);

};

Image::~Image() {
	stbi_image_free(data);
};

bool Image::read(const char* filename) {
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;

};
bool Image::write(const char* filename) {
	ImageType  type = getFileType(filename);
	int success = 0; 
	switch (type) {
		case PNG: 
			success = stbi_write_png(filename, w, h, channels, data, w * channels);
			break;
		case JPG:
			success = stbi_write_jpg(filename, w, h, channels, data, 100);
	}
	return success != 0;
}


ImageType Image::getFileType(const char* filename) {

	const char* ext = strrchr(filename, '.');
	if (ext != nullptr) {
		if (strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if (strcmp(ext, ".jpg") == 0) {
			return JPG;
		};
	};
	return PNG;
};


Image& Image::blacknwhite() {
	
		if (channels != 1) {
			grayscale();
		}
		for (int i = 0; i < size; i++) {
			uint8_t value = data[i] > 127 ? 255 : 0;
			memset(data + i, value, 1);
		}
		return *this;

}

Image& Image::grayscale() {
	if (channels < 3) {
		printf("Image %p is already a grayscale image", this);
	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = 0.2126 * data[i] + 0.7152 * data[i + 1] + 0.722 * data[i + 2];
			memset(data + i, gray, 3);
		}
	}
	return *this;
};



Image& Image::edge_detection() {
	blacknwhite(); // Ensure image is black and white for edge detection
	size_t size = w * h * channels;
	uint8_t* temp = new uint8_t[size];
	memset(temp, 0, size);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int gx = gradX(x, y);
			int gy = gradY(x, y);

			int magnitude = static_cast<int>(sqrt(gx * gx + gy * gy));
			uint8_t value = magnitude > 15 ? 255 : 0; // Thresholding

			for (int c = 0; c < channels; ++c) {
				temp[(y * w + x) * channels + c] = value;
			}
		}
	}

	memcpy(data, temp, size);
	delete[] temp;

	return *this;
}

int Image::gradX(int x, int y) {
	if (x > 0 && x < w - 1) {
		int left = data[(y * w + x - 1) * channels];
		int right = data[(y * w + x + 1) * channels];
		return (right - left) / 2;
	}
	else if (x == 0) {
		int right = data[(y * w + x + 1) * channels];
		return right - data[(y * w + x) * channels];
	}
	else {
		int left = data[(y * w + x - 1) * channels];
		return data[(y * w + x) * channels] - left;
	}
}

int Image::gradY(int x, int y) {
	if (y > 0 && y < h - 1) {
		int top = data[((y - 1) * w + x) * channels];
		int bottom = data[((y + 1) * w + x) * channels];
		return (bottom - top) / 2;
	}
	else if (y == 0) {
		int bottom = data[((y + 1) * w + x) * channels];
		return bottom - data[(y * w + x) * channels];
	}
	else {
		int top = data[((y - 1) * w + x) * channels];
		return data[(y * w + x) * channels] - top;
	}
}

