export module Game;

export namespace mt
{
	class Game 
	{
	protected:
		// Probably pre and post?
		virtual void physicsUpdate() {};
		virtual void inputUpdate() {};
		virtual void renderUpdate() {};

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