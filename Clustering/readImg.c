#pragma warning(disable:4996)

#include<stdio.h>
#include"readImg.h"

const char PATH[100] = "D:\\msp_AI\\Clustering\\data\\";
const char IMG_NAME[100] = "I";
const char EXTENSION[100] = ".img";

Image read_file()
{
	Image image;
	image.img = (Float64*)calloc(IMG_CNT * IMG_SIZE, sizeof(Float64));
	image.block = (Float64*)calloc(IMG_CNT * IMG_SIZE, sizeof(Float64));

	FILE* fp;
	UInt8* readImg = (UInt8*)calloc(IMG_SIZE, sizeof(UInt8));

	for (int idx = 0; idx < IMG_CNT; idx++)
	{
		Str filePath[100];
		Str num[10];

		strcpy(filePath, PATH);
		strcat(filePath, IMG_NAME);
		itoa(idx, num, 10);
		strcat(filePath, num);
		strcat(filePath, EXTENSION);


		fopen_s(&fp, filePath, "rb");
		fread(readImg, sizeof(UInt8), IMG_SIZE, fp);
		fclose(fp);

		for (int i = 0; i < IMG_SIZE; i++)
			image.img[idx * IMG_SIZE + i] = readImg[i];
	}

	free(readImg);

	return image;
}

int Img2Block(Image* image)
{
	Float64* block = (Float64*)calloc(BLOCK_SIZE * BLOCK_SIZE, sizeof(Float64));
	image->blockCount = 0;

	for (int idx = 0; idx < IMG_CNT; idx++)
	{
		Int32 blockIdx = 0;
		for (int y = 0; y < HEIGHT; y += BLOCK_SIZE)
			for (int x = 0; x < WIDTH; x += BLOCK_SIZE)
			{
				get_block(image->img, idx, x, y, block);
				for (int i = 0; i < BLOCK_SIZE * BLOCK_SIZE; i++)
					image->block[idx * IMG_SIZE + blockIdx * BLOCK_SIZE * BLOCK_SIZE + i] = block[i];
				blockIdx++;
				image->blockCount++;
			}
	}

	free(block);

	return 0;
}

int get_block(Float64* data, Int32 imgIdx, Int32 topLeftX, Int32 topLeftY, Float64* block)
{
	for (int y = 0; y < BLOCK_SIZE; y++)
		for (int x = 0; x < BLOCK_SIZE; x++)
			block[y * BLOCK_SIZE + x] = data[imgIdx * IMG_SIZE + (topLeftY + y) * WIDTH + topLeftX + x];

	return 0;
}

int destroy_image(Image* image)
{
	free(image->img);
	free(image->block);
}