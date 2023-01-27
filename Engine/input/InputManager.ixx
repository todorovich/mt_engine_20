// Copyright 2023 Micho Todorovich, all rights reserved.
export module InputManager;

export import <map>;
export import <set>;
export import <variant>;

export import InputModel;
export import ObjectPool;

// Todo: Relative Mouse Position and locking mouse.
export namespace mt::input
{
    class InputManager
    {
        bool _isMouseRelative = false;

	protected:
		mt::Engine& _engine;

		virtual void processInput() noexcept = 0; // friend engine, make protected?

		void setIsMouseRelative(bool isMouseRelative = true)
		{
			_isMouseRelative = isMouseRelative;
		}

    public:
		friend mt::Engine;

        InputManager(mt::Engine& engine) noexcept
            : _engine(engine)
        {}

        virtual ~InputManager() noexcept = default;

        InputManager(const InputManager &other) noexcept = delete;
        
        InputManager(InputManager&& other) noexcept = default;

        InputManager& operator=(const InputManager &other) noexcept = delete;

        InputManager& operator=(InputManager&& other) noexcept = default;

		bool getIsMouseRelative() const { return _isMouseRelative; }

        virtual void acceptInput(
            InputType input_type,
			std::variant<std::monostate, InputData1D, InputData2D, InputData3D> data = std::monostate()
        ) noexcept = 0;

		virtual void toggleRelativeMouse() noexcept  = 0;

        virtual void registerInputHandler(InputHandler input_handler, InputType input_types) noexcept = 0;

		// Templated functions for processing varargs of input types, so that more than one trigger may be associated
		// with each InputHandler
        template <typename First>
        void registerInputHandler(InputHandler input_handler, const First& first) noexcept
        {
            registerInputHandler(input_handler, first);
        }

        template <typename First, typename... Rest> 
        void registerInputHandler(InputHandler input_handler, const First& first, const Rest&... rest) noexcept
        {
            registerInputHandler(input_handler, first);
            registerInputHandler(input_handler, rest...); // recursive call using pack expansion syntax
        }
    };
}