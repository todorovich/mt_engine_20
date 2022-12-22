export module Status;

export namespace mt
{
	enum class Status
	{
		incomplete = -2,
		failure = -1,
		negative = false, // 0
		affirmative = true,  // 1
		success = 2,
	};
}