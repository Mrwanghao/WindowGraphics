#pragma once

class BMPFile 
{
private:
	unsigned char* fileHeader;
	unsigned int dataPosition;
	unsigned int bmpSize;

public:
	unsigned int width;
	unsigned int height; 
	unsigned char* data;
	BMPFile();
	~BMPFile();
	bool loadBMP(const char* fileName);

	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }

private:
	void swapColor(unsigned char* a, unsigned char* b);
};