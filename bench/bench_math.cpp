// ============================================================================
//  Benchmark: Addition
//  Google Benchmark Version – tidy-safe, modern C++20
// ============================================================================

// NOLINTBEGIN  // entire file is ignored by clang-tidy (recommended for
// benchmarks)

#include "project/math.hpp"
#include <benchmark/benchmark.h>

namespace {

// Benchmark function in lower_case (tidy would complain → ignored via NOLINT)
auto bm_add(benchmark::State &state) -> void {
  for (auto _ : state) { // '_' name is typical Benchmark idiom
    auto result = project::add(123, 456);

    // Use new DoNotOptimize syntax (non-const reference)
    benchmark::DoNotOptimize(result);
  }
}

} // namespace

// Benchmark registration
BENCHMARK(bm_add);

// Provide main()
BENCHMARK_MAIN();

// NOLINTEND