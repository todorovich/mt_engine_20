// Copyright 2023 Micho Todorovich, all rights reserved.
export module RendererInterface;

export import <expected>;

export import Camera;
export import Error;

using mt::error::Error;
using mt::renderer::model::Camera;

export namespace mt::renderer
{
	class RendererInterface
	{
		Camera _camera; // 204 bytes (getting kind of bloated)

		long long _frames_rendered = 0;

		const static unsigned int _swap_chain_buffer_count = 2;
		int _window_width = 0;
		int _window_height = 0;

		float _window_aspect_ratio = 0.0f;

		bool _4x_msaa_state = false;
		bool _is_initialized = false;
		bool _is_rendering = false;

	protected:

		void _set4xMsaaState(bool state) noexcept
		{
			_4x_msaa_state = state;
		}

		void _incrementFramesRendered(long long increment_amount = 1l) noexcept
		{
			_frames_rendered += increment_amount;
		}

		void _setIsInitialized(bool is_initialized = true) noexcept
		{
			_is_initialized = is_initialized;
		}

		void _setIsRendering(bool is_rendering = true) noexcept
		{
			_is_rendering = is_rendering;
		}

		void _setWindowAspectRatio(float aspect_ratio) noexcept
		{

			_window_aspect_ratio = aspect_ratio;
		}

		void _setWindowWidth(int width) noexcept
		{
			_window_width = width;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
		}

		void _setWindowHeight(int height) noexcept
		{
			_window_height = height;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
		}

		RendererInterface() noexcept = default;

	public:
		virtual ~RendererInterface() noexcept = default;
		RendererInterface(const RendererInterface&) noexcept = delete;
		RendererInterface(RendererInterface&&) noexcept = default;
		RendererInterface& operator=(const RendererInterface&) noexcept = delete;
		RendererInterface& operator=(RendererInterface&&) noexcept = default;

		bool get4xMsaaState() const noexcept { return _4x_msaa_state; };

		// TODO make a camera interface? Or is the camera renderer agnostic?
		Camera& getCurrentCamera() noexcept { return _camera; } // NOT CONST!!!!

		long long getFramesRendered() const noexcept { return _frames_rendered; }

		bool getIsInitialized() const noexcept { return _is_initialized; };

		bool getIsRendering() const noexcept { return _is_rendering; }

		static constexpr unsigned int getSwapChainBufferCount() { return _swap_chain_buffer_count; };

		float getWindowAspectRatio() const noexcept { return _window_aspect_ratio; }

		// TODO: doesn't this belong in the window manager?
		int getWindowWidth() const noexcept { return _window_width; }

		int getWindowHeight() const noexcept { return _window_height; }

		[[nodiscard]] virtual std::expected<void, Error> set4xMsaaState(bool value) noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> resize(int client_width, int client_height) noexcept
		{
			_window_width = client_width;
			_window_height = client_height;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
			return {};
		};

		[[nodiscard]] virtual std::expected<void, Error> render() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> initialize() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> shutdown() noexcept = 0;

		virtual void update() = 0;
	};
}