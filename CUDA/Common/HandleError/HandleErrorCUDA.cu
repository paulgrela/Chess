
#ifdef WINDOWS_PLATFORM
#include "Windows.h"
#endif

#include "HandleErrorCUDA.h"

#include "stdio.h"

void HandleError(cudaError_t err, const char* file, int line)
{
	if (err != cudaSuccess)
	{
		printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
		cudaDeviceReset();
		exit(EXIT_FAILURE);
	}
}

#define HANDLE_ERROR(err) (HandleError(err, __FILE__, __LINE__))

