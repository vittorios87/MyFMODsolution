#include "pch.h"
#include "Singleton.h"

namespace MySmallRadioApp
{
	namespace singleton {

		template <class T> T*  Singleton<T>::classInstance;
		template <class T> T* Singleton<T>::GetInstance()
		{
			if (classInstance == nullptr)
			{
				classInstance = new T();
			}

			return classInstance;
		}
		template <class T> Singleton<T>::Singleton()
		{
		}

		template <class T> Singleton<T>::~Singleton()
		{
		}
	}
}