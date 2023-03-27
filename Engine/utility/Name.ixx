module;

#include "xxhash.h"

export module Name;

export import <string>;
export import <string_view>;

export namespace mt::utility
{
	struct Name
	{
		XXH64_hash_t hash{};

		// TODO: try to make this fully constexpr.
		explicit Name(std::wstring string) noexcept
			: hash(XXH64(string.c_str(), string.size() * sizeof(std::wstring::value_type), 47))
		{};

		constexpr Name(const Name&) noexcept = default;
		constexpr Name(Name&&) noexcept = default;
		Name& operator=(const Name&) noexcept = default;
		Name& operator=(Name&&) noexcept = default;
		constexpr auto operator<=>(const Name&) const noexcept = default;
	};
}