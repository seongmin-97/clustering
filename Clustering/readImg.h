#pragma once
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "type.h"

Image read_file();
int Img2Block(Image* image);
int get_block(Float64* data, Int32 imgIdx, Int32 topLeftX, Int32 topLeftY, Float64* block);
int destroy_image(Image* image);