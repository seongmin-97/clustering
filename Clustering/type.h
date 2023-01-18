#pragma once
#include<stdio.h>
#include<stdlib.h>

#define WIDTH    512
#define HEIGHT	 512
#define IMG_SIZE WIDTH*HEIGHT
#define IMG_CNT  19

#define BLOCK_SIZE  2
#define VECTOR_SIZE BLOCK_SIZE*BLOCK_SIZE

#define CLUSTER   128
#define THRESHOLD 0.005

typedef unsigned char UInt8;
typedef char          Int8;
typedef int           Int32;
typedef float         Float32;
typedef double        Float64;

typedef char          Str;

typedef enum Boolean
{
	False, True
} Bool;


typedef struct image_
{
	Float64* img;
	Float64* block;
	Int32 blockCount;
} Image;

typedef struct cluster_
{
	Float64** center;
	Int32* clusterIdx;
	Int32* elementCount;
} Cluster;