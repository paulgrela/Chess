#pragma once

#ifndef _CHESS_MULTI_LOCK_GUARD_H_
#define _CHESS_MULTI_LOCK_GUARD_H_

#include <vector>
#include <memory>
#include <mutex>

class MultiLockGuard
{
private:
	std::vector<std::unique_ptr<std::mutex>>* MutexObjects;
public:
	MultiLockGuard(std::vector<std::unique_ptr<std::mutex>>* MutexObjectsParameter)
	{
		MutexObjects = MutexObjectsParameter;

		for (auto& MutexObject : *MutexObjects)
			MutexObject->lock();
	}

	~MultiLockGuard()
	{
		for (auto& MutexObject : *MutexObjects)
			MutexObject->unlock();
	}
};

#endif
