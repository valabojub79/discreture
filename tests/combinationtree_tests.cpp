#include <gtest/gtest.h>
#include <iostream>

#include "CombinationsTree.hpp"

using namespace std;
using namespace dscr;

template <class Combination>
void check_combination_tree(const Combination& x, int n, int k)
{
	ASSERT_TRUE(std::is_sorted(x.begin(),x.end()));
	ASSERT_EQ(x.size(), k);
	if (k > 0)
	{
		ASSERT_TRUE(x.front() >= 0);
		ASSERT_TRUE(x.back() < n);
	}
}

void check_combination_tree_index(const combinations_tree& X, const combinations_tree::combination& x, int i)
{
	ASSERT_EQ(i,X.get_index(x));
	ASSERT_EQ(x,X[i]);
}



TEST(CombinationsTree,ForwardIteration)
{
	
	for (int n = 0; n < 10; ++n)
	{
		long total = 0;
		for (int k = 0; k <= n+1; ++k) //even k+1
		{
			combinations_tree X(n, k);
			
			set<combinations_tree::combination> S(X.begin(),X.end());
			ASSERT_EQ(X.size(), S.size()); //This checks everything is different.
			
			long i = 0;

			for (const auto& x : X)
			{
				check_combination_tree(x,n,k);
				check_combination_tree_index(X,x,i);
				++i;
				++total;
			}
		}
		ASSERT_EQ(total, 1<<n);
	}
}

TEST(CombinationsTree,ReverseIteration)
{
	for (int n = 0; n < 10; ++n)
	{
		long total = 0;
		for (int k = 0; k <= n+1; ++k) //even k+1
		{
			combinations_tree X(n, k);
			long i = 0;

			for (auto it = X.rbegin(); it != X.rend(); ++it)
			{
				auto x = *it;
				check_combination_tree(x,n,k);
				
				check_combination_tree_index(X,x,X.size()-i-1);
				++i;
				++total;
			}
		}
		ASSERT_EQ(total, 1<<n);
	}
}

TEST(CombinationsTree,RandomAccess)
{
	//combined test
	int n = 10;
	int k = 5;
	combinations_tree X(n,k);
	int i = 25;
	auto it = X.begin()+i;
	
	do
	{
		check_combination_tree(*it,n,k);
		check_combination_tree_index(X,*it,i);
		++it;
		++i;
	} while (it != X.end());
}



TEST(CombinationsTree,Bidirectional)
{
	long n = 38L;
	long k = 16L;
	long r = 10000000000L;
	
	combinations_tree X(n,k);
	ASSERT_EQ(X.size(),22239974430LL);
	
	auto t = X.begin()+r;
	
	auto s = t;
	
	++t;
	--t;
	check_combination_tree(*t,n,k);
	ASSERT_EQ(*t,*s);
	
	t += 500;
	--t;
	t -= 499;
	
	check_combination_tree(*t,n,k);
	ASSERT_EQ(*t,*s);
	
	t -= 250;
	++t;
	++t;
	s -= 248;
	check_combination_tree(*t,n,k);
	ASSERT_EQ(*t,*s);
}

TEST(CombinationsTree,ForEach)
{
	for (int n = 0; n < 20; ++n)
	{
		for (int k = 0; k <= n; ++k)
		{
			combinations_tree X(n,k);
			auto it = X.begin();
			X.for_each([&it](const combinations_tree::combination& x)
			{
				ASSERT_EQ(x,*it);
				++it;
			});
		}
	}
}

TEST(CombinationsTree,CorrectOrder)
{
	for (int n = 0; n < 10; ++n)
	{
		for (int k = 0; k <= n; ++k)
		{
			combinations_tree X(n,k);
			for (auto it = X.begin(); it != X.end(); ++it)
			{
				auto itnext = it + 1;

				if (itnext == X.end())
					break;

				ASSERT_TRUE(X.compare(*it, *itnext));
			}
		}
	}
}

TEST(CombinationsTree,EdgeCases)
{
	basic_combinations_tree<unsigned char> Z(5, 8);

	for (const auto& z : Z)
	{
		check_combination_tree(z,5,8);
		ASSERT_TRUE(false);
	}
}

TEST(CombinationsTree,FindIf)
{
	combinations_tree W(20, 6);

	auto predicate1 = [](const combinations_tree::combination& comb) -> bool
	{
		for (size_t i = 0; i < comb.size() - 1; ++i)
		{
			if (2 * comb[i] + 1 > comb[i + 1])
				return false;
		}

		return true;
	};
	
	auto predicate2 = [](const combinations_tree::combination& comb) -> bool
	{
		for (size_t i = 0; i < comb.size() - 1; ++i)
		{
			if (comb[i] + 3 < comb[i + 1])
				return false;

			if (comb[i] + 1 == comb[i + 1])
				return false;
		}

		return true;
	};

	auto iter = W.find_if(predicate1);

	if (iter != W.end())
	{
		ASSERT_TRUE(predicate1(*iter));
	}
	else
	{
		// verify that indeed no combination satisfies the predicate
		if (W.get_n() < 30)
		{
			for (const auto& w : W)
			{
				ASSERT_FALSE(predicate1(w));
			}
		}
	}

// 	return true;

	auto T = W.find_all(predicate2);
	
	size_t numpred2 = 0;
	for (auto& t : T)
	{
		check_combination_tree(t,W.get_n(), W.get_k());
		++numpred2;
	}
	
	

	if (W.get_n() < 30)
	{
		size_t numpred = 0;
		for (const auto& w : W)
		{
			if (predicate2(w))
			{
				++numpred;
			}
		}
		ASSERT_EQ(numpred,numpred2);
	}
}


TEST(CombinationsTree, PartitionPoint)
{
	int n = 60;
	int k = 30;
	combinations_tree X(n,k);
	auto comb = *std::partition_point(X.begin(), X.end(), [](const auto& x)
	{
		return x.front() < 26;
	});
	
	// comb should be == to {26,27,...,55}
	
	ASSERT_EQ(comb.back(), 55);
	ASSERT_EQ(comb.front(), 26);
	check_combination_tree(comb, n, k);
	
	auto rcomb = *std::partition_point(X.rbegin(), X.rend(), 
	[](const auto& x)
	{
		return x.front() > 0;
	});
	
	//rcomb should be {0, 31,... , 59}
	
	check_combination_tree(rcomb,n,k);
	ASSERT_EQ(rcomb[0], 0);
	ASSERT_EQ(rcomb[1], 31);
	ASSERT_EQ(rcomb.back(), 59);
	
}
