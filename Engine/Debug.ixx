export module Debug;

export namespace mt
{
#if defined(DEBUG) || defined(_DEBUG)
	constexpr bool DEBUG = true;
#else
	constexpr bool DEBUG = false;
#endif
}