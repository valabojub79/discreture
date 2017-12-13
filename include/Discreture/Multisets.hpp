#pragma once
#include "VectorHelpers.hpp"
#include "Misc.hpp"

namespace dscr
{
template<class IntType>
class basic_multisets
{
public:
	using difference_type = long long;
	using size_type = long long;
	using value_type = std::vector<IntType>;
	using multiset = value_type;
private:
	std::vector<IntType> m_total;
public:

	////////////////////////////////////////////////////////////
	/// \brief class of all submultiset of a given set, expressed as incidence std::vectors with multiplicities
	/// \param IntType can be an int, uint, etc. It can be signed or unsigned (the negatives are not used)
	/// # Example:
	///
	///	 multisets X({1,0,3,1});
	///		for (const auto& x : X)
	///			std::cout << x << " ";
	///
	/// Prints out:
	///
	/// 	[ 0 0 0 0 ]
	///		[ 1 0 0 0 ]
	///		[ 0 0 1 0 ]
	///		[ 1 0 1 0 ]
	///		[ 0 0 2 0 ]
	///		[ 1 0 2 0 ]
	///		[ 0 0 3 0 ]
	///		[ 1 0 3 0 ]
	///		[ 0 0 0 1 ]
	///		[ 1 0 0 1 ]
	///		[ 0 0 1 1 ]
	///		[ 1 0 1 1 ]
	///		[ 0 0 2 1 ]
	///		[ 1 0 2 1 ]
	///		[ 0 0 3 1 ]
	///		[ 1 0 3 1 ]
	///
	/// TODO: Make it a random-access class and more like the others. It's not hard.
	///
	////////////////////////////////////////////////////////////
	basic_multisets(const std::vector<IntType>& set) : m_total(set), m_begin(m_total), m_end(m_total)
	{
		m_end.m_atend = true;
	}



	basic_multisets(IntType size, IntType n = 1) : m_total(size, n), m_begin(m_total), m_end(m_total)
	{
		m_end.m_atend = true;
	}

	size_type size() const
	{
		size_type toReturn = 1;

		for (auto x : m_total)
			toReturn *= (x + 1);

		return toReturn;
	}

	class iterator : public std::iterator<std::forward_iterator_tag, std::vector<IntType>>
	{
	private:
		std::vector<IntType> m_submulti;
		bool m_atend;
		const std::vector<IntType>& m_total;
	public:
		//iterator() : m_submulti(), m_atend(false), m_total(nullptr) {} //empty initializer

		iterator(const std::vector<IntType>& total) : m_submulti(total.size(), 0), m_atend(false), m_total(total)
		{
			//how it starts
		}


		//prefix
		inline iterator& operator++()
		{
			for (size_t i = 0; i < m_submulti.size(); ++i)
			{
				if (m_submulti[i] + 1 <= m_total[i])
				{
					++m_submulti[i];

					return *this;
				}
				m_submulti[i] = 0;
			}

			m_atend = true;
			return *this;
		}

		const std::vector<IntType>& operator*() const
		{
			return m_submulti;
		}

		std::vector<IntType>& operator*()
		{
			return m_submulti;
		}

		bool operator==(const iterator& it) const
		{
// 				std::cout << "should not run!" << endl;
			if (m_submulti.size() != it.m_submulti.size())
				return false;

			for (size_t i = 0; i < m_submulti.size(); ++i)
			{
				if (m_submulti[i] != (*it)[i])
					return false;
			}

			return true;
		}

		bool operator!=(const iterator& it) const
		{
			if (it.m_atend)
			{
				if (m_atend == true)
					return false;

				return true;
			}

			return !((*this) == it);
		}

		friend class basic_multisets;
	};

	const iterator& begin() const
	{
		return m_begin;
	}

	const iterator& end() const
	{
		return m_end;
	}

private:
	iterator m_begin;
	iterator m_end;

};
using multisets = basic_multisets<int>;

// 	template <class IntType>
// 	class MultisetIterator
// 	{
// 	public:
// 		MultisetIterator(const std::vector<IntType>& set) : m_total(set), m_size(set.size()) { }
//
// 		std::vector<IntType> First()
// 		{
// 			return std::vector<IntType>(m_size,0);
// 		}
//
// 		bool Next(std::vector<IntType>& subset)
// 		{
// 			for (nuint i = 0; i < m_size; ++i)
// 			{
// 				if (subset[i] < m_total[i])
// 				{
// 					++subset[i];
// 					for (nuint j = 0; j < i; ++j)
// 					{
// 						subset[j] = 0;
// 					}
// 					return true;
// 				}
// 			}
//
// 			return false;
// 		}
// 	private:
// 		std::vector<IntType> m_total;
// 		nuint m_size;
// 	};
//
// 	template <class IntType>
// 	class MultisetIteratorDecreasingOrder
// 	{
// 		public:
// 		MultisetIteratorDecreasingOrder(const std::vector<IntType>& total) : m_total(total), m_numElementsInSubset(0), m_size(total.size()), m_first(true)
// 		{
// 		}
//
// 		std::vector<IntType> First()
// 		{
// 			return m_total;
// 		}
//
// 		std::vector<IntType> First(nuint numelements)
// 		{
// 			std::vector<IntType> W(m_size,0);
// 			ConcentrateToEnd(W,m_size-1,numelements);
// 			return W;
// 		}
//
// 		int Next(std::vector<IntType>& subset)
// 		{
// 			if (m_first)
// 			{
// 				m_numElementsInSubset = Sum(subset);
// 				m_first = false;
// 			}
//
// 			bool capacityToReceiveFound = false;
// 			nuint availableElements = 1;
// 			for (nuint i = 0 ; i < m_size; ++i)
// 			{
// 				if (!capacityToReceiveFound)
// 				{
// 					if (subset[i] < m_total[i])
// 						capacityToReceiveFound = true;
// 					availableElements += nuint(subset[i]);
// 					continue;
// 				}
//
// 				if (subset[i] == 0)
// 					continue;
//
// 				// We are at a case where subset[i] != 0, and there is someone before with capacity to receive.
//
// 				--subset[i];
//
// 				//std::cout << endl << "Before concentrate = " << subset << endl;
// 				//std::cout << "i-1 = " << i-1 << endl;
// 				//std::cout << "available = " << int(availableElements) << endl;
// 				ConcentrateToEnd(subset, i-1, availableElements);
//
// 				//std::cout << "After concentrate = " << subset << endl;
//
// 				return m_numElementsInSubset;
// 			}
//
// 			//std::cout << "Finished for loop, we are at a limit case! " << endl;
// 			// If we are at the limit case, all filled then all empty,
// 			--m_numElementsInSubset;
//
// 			if (m_numElementsInSubset < 0)
// 				return -1;
//
// 			ConcentrateToEnd(subset, m_size-1, m_numElementsInSubset);
//
// 			//std::cout << "Next subset will be = " << m_current << endl;
// 			return m_numElementsInSubset;
// 		}
// 	private:
// 		inline void ConcentrateToEnd(std::vector<IntType>& subset, int where, nuint available)
// 		{
// 			for (int i = where; i >= 0; --i)
// 			{
// 				subset[i] = IntType(std::min(available, nuint(m_total[i])));
// 				available -= subset[i];
// 			}
// 		}
// 		std::vector<IntType> m_total;
// 		int m_numElementsInSubset;
// 		nuint m_size;
// 		bool m_first;
// 	};
}
