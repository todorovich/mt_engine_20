// Copyright 2023 Micho Todorovich, all rights reserved.
export module Game;

export namespace mt
{
	class Game 
	{
	public:

		Game() noexcept = default;
		virtual ~Game() noexcept  = default;

		Game(const Game& other) noexcept = delete;
		Game(Game&& other) noexcept = delete;
		Game& operator=(const Game& other) noexcept = delete;
		Game& operator=(Game&& other) noexcept = delete;

		virtual void physicsUpdate() noexcept {};
		virtual void inputUpdate() noexcept {};
		virtual void renderUpdate() noexcept {};

		friend class Engine;
	};
}