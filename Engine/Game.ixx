export module Game;

export namespace mt
{
	class Game 
	{
	protected:
		// Probably pre and post?
		virtual void physicsUpdate() noexcept {};
		virtual void inputUpdate() noexcept {};
		virtual void renderUpdate() noexcept {};

	public:

		Game() = default;
		virtual ~Game() = default;

		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;

		friend class Engine;
	};
}