#include "BMPFile.h"
#include <stdio.h>
#include <memory>

BMPFile::BMPFile()
{
	fileHeader = new unsigned char[54];
}

BMPFile::~BMPFile()
{
	if (fileHeader)
	{
		free(fileHeader);
		fileHeader = nullptr;
	}

	if (data)
	{
		free(data);
		data = nullptr;
	}

}

bool BMPFile::loadBMP(const char * fileName)
{
	FILE* bmpFILE = fopen(fileName, "rb");
	if (!bmpFILE)
		return false;
	
	if (fread(fileHeader, 1, 54, bmpFILE) != 54)
		return false;

	if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
		return false;

	dataPosition = *(int*)&fileHeader[0xa];
	width = *(int*)&fileHeader[0x12];
	height = *(int*)&fileHeader[0x16]; // 为负数 那么就是倒放的
	bmpSize = *(int*)&fileHeader[0x22];

	if (bmpSize == 0)
		bmpSize = width * height * 3;

	if (dataPosition == 0)
		dataPosition = 54;

	unsigned int size = width * height * 3;
	data = new unsigned char[size];
	fread(data, 1, bmpSize, bmpFILE);
	fclose(bmpFILE);

	for (int i = 0; i < (int)bmpSize; i+=3)
	{
		swapColor(&data[i], &data[i+2]);
	}

	return true;
}

void BMPFile::swapColor(unsigned char * a, unsigned char * b)
{
	unsigned char temp = *a;
	*a = *b;
	*b = temp;
}
