#pragma once

#include <cstdint>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

#ifndef MY_LOCKER_MUTEX
#define MY_LOCKER_MUTEX         std::unique_lock<std::mutex> locker(g_lock);
#define MY_LOCKER_MUTEX_UNLOCK  locker.unlock();
#define MY_LOCKER_MUTEX_LOCK    locker.lock();
#endif

template <class T>
class SingletonPattern
{
	SingletonPattern(const T& src) = delete;
	T& operator=(const T& rhs) = delete;

protected:
	static std::unique_ptr<T>       ptr_instance;
	static std::mutex               g_lock;
	static std::condition_variable  g_check;
	static std::atomic<bool>        was_init;
	SingletonPattern() = default;
	virtual ~SingletonPattern() = default;

public:
	static T& getInstance() noexcept
	{
		if (ptr_instance == nullptr)
			ptr_instance.reset(new T);

		return *ptr_instance;
	}
};

// init of static properties
template <class T>
std::unique_ptr<T>      SingletonPattern<T>::ptr_instance{ nullptr };

template <class T>
std::mutex              SingletonPattern<T>::g_lock;

template <class T>
std::condition_variable SingletonPattern<T>::g_check;

template <class T>
std::atomic<bool>       SingletonPattern<T>::was_init{ false };