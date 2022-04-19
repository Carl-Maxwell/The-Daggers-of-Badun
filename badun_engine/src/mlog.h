#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
public:
	std::shared_ptr<spdlog::logger> m_Logger;

	static Log& the() {
		static Log* my_log = nullptr;
		if (!my_log) {
			my_log = new Log;
		}
		return *my_log;
	}
	Log()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_Logger = spdlog::stdout_color_mt("Daggers of Badun");
		m_Logger->set_level(spdlog::level::trace);
	}

	template<typename... Args>
	static inline void white(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		the().m_Logger->trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static inline void green(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		the().m_Logger->info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static inline void yellow(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		the().m_Logger->warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static inline void red(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		the().m_Logger->error(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static inline void redback(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		the().m_Logger->critical(fmt, std::forward<Args>(args)...);
		// red background, white text, fairly hard to read
	}

	template<typename... Args>
	static inline void log(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		green(fmt, std::forward<Args>(args)...);
		// TODO randomly choose a color?
	}
};
