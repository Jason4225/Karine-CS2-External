#pragma once

struct ViewMatrix_t
{
	float* operator[](int idx)
	{
		return matrix[idx];
	}

	float matrix[4][4];
};