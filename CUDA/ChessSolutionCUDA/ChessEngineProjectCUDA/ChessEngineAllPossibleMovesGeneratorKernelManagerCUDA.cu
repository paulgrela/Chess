
#include "DestinationPlatform.h"

#include "TerminalColorsUtils.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "HandleErrorCUDA.h"

#include "DateTimeUtils.h"
#include "ExceptionsMacro.h"

#include "ChessEngineThreadsData.h"

#include "ChessEngineMainThreadsManager.h"

#include "ChessEngineAllPossibleMovesGeneratorKernelCUDA.h"
#include "ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA.h"

using namespace std;

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::PrintSizesOfStartDataforCUDA()
{
	try
	{
		LoggersManagerObject.Log(STREAM("NUMBER OF ELEMENTS OF ARRAY TO SEND TO CUDA = " << ChessEngineThreadsData::ChessEngineThreadsDataArraySize << EndLineChar));
		LoggersManagerObject.Log(STREAM("SIZE OF ARRAY TO SEND TO CUDA = " << ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessEngineThreadDataCUDA) << EndLineChar));
		LoggersManagerObject.Log(STREAM("NUMBER OF GPU CUDA USED MEMORY = " << ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessEngineThreadDataCUDA) + 3 * ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathsCUDA) + 2 * ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathCUDA) << EndLineChar));
	}
	CATCH("printing sizes of start data for cuda")
}

