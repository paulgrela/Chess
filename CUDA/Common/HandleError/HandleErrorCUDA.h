
#ifndef _CUDA_HANDLE_ERROR_H_
#define _CUDA_HANDLE_ERROR_H_

#include "cuda_runtime.h"

void HandleError(cudaError_t err, const char* file, int line);

#define HANDLE_ERROR(err) (HandleError(err, __FILE__, __LINE__))

#endif
