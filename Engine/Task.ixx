export module Task;

export import <expected>;

export import Error;

export namespace mt
{
	class Task {
	public:
		virtual std::expected<void, mt::Error> operator()() = 0;
	};
}