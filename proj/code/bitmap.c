#include "bitmap.h"

Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	size_t i;
	uint16_t garbage;
	if ((bitmapInfoHeader.width % 2) != 0)
	{
		for (i = 0; i < bitmapInfoHeader.height; ++i)
		{
			fread((uint16_t *)bitmapImage + i * bitmapInfoHeader.width, bitmapInfoHeader.width * bitmapInfoHeader.bits / 8, 1, filePtr);
			fread(&garbage, 2, 1, filePtr);
		}
	}
	else
	{
		fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);
	}

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}

void drawMouse(Bitmap* bmp, int x, int y, Alignment alignment, char * buffer) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > H_RES || y + height < 0
			|| y > V_RES)
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > H_RES)
			drawWidth = H_RES;
	} else if (x + drawWidth >= H_RES) {
		drawWidth = H_RES - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= V_RES)
			continue;

		bufferStartPos = buffer;
		bufferStartPos += x * 2 + pos * H_RES * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
		unsigned int k = 0;
		for (k; k < drawWidth; k++)
		{
			if (*(uint16_t *)imgStartPos == 0x7E0)
			{

			}
			else
			{
				memcpy(bufferStartPos, imgStartPos, 2);
			}
			bufferStartPos += 2;
			imgStartPos += 2;
		}
	}
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, char * buffer)
{
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > H_RES || y + height < 0
			|| y > V_RES)
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > H_RES)
			drawWidth = H_RES;
	} else if (x + drawWidth >= H_RES) {
		drawWidth = H_RES - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= V_RES)
			continue;

		bufferStartPos = buffer;
		bufferStartPos += x * 2 + pos * H_RES * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}
