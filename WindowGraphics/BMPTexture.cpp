#include "BMPTexture.h"
#include "BMPFile.h"
#include "Sampler.h"
#include <string>

BMPTexture::BMPTexture(const char * fileName)
{
	BMPFile *bmpfile = new BMPFile();
	if (!bmpfile->loadBMP(fileName))
	{
		sampler = nullptr;
		return;
	}

	sampler = new Sampler(bmpfile->GetWidth(), bmpfile->GetHeight());
	memcpy(sampler->imageData, bmpfile->data, bmpfile->height * bmpfile->width * 3 * sizeof(unsigned char));
	
	delete bmpfile;
}

BMPTexture::~BMPTexture()
{
	if (sampler)
	{
		delete sampler;
		sampler = nullptr;
	}
}
