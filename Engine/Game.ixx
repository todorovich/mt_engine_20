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

		Game() noexcept = default;
		virtual ~Game() noexcept  = default;

		Game(const Game& other) noexcept = delete;
		Game(Game&& other) noexcept = delete;
		Game& operator=(const Game& other) noexcept = delete;
		Game& operator=(Game&& other) noexcept = delete;

		friend class Engine;
	};
}