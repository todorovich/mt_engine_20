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

	public:
		RendererInterface(float horizontal_fov_degrees = 90.0f, float aspect_ratio = 16.0f/9.0f) noexcept
			: _camera(horizontal_fov_degrees, aspect_ratio)
		{

		}

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


		[[nodiscard]] virtual std::expected<void, Error> set4xMsaaState(bool value) noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> onResize() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> render() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> initialize() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> shutdown() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> update() noexcept = 0;
	};
}