#pragma once

#include"type.h"

Float64 get_pixel(Float64* data, Int32 imgIdx, Int32 x, Int32 y);
int get_blockVector(Float64* block, Int32 blockIdx, Float64* vector);
Int32 get_blockIdx(Int32 imgIdx, Int32 topLeftX, Int32 topLeftY);