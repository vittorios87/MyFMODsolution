#pragma once
namespace MySmallRadioApp
{
	namespace singleton {
		template <class T> class Singleton
		{
		private:
			static T* classInstance;
		public:
			static T* GetInstance();

			~Singleton();
		protected:
			Singleton();

		};
	}
}
