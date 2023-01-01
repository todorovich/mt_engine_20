module;

#include <DirectXMath.h>
#include <cstdlib>

export module MathUtility;

export namespace mt::math
{
	constexpr DirectX::XMFLOAT4X4 Identity4x4()
	{
		return DirectX::XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	float randomFloat(float low, float high)
	{
		return low + static_cast<float>(rand() / static_cast<float>(RAND_MAX/(high-low)));
	}
}