// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <d3d12.h>
#include <DirectXMath.h>

module Camera;

import std;

using namespace mt::renderer::model;
using namespace DirectX;
using namespace mt;
using namespace std::numbers;

// TODO: camera move is effected by fov. Make that not true;
Camera::Camera(float horizontal_fov_degrees, float aspect_ratio) noexcept
	: _view_matrix_requires_update(false)
{
	auto x_fov_radians = horizontal_fov_degrees * pi_v<float> / 180.0f;
	auto y_fov_radian = x_fov_radians / aspect_ratio;
	auto near_z = 1.0f;
	auto far_z = 1000.f;
	setLens(y_fov_radian, aspect_ratio, near_z, far_z);
	lookAt(_position, XMFLOAT3(0.0f, 0.0f, 0.0f), _up);
}

Camera::~Camera() noexcept
{
}

XMVECTOR Camera::getPosition() const noexcept
{
	return XMLoadFloat3(&_position);
}

XMFLOAT3 Camera::getPosition3f() const noexcept
{
	return _position;
}

void Camera::setPosition(float x, float y, float z) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	_position = XMFLOAT3(x, y, z);
	_view_matrix_requires_update = true;
}

void Camera::setPosition(const XMFLOAT3& v) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	_position = v;
	_view_matrix_requires_update = true;
}

XMVECTOR Camera::getRightVector() const noexcept
{
	return XMLoadFloat3(&_right);
}

XMFLOAT3 Camera::getRightFloats() const noexcept
{
	return _right;
}

XMVECTOR Camera::getUpVector() const noexcept
{
	return XMLoadFloat3(&_up);
}

XMFLOAT3 Camera::getUpFloats() const noexcept
{
	return _up;
}

XMVECTOR Camera::getLookVector() const noexcept
{
	return XMLoadFloat3(&_look);
}

XMFLOAT3 Camera::getLookFloats() const noexcept
{
	return _look;
}

float Camera::getNearZ() const noexcept
{
	return _frustum_near_z;
}

float Camera::getFarZ() const noexcept
{
	return _frustum_far_z;
}

float Camera::getAspectRatio() const noexcept
{
	return _frustum_aspect_ratio;
}

float Camera::getFovY() const noexcept
{
	return _frustum_fov_y;
}

float Camera::getFovX() const noexcept
{
	float halfWidth = 0.5f * getNearWindowWidth();
	return 2.0f * atanf(halfWidth / _frustum_near_z);
}

float Camera::getNearWindowWidth() const noexcept
{
	return _frustum_aspect_ratio * _frustum_near_window_height;
}

float Camera::getNearWindowHeight() const noexcept
{
	return _frustum_near_window_height;
}

float Camera::getFarWindowWidth() const noexcept
{
	return _frustum_aspect_ratio * _frustum_far_window_height;
}

float Camera::getFarWindowHeight() const noexcept
{
	return _frustum_far_window_height;
}

void Camera::setLens(float fovY, float aspect, float zn, float zf) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	// cache properties
	_frustum_fov_y = fovY;
	_frustum_aspect_ratio = aspect;
	_frustum_near_z = zn;
	_frustum_far_z = zf;

	_frustum_near_window_height = 2.0f * _frustum_near_z * tanf(0.5f * _frustum_fov_y);
	_frustum_far_window_height = 2.0f * _frustum_far_z * tanf(0.5f * _frustum_fov_y);

	XMMATRIX P = XMMatrixPerspectiveFovLH(_frustum_fov_y, _frustum_aspect_ratio, _frustum_near_z, _frustum_far_z);
	XMStoreFloat4x4(&_projection, P);

	_view_matrix_requires_update = true;
}

void Camera::lookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up) noexcept
{
	lookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::lookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&_position, pos);
	XMStoreFloat3(&_look, L);
	XMStoreFloat3(&_right, R);
	XMStoreFloat3(&_up, U);

	_view_matrix_requires_update = true;
}

