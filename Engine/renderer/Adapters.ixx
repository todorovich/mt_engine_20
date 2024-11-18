// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <wrl.h>

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
//#include <DirectXCollision.h>

export module Adapters;

import std;
import Windows;

using namespace windows;
using Microsoft::WRL::ComPtr;

export namespace mt::renderer 
{
	void logAdapters(const IDXGIFactory4 * const _dxgi_factory, const DXGI_FORMAT& _back_buffer_format);

	void logAdapterOutputs(const IDXGIAdapter * const  adapter, const DXGI_FORMAT& _back_buffer_format);
	
	void logOutputDisplayModes(const IDXGIOutput * const  output, DXGI_FORMAT format);

	void logAdapters(IDXGIFactory4 * const _dxgi_factory, const DXGI_FORMAT& _back_buffer_format)
	{
		IDXGIAdapter* adapter = nullptr;
		std::vector<IDXGIAdapter*> adapterList;
		for (UINT i = 0; _dxgi_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			std::wstring text = L"***Adapter: ";
			text += desc.Description;
			text += L"\n";

			OutputDebugStringW(text.c_str());

			adapterList.push_back(adapter);
		}

		for (size_t i = 0; i < adapterList.size(); ++i)
		{
			logAdapterOutputs(adapterList[i], _back_buffer_format);
			adapterList[i]->Release();
			adapterList[i] = nullptr;
		}
	}

	void logAdapterOutputs(IDXGIAdapter * const adapter, const DXGI_FORMAT& _back_buffer_format)
	{
		UINT i = 0;
		IDXGIOutput* output = nullptr;
		while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC desc;
			output->GetDesc(&desc);

			std::wstring text = L"***Output: ";
			text += desc.DeviceName;
			text += L"\n";
			OutputDebugStringW(text.c_str());

			logOutputDisplayModes(output, _back_buffer_format);

			output->Release();
			output = nullptr;

			++i;
		}
	}

	void logOutputDisplayModes(IDXGIOutput * const output, DXGI_FORMAT format)
	{
		UINT count = 0;
		UINT flags = 0;

		// Call with nullptr to get list count.
		output->GetDisplayModeList(format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);
		output->GetDisplayModeList(format, flags, &count, &modeList[0]);

		for (auto& x : modeList)
		{
			UINT n = x.RefreshRate.Numerator;
			UINT d = x.RefreshRate.Denominator;
			std::wstring text =
				L"Width = " + std::to_wstring(x.Width) + L" " +
				L"Height = " + std::to_wstring(x.Height) + L" " +
				L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
				L"\n";

			::OutputDebugStringW(text.c_str());
		}
	}
}