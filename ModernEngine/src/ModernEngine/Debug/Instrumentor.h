#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace ModernEngine {

	struct ProfileResult
	{
		std::string Name;
		std::chrono::duration<float, std::micro> Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id threadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor 
	{
	public:
		Instrumentor()
			:m_CurrentSession(nullptr)
		{

		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);

			if (m_CurrentSession)
			{
				// If there is a current session when the begin session function gets called, 
				// then close the current before starting the new one.

				// The logger might be initialized before the session
				if (Log::GetCoreLogger())
					MN_CORE_ERROR("Begin Session {0} called when there is already session {1}", name, m_CurrentSession->Name);

				InternalEndSession();
			}

			m_OutputStream.open(filepath);
			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else if (Log::GetCoreLogger())
				MN_CORE_ERROR("Could not open the results file {0}", filepath);
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << "{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ",";
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);

			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}

			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		InstrumentationSession* m_CurrentSession;
		std::mutex m_Mutex;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_TimeStartingPoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			std::chrono::time_point<std::chrono::steady_clock> TimeEndingPoint = std::chrono::steady_clock::now();
			auto start = std::chrono::duration<float, std::micro>{m_TimeStartingPoint.time_since_epoch()};
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(TimeEndingPoint).time_since_epoch()- 
							   std::chrono::time_point_cast<std::chrono::microseconds>(m_TimeStartingPoint).time_since_epoch();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name , start, elapsedTime, std::this_thread::get_id()});
			m_Stopped = true;
		}

	private:
		const char* m_Name;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_TimeStartingPoint;
	};

	namespace InstrumentorUtils
	{
		template<size_t N>

	}
}

#define MN_PROFILE 1
#if MN_PROFILE 
// MN_FUNC_SIG was created instead of FUNC_SIG, because FUNC_SIG may not be available for all compilers.
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define MN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define MN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
	#define MN_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define MN_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define MN_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define MN_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define MN_FUNC_SIG __func__
#else
	#define MN_FUNC_SIG "MN_FUNC_SIG unknown!"
#endif
	#define MN_PROFILE_BEGIN_SESSION(name, filepath)	::ModernEngine::Instrumentor::Get().BeginSession(name, filepath)
	#define MN_PROFILE_END_SESSION()	::ModernEngine::Instrumentor::Get().EndSession()
	#define MN_PROFILE_SCOPE(name)	::ModernEngine::InstrumentationTimer timer##__LINE__(name)
	#define MN_PROFILE_FUNCTION()	MN_PROFILE_SCOPE(MN_FUNC_SIG)
#else
	#define MN_PROFILE_BEGIN_SESSION(name)
	#define MN_PROFILE_END_SESSION()
	#define MN_PROFILE_SCOPE(name)	
	#define MN_PROFILE_FUNCTION()	
#endif
