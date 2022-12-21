module;

#include <d3d12.h>
#include <wrl.h>
#include "d3dx12.h"

export module UploadBuffer;

export import DirectXUtility;

export template<typename T> class UploadBuffer
{
    Microsoft::WRL::ComPtr<ID3D12Resource> _upload_buffer;
    BYTE* _mapped_data = nullptr;

    UINT element_byte_size = 0;
    bool _is_constant_buffer = false;

public:
    UploadBuffer(ID3D12Device* device, UINT64 element_count, bool is_constant_buffer) :
        _is_constant_buffer(is_constant_buffer)
    {
        element_byte_size = sizeof(T);

        // Constant buffer elements need to be multiples of 256 bytes.
        // This is because the hardware can only view constant data 
        // at m*256 byte offsets and of n*256 byte lengths. 
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; // multiple of 256
        // UINT   SizeInBytes;   // multiple of 256
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (is_constant_buffer) element_byte_size = mt::renderer::CalcConstantBufferByteSize(sizeof(T));

        auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(element_byte_size * element_count);
        mt::renderer::ThrowIfFailed(
            device->CreateCommittedResource(
                &heap_properties,
                D3D12_HEAP_FLAG_NONE,
                &resource_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&_upload_buffer)
            ),
            "",
            __FILE__,
            __LINE__
        );

        mt::renderer::ThrowIfFailed(
            _upload_buffer->Map(0, nullptr, reinterpret_cast<void**>(&_mapped_data)),
            "",
            __FILE__,
            __LINE__
        );

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer()
    {
        if (_upload_buffer != nullptr)
            _upload_buffer->Unmap(0, nullptr);

        _mapped_data = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return _upload_buffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&_mapped_data[elementIndex * element_byte_size], &data, sizeof(T));
    }
};