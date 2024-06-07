#ifndef UNI_AIZO_P_AIZO_TOOL_TIMER_HPP
#define UNI_AIZO_P_AIZO_TOOL_TIMER_HPP

#include <chrono>

namespace aizo::tool {

/**
 * @brief Timer class to measure the time taken by a function.
 * @headerfile aizo_tool_timer.hpp
 *
 * @tparam Func Function type.
 * @tparam Clock Clock type.
 *
 * @note Func should be callable
 *
 * @attention Clock must satisfy C++ named requirement Clock.
 */
template< typename Func, typename Clock = std::chrono::high_resolution_clock >
requires std::chrono::is_clock_v< Clock >
class Timer {
public:
  /**
   * @brief Duration unit for the timer.
   * @note NO_UNIT is used to indicate that the duration has not been measured.
   */
  enum class DurationUnit : std::int_fast8_t {
    NO_UNIT = -1,
    NANOSECONDS,
    MICROSECONDS,
    MILLISECONDS,
    SECONDS,
    MINUTES,
    HOURS
  };

  /**
   * @brief Construct a new Timer object. Does not measure yet.
   * @param func Function to measure.
   */
  explicit Timer(Func func):
    func{ func }, duration{ 0.0, DurationUnit::NO_UNIT } {
  }

  /**
   * @brief Measure the time taken by the function.
   * @tparam Args Argument types.
   * @param args Arguments to pass to the function.
   * @return Result of the function.
   */
  template< typename... Args >
  std::invoke_result_t< Func, Args... > operator()(Args&&... args) {
    using nano  = std::chrono::duration< double, std::nano >;
    using micro = std::chrono::duration< double, std::micro >;
    using milli = std::chrono::duration< double, std::milli >;
    using sec   = std::chrono::duration< double >;
    using min   = std::chrono::duration< double, std::ratio< 60 > >;
    using hour  = std::chrono::duration< double, std::ratio< 3600 > >;

    // Measure time taken by the function. Convert to appropriate unit.
    if constexpr (std::is_void_v< std::invoke_result_t< Func, Args... > >) {
      const auto start = Clock::now();
      func(std::forward< Args >(args)...);
      const auto end     = Clock::now();
      const auto elapsed = end - start;

      duration = { milli{ elapsed }.count(), DurationUnit::MILLISECONDS };

    } else {
      const auto start   = Clock::now();
      const auto result  = func(std::forward< Args >(args)...);
      const auto end     = Clock::now();
      const auto elapsed = end - start;

      duration = { milli{ elapsed }.count(), DurationUnit::MILLISECONDS };

      return result;
    }
  }

  /**
   * @brief Get the duration of the last measured function.
   * @return Duration and unit enum.
   */
  [[nodiscard]] std::pair< double, DurationUnit > getDuration() const {
    return duration;
  }

  /**
   * @brief Get the duration of the last measured function.
   * @return Duration and unit string.
   */
  [[nodiscard]] std::pair< double, std::string_view > getDurationStr() const {
    const auto [time, unit] = duration;

    switch (unit) {
    case DurationUnit::NO_UNIT:
      return { time, "No unit" };
    case DurationUnit::NANOSECONDS:
      return { time, "ns" };
    case DurationUnit::MICROSECONDS:
      return { time, "us" };
    case DurationUnit::MILLISECONDS:
      return { time, "ms" };
    case DurationUnit::SECONDS:
      return { time, "s" };
    case DurationUnit::MINUTES:
      return { time, "m" };
    case DurationUnit::HOURS:
      return { time, "h" };
    }

    return { time, "No unit" };
  }

private:
  Func                              func{ nullptr };
  std::pair< double, DurationUnit > duration{ 0.0, DurationUnit::NO_UNIT };
};

} // namespace aizo::tool

#endif // UNI_AIZO_P_AIZO_TOOL_TIMER_HPP
