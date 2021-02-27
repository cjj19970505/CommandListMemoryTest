#include "pch.h"


using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();
    com_ptr<ID3D12Debug> debugController;
    winrt::check_hresult(D3D12GetDebugInterface(guid_of<ID3D12Debug>(), debugController.put_void()));
    debugController->EnableDebugLayer();
    com_ptr<IDXGIFactory4> dxgiFactory;
    check_hresult(CreateDXGIFactory1(guid_of<IDXGIFactory4>(), dxgiFactory.put_void()));
    com_ptr<IDXGIAdapter1> dxgiAdapter{ nullptr };
    check_hresult(dxgiFactory->EnumAdapters1(0, dxgiAdapter.put()));
    com_ptr<ID3D12Device> device;
    check_hresult(D3D12CreateDevice(dxgiAdapter.get(), D3D_FEATURE_LEVEL_11_0, guid_of<ID3D12Device>(), device.put_void()));
    com_ptr<ID3D12Device4> device4 = device.as<ID3D12Device4>();
    

    //Block 1
    //This block leads to memory leak
    {
        com_ptr<ID3D12GraphicsCommandList> commandList;
        device4->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, guid_of<ID3D12GraphicsCommandList>(), commandList.put_void());
        while (true)
        {
            com_ptr<ID3D12CommandAllocator> commandAllocator;
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, guid_of<ID3D12CommandAllocator>(), commandAllocator.put_void());
            commandList->Reset(commandAllocator.get(), nullptr);
            commandList->Close();
            commandAllocator->Reset();
        }
    }
    
    //Block 2
    //This block act normally
    /*{
        com_ptr<ID3D12GraphicsCommandList> commandList;
        while (true)
        {
            com_ptr<ID3D12CommandAllocator> commandAllocator;
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, guid_of<ID3D12CommandAllocator>(), commandAllocator.put_void());
            com_ptr<ID3D12GraphicsCommandList> commandList;
            device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.get(), nullptr, guid_of<ID3D12GraphicsCommandList>(), commandList.put_void());
            commandList->Close();
            commandAllocator->Reset();
        }
    }*/

    //Block 3
    //This block represents most common workflow
    /*{
        com_ptr<ID3D12GraphicsCommandList> commandList;
        device4->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, guid_of<ID3D12GraphicsCommandList>(), commandList.put_void());
        com_ptr<ID3D12CommandAllocator> commandAllocator;
        device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, guid_of<ID3D12CommandAllocator>(), commandAllocator.put_void());
        while (true)
        {
            commandList->Reset(commandAllocator.get(), nullptr);
            commandList->Close();
            commandAllocator->Reset();
        }
    }*/
    

}
