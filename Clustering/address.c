#include"address.h"

Float64 get_pixel(Float64* data, Int32 imgIdx, Int32 x, Int32 y)
{
	return data[imgIdx * IMG_SIZE + y * WIDTH + x];
}

int get_blockVector(Float64* block, Int32 blockIdx, Float64* vector)
{
	for (int i = 0; i < VECTOR_SIZE; i++)
		vector[i] = block[blockIdx * VECTOR_SIZE + i];

	return 0;
}

Int32 get_blockIdx(Int32 imgIdx, Int32 topLeftX, Int32 topLeftY)
{
	Int32 blockCount = (WIDTH / BLOCK_SIZE) * (HEIGHT / BLOCK_SIZE);
	Int32 blockWidth = WIDTH / BLOCK_SIZE;
	Int32 blockX = topLeftX / BLOCK_SIZE;
	Int32 blockY = topLeftY / BLOCK_SIZE;

	return imgIdx * blockCount + blockY * blockWidth + blockX;
}