#ifdef WINDOWS_PLATFORM
void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::PrintDurationTimesOfCUDAKernelAlgorithmExecution(const float msecTotal1, const float msecTotal2, const float msecTotal3, const chrono::time_point<chrono::steady_clock>& start_time, const chrono::time_point<chrono::steady_clock>& stop_time)
#endif
#ifdef UNIX_PLATFORM
void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::PrintDurationTimesOfCUDAKernelAlgorithmExecution(const float msecTotal1, const float msecTotal2, const float msecTotal3, const chrono::system_clock::time_point& start_time, const chrono::system_clock::time_point& stop_time)
#endif
{
	try
	{
		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("TimeCudaEventGPU 1 = " << msecTotal1 << " msec"));
		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("TimeCudaEventGPU 2 = " << msecTotal2 << " msec"));
		LoggersManagerObject.LogInColorTerminal(terminal_colors_utils::blue, STREAM("TimeCudaEventGPU 3 = " << msecTotal3 << " msec"));

		PrintDurationTimeAllFormats(start_time, stop_time, "Execution of adding found paths to vectors of mates paths has taken time: ", "executing printing duration_time");
	}
	CATCH("printing duration times of cuda kernel algorithm execution")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::CopyConstantValuesToCUDA()
{
	try
	{
		HANDLE_ERROR(cudaMemcpyToSymbol(ReverseColorOfPiecesCUDA, &ChessEngineConfigurationFileReaderWriterObject.ReverseColorOfPieces, sizeof(BoolTypeCUDA)));

		LengthTypeCUDA MaxDeepLevelLocal[1] = { static_cast<LengthTypeCUDA>(ChessEngineConfigurationFileReaderWriterObject.MaxDeepLevel) };		
		HANDLE_ERROR(cudaMemcpyToSymbol(MaxDeepLevelConstantCUDA, MaxDeepLevelLocal, sizeof(LengthTypeCUDA)));

		HANDLE_ERROR(cudaMemcpyToSymbol(PiecePowerCUDA, ChessEngineConfigurationFileReaderWriterObject.PiecePower, NumberOfPieces * sizeof(PathScoreTypeCUDA)));

		LoggersManagerObject.Log(STREAM("DeepLevel = " << ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[0].DeepLevel << " MaxDeepLevel = " << MaxDeepLevelLocal[0] << EndLineChar));
	}
	CATCH("copying constant values to cuda")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::GetMemoryForThreadsDataArray()
{
	try
	{
		ChessEngineThreadsData::ChessEngineThreadsDataArraySize = 0;
		HANDLE_ERROR(cudaHostAlloc((void**)&ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer, MaxNumberOfChessBoardsForFastExecution * sizeof(ChessEngineThreadDataCUDA), cudaHostAllocDefault));
	}
	CATCH("getting memory for threads data array")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::FreeMemoryForThreadsDataArray()
{
	try
	{
		HANDLE_ERROR(cudaFreeHost(ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer));
	}
	CATCH("freeing memory for threads data array")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::GetMemoryForChessEngineKernelDataCUDA()
{
	try
	{
		HANDLE_ERROR(cudaMalloc((void**)&ThreadStartDataCUDAArrayDevicePointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessEngineThreadDataCUDA)));
		HANDLE_ERROR(cudaHostAlloc((void**)&AllFoundMovesPathsToMateWhitePointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathsCUDA), cudaHostAllocDefault));
		HANDLE_ERROR(cudaHostAlloc((void**)&AllFoundMovesPathsToMateBlackPointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathsCUDA), cudaHostAllocDefault));
		HANDLE_ERROR(cudaHostAlloc((void**)&AllFoundMovesPathsToStalematePointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathsCUDA), cudaHostAllocDefault));
		HANDLE_ERROR(cudaHostAlloc((void**)&TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathCUDA), cudaHostAllocDefault));
		HANDLE_ERROR(cudaHostAlloc((void**)&TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessScoredMovesPathCUDA), cudaHostAllocDefault));
	}
	CATCH("getting memory for chess state analyzer kernel data CUDA")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::FreeMemoryForChessEngineKernelDataCUDA()
{
	try
	{
		HANDLE_ERROR(cudaFree(ThreadStartDataCUDAArrayDevicePointer));
		HANDLE_ERROR(cudaFreeHost(AllFoundMovesPathsToMateWhitePointer));
		HANDLE_ERROR(cudaFreeHost(AllFoundMovesPathsToMateBlackPointer));
		HANDLE_ERROR(cudaFreeHost(AllFoundMovesPathsToStalematePointer));
		HANDLE_ERROR(cudaFreeHost(TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer));
		HANDLE_ERROR(cudaFreeHost(TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer));
	}
	CATCH("freeing memory for chess state analyzer kernel data CUDA")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::CopyThreadStartDataCUDAArrayToDevice()
{
	try
	{
		for (uint64_t ThreadStartDataCUDAArrayElementIndex = 0; ThreadStartDataCUDAArrayElementIndex < ChessEngineThreadsData::ChessEngineThreadsDataArraySize; ThreadStartDataCUDAArrayElementIndex++)
		{
			ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[ThreadStartDataCUDAArrayElementIndex].AllFoundMovesPathsToMatePointer[White] = &AllFoundMovesPathsToMateWhitePointer[ThreadStartDataCUDAArrayElementIndex];
			ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[ThreadStartDataCUDAArrayElementIndex].AllFoundMovesPathsToMatePointer[Black] = &AllFoundMovesPathsToMateBlackPointer[ThreadStartDataCUDAArrayElementIndex];
			ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[ThreadStartDataCUDAArrayElementIndex].AllFoundMovesPathsToStalematePointer = &AllFoundMovesPathsToStalematePointer[ThreadStartDataCUDAArrayElementIndex];
			ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[ThreadStartDataCUDAArrayElementIndex].TheHighestScoredFoundCaptureMovesPathsPointer.WhitePath = &TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer[ThreadStartDataCUDAArrayElementIndex];
			ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer[ThreadStartDataCUDAArrayElementIndex].TheHighestScoredFoundCaptureMovesPathsPointer.BlackPath = &TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer[ThreadStartDataCUDAArrayElementIndex];
		}
		HANDLE_ERROR(cudaMemcpy(ThreadStartDataCUDAArrayDevicePointer, ChessEngineThreadsData::ChessEngineThreadsDataArrayPointer, ChessEngineThreadsData::ChessEngineThreadsDataArraySize * sizeof(ChessEngineThreadDataCUDA), cudaMemcpyHostToDevice));
	}
	CATCH("copying thread start data cuda array to device")
}


void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::CopyChessScoredFoundCaptureMovesPathsCUDAToStatisticsFoundMovesPaths(ChessScoredMovesPathCUDA* TheHighestScoredFoundCaptureMovesPathPointer, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreTypeCUDA x, const PathScoreTypeCUDA y))
{
	try
	{
		for (uint64_t ThreadIndex = 0; ThreadIndex < ChessEngineThreadsData::ChessEngineThreadsDataArraySize; ThreadIndex++)
			if (Comparison(TheHighestScoredFoundCaptureMovesPathPointer[ThreadIndex].Score, ChessScoredMovesPathObject.Score))
				CopyChessScoredMovesPathCUDAObjectToChessScoredMovesPathObject(ChessScoredMovesPathObject, TheHighestScoredFoundCaptureMovesPathPointer[ThreadIndex]);
	}
	CATCH("copying chess scored found capture moves paths cuda to statistics found moves paths")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::CopyChessScoredMovesPathsCUDAToStatisticsFoundMovesPaths(ChessScoredMovesPathsCUDA* const AllFoundMovesPathsToMatePointer, std::vector<ChessScoredMovesPath>& AllFoundMovesPaths, ChessScoredMovesPath& ChessScoredMovesPathObject, bool (*Comparison)(const PathScoreTypeCUDA x, const PathScoreTypeCUDA y))
{
	try
	{
		AllFoundMovesPaths.reserve(AllFoundMovesPaths.size() + ChessEngineThreadsData::ChessEngineThreadsDataArraySize);
		for (uint64_t ThreadIndex = 0; ThreadIndex < ChessEngineThreadsData::ChessEngineThreadsDataArraySize; ThreadIndex++)
			for (uint64_t ChessPathIndex = 0; ChessPathIndex < AllFoundMovesPathsToMatePointer[ThreadIndex].NumberOfFoundMovesPaths; ChessPathIndex++)
			{
				AllFoundMovesPaths.emplace_back(ChessScoredMovesPath(AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].Score, AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].TypeSum, AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].MovesPathLength, AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].MovesPath));

				if (Comparison(AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].Score, ChessScoredMovesPathObject.Score) || (ChessScoredMovesPathObject.MovesPath.empty() == true && AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex].MovesPathLength > 0))
					CopyChessScoredMovesPathCUDAObjectToChessScoredMovesPathObject(ChessScoredMovesPathObject, AllFoundMovesPathsToMatePointer[ThreadIndex].MovesPaths[ChessPathIndex]);
			}
	}
	CATCH("copying chess scored moves paths cuda to statistics found moves paths")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::CopyChessScoredMovesPathsCUDAToStatisticsMovesPaths(ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorObject)
{
	try
	{
		CopyChessScoredMovesPathsCUDAToStatisticsFoundMovesPaths(AllFoundMovesPathsToMateWhitePointer, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[White], ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[Black], [](const PathScoreTypeCUDA Score1, const PathScoreTypeCUDA Score2) { return Score1 > Score2; });
		CopyChessScoredMovesPathsCUDAToStatisticsFoundMovesPaths(AllFoundMovesPathsToMateBlackPointer, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.AllFoundMovesPathsToMate[Black], ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.TheHighestScoredFoundMateMovesPathForAdvantageOf[White], [](const PathScoreTypeCUDA Score1, const PathScoreTypeCUDA Score2) { return Score1 < Score2; });
		CopyChessScoredMovesPathsCUDAToStatisticsFoundMovesPaths(AllFoundMovesPathsToStalematePointer, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.AllFoundMovesPathsToStalemate, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.TheHighestScoredFoundStalemateMovesPath, [](const PathScoreTypeCUDA Score1, const PathScoreTypeCUDA Score2) { return Score1 < Score2; });
		CopyChessScoredFoundCaptureMovesPathsCUDAToStatisticsFoundMovesPaths(TheHighestScoredFoundCaptureMovesPathForAdvantageOfBlackPointer, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[Black], [](const PathScoreTypeCUDA Score1, const PathScoreTypeCUDA Score2) { return Score1 < Score2; });
		CopyChessScoredFoundCaptureMovesPathsCUDAToStatisticsFoundMovesPaths(TheHighestScoredFoundCaptureMovesPathForAdvantageOfWhitePointer, ChessEngineAllPossibleMovesGeneratorObject.ChessEngineResultsStatisticsObject.TheHighestScoredFoundCaptureMovesPathForAdvantageOf[White], [](const PathScoreTypeCUDA Score1, const PathScoreTypeCUDA Score2) { return Score1 > Score2; });
	}
	CATCH("copying chess scored moves paths cuda to statistics moves paths")
}

