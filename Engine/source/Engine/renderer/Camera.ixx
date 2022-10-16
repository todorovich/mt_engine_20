//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple first person style camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be constructed.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
//***************************************************************************************
module;

#include <d3d12.h>
#include <DirectXMath.h>

export module Camera;

import Status;
import std.threading;

import MathHelper;

export namespace mt::camera
{
    class Camera
    {
    public:

        Camera();

        Camera(const Camera &other) = delete;

        ~Camera();

        // Get/Set world camera position.
        DirectX::XMVECTOR GetPosition() const;

        DirectX::XMFLOAT3 GetPosition3f() const;

        mt::Status SetPosition(float x, float y, float z);

        mt::Status SetPosition(const DirectX::XMFLOAT3 &v);

        // Get camera basis vectors.
        DirectX::XMVECTOR GetRight() const;

        DirectX::XMFLOAT3 GetRight3f() const;

        DirectX::XMVECTOR GetUp() const;

        DirectX::XMFLOAT3 GetUp3f() const;

        DirectX::XMVECTOR GetLook() const;

        DirectX::XMFLOAT3 GetLook3f() const;

        // Get frustum properties.
        float GetNearZ() const;

        float GetFarZ() const;

        float GetAspect() const;

        float GetFovY() const;

        float GetFovX() const;

        // Get near and far plane dimensions in view space coordinates.
        float GetNearWindowWidth() const;

        float GetNearWindowHeight() const;

        float GetFarWindowWidth() const;

        float GetFarWindowHeight() const;

        // Set frustum.
        mt::Status SetLens(float fovY, float aspect, float zn, float zf);

        // Define camera space via LookAt parameters.
        mt::Status LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);

        mt::Status LookAt(const DirectX::XMFLOAT3 &pos, const DirectX::XMFLOAT3 &target, const DirectX::XMFLOAT3 &up);

        // Get View/Proj matrices.
        DirectX::XMMATRIX GetView() const;

        DirectX::XMMATRIX GetProj() const;

        DirectX::XMFLOAT4X4 GetView4x4f() const;

        DirectX::XMFLOAT4X4 GetProj4x4f() const;

        // Strafe/Walk the camera a distance d.
        mt::Status Strafe(float d);

        mt::Status Walk(float d);

        // Rotate the camera.
        mt::Status Pitch(float angle);

        mt::Status RotateY(float angle);

        // After modifying camera position/orientation, call to rebuild the view matrix.
        void UpdateViewMatrix();

        void SetDirty()
        {
            _is_view_dirty = true;
        }

    private:

        // 64 bytes
        DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
        DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();

        // 12 bytes
        // Camera coordinate system with coordinates relative to world space.
        DirectX::XMFLOAT3 mPosition = {0.0f, 0.0f, 10.0f};
        DirectX::XMFLOAT3 mRight    = {1.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 mUp       = {0.0f, 1.0f, 0.0f};
        DirectX::XMFLOAT3 mLook     = {0.0f, 0.0f, 1.0f};

        // 4 bytes
        // Cache frustum properties.
        float _frustum_near_z            = 0.0f;
        float _frustum_far_z             = 0.0f;
        float _frustum_aspect_ratio      = 0.0f;
        float _frustum_fov_y             = 0.0f;
        float frustum_near_window_height = 0.0f;
        float _frustum_far_window_height = 0.0f;

        // 1 byte
        bool _is_view_dirty;

        std::mutex _camera_mutex;
    };
}

module: private;

using namespace mt::camera;
using namespace DirectX;
using namespace mt;

Camera::Camera()
	: _is_view_dirty(false)
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
	LookAt(mPosition, XMFLOAT3(0.0f, 0.0f, 0.0f), mUp);
}

Camera::~Camera()
{
}

XMVECTOR Camera::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}


XMFLOAT3 Camera::GetPosition3f()const
{
	return mPosition;
}

Status Camera::SetPosition(float x, float y, float z)
{
	std::scoped_lock lock(_camera_mutex);
	mPosition = XMFLOAT3(x, y, z);
	_is_view_dirty = true;

	return Status::success;
}

