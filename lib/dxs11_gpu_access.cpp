#include "dxs11_gpu_access.h"

template<class T>
DX::DX11::CORE::ConstantBuffer DX::DX11::GPU_ACCESS<T>::cBuffer;

template<class T>
void *DX::DX11::GPU_ACCESS<T>::matrix = nullptr;