void ChessEngineAllPossibleMovesGeneratorKernelManagerCUDA::PrepareDataForKernelAndExecuteKernelCUDA()
{
	try
	{
		if (ChessEngineThreadsData::ChessEngineThreadsDataArraySize > 0)
		{
			PrintSizesOfStartDataforCUDA();

			CopyConstantValuesToCUDA();

			cudaEvent_t StartCudaEvent1;
			HANDLE_ERROR(cudaEventCreate(&StartCudaEvent1));
			cudaEvent_t StopCudaEvent1;
			HANDLE_ERROR(cudaEventCreate(&StopCudaEvent1));

			cudaEvent_t StartCudaEvent2;
			HANDLE_ERROR(cudaEventCreate(&StartCudaEvent2));
			cudaEvent_t StopCudaEvent2;
			HANDLE_ERROR(cudaEventCreate(&StopCudaEvent2));

			cudaEvent_t StartCudaEvent3;
			HANDLE_ERROR(cudaEventCreate(&StartCudaEvent3));
			cudaEvent_t StopCudaEvent3;
			HANDLE_ERROR(cudaEventCreate(&StopCudaEvent3));

			GetMemoryForChessEngineKernelDataCUDA();
			HANDLE_ERROR(cudaEventRecord(StartCudaEvent1, NULL));
			//GetMemoryForChessEngineKernelDataCUDA();
			CopyThreadStartDataCUDAArrayToDevice();
			HANDLE_ERROR(cudaEventRecord(StopCudaEvent1, NULL));
			HANDLE_ERROR(cudaEventSynchronize(StopCudaEvent1));


			dim3 blocks(1024, 1024);
			dim3 threads(16, 16);
			HANDLE_ERROR(cudaEventRecord(StartCudaEvent3, NULL));

			ChessEngineAllPossibleMovesGeneratorKernelCUDA<<<blocks, threads>>>(ThreadStartDataCUDAArrayDevicePointer, static_cast<unsigned int>(ChessEngineThreadsData::ChessEngineThreadsDataArraySize));
			
			HANDLE_ERROR(cudaEventRecord(StopCudaEvent3, NULL));
			HANDLE_ERROR(cudaEventSynchronize(StopCudaEvent3));


			HANDLE_ERROR(cudaEventRecord(StartCudaEvent2, NULL));
			const auto start_time_2 = chrono::high_resolution_clock::now();
			CopyChessScoredMovesPathsCUDAToStatisticsMovesPaths(ChessEngineMainThreadsManagerLocalReference.ChessEngineAllPossibleMovesGeneratorObjects[0]);
			const auto stop_time_2 = chrono::high_resolution_clock::now();
			HANDLE_ERROR(cudaEventRecord(StopCudaEvent2, NULL));
			HANDLE_ERROR(cudaEventSynchronize(StopCudaEvent2));

			float msecTotal1 = 0.0f;
			HANDLE_ERROR(cudaEventElapsedTime(&msecTotal1, StartCudaEvent1, StopCudaEvent1));
			float msecTotal2 = 0.0f;
			HANDLE_ERROR(cudaEventElapsedTime(&msecTotal2, StartCudaEvent2, StopCudaEvent2));
			float msecTotal3 = 0.0f;
			HANDLE_ERROR(cudaEventElapsedTime(&msecTotal3, StartCudaEvent3, StopCudaEvent3));

			FreeMemoryForChessEngineKernelDataCUDA();

			PrintDurationTimesOfCUDAKernelAlgorithmExecution(msecTotal1, msecTotal2, msecTotal3, start_time_2, stop_time_2);
		}
	}
	CATCH("executing cuda kernel algorithm")
}
