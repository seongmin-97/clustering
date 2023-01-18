#include"clustering.h"
#include"address.h"
#include<float.h>
#include<math.h>
#include<time.h>

int clustering(Image* image)
{

	Int32 nowCluster = 1;
	Cluster cluster = initalize_cluster(image, image->blockCount);
	
	Float64 mse;
	Float64 move = 0.0;


	while (True)
	{
		if (move < THRESHOLD)
		{
			nowCluster <<= 1;

			if (nowCluster > CLUSTER)
				break;

			devide_cluster(&cluster, nowCluster);
		}

		relocation_cluster(&cluster, image, nowCluster);
		move = relocation_center(&cluster, image, nowCluster);
		mse = get_mse(&cluster, image, nowCluster);

		printf("cluster 수 : %d   mse : %f  move : %f \n", nowCluster, mse, move);
	}

	destroy_cluster(&cluster);

	return 0;
}

Cluster initalize_cluster(Image* image, Int32 blockCount)
{
	Cluster cluster;

	cluster.center = (Float64**)calloc(CLUSTER, sizeof(Float64*));

	for (int i = 0; i < CLUSTER; i++)
		cluster.center[i] = (Float64*)calloc(VECTOR_SIZE, sizeof(Float64));

	cluster.clusterIdx = (Int32*)calloc(image->blockCount, sizeof(Int32));
	cluster.elementCount = (Int32*)calloc(CLUSTER, sizeof(Int32));
	
	get_center(&cluster, image, image->blockCount, 0);
	
	return cluster;
}

int devide_cluster(Cluster* cluster, Int32 nowCluster) // nowCluster가 clusterCount보다 크면 안된다
{
	// 평균에 epsilon 더해 새로운 클러스터의 중심을 구하자
	Int32 beforeCluster = nowCluster / 2;
	Float64* epsilon = (Float64*)calloc(VECTOR_SIZE, sizeof(Float64));
	Float64** centerBuf = (Float64**)calloc(beforeCluster, sizeof(Float64*));
	
	for (int idx = 0; idx < beforeCluster; idx++)
	{
		centerBuf[idx] = (Float64*)calloc(VECTOR_SIZE, sizeof(Float64));
		for (int i = 0; i < VECTOR_SIZE; i++)
			centerBuf[idx][i] = cluster->center[idx][i];
	}

	
	for (int idx = 0; idx < nowCluster; idx += 2)
	{
		get_epsilon_vector(epsilon, nowCluster);
		for (int n = 0; n < 2; n++)
			for (int i = 0; i < VECTOR_SIZE; i++)
			{
				if (n == 0)
					cluster->center[idx][i] = centerBuf[idx / 2][i] + epsilon[i];
				else
					cluster->center[idx + n][i] = centerBuf[idx / 2][i] - epsilon[i];
			}
	}


	for (int idx = 0; idx < beforeCluster; idx++)
		free(centerBuf[idx]);

	free(centerBuf);
	free(epsilon);

	return 0;
}

int relocation_cluster(Cluster* cluster, Image* image, Int32 nowCluster)
{
	Int32 clusterCenter;
	Float64 distance;
	Float64 blockVector[VECTOR_SIZE];

	// cluster에 속한 블록 삭제
	for (int clusterIdx = 0; clusterIdx < nowCluster; clusterIdx++)
		cluster->elementCount[clusterIdx] = 0;

	for (int blockIdx = 0; blockIdx < image->blockCount; blockIdx++)
	{
		Float64 minDistance = DBL_MAX;
		for (int centerIdx = 0; centerIdx < nowCluster; centerIdx++)
		{
			get_blockVector(image->block, blockIdx, blockVector);
			distance = get_distance(cluster->center[centerIdx], blockVector);

			if (distance < minDistance)
			{
				clusterCenter = centerIdx;
				minDistance = distance;
			}
		}
		cluster->clusterIdx[blockIdx] = clusterCenter;
		cluster->elementCount[clusterCenter]++;
	}

	return 0;
}

Float64 relocation_center(Cluster* cluster, Image* image, Int32 nowCluster)
{
	Float64 move = 0.0;
	Float64 newCenter[CLUSTER][VECTOR_SIZE] = {0};
	Float64 blockVector[VECTOR_SIZE] = {0};


	// 새로운 center 구하기
	for (int clusterIdx = 0; clusterIdx < nowCluster; clusterIdx++)
		for (int blockIdx = 0; blockIdx < image->blockCount; blockIdx++)
		{
			if (cluster->clusterIdx[blockIdx] != clusterIdx)
				continue;
			get_blockVector(image->block, blockIdx, blockVector);
			for (int i = 0; i < VECTOR_SIZE; i++)
				newCenter[clusterIdx][i] += blockVector[i];
		}

	for (int clusterIdx = 0; clusterIdx < nowCluster; clusterIdx++)
		for (int i = 0; i < VECTOR_SIZE; i++)
			newCenter[clusterIdx][i] /= cluster->elementCount[clusterIdx];

	// center의 평균 이동 거리 구하기
	for (int clusterIdx = 0; clusterIdx < nowCluster; clusterIdx++)
		move += get_distance(cluster->center[clusterIdx], newCenter[clusterIdx]);

	// 새로운 center 저장
	for (int clusterIdx = 0; clusterIdx < nowCluster; clusterIdx++)
		for (int i = 0; i < VECTOR_SIZE; i++)
			cluster->center[clusterIdx][i] = newCenter[clusterIdx][i];

	return sqrt(move / (nowCluster * nowCluster)) ;
}

int get_center(Cluster* cluster, Image* image, Int32 elementCount, Int32 clusterIdx)
{
	for (int idx = 0; idx < elementCount; idx++)
		for (int i = 0; i < VECTOR_SIZE; i++)
			cluster->center[clusterIdx][i] += (image->block[idx * VECTOR_SIZE + i] / elementCount);

	return 0;
}

int get_epsilon_vector(Float64* epsilon, Int32 nowCluster)
{
	srand(time(NULL));
	for (int i = 0; i < VECTOR_SIZE; i++)
		epsilon[i] = ((rand() / (Float64) (RAND_MAX)) * 2 - 1) * (100.0 / nowCluster);

	return 0;
}

Float64 get_mse(Cluster* cluster, Image* image, Int32 nowCluster)
{
	Float64 mse = 0;
	Float64 clusterMSE = 0;
	Float64 blockVector[VECTOR_SIZE];

	for (int vecIdx = 0; vecIdx < image->blockCount; vecIdx++)
	{
		get_blockVector(image->block, vecIdx, blockVector);
		mse += get_distance(cluster->center[cluster->clusterIdx[vecIdx]], blockVector);
	}

	return mse /= image->blockCount;
}

Float64 get_distance(Float64* center, Float64* vector)
{
	Float64 distance = 0;
	for (int i = 0; i < VECTOR_SIZE; i++)
		distance += (center[i] - vector[i]) * (center[i] - vector[i]);

	return distance;
}

int destroy_cluster(Cluster* cluster)
{
	for (int i = 0; i < CLUSTER; i++)
	{
		free(cluster->center[i]);
	}
	
	free(cluster->center);
	free(cluster->clusterIdx);
	free(cluster->elementCount);
	
	return 0;
}