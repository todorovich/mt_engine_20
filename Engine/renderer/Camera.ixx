// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>

export module Camera;

import <mutex>;

export import MathUtility;

export namespace mt::renderer
{
    class Camera
    {
		DirectX::XMFLOAT4X4 _view = mt::math::Identity4x4();
		DirectX::XMFLOAT4X4 _projection = mt::math::Identity4x4();

		// Camera coordinate system with coordinates relative to world space.
		DirectX::XMFLOAT3 _position = { 0.0f, 0.0f, 10.0f };
		DirectX::XMFLOAT3 _right = { 1.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 _up = { 0.0f, 1.0f, 0.0f };
		DirectX::XMFLOAT3 _look = { 0.0f, 0.0f, 1.0f };

		// Cache frustum properties.
		float _frustum_near_z = 0.0f;
		float _frustum_far_z = 0.0f;
		float _frustum_aspect_ratio = 0.0f;
		float _frustum_fov_y = 0.0f;
		float _frustum_near_window_height = 0.0f;
		float _frustum_far_window_height = 0.0f;

		std::mutex _camera_mutex;

		bool _view_matrix_requires_update;

    public:

        Camera() noexcept;
        ~Camera() noexcept;
		Camera(const Camera& other) noexcept = delete;
		Camera(Camera&& other) noexcept = default;
		Camera& operator=(const Camera& other) noexcept = delete;
		Camera& operator=(Camera&& other) noexcept = default;

        // Get/Set world camera position.
        DirectX::XMVECTOR getPosition() const noexcept;

        DirectX::XMFLOAT3 getPosition3f() const noexcept;

        void setPosition(float x, float y, float z) noexcept;

        void setPosition(const DirectX::XMFLOAT3 &v) noexcept;

        // Get camera basis vectors.
        DirectX::XMVECTOR getRightVector() const noexcept;

        DirectX::XMFLOAT3 getRightFloats() const noexcept;

        DirectX::XMVECTOR getUpVector() const noexcept;

        DirectX::XMFLOAT3 getUpFloats() const noexcept;

        DirectX::XMVECTOR getLookVector() const noexcept;

        DirectX::XMFLOAT3 getLookFloats() const noexcept;

        // Get frustum properties.
        float getNearZ() const noexcept;

        float getFarZ() const noexcept;

        float getAspectRatio() const noexcept;

        float getFovY() const noexcept;

        float getFovX() const noexcept;

        // Get near and far plane dimensions in view space coordinates.
        float getNearWindowWidth() const noexcept;

        float getNearWindowHeight() const noexcept;

        float getFarWindowWidth() const noexcept;

        float getFarWindowHeight() const noexcept;

        // Set frustum.
        void setLens(float fovY, float aspect, float zn, float zf) noexcept;

        // Define camera space via lookAt parameters.
        void lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp) noexcept;

        void lookAt(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &target, const DirectX::XMFLOAT3 &up) noexcept;

        // Get View/Proj matrices.
        DirectX::XMMATRIX getViewMatrix() const noexcept;

        DirectX::XMMATRIX getProjectionMatrix() const noexcept;

        DirectX::XMFLOAT4X4 getViewFloats() const noexcept;

        DirectX::XMFLOAT4X4 getProjectionFloats() const noexcept;

        // strafe/walk the camera a distance d.
        void strafe(float d) noexcept;

        void walk(float d) noexcept;

        void fly(float d) noexcept;

        // Rotate the camera.
        void pitch(float angle) noexcept;

        void rotateY(float angle) noexcept;

        // After modifying camera position/orientation, call to rebuild the view matrix.
        void updateViewMatrix() noexcept;

        void viewMatrixRequiresUpdate() noexcept { _view_matrix_requires_update = true; }
    };
}