Status Camera::SetPosition(const XMFLOAT3& v)
{
	std::scoped_lock lock(_camera_mutex);
	mPosition = v;
	_is_view_dirty = true;

	return Status::success;
}

XMVECTOR Camera::GetRight()const
{

	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Camera::GetRight3f()const
{

	return mRight;
}

XMVECTOR Camera::GetUp()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Camera::GetUp3f()const
{
	return mUp;
}

XMVECTOR Camera::GetLook()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Camera::GetLook3f()const
{
	return mLook;
}

float Camera::GetNearZ()const
{
	return _frustum_near_z;
}

float Camera::GetFarZ()const
{
	return _frustum_far_z;
}

float Camera::GetAspect()const
{
	return _frustum_aspect_ratio;
}

float Camera::GetFovY()const
{
	return _frustum_fov_y;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / _frustum_near_z);
}

float Camera::GetNearWindowWidth()const
{
	return _frustum_aspect_ratio * frustum_near_window_height;
}

float Camera::GetNearWindowHeight()const
{
	return frustum_near_window_height;
}

float Camera::GetFarWindowWidth()const
{
	return _frustum_aspect_ratio * _frustum_far_window_height;
}

float Camera::GetFarWindowHeight()const
{
	return _frustum_far_window_height;
}

Status Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	std::scoped_lock lock(_camera_mutex);
	// cache properties
	_frustum_fov_y = fovY;
	_frustum_aspect_ratio = aspect;
	_frustum_near_z = zn;
	_frustum_far_z = zf;

	frustum_near_window_height = 2.0f * _frustum_near_z * tanf(0.5f * _frustum_fov_y);
	_frustum_far_window_height = 2.0f * _frustum_far_z * tanf(0.5f * _frustum_fov_y);

	XMMATRIX P = XMMatrixPerspectiveFovLH(_frustum_fov_y, _frustum_aspect_ratio, _frustum_near_z, _frustum_far_z);
	XMStoreFloat4x4(&mProj, P);

	_is_view_dirty = true;

	return Status::success;
}

Status Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	std::scoped_lock lock(_camera_mutex);
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	_is_view_dirty = true;

	return Status::success;
}


Status Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	return LookAt(P, T, U);;
}


XMMATRIX Camera::GetView()const
{
	assert(!_is_view_dirty);
	return XMLoadFloat4x4(&mView);
}

XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&mProj);
}


XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!_is_view_dirty);
	return mView;
}

XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return mProj;
}

Status Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

	_is_view_dirty = true;

	return Status::success;
}

Status Camera::Walk(float d)
{
	std::scoped_lock lock(_camera_mutex);
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	_is_view_dirty = true;

	return Status::success;
}

Status Camera::Pitch(float angle)
{
	std::scoped_lock lock(_camera_mutex);
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	_is_view_dirty = true;

	return Status::success;
}

Status Camera::RotateY(float angle)
{
	std::scoped_lock lock(_camera_mutex);
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	_is_view_dirty = true;

	return Status::success;
}

void Camera::UpdateViewMatrix()
{
	if (_is_view_dirty)
	{
		std::scoped_lock lock(_camera_mutex);

		XMVECTOR R = XMLoadFloat3(&mRight);
		XMVECTOR U = XMLoadFloat3(&mUp);
		XMVECTOR L = XMLoadFloat3(&mLook);
		XMVECTOR P = XMLoadFloat3(&mPosition);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&mRight, R);
		XMStoreFloat3(&mUp, U);
		XMStoreFloat3(&mLook, L);

		mView(0, 0) = mRight.x;
		mView(1, 0) = mRight.y;
		mView(2, 0) = mRight.z;
		mView(3, 0) = x;

		mView(0, 1) = mUp.x;
		mView(1, 1) = mUp.y;
		mView(2, 1) = mUp.z;
		mView(3, 1) = y;

		mView(0, 2) = mLook.x;
		mView(1, 2) = mLook.y;
		mView(2, 2) = mLook.z;
		mView(3, 2) = z;

		mView(0, 3) = 0.0f;
		mView(1, 3) = 0.0f;
		mView(2, 3) = 0.0f;
		mView(3, 3) = 1.0f;

		_is_view_dirty = false;
	}
}


