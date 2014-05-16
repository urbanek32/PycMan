#ifndef TSINGLETON_HPP
#define TSINGLETON_HPP

template <class T>
class TSingleton
{
protected:
	static T *m_pSelfInstance;

public:
	virtual ~TSingleton()
	{

	}

	static T *GetInstance()
	{
		if (!m_pSelfInstance)
		{
			m_pSelfInstance = new T();
		}
		return m_pSelfInstance;
	}

	static T& GetReference()
	{
		T *pointer = GetInstance();
		return (*pointer);
	}

	static void DestroyInstance()
	{
		if (m_pSelfInstance)
		{
			delete m_pSelfInstance;
			m_pSelfInstance = 0;
		}
	}
};

template<class T>
T* TSingleton<T>::m_pSelfInstance = 0;

#endif // TSINGLETON_HPP