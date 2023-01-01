module;

#include <DirectXMath.h>

export module Camera;

import <mutex>;

export import MathUtility;
export import Status;

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

        Camera();
        ~Camera();
		Camera(const Camera& other) = delete;
		Camera(Camera&& other) = default;
		Camera& operator=(const Camera& other) = delete;
		Camera& operator=(Camera&& other) = default;


        // Get/Set world camera position.
        DirectX::XMVECTOR getPosition() const;

        DirectX::XMFLOAT3 getPosition3f() const;

        mt::Status setPosition(float x, float y, float z);

        mt::Status setPosition(const DirectX::XMFLOAT3 &v);

        // Get camera basis vectors.
        DirectX::XMVECTOR getRightVector() const;

        DirectX::XMFLOAT3 getRightFloats() const;

        DirectX::XMVECTOR getUpVector() const;

        DirectX::XMFLOAT3 getUpFloats() const;

        DirectX::XMVECTOR getLookVector() const;

        DirectX::XMFLOAT3 getLookFloats() const;

        // Get frustum properties.
        float getNearZ() const;

        float getFarZ() const;

        float getAspectRatio() const;

        float getFovY() const;

        float getFovX() const;

        // Get near and far plane dimensions in view space coordinates.
        float getNearWindowWidth() const;

        float getNearWindowHeight() const;

        float getFarWindowWidth() const;

        float getFarWindowHeight() const;

        // Set frustum.
        mt::Status setLens(float fovY, float aspect, float zn, float zf);

        // Define camera space via lookAt parameters.
        mt::Status lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);

        mt::Status lookAt(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &target, const DirectX::XMFLOAT3 &up);

        // Get View/Proj matrices.
        DirectX::XMMATRIX getViewMatrix() const;

        DirectX::XMMATRIX getProjectionMatrix() const;

        DirectX::XMFLOAT4X4 getViewFloats() const;

        DirectX::XMFLOAT4X4 getProjectionFloats() const;

        // strafe/walk the camera a distance d.
        mt::Status strafe(float d);

        mt::Status walk(float d);

        Status fly(float d);

        // Rotate the camera.
        mt::Status pitch(float angle);

        mt::Status rotateY(float angle);

        // After modifying camera position/orientation, call to rebuild the view matrix.
        void updateViewMatrix();

        void viewMatrixRequiresUpdate() { _view_matrix_requires_update = true; }
    };
}
