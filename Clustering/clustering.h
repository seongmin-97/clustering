#pragma once
#include"type.h"

int clustering(Image* image);
Cluster initalize_cluster(Image* image, Int32 blockCount);
int devide_cluster(Cluster* cluster, Int32 nowCluster);
int relocate_cluster(Cluster* cluster, Image* image, Int32 nowCluster);
Float64 relocate_center(Cluster* cluster, Image* image, Int32 nowCluster);
int get_center(Cluster* cluster, Image* image, Int32 elementCount, Int32 clusterIdx);
int get_epsilon_vector(Float64* epsilon, Int32 nowCluster);
Float64 get_mse(Cluster* cluster, Image* image, Int32 nowCluster);
Float64 get_distance(Float64* center, Float64* vector);
int destroy_cluster(Cluster* cluster);