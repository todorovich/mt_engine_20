export module Constants;

export namespace mt
{
#if defined(DEBUG) || defined(_DEBUG)
	constexpr bool IS_DEBUG = true;
#else
	constexpr bool IS_DEBUG = false;
#endif
}