#pragma once
#include <functional>
#include <type_traits>

class TryEx {
public:
	template<typename T>
	struct Rsp {
		T v;
		std::string ExMsg;
	};

	template<typename Func, typename ...Args >
	static TryEx::Rsp<typename std::result_of<Func(Args...)>::type> Run(Func&& func, Args&& ...args);

protected:

	template<typename T,typename Func, typename... Args>
	static TryEx::Rsp<T> createFunction(Func& f, Args && ... args);

	template<typename T>
	static void SEHTryRun(std::function<T()> &m_func, TryEx::Rsp<T> &rsp);

	template<typename T>
	static void CppTryRun(std::function<T()> &m_func, TryEx::Rsp<T> &rsp);

};

template<typename Func, typename ...Args>
inline TryEx::Rsp<typename std::result_of<Func(Args...)>::type> TryEx::Run(Func && func, Args && ...args)
{
	typedef typename std::result_of<Func(Args...)>::type FunRspType;
	Rsp<FunRspType> rsp = createFunction<FunRspType>(std::forward<Func>(func), std::forward<Args>(args)...);

	return rsp;
}

template<typename T, typename Func, typename ...Args>
inline TryEx::Rsp<T> TryEx::createFunction(Func & f, Args && ...args) {
	Rsp<T> rsp;

	std::function<T()>m_func = [&f, &args...]{ return f(args...); };

#ifdef _MSC_VER
	SEHTryRun(m_func, rsp);
#else
	CppTryRun(m_func, rsp);
#endif // _MSC_VER

	return rsp;
}

template<typename T>
inline void TryEx::SEHTryRun(std::function<T()>& m_func, TryEx::Rsp<T>& rsp) {
#ifdef _MSC_VER
	__try
	{
#endif // _MSC_VER


		CppTryRun(m_func, rsp);


#ifdef _MSC_VER
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rsp.ExMsg = "SEH exception unknown";
	}
#endif // _MSC_VER
}

template<typename T>
inline void TryEx::CppTryRun(std::function<T()>& m_func, TryEx::Rsp<T>& rsp) {
	try {
		rsp.v = m_func();
	}
	catch (CException* e)
	{
		char szMsg[MAX_PATH] = { 0 }; //MAX_PATH:260
		e->GetErrorMessage(szMsg, MAX_PATH);
		rsp.ExMsg = "CException:";
		rsp.ExMsg += szMsg;
	}
	catch (std::exception& e)
	{
		rsp.ExMsg = "std::exception:";
		rsp.ExMsg += e.what();
	}
	catch (...)
	{
		rsp.ExMsg = "exception unknown";
	}
	return;
}
