// Copyright 2023 Micho Todorovich, all rights reserved.
export module Task;

export import <expected>;

export import Error;

export namespace mt::task
{
	class Task {
	public:
		virtual std::expected<void, mt::error::Error> operator()() = 0;
	};
}