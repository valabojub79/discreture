#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include "do_not_optimize.hpp"
#include "TimeHelpers.hpp"
#include "external/rang.hpp"

template <class Func>
double Benchmark(Func f, int numtimes = 8)
{
	f(); //warm up
	dscr::Chronometer C;
	for (int i = 0; i < numtimes; ++i)
	{
		f();
	}
	double avg = C.Reset()/numtimes;
	
	return avg;
}

template <class Container>
double FWIterationBenchmark(const Container& A)
{
	return Benchmark([&A]()
	{
		for (auto& a : A)
		{
			DoNotOptimize(a);
		}
	});
	
}

template <class Container>
double ReverseIterationBenchmark(const Container& A)
{
	return Benchmark([&A]()
	{
		for (auto it = A.rbegin(); it != A.rend(); ++it)
		{
			DoNotOptimize(*it);
		}
	});
}

template <class Container>
double ForEachBenchmark(const Container& A)
{
	return Benchmark([&A]()
	{
		A.for_each([](const auto& a)
		{
			DoNotOptimize(a);
		});
	});
}