XMMATRIX Camera::getViewMatrix() const noexcept
{
	assert(!_view_matrix_requires_update);
	return XMLoadFloat4x4(&_view);
}

XMMATRIX Camera::getProjectionMatrix() const noexcept
{
	return XMLoadFloat4x4(&_projection);
}

XMFLOAT4X4 Camera::getViewFloats() const noexcept
{
	assert(!_view_matrix_requires_update);
	return _view;
}

XMFLOAT4X4 Camera::getProjectionFloats() const noexcept
{
	return _projection;
}

void Camera::strafe(float d) noexcept
{
	std::scoped_lock lock(_camera_mutex);

	XMVECTOR scalar = XMVectorReplicate(d);
	XMVECTOR right = XMLoadFloat3(&_right);
	XMVECTOR position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(scalar, right, position));

	_view_matrix_requires_update = true;
}

void Camera::walk(float d) noexcept
{
	static int i = 0;
	
	std::scoped_lock lock(_camera_mutex);

	XMVECTOR scalar = XMVectorReplicate(d);
	XMVECTOR look = XMLoadFloat3(&_look);
	XMVECTOR position = XMLoadFloat3(&_position);

	//OutputDebugStringW((std::to_wstring(++i) + L'\n' + std::to_wstring(_position.x) + L' ' + std::to_wstring(_position.y) + L' ' + std::to_wstring(_position.z)).c_str());

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(scalar, look, position));

	_view_matrix_requires_update = true;
}

void Camera::fly(float d) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	
	XMVECTOR scalar = XMVectorReplicate(d);
	XMVECTOR up = XMLoadFloat3(&_up);
	XMVECTOR position = XMLoadFloat3(&_position);

	XMStoreFloat3(&_position, XMVectorMultiplyAdd(scalar, up, position));

	_view_matrix_requires_update = true;
}

void Camera::pitch(float angle) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	// Rotate up and look vector about the right vector.

	XMMATRIX new_right_vector = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), new_right_vector));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), new_right_vector));

	_view_matrix_requires_update = true;
}

void Camera::rotateY(float angle) noexcept
{
	std::scoped_lock lock(_camera_mutex);
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), R));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), R));

	_view_matrix_requires_update = true;
}

void Camera::updateViewMatrix() noexcept
{
	if (_view_matrix_requires_update)
	{
		std::scoped_lock lock(_camera_mutex);

		XMVECTOR right = XMLoadFloat3(&_right);
		XMVECTOR up = XMLoadFloat3(&_up);
		XMVECTOR look = XMLoadFloat3(&_look);
		XMVECTOR position = XMLoadFloat3(&_position);

		// Keep camera'scalar axes orthogonal to each other and of unit length.
		look = XMVector3Normalize(look);
		up = XMVector3Normalize(XMVector3Cross(look, right));

		// up, look already ortho-normal, so no need to normalize cross product.
		right = XMVector3Cross(up, look);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(position, right));
		float y = -XMVectorGetX(XMVector3Dot(position, up));
		float z = -XMVectorGetX(XMVector3Dot(position, look));

		XMStoreFloat3(&_right, right);
		XMStoreFloat3(&_up, up);
		XMStoreFloat3(&_look, look);

		_view(0, 0) = _right.x;
		_view(1, 0) = _right.y;
		_view(2, 0) = _right.z;
		_view(3, 0) = x;

		_view(0, 1) = _up.x;
		_view(1, 1) = _up.y;
		_view(2, 1) = _up.z;
		_view(3, 1) = y;

		_view(0, 2) = _look.x;
		_view(1, 2) = _look.y;
		_view(2, 2) = _look.z;
		_view(3, 2) = z;

		_view(0, 3) = 0.0f;
		_view(1, 3) = 0.0f;
		_view(2, 3) = 0.0f;
		_view(3, 3) = 1.0f;

		_view_matrix_requires_update = false;
	}
}
