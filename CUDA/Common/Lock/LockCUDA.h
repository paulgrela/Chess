
#ifndef _CUDA_LOCK_H_
#define _CUDA_LOCK_H_

#include "cuda_runtime.h"
#include "HandleErrorCUDA.h"

struct LockCUDA 
{
    int *mutex;

	LockCUDA()
	{
        HANDLE_ERROR(cudaMalloc((void**)&mutex, sizeof(int)));
        HANDLE_ERROR(cudaMemset(mutex, 0, sizeof(int)));
    }

    ~LockCUDA()
	{
        cudaFree(mutex);
    }

    __device__ void Lock() 
	{
        while(atomicCAS(mutex, 0, 1) != 0);
    }

    __device__ void Unlock() 
	{
        atomicExch(mutex, 0);
    }
};

#endif
