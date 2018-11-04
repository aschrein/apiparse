#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11_3.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <unordered_map>
#include <fstream>
#include <assert.h>
#include <mutex>
static std::ofstream &out()
{
  static std::ofstream file("log");
  file.setf(std::ios::unitbuf);
  return file;
}
std::mutex &getGlobalLock()
{
  static std::mutex m;
  return m;
}
#define GLOBAL_LOCK std::lock_guard<std::mutex> GLOBAL_LOCK_VAR(getGlobalLock())
static std::unordered_map<size_t, size_t> &getWrapTable()
{
  static std::unordered_map<size_t, size_t> table;
  return table;
}
static std::unordered_map<size_t, size_t> &getUnwrapTable()
{
  static std::unordered_map<size_t, size_t> table;
  return table;
}
template<typename T, typename WT>
T *getWrapper(T *t)
{
  GLOBAL_LOCK;
  auto &wt = getWrapTable();
  auto fd = wt.find(reinterpret_cast<size_t>((void*)t));
  if (fd == wt.end()) {
    auto *wrap = new WT(t);
    return reinterpret_cast<T*>(wrap);
  }
  else
  {
    return reinterpret_cast<T*>((*fd).second);
  }
}
template<typename T>
T *unwrap(T *t)
{
  GLOBAL_LOCK;
  auto &uwt = getUnwrapTable();
  auto fd = uwt.find(reinterpret_cast<size_t>((void*)t));
  if (fd == uwt.end()) {
    return t;
  }
  else
  {
    return reinterpret_cast<T*>((*fd).second);
  }
}
std::ostream& operator<<(std::ostream& os, REFGUID guid) {

  os << std::uppercase;
  os.width(8);
  os << std::hex << guid.Data1 << '-';

  os.width(4);
  os << std::hex << guid.Data2 << '-';

  os.width(4);
  os << std::hex << guid.Data3 << '-';

  os.width(2);
  os << std::hex
    << static_cast<short>(guid.Data4[0])
    << static_cast<short>(guid.Data4[1])
    << '-'
    << static_cast<short>(guid.Data4[2])
    << static_cast<short>(guid.Data4[3])
    << static_cast<short>(guid.Data4[4])
    << static_cast<short>(guid.Data4[5])
    << static_cast<short>(guid.Data4[6])
    << static_cast<short>(guid.Data4[7]);
  os << std::nouppercase;
  return os;
}

template<typename T> void HandleWrap(
  const IID & riid, 
  T ** ppvObject) {
  if(riid == __uuidof(IUnknown)) {
    *ppvObject = (T*)getWrapper<IUnknown, WrappedID3D12LibraryReflection>((IUnknown*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIObject)) {
    *ppvObject = (T*)getWrapper<IDXGIObject, WrappedIDXGIFactory5>((IDXGIObject*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDeviceSubObject)) {
    *ppvObject = (T*)getWrapper<IDXGIDeviceSubObject, WrappedIDXGISwapChain4>((IDXGIDeviceSubObject*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIResource)) {
    *ppvObject = (T*)getWrapper<IDXGIResource, WrappedIDXGIResource1>((IDXGIResource*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIKeyedMutex)) {
    *ppvObject = (T*)getWrapper<IDXGIKeyedMutex, WrappedIDXGIKeyedMutex>((IDXGIKeyedMutex*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISurface)) {
    *ppvObject = (T*)getWrapper<IDXGISurface, WrappedIDXGISurface2>((IDXGISurface*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISurface1)) {
    *ppvObject = (T*)getWrapper<IDXGISurface1, WrappedIDXGISurface2>((IDXGISurface1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIAdapter)) {
    *ppvObject = (T*)getWrapper<IDXGIAdapter, WrappedIDXGIAdapter3>((IDXGIAdapter*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput, WrappedIDXGIOutput5>((IDXGIOutput*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChain)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChain, WrappedIDXGISwapChain4>((IDXGISwapChain*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory, WrappedIDXGIFactory5>((IDXGIFactory*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDevice)) {
    *ppvObject = (T*)getWrapper<IDXGIDevice, WrappedIDXGIDevice4>((IDXGIDevice*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory1)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory1, WrappedIDXGIFactory5>((IDXGIFactory1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIAdapter1)) {
    *ppvObject = (T*)getWrapper<IDXGIAdapter1, WrappedIDXGIAdapter3>((IDXGIAdapter1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDevice1)) {
    *ppvObject = (T*)getWrapper<IDXGIDevice1, WrappedIDXGIDevice4>((IDXGIDevice1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D10Blob)) {
    *ppvObject = (T*)getWrapper<ID3D10Blob, WrappedID3D10Blob>((ID3D10Blob*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3DDestructionNotifier)) {
    *ppvObject = (T*)getWrapper<ID3DDestructionNotifier, WrappedID3DDestructionNotifier>((ID3DDestructionNotifier*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceChild)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceChild, WrappedID3D11VideoContext3>((ID3D11DeviceChild*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DepthStencilState)) {
    *ppvObject = (T*)getWrapper<ID3D11DepthStencilState, WrappedID3D11DepthStencilState>((ID3D11DepthStencilState*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11BlendState)) {
    *ppvObject = (T*)getWrapper<ID3D11BlendState, WrappedID3D11BlendState1>((ID3D11BlendState*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RasterizerState)) {
    *ppvObject = (T*)getWrapper<ID3D11RasterizerState, WrappedID3D11RasterizerState2>((ID3D11RasterizerState*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Resource)) {
    *ppvObject = (T*)getWrapper<ID3D11Resource, WrappedID3D11Texture3D1>((ID3D11Resource*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Buffer)) {
    *ppvObject = (T*)getWrapper<ID3D11Buffer, WrappedID3D11Buffer>((ID3D11Buffer*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Texture1D)) {
    *ppvObject = (T*)getWrapper<ID3D11Texture1D, WrappedID3D11Texture1D>((ID3D11Texture1D*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Texture2D)) {
    *ppvObject = (T*)getWrapper<ID3D11Texture2D, WrappedID3D11Texture2D1>((ID3D11Texture2D*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Texture3D)) {
    *ppvObject = (T*)getWrapper<ID3D11Texture3D, WrappedID3D11Texture3D1>((ID3D11Texture3D*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11View)) {
    *ppvObject = (T*)getWrapper<ID3D11View, WrappedID3D11UnorderedAccessView1>((ID3D11View*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderResourceView)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>((ID3D11ShaderResourceView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RenderTargetView)) {
    *ppvObject = (T*)getWrapper<ID3D11RenderTargetView, WrappedID3D11RenderTargetView1>((ID3D11RenderTargetView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DepthStencilView)) {
    *ppvObject = (T*)getWrapper<ID3D11DepthStencilView, WrappedID3D11DepthStencilView>((ID3D11DepthStencilView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11UnorderedAccessView)) {
    *ppvObject = (T*)getWrapper<ID3D11UnorderedAccessView, WrappedID3D11UnorderedAccessView1>((ID3D11UnorderedAccessView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VertexShader)) {
    *ppvObject = (T*)getWrapper<ID3D11VertexShader, WrappedID3D11VertexShader>((ID3D11VertexShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11HullShader)) {
    *ppvObject = (T*)getWrapper<ID3D11HullShader, WrappedID3D11HullShader>((ID3D11HullShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DomainShader)) {
    *ppvObject = (T*)getWrapper<ID3D11DomainShader, WrappedID3D11DomainShader>((ID3D11DomainShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11GeometryShader)) {
    *ppvObject = (T*)getWrapper<ID3D11GeometryShader, WrappedID3D11GeometryShader>((ID3D11GeometryShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11PixelShader)) {
    *ppvObject = (T*)getWrapper<ID3D11PixelShader, WrappedID3D11PixelShader>((ID3D11PixelShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ComputeShader)) {
    *ppvObject = (T*)getWrapper<ID3D11ComputeShader, WrappedID3D11ComputeShader>((ID3D11ComputeShader*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11InputLayout)) {
    *ppvObject = (T*)getWrapper<ID3D11InputLayout, WrappedID3D11InputLayout>((ID3D11InputLayout*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11SamplerState)) {
    *ppvObject = (T*)getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>((ID3D11SamplerState*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Asynchronous)) {
    *ppvObject = (T*)getWrapper<ID3D11Asynchronous, WrappedID3D11Query1>((ID3D11Asynchronous*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Query)) {
    *ppvObject = (T*)getWrapper<ID3D11Query, WrappedID3D11Query1>((ID3D11Query*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Predicate)) {
    *ppvObject = (T*)getWrapper<ID3D11Predicate, WrappedID3D11Predicate>((ID3D11Predicate*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Counter)) {
    *ppvObject = (T*)getWrapper<ID3D11Counter, WrappedID3D11Counter>((ID3D11Counter*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ClassInstance)) {
    *ppvObject = (T*)getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>((ID3D11ClassInstance*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ClassLinkage)) {
    *ppvObject = (T*)getWrapper<ID3D11ClassLinkage, WrappedID3D11ClassLinkage>((ID3D11ClassLinkage*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11CommandList)) {
    *ppvObject = (T*)getWrapper<ID3D11CommandList, WrappedID3D11CommandList>((ID3D11CommandList*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceContext)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceContext, WrappedID3D11DeviceContext4>((ID3D11DeviceContext*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoDecoder)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoDecoder, WrappedID3D11VideoDecoder>((ID3D11VideoDecoder*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoProcessorEnumerator)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoProcessorEnumerator, WrappedID3D11VideoProcessorEnumerator1>((ID3D11VideoProcessorEnumerator*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoProcessor)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoProcessor, WrappedID3D11VideoProcessor>((ID3D11VideoProcessor*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11AuthenticatedChannel)) {
    *ppvObject = (T*)getWrapper<ID3D11AuthenticatedChannel, WrappedID3D11AuthenticatedChannel>((ID3D11AuthenticatedChannel*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11CryptoSession)) {
    *ppvObject = (T*)getWrapper<ID3D11CryptoSession, WrappedID3D11CryptoSession>((ID3D11CryptoSession*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoDecoderOutputView)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoDecoderOutputView, WrappedID3D11VideoDecoderOutputView>((ID3D11VideoDecoderOutputView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoProcessorInputView)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoProcessorInputView, WrappedID3D11VideoProcessorInputView>((ID3D11VideoProcessorInputView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoProcessorOutputView)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoProcessorOutputView, WrappedID3D11VideoProcessorOutputView>((ID3D11VideoProcessorOutputView*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoContext)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoContext, WrappedID3D11VideoContext3>((ID3D11VideoContext*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoDevice)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoDevice, WrappedID3D11VideoDevice2>((ID3D11VideoDevice*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device)) {
    *ppvObject = (T*)getWrapper<ID3D11Device, WrappedID3D11Device5>((ID3D11Device*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Debug)) {
    *ppvObject = (T*)getWrapper<ID3D11Debug, WrappedID3D11Debug>((ID3D11Debug*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11SwitchToRef)) {
    *ppvObject = (T*)getWrapper<ID3D11SwitchToRef, WrappedID3D11SwitchToRef>((ID3D11SwitchToRef*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11TracingDevice)) {
    *ppvObject = (T*)getWrapper<ID3D11TracingDevice, WrappedID3D11TracingDevice>((ID3D11TracingDevice*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RefTrackingOptions)) {
    *ppvObject = (T*)getWrapper<ID3D11RefTrackingOptions, WrappedID3D11RefTrackingOptions>((ID3D11RefTrackingOptions*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RefDefaultTrackingOptions)) {
    *ppvObject = (T*)getWrapper<ID3D11RefDefaultTrackingOptions, WrappedID3D11RefDefaultTrackingOptions>((ID3D11RefDefaultTrackingOptions*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11InfoQueue)) {
    *ppvObject = (T*)getWrapper<ID3D11InfoQueue, WrappedID3D11InfoQueue>((ID3D11InfoQueue*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3DDeviceContextState)) {
    *ppvObject = (T*)getWrapper<ID3DDeviceContextState, WrappedID3DDeviceContextState>((ID3DDeviceContextState*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDisplayControl)) {
    *ppvObject = (T*)getWrapper<IDXGIDisplayControl, WrappedIDXGIDisplayControl>((IDXGIDisplayControl*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutputDuplication)) {
    *ppvObject = (T*)getWrapper<IDXGIOutputDuplication, WrappedIDXGIOutputDuplication>((IDXGIOutputDuplication*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISurface2)) {
    *ppvObject = (T*)getWrapper<IDXGISurface2, WrappedIDXGISurface2>((IDXGISurface2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIResource1)) {
    *ppvObject = (T*)getWrapper<IDXGIResource1, WrappedIDXGIResource1>((IDXGIResource1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDevice2)) {
    *ppvObject = (T*)getWrapper<IDXGIDevice2, WrappedIDXGIDevice4>((IDXGIDevice2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChain1)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChain1, WrappedIDXGISwapChain4>((IDXGISwapChain1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory2)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory2, WrappedIDXGIFactory5>((IDXGIFactory2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIAdapter2)) {
    *ppvObject = (T*)getWrapper<IDXGIAdapter2, WrappedIDXGIAdapter3>((IDXGIAdapter2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput1)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput1, WrappedIDXGIOutput5>((IDXGIOutput1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11BlendState1)) {
    *ppvObject = (T*)getWrapper<ID3D11BlendState1, WrappedID3D11BlendState1>((ID3D11BlendState1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RasterizerState1)) {
    *ppvObject = (T*)getWrapper<ID3D11RasterizerState1, WrappedID3D11RasterizerState2>((ID3D11RasterizerState1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceContext1)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceContext1, WrappedID3D11DeviceContext4>((ID3D11DeviceContext1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoContext1)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoContext1, WrappedID3D11VideoContext3>((ID3D11VideoContext1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoDevice1)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoDevice1, WrappedID3D11VideoDevice2>((ID3D11VideoDevice1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoProcessorEnumerator1)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoProcessorEnumerator1, WrappedID3D11VideoProcessorEnumerator1>((ID3D11VideoProcessorEnumerator1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device1)) {
    *ppvObject = (T*)getWrapper<ID3D11Device1, WrappedID3D11Device5>((ID3D11Device1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3DUserDefinedAnnotation)) {
    *ppvObject = (T*)getWrapper<ID3DUserDefinedAnnotation, WrappedID3DUserDefinedAnnotation>((ID3DUserDefinedAnnotation*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDevice3)) {
    *ppvObject = (T*)getWrapper<IDXGIDevice3, WrappedIDXGIDevice4>((IDXGIDevice3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChain2)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChain2, WrappedIDXGISwapChain4>((IDXGISwapChain2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput2)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput2, WrappedIDXGIOutput5>((IDXGIOutput2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory3)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory3, WrappedIDXGIFactory5>((IDXGIFactory3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDecodeSwapChain)) {
    *ppvObject = (T*)getWrapper<IDXGIDecodeSwapChain, WrappedIDXGIDecodeSwapChain>((IDXGIDecodeSwapChain*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactoryMedia)) {
    *ppvObject = (T*)getWrapper<IDXGIFactoryMedia, WrappedIDXGIFactoryMedia>((IDXGIFactoryMedia*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChainMedia)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChainMedia, WrappedIDXGISwapChainMedia>((IDXGISwapChainMedia*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput3)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput3, WrappedIDXGIOutput5>((IDXGIOutput3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceContext2)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceContext2, WrappedID3D11DeviceContext4>((ID3D11DeviceContext2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device2)) {
    *ppvObject = (T*)getWrapper<ID3D11Device2, WrappedID3D11Device5>((ID3D11Device2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Texture2D1)) {
    *ppvObject = (T*)getWrapper<ID3D11Texture2D1, WrappedID3D11Texture2D1>((ID3D11Texture2D1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Texture3D1)) {
    *ppvObject = (T*)getWrapper<ID3D11Texture3D1, WrappedID3D11Texture3D1>((ID3D11Texture3D1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RasterizerState2)) {
    *ppvObject = (T*)getWrapper<ID3D11RasterizerState2, WrappedID3D11RasterizerState2>((ID3D11RasterizerState2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderResourceView1)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderResourceView1, WrappedID3D11ShaderResourceView1>((ID3D11ShaderResourceView1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11RenderTargetView1)) {
    *ppvObject = (T*)getWrapper<ID3D11RenderTargetView1, WrappedID3D11RenderTargetView1>((ID3D11RenderTargetView1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11UnorderedAccessView1)) {
    *ppvObject = (T*)getWrapper<ID3D11UnorderedAccessView1, WrappedID3D11UnorderedAccessView1>((ID3D11UnorderedAccessView1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Query1)) {
    *ppvObject = (T*)getWrapper<ID3D11Query1, WrappedID3D11Query1>((ID3D11Query1*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceContext3)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceContext3, WrappedID3D11DeviceContext4>((ID3D11DeviceContext3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Fence)) {
    *ppvObject = (T*)getWrapper<ID3D11Fence, WrappedID3D11Fence>((ID3D11Fence*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11DeviceContext4)) {
    *ppvObject = (T*)getWrapper<ID3D11DeviceContext4, WrappedID3D11DeviceContext4>((ID3D11DeviceContext4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device3)) {
    *ppvObject = (T*)getWrapper<ID3D11Device3, WrappedID3D11Device5>((ID3D11Device3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChain3)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChain3, WrappedIDXGISwapChain4>((IDXGISwapChain3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput4)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput4, WrappedIDXGIOutput5>((IDXGIOutput4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory4)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory4, WrappedIDXGIFactory5>((IDXGIFactory4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIAdapter3)) {
    *ppvObject = (T*)getWrapper<IDXGIAdapter3, WrappedIDXGIAdapter3>((IDXGIAdapter3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIOutput5)) {
    *ppvObject = (T*)getWrapper<IDXGIOutput5, WrappedIDXGIOutput5>((IDXGIOutput5*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGISwapChain4)) {
    *ppvObject = (T*)getWrapper<IDXGISwapChain4, WrappedIDXGISwapChain4>((IDXGISwapChain4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIDevice4)) {
    *ppvObject = (T*)getWrapper<IDXGIDevice4, WrappedIDXGIDevice4>((IDXGIDevice4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(IDXGIFactory5)) {
    *ppvObject = (T*)getWrapper<IDXGIFactory5, WrappedIDXGIFactory5>((IDXGIFactory5*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device4)) {
    *ppvObject = (T*)getWrapper<ID3D11Device4, WrappedID3D11Device5>((ID3D11Device4*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Device5)) {
    *ppvObject = (T*)getWrapper<ID3D11Device5, WrappedID3D11Device5>((ID3D11Device5*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Multithread)) {
    *ppvObject = (T*)getWrapper<ID3D11Multithread, WrappedID3D11Multithread>((ID3D11Multithread*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoContext2)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoContext2, WrappedID3D11VideoContext3>((ID3D11VideoContext2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoDevice2)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoDevice2, WrappedID3D11VideoDevice2>((ID3D11VideoDevice2*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11VideoContext3)) {
    *ppvObject = (T*)getWrapper<ID3D11VideoContext3, WrappedID3D11VideoContext3>((ID3D11VideoContext3*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderReflectionType)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderReflectionType, WrappedID3D11ShaderReflectionType>((ID3D11ShaderReflectionType*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderReflectionVariable)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderReflectionVariable, WrappedID3D11ShaderReflectionVariable>((ID3D11ShaderReflectionVariable*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderReflectionConstantBuffer)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderReflectionConstantBuffer, WrappedID3D11ShaderReflectionConstantBuffer>((ID3D11ShaderReflectionConstantBuffer*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ShaderReflection)) {
    *ppvObject = (T*)getWrapper<ID3D11ShaderReflection, WrappedID3D11ShaderReflection>((ID3D11ShaderReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11LibraryReflection)) {
    *ppvObject = (T*)getWrapper<ID3D11LibraryReflection, WrappedID3D11LibraryReflection>((ID3D11LibraryReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11FunctionReflection)) {
    *ppvObject = (T*)getWrapper<ID3D11FunctionReflection, WrappedID3D11FunctionReflection>((ID3D11FunctionReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11FunctionParameterReflection)) {
    *ppvObject = (T*)getWrapper<ID3D11FunctionParameterReflection, WrappedID3D11FunctionParameterReflection>((ID3D11FunctionParameterReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Module)) {
    *ppvObject = (T*)getWrapper<ID3D11Module, WrappedID3D11Module>((ID3D11Module*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11ModuleInstance)) {
    *ppvObject = (T*)getWrapper<ID3D11ModuleInstance, WrappedID3D11ModuleInstance>((ID3D11ModuleInstance*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11Linker)) {
    *ppvObject = (T*)getWrapper<ID3D11Linker, WrappedID3D11Linker>((ID3D11Linker*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11LinkingNode)) {
    *ppvObject = (T*)getWrapper<ID3D11LinkingNode, WrappedID3D11LinkingNode>((ID3D11LinkingNode*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D11FunctionLinkingGraph)) {
    *ppvObject = (T*)getWrapper<ID3D11FunctionLinkingGraph, WrappedID3D11FunctionLinkingGraph>((ID3D11FunctionLinkingGraph*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12ShaderReflectionType)) {
    *ppvObject = (T*)getWrapper<ID3D12ShaderReflectionType, WrappedID3D12ShaderReflectionType>((ID3D12ShaderReflectionType*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12ShaderReflectionVariable)) {
    *ppvObject = (T*)getWrapper<ID3D12ShaderReflectionVariable, WrappedID3D12ShaderReflectionVariable>((ID3D12ShaderReflectionVariable*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12ShaderReflectionConstantBuffer)) {
    *ppvObject = (T*)getWrapper<ID3D12ShaderReflectionConstantBuffer, WrappedID3D12ShaderReflectionConstantBuffer>((ID3D12ShaderReflectionConstantBuffer*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12ShaderReflection)) {
    *ppvObject = (T*)getWrapper<ID3D12ShaderReflection, WrappedID3D12ShaderReflection>((ID3D12ShaderReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12LibraryReflection)) {
    *ppvObject = (T*)getWrapper<ID3D12LibraryReflection, WrappedID3D12LibraryReflection>((ID3D12LibraryReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12FunctionReflection)) {
    *ppvObject = (T*)getWrapper<ID3D12FunctionReflection, WrappedID3D12FunctionReflection>((ID3D12FunctionReflection*)*ppvObject);
    return;
  }
  if(riid == __uuidof(ID3D12FunctionParameterReflection)) {
    *ppvObject = (T*)getWrapper<ID3D12FunctionParameterReflection, WrappedID3D12FunctionParameterReflection>((ID3D12FunctionParameterReflection*)*ppvObject);
    return;
  }
  {
    out() << "[WARNING] Unknown(" << *ppvObject << ") riid:" << riid << "\n";
  }
}
#if 0
begin class IDXGIKeyedMutex
inherits:
  IDXGIDeviceSubObject
  IDXGIObject
  IUnknown
methods:
  AcquireSync
  ReleaseSync
end class
#endif
class WrappedIDXGIKeyedMutex : public IDXGIKeyedMutex {
private:
  IDXGIKeyedMutex *m_pIDXGIKeyedMutex;
  IDXGIDeviceSubObject *m_pIDXGIDeviceSubObject;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIKeyedMutex(T *pWrapped) {
    out() << "[CREATE] IDXGIKeyedMutex(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIDeviceSubObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDeviceSubObject), (void **)&m_pIDXGIDeviceSubObject);
    if (m_pIDXGIDeviceSubObject) {
      m_pIDXGIDeviceSubObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDeviceSubObject << " -> " << this << "\n";
    }
    m_pIDXGIKeyedMutex = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIKeyedMutex), (void **)&m_pIDXGIKeyedMutex);
    if (m_pIDXGIKeyedMutex) {
      m_pIDXGIKeyedMutex->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIKeyedMutex);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIKeyedMutex)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIKeyedMutex << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall AcquireSync(
    UINT64 Key, 
    DWORD dwMilliseconds
  ) override;
  HRESULT __stdcall ReleaseSync(
    UINT64 Key
  ) override;
  HRESULT __stdcall GetDevice(
    const IID & riid, 
    void ** ppDevice
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D10Blob
inherits:
  IUnknown
methods:
  GetBufferPointer
  GetBufferSize
end class
#endif
class WrappedID3D10Blob : public ID3D10Blob {
private:
  ID3D10Blob *m_pID3D10Blob;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D10Blob(T *pWrapped) {
    out() << "[CREATE] ID3D10Blob(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D10Blob = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D10Blob), (void **)&m_pID3D10Blob);
    if (m_pID3D10Blob) {
      m_pID3D10Blob->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D10Blob);
      wt[reinterpret_cast<size_t>((void*)m_pID3D10Blob)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D10Blob << " -> " << this << "\n";
    }
  }
  LPVOID __stdcall GetBufferPointer() override;
  SIZE_T __stdcall GetBufferSize() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3DDestructionNotifier
inherits:
  IUnknown
methods:
  RegisterDestructionCallback
  UnregisterDestructionCallback
end class
#endif
class WrappedID3DDestructionNotifier : public ID3DDestructionNotifier {
private:
  ID3DDestructionNotifier *m_pID3DDestructionNotifier;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3DDestructionNotifier(T *pWrapped) {
    out() << "[CREATE] ID3DDestructionNotifier(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3DDestructionNotifier = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3DDestructionNotifier), (void **)&m_pID3DDestructionNotifier);
    if (m_pID3DDestructionNotifier) {
      m_pID3DDestructionNotifier->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3DDestructionNotifier);
      wt[reinterpret_cast<size_t>((void*)m_pID3DDestructionNotifier)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3DDestructionNotifier << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall RegisterDestructionCallback(
    PFN_DESTRUCTION_CALLBACK callbackFn, 
    void * pData, 
    UINT * pCallbackID
  ) override;
  HRESULT __stdcall UnregisterDestructionCallback(
    UINT callbackID
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3DInclude
inherits:
methods:
  Open
  Close
end class
#endif
class WrappedID3DInclude : public ID3DInclude {
private:
  ID3DInclude *m_pID3DInclude;
public:
  template<typename T>
  WrappedID3DInclude(T *pWrapped) {
    out() << "[CREATE] ID3DInclude(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3DInclude = nullptr;
    m_pID3DInclude = (ID3DInclude*)pWrapped;
  }
  HRESULT __stdcall Open(
    D3D_INCLUDE_TYPE IncludeType, 
    LPCSTR pFileName, 
    LPCVOID pParentData, 
    LPCVOID * ppData, 
    UINT * pBytes
  ) override;
  HRESULT __stdcall Close(
    LPCVOID pData
  ) override;
};
#if 0
begin class ID3D11DepthStencilState
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11DepthStencilState : public ID3D11DepthStencilState {
private:
  ID3D11DepthStencilState *m_pID3D11DepthStencilState;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11DepthStencilState(T *pWrapped) {
    out() << "[CREATE] ID3D11DepthStencilState(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11DepthStencilState = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DepthStencilState), (void **)&m_pID3D11DepthStencilState);
    if (m_pID3D11DepthStencilState) {
      m_pID3D11DepthStencilState->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DepthStencilState);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DepthStencilState)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DepthStencilState << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_DEPTH_STENCIL_DESC * pDesc
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Buffer
inherits:
  ID3D11Resource
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11Buffer : public ID3D11Buffer {
private:
  ID3D11Buffer *m_pID3D11Buffer;
  ID3D11Resource *m_pID3D11Resource;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Buffer(T *pWrapped) {
    out() << "[CREATE] ID3D11Buffer(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Resource = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Resource), (void **)&m_pID3D11Resource);
    if (m_pID3D11Resource) {
      m_pID3D11Resource->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Resource);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Resource)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Resource << " -> " << this << "\n";
    }
    m_pID3D11Buffer = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Buffer), (void **)&m_pID3D11Buffer);
    if (m_pID3D11Buffer) {
      m_pID3D11Buffer->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Buffer);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Buffer)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Buffer << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_BUFFER_DESC * pDesc
  ) override;
  void __stdcall GetType(
    D3D11_RESOURCE_DIMENSION * pResourceDimension
  ) override;
  void __stdcall SetEvictionPriority(
    UINT EvictionPriority
  ) override;
  UINT __stdcall GetEvictionPriority() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Texture1D
inherits:
  ID3D11Resource
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11Texture1D : public ID3D11Texture1D {
private:
  ID3D11Texture1D *m_pID3D11Texture1D;
  ID3D11Resource *m_pID3D11Resource;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Texture1D(T *pWrapped) {
    out() << "[CREATE] ID3D11Texture1D(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Resource = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Resource), (void **)&m_pID3D11Resource);
    if (m_pID3D11Resource) {
      m_pID3D11Resource->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Resource);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Resource)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Resource << " -> " << this << "\n";
    }
    m_pID3D11Texture1D = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Texture1D), (void **)&m_pID3D11Texture1D);
    if (m_pID3D11Texture1D) {
      m_pID3D11Texture1D->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Texture1D);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Texture1D)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Texture1D << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_TEXTURE1D_DESC * pDesc
  ) override;
  void __stdcall GetType(
    D3D11_RESOURCE_DIMENSION * pResourceDimension
  ) override;
  void __stdcall SetEvictionPriority(
    UINT EvictionPriority
  ) override;
  UINT __stdcall GetEvictionPriority() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11DepthStencilView
inherits:
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11DepthStencilView : public ID3D11DepthStencilView {
private:
  ID3D11DepthStencilView *m_pID3D11DepthStencilView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11DepthStencilView(T *pWrapped) {
    out() << "[CREATE] ID3D11DepthStencilView(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11DepthStencilView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DepthStencilView), (void **)&m_pID3D11DepthStencilView);
    if (m_pID3D11DepthStencilView) {
      m_pID3D11DepthStencilView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DepthStencilView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DepthStencilView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DepthStencilView << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VertexShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11VertexShader : public ID3D11VertexShader {
private:
  ID3D11VertexShader *m_pID3D11VertexShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VertexShader(T *pWrapped) {
    out() << "[CREATE] ID3D11VertexShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11VertexShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VertexShader), (void **)&m_pID3D11VertexShader);
    if (m_pID3D11VertexShader) {
      m_pID3D11VertexShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VertexShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VertexShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VertexShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11HullShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11HullShader : public ID3D11HullShader {
private:
  ID3D11HullShader *m_pID3D11HullShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11HullShader(T *pWrapped) {
    out() << "[CREATE] ID3D11HullShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11HullShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11HullShader), (void **)&m_pID3D11HullShader);
    if (m_pID3D11HullShader) {
      m_pID3D11HullShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11HullShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11HullShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11HullShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11DomainShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11DomainShader : public ID3D11DomainShader {
private:
  ID3D11DomainShader *m_pID3D11DomainShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11DomainShader(T *pWrapped) {
    out() << "[CREATE] ID3D11DomainShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11DomainShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DomainShader), (void **)&m_pID3D11DomainShader);
    if (m_pID3D11DomainShader) {
      m_pID3D11DomainShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DomainShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DomainShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DomainShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11GeometryShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11GeometryShader : public ID3D11GeometryShader {
private:
  ID3D11GeometryShader *m_pID3D11GeometryShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11GeometryShader(T *pWrapped) {
    out() << "[CREATE] ID3D11GeometryShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11GeometryShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11GeometryShader), (void **)&m_pID3D11GeometryShader);
    if (m_pID3D11GeometryShader) {
      m_pID3D11GeometryShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11GeometryShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11GeometryShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11GeometryShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11PixelShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11PixelShader : public ID3D11PixelShader {
private:
  ID3D11PixelShader *m_pID3D11PixelShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11PixelShader(T *pWrapped) {
    out() << "[CREATE] ID3D11PixelShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11PixelShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11PixelShader), (void **)&m_pID3D11PixelShader);
    if (m_pID3D11PixelShader) {
      m_pID3D11PixelShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11PixelShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11PixelShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11PixelShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11ComputeShader
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11ComputeShader : public ID3D11ComputeShader {
private:
  ID3D11ComputeShader *m_pID3D11ComputeShader;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ComputeShader(T *pWrapped) {
    out() << "[CREATE] ID3D11ComputeShader(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11ComputeShader = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ComputeShader), (void **)&m_pID3D11ComputeShader);
    if (m_pID3D11ComputeShader) {
      m_pID3D11ComputeShader->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ComputeShader);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ComputeShader)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ComputeShader << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11InputLayout
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11InputLayout : public ID3D11InputLayout {
private:
  ID3D11InputLayout *m_pID3D11InputLayout;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11InputLayout(T *pWrapped) {
    out() << "[CREATE] ID3D11InputLayout(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11InputLayout = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11InputLayout), (void **)&m_pID3D11InputLayout);
    if (m_pID3D11InputLayout) {
      m_pID3D11InputLayout->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11InputLayout);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11InputLayout)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11InputLayout << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11SamplerState
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11SamplerState : public ID3D11SamplerState {
private:
  ID3D11SamplerState *m_pID3D11SamplerState;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11SamplerState(T *pWrapped) {
    out() << "[CREATE] ID3D11SamplerState(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11SamplerState = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11SamplerState), (void **)&m_pID3D11SamplerState);
    if (m_pID3D11SamplerState) {
      m_pID3D11SamplerState->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11SamplerState);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11SamplerState)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11SamplerState << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_SAMPLER_DESC * pDesc
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Predicate
inherits:
  ID3D11Query
  ID3D11Asynchronous
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3D11Predicate : public ID3D11Predicate {
private:
  ID3D11Predicate *m_pID3D11Predicate;
  ID3D11Query *m_pID3D11Query;
  ID3D11Asynchronous *m_pID3D11Asynchronous;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Predicate(T *pWrapped) {
    out() << "[CREATE] ID3D11Predicate(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Asynchronous = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Asynchronous), (void **)&m_pID3D11Asynchronous);
    if (m_pID3D11Asynchronous) {
      m_pID3D11Asynchronous->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Asynchronous << " -> " << this << "\n";
    }
    m_pID3D11Query = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Query), (void **)&m_pID3D11Query);
    if (m_pID3D11Query) {
      m_pID3D11Query->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Query);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Query)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Query << " -> " << this << "\n";
    }
    m_pID3D11Predicate = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Predicate), (void **)&m_pID3D11Predicate);
    if (m_pID3D11Predicate) {
      m_pID3D11Predicate->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Predicate);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Predicate)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Predicate << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_QUERY_DESC * pDesc
  ) override;
  UINT __stdcall GetDataSize() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Counter
inherits:
  ID3D11Asynchronous
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11Counter : public ID3D11Counter {
private:
  ID3D11Counter *m_pID3D11Counter;
  ID3D11Asynchronous *m_pID3D11Asynchronous;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Counter(T *pWrapped) {
    out() << "[CREATE] ID3D11Counter(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Asynchronous = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Asynchronous), (void **)&m_pID3D11Asynchronous);
    if (m_pID3D11Asynchronous) {
      m_pID3D11Asynchronous->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Asynchronous << " -> " << this << "\n";
    }
    m_pID3D11Counter = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Counter), (void **)&m_pID3D11Counter);
    if (m_pID3D11Counter) {
      m_pID3D11Counter->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Counter);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Counter)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Counter << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_COUNTER_DESC * pDesc
  ) override;
  UINT __stdcall GetDataSize() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11ClassInstance
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetClassLinkage
  GetDesc
  GetInstanceName
  GetTypeName
end class
#endif
class WrappedID3D11ClassInstance : public ID3D11ClassInstance {
private:
  ID3D11ClassInstance *m_pID3D11ClassInstance;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ClassInstance(T *pWrapped) {
    out() << "[CREATE] ID3D11ClassInstance(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11ClassInstance = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ClassInstance), (void **)&m_pID3D11ClassInstance);
    if (m_pID3D11ClassInstance) {
      m_pID3D11ClassInstance->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ClassInstance);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ClassInstance)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ClassInstance << " -> " << this << "\n";
    }
  }
  void __stdcall GetClassLinkage(
    ID3D11ClassLinkage ** ppLinkage
  ) override;
  void __stdcall GetDesc(
    D3D11_CLASS_INSTANCE_DESC * pDesc
  ) override;
  void __stdcall GetInstanceName(
    LPSTR pInstanceName, 
    SIZE_T * pBufferLength
  ) override;
  void __stdcall GetTypeName(
    LPSTR pTypeName, 
    SIZE_T * pBufferLength
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11ClassLinkage
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetClassInstance
  CreateClassInstance
end class
#endif
class WrappedID3D11ClassLinkage : public ID3D11ClassLinkage {
private:
  ID3D11ClassLinkage *m_pID3D11ClassLinkage;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ClassLinkage(T *pWrapped) {
    out() << "[CREATE] ID3D11ClassLinkage(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11ClassLinkage = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ClassLinkage), (void **)&m_pID3D11ClassLinkage);
    if (m_pID3D11ClassLinkage) {
      m_pID3D11ClassLinkage->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ClassLinkage);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ClassLinkage)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ClassLinkage << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall GetClassInstance(
    LPCSTR pClassInstanceName, 
    UINT InstanceIndex, 
    ID3D11ClassInstance ** ppInstance
  ) override;
  HRESULT __stdcall CreateClassInstance(
    LPCSTR pClassTypeName, 
    UINT ConstantBufferOffset, 
    UINT ConstantVectorOffset, 
    UINT TextureOffset, 
    UINT SamplerOffset, 
    ID3D11ClassInstance ** ppInstance
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11CommandList
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetContextFlags
end class
#endif
class WrappedID3D11CommandList : public ID3D11CommandList {
private:
  ID3D11CommandList *m_pID3D11CommandList;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11CommandList(T *pWrapped) {
    out() << "[CREATE] ID3D11CommandList(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11CommandList = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11CommandList), (void **)&m_pID3D11CommandList);
    if (m_pID3D11CommandList) {
      m_pID3D11CommandList->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11CommandList);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11CommandList)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11CommandList << " -> " << this << "\n";
    }
  }
  UINT __stdcall GetContextFlags() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoDecoder
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetCreationParameters
  GetDriverHandle
end class
#endif
class WrappedID3D11VideoDecoder : public ID3D11VideoDecoder {
private:
  ID3D11VideoDecoder *m_pID3D11VideoDecoder;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoDecoder(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoDecoder(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11VideoDecoder = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoDecoder), (void **)&m_pID3D11VideoDecoder);
    if (m_pID3D11VideoDecoder) {
      m_pID3D11VideoDecoder->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoDecoder);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoDecoder)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoDecoder << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall GetCreationParameters(
    D3D11_VIDEO_DECODER_DESC * pVideoDesc, 
    D3D11_VIDEO_DECODER_CONFIG * pConfig
  ) override;
  HRESULT __stdcall GetDriverHandle(
    HANDLE * pDriverHandle
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoProcessor
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetContentDesc
  GetRateConversionCaps
end class
#endif
class WrappedID3D11VideoProcessor : public ID3D11VideoProcessor {
private:
  ID3D11VideoProcessor *m_pID3D11VideoProcessor;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoProcessor(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoProcessor(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11VideoProcessor = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoProcessor), (void **)&m_pID3D11VideoProcessor);
    if (m_pID3D11VideoProcessor) {
      m_pID3D11VideoProcessor->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessor);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessor)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoProcessor << " -> " << this << "\n";
    }
  }
  void __stdcall GetContentDesc(
    D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc
  ) override;
  void __stdcall GetRateConversionCaps(
    D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11AuthenticatedChannel
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetCertificateSize
  GetCertificate
  GetChannelHandle
end class
#endif
class WrappedID3D11AuthenticatedChannel : public ID3D11AuthenticatedChannel {
private:
  ID3D11AuthenticatedChannel *m_pID3D11AuthenticatedChannel;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11AuthenticatedChannel(T *pWrapped) {
    out() << "[CREATE] ID3D11AuthenticatedChannel(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11AuthenticatedChannel = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11AuthenticatedChannel), (void **)&m_pID3D11AuthenticatedChannel);
    if (m_pID3D11AuthenticatedChannel) {
      m_pID3D11AuthenticatedChannel->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11AuthenticatedChannel);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11AuthenticatedChannel)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11AuthenticatedChannel << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall GetCertificateSize(
    UINT * pCertificateSize
  ) override;
  HRESULT __stdcall GetCertificate(
    UINT CertificateSize, 
    BYTE * pCertificate
  ) override;
  void __stdcall GetChannelHandle(
    HANDLE * pChannelHandle
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11CryptoSession
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  GetCryptoType
  GetDecoderProfile
  GetCertificateSize
  GetCertificate
  GetCryptoSessionHandle
end class
#endif
class WrappedID3D11CryptoSession : public ID3D11CryptoSession {
private:
  ID3D11CryptoSession *m_pID3D11CryptoSession;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11CryptoSession(T *pWrapped) {
    out() << "[CREATE] ID3D11CryptoSession(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11CryptoSession = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11CryptoSession), (void **)&m_pID3D11CryptoSession);
    if (m_pID3D11CryptoSession) {
      m_pID3D11CryptoSession->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11CryptoSession);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11CryptoSession)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11CryptoSession << " -> " << this << "\n";
    }
  }
  void __stdcall GetCryptoType(
    GUID * pCryptoType
  ) override;
  void __stdcall GetDecoderProfile(
    GUID * pDecoderProfile
  ) override;
  HRESULT __stdcall GetCertificateSize(
    UINT * pCertificateSize
  ) override;
  HRESULT __stdcall GetCertificate(
    UINT CertificateSize, 
    BYTE * pCertificate
  ) override;
  void __stdcall GetCryptoSessionHandle(
    HANDLE * pCryptoSessionHandle
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoDecoderOutputView
inherits:
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11VideoDecoderOutputView : public ID3D11VideoDecoderOutputView {
private:
  ID3D11VideoDecoderOutputView *m_pID3D11VideoDecoderOutputView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoDecoderOutputView(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoDecoderOutputView(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11VideoDecoderOutputView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoDecoderOutputView), (void **)&m_pID3D11VideoDecoderOutputView);
    if (m_pID3D11VideoDecoderOutputView) {
      m_pID3D11VideoDecoderOutputView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoDecoderOutputView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoDecoderOutputView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoDecoderOutputView << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoProcessorInputView
inherits:
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11VideoProcessorInputView : public ID3D11VideoProcessorInputView {
private:
  ID3D11VideoProcessorInputView *m_pID3D11VideoProcessorInputView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoProcessorInputView(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoProcessorInputView(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11VideoProcessorInputView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoProcessorInputView), (void **)&m_pID3D11VideoProcessorInputView);
    if (m_pID3D11VideoProcessorInputView) {
      m_pID3D11VideoProcessorInputView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorInputView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorInputView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoProcessorInputView << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoProcessorOutputView
inherits:
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc
end class
#endif
class WrappedID3D11VideoProcessorOutputView : public ID3D11VideoProcessorOutputView {
private:
  ID3D11VideoProcessorOutputView *m_pID3D11VideoProcessorOutputView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoProcessorOutputView(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoProcessorOutputView(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11VideoProcessorOutputView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoProcessorOutputView), (void **)&m_pID3D11VideoProcessorOutputView);
    if (m_pID3D11VideoProcessorOutputView) {
      m_pID3D11VideoProcessorOutputView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorOutputView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorOutputView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoProcessorOutputView << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Debug
inherits:
  IUnknown
methods:
  SetFeatureMask
  GetFeatureMask
  SetPresentPerRenderOpDelay
  GetPresentPerRenderOpDelay
  SetSwapChain
  GetSwapChain
  ValidateContext
  ReportLiveDeviceObjects
  ValidateContextForDispatch
end class
#endif
class WrappedID3D11Debug : public ID3D11Debug {
private:
  ID3D11Debug *m_pID3D11Debug;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Debug(T *pWrapped) {
    out() << "[CREATE] ID3D11Debug(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11Debug = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Debug), (void **)&m_pID3D11Debug);
    if (m_pID3D11Debug) {
      m_pID3D11Debug->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Debug);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Debug)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Debug << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetFeatureMask(
    UINT Mask
  ) override;
  UINT __stdcall GetFeatureMask() override;
  HRESULT __stdcall SetPresentPerRenderOpDelay(
    UINT Milliseconds
  ) override;
  UINT __stdcall GetPresentPerRenderOpDelay() override;
  HRESULT __stdcall SetSwapChain(
    IDXGISwapChain * pSwapChain
  ) override;
  HRESULT __stdcall GetSwapChain(
    IDXGISwapChain ** ppSwapChain
  ) override;
  HRESULT __stdcall ValidateContext(
    ID3D11DeviceContext * pContext
  ) override;
  HRESULT __stdcall ReportLiveDeviceObjects(
    D3D11_RLDO_FLAGS Flags
  ) override;
  HRESULT __stdcall ValidateContextForDispatch(
    ID3D11DeviceContext * pContext
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11SwitchToRef
inherits:
  IUnknown
methods:
  SetUseRef
  GetUseRef
end class
#endif
class WrappedID3D11SwitchToRef : public ID3D11SwitchToRef {
private:
  ID3D11SwitchToRef *m_pID3D11SwitchToRef;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11SwitchToRef(T *pWrapped) {
    out() << "[CREATE] ID3D11SwitchToRef(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11SwitchToRef = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11SwitchToRef), (void **)&m_pID3D11SwitchToRef);
    if (m_pID3D11SwitchToRef) {
      m_pID3D11SwitchToRef->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11SwitchToRef);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11SwitchToRef)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11SwitchToRef << " -> " << this << "\n";
    }
  }
  BOOL __stdcall SetUseRef(
    BOOL UseRef
  ) override;
  BOOL __stdcall GetUseRef() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11TracingDevice
inherits:
  IUnknown
methods:
  SetShaderTrackingOptionsByType
  SetShaderTrackingOptions
end class
#endif
class WrappedID3D11TracingDevice : public ID3D11TracingDevice {
private:
  ID3D11TracingDevice *m_pID3D11TracingDevice;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11TracingDevice(T *pWrapped) {
    out() << "[CREATE] ID3D11TracingDevice(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11TracingDevice = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11TracingDevice), (void **)&m_pID3D11TracingDevice);
    if (m_pID3D11TracingDevice) {
      m_pID3D11TracingDevice->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11TracingDevice);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11TracingDevice)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11TracingDevice << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetShaderTrackingOptionsByType(
    UINT ResourceTypeFlags, 
    UINT Options
  ) override;
  HRESULT __stdcall SetShaderTrackingOptions(
    struct IUnknown * pShader, 
    UINT Options
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11RefTrackingOptions
inherits:
  IUnknown
methods:
  SetTrackingOptions
end class
#endif
class WrappedID3D11RefTrackingOptions : public ID3D11RefTrackingOptions {
private:
  ID3D11RefTrackingOptions *m_pID3D11RefTrackingOptions;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11RefTrackingOptions(T *pWrapped) {
    out() << "[CREATE] ID3D11RefTrackingOptions(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11RefTrackingOptions = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RefTrackingOptions), (void **)&m_pID3D11RefTrackingOptions);
    if (m_pID3D11RefTrackingOptions) {
      m_pID3D11RefTrackingOptions->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RefTrackingOptions);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RefTrackingOptions)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RefTrackingOptions << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetTrackingOptions(
    UINT uOptions
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11RefDefaultTrackingOptions
inherits:
  IUnknown
methods:
  SetTrackingOptions
end class
#endif
class WrappedID3D11RefDefaultTrackingOptions : public ID3D11RefDefaultTrackingOptions {
private:
  ID3D11RefDefaultTrackingOptions *m_pID3D11RefDefaultTrackingOptions;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11RefDefaultTrackingOptions(T *pWrapped) {
    out() << "[CREATE] ID3D11RefDefaultTrackingOptions(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11RefDefaultTrackingOptions = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RefDefaultTrackingOptions), (void **)&m_pID3D11RefDefaultTrackingOptions);
    if (m_pID3D11RefDefaultTrackingOptions) {
      m_pID3D11RefDefaultTrackingOptions->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RefDefaultTrackingOptions);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RefDefaultTrackingOptions)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RefDefaultTrackingOptions << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetTrackingOptions(
    UINT ResourceTypeFlags, 
    UINT Options
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11InfoQueue
inherits:
  IUnknown
methods:
  SetMessageCountLimit
  ClearStoredMessages
  GetMessageW
  GetNumMessagesAllowedByStorageFilter
  GetNumMessagesDeniedByStorageFilter
  GetNumStoredMessages
  GetNumStoredMessagesAllowedByRetrievalFilter
  GetNumMessagesDiscardedByMessageCountLimit
  GetMessageCountLimit
  AddStorageFilterEntries
  GetStorageFilter
  ClearStorageFilter
  PushEmptyStorageFilter
  PushCopyOfStorageFilter
  PushStorageFilter
  PopStorageFilter
  GetStorageFilterStackSize
  AddRetrievalFilterEntries
  GetRetrievalFilter
  ClearRetrievalFilter
  PushEmptyRetrievalFilter
  PushCopyOfRetrievalFilter
  PushRetrievalFilter
  PopRetrievalFilter
  GetRetrievalFilterStackSize
  AddMessage
  AddApplicationMessage
  SetBreakOnCategory
  SetBreakOnSeverity
  SetBreakOnID
  GetBreakOnCategory
  GetBreakOnSeverity
  GetBreakOnID
  SetMuteDebugOutput
  GetMuteDebugOutput
end class
#endif
class WrappedID3D11InfoQueue : public ID3D11InfoQueue {
private:
  ID3D11InfoQueue *m_pID3D11InfoQueue;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11InfoQueue(T *pWrapped) {
    out() << "[CREATE] ID3D11InfoQueue(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11InfoQueue = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11InfoQueue), (void **)&m_pID3D11InfoQueue);
    if (m_pID3D11InfoQueue) {
      m_pID3D11InfoQueue->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11InfoQueue);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11InfoQueue)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11InfoQueue << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetMessageCountLimit(
    UINT64 MessageCountLimit
  ) override;
  void __stdcall ClearStoredMessages() override;
  HRESULT __stdcall GetMessageW(
    UINT64 MessageIndex, 
    D3D11_MESSAGE * pMessage, 
    SIZE_T * pMessageByteLength
  ) override;
  UINT64 __stdcall GetNumMessagesAllowedByStorageFilter() override;
  UINT64 __stdcall GetNumMessagesDeniedByStorageFilter() override;
  UINT64 __stdcall GetNumStoredMessages() override;
  UINT64 __stdcall GetNumStoredMessagesAllowedByRetrievalFilter() override;
  UINT64 __stdcall GetNumMessagesDiscardedByMessageCountLimit() override;
  UINT64 __stdcall GetMessageCountLimit() override;
  HRESULT __stdcall AddStorageFilterEntries(
    D3D11_INFO_QUEUE_FILTER * pFilter
  ) override;
  HRESULT __stdcall GetStorageFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter, 
    SIZE_T * pFilterByteLength
  ) override;
  void __stdcall ClearStorageFilter() override;
  HRESULT __stdcall PushEmptyStorageFilter() override;
  HRESULT __stdcall PushCopyOfStorageFilter() override;
  HRESULT __stdcall PushStorageFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter
  ) override;
  void __stdcall PopStorageFilter() override;
  UINT __stdcall GetStorageFilterStackSize() override;
  HRESULT __stdcall AddRetrievalFilterEntries(
    D3D11_INFO_QUEUE_FILTER * pFilter
  ) override;
  HRESULT __stdcall GetRetrievalFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter, 
    SIZE_T * pFilterByteLength
  ) override;
  void __stdcall ClearRetrievalFilter() override;
  HRESULT __stdcall PushEmptyRetrievalFilter() override;
  HRESULT __stdcall PushCopyOfRetrievalFilter() override;
  HRESULT __stdcall PushRetrievalFilter(
    D3D11_INFO_QUEUE_FILTER * pFilter
  ) override;
  void __stdcall PopRetrievalFilter() override;
  UINT __stdcall GetRetrievalFilterStackSize() override;
  HRESULT __stdcall AddMessage(
    D3D11_MESSAGE_CATEGORY Category, 
    D3D11_MESSAGE_SEVERITY Severity, 
    D3D11_MESSAGE_ID ID, 
    LPCSTR pDescription
  ) override;
  HRESULT __stdcall AddApplicationMessage(
    D3D11_MESSAGE_SEVERITY Severity, 
    LPCSTR pDescription
  ) override;
  HRESULT __stdcall SetBreakOnCategory(
    D3D11_MESSAGE_CATEGORY Category, 
    BOOL bEnable
  ) override;
  HRESULT __stdcall SetBreakOnSeverity(
    D3D11_MESSAGE_SEVERITY Severity, 
    BOOL bEnable
  ) override;
  HRESULT __stdcall SetBreakOnID(
    D3D11_MESSAGE_ID ID, 
    BOOL bEnable
  ) override;
  BOOL __stdcall GetBreakOnCategory(
    D3D11_MESSAGE_CATEGORY Category
  ) override;
  BOOL __stdcall GetBreakOnSeverity(
    D3D11_MESSAGE_SEVERITY Severity
  ) override;
  BOOL __stdcall GetBreakOnID(
    D3D11_MESSAGE_ID ID
  ) override;
  void __stdcall SetMuteDebugOutput(
    BOOL bMute
  ) override;
  BOOL __stdcall GetMuteDebugOutput() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3DDeviceContextState
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
end class
#endif
class WrappedID3DDeviceContextState : public ID3DDeviceContextState {
private:
  ID3DDeviceContextState *m_pID3DDeviceContextState;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3DDeviceContextState(T *pWrapped) {
    out() << "[CREATE] ID3DDeviceContextState(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3DDeviceContextState = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3DDeviceContextState), (void **)&m_pID3DDeviceContextState);
    if (m_pID3DDeviceContextState) {
      m_pID3DDeviceContextState->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3DDeviceContextState);
      wt[reinterpret_cast<size_t>((void*)m_pID3DDeviceContextState)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3DDeviceContextState << " -> " << this << "\n";
    }
  }
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIDisplayControl
inherits:
  IUnknown
methods:
  IsStereoEnabled
  SetStereoEnabled
end class
#endif
class WrappedIDXGIDisplayControl : public IDXGIDisplayControl {
private:
  IDXGIDisplayControl *m_pIDXGIDisplayControl;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIDisplayControl(T *pWrapped) {
    out() << "[CREATE] IDXGIDisplayControl(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIDisplayControl = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDisplayControl), (void **)&m_pIDXGIDisplayControl);
    if (m_pIDXGIDisplayControl) {
      m_pIDXGIDisplayControl->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDisplayControl);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDisplayControl)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDisplayControl << " -> " << this << "\n";
    }
  }
  BOOL __stdcall IsStereoEnabled() override;
  void __stdcall SetStereoEnabled(
    BOOL enabled
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIOutputDuplication
inherits:
  IDXGIObject
  IUnknown
methods:
  GetDesc
  AcquireNextFrame
  GetFrameDirtyRects
  GetFrameMoveRects
  GetFramePointerShape
  MapDesktopSurface
  UnMapDesktopSurface
  ReleaseFrame
end class
#endif
class WrappedIDXGIOutputDuplication : public IDXGIOutputDuplication {
private:
  IDXGIOutputDuplication *m_pIDXGIOutputDuplication;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIOutputDuplication(T *pWrapped) {
    out() << "[CREATE] IDXGIOutputDuplication(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIOutputDuplication = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutputDuplication), (void **)&m_pIDXGIOutputDuplication);
    if (m_pIDXGIOutputDuplication) {
      m_pIDXGIOutputDuplication->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutputDuplication);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutputDuplication)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutputDuplication << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc(
    DXGI_OUTDUPL_DESC * pDesc
  ) override;
  HRESULT __stdcall AcquireNextFrame(
    UINT TimeoutInMilliseconds, 
    DXGI_OUTDUPL_FRAME_INFO * pFrameInfo, 
    IDXGIResource ** ppDesktopResource
  ) override;
  HRESULT __stdcall GetFrameDirtyRects(
    UINT DirtyRectsBufferSize, 
    RECT * pDirtyRectsBuffer, 
    UINT * pDirtyRectsBufferSizeRequired
  ) override;
  HRESULT __stdcall GetFrameMoveRects(
    UINT MoveRectsBufferSize, 
    DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer, 
    UINT * pMoveRectsBufferSizeRequired
  ) override;
  HRESULT __stdcall GetFramePointerShape(
    UINT PointerShapeBufferSize, 
    void * pPointerShapeBuffer, 
    UINT * pPointerShapeBufferSizeRequired, 
    DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo
  ) override;
  HRESULT __stdcall MapDesktopSurface(
    DXGI_MAPPED_RECT * pLockedRect
  ) override;
  HRESULT __stdcall UnMapDesktopSurface() override;
  HRESULT __stdcall ReleaseFrame() override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGISurface2
inherits:
  IDXGISurface1
  IDXGISurface
  IDXGIDeviceSubObject
  IDXGIObject
  IUnknown
methods:
  GetResource
end class
#endif
class WrappedIDXGISurface2 : public IDXGISurface2 {
private:
  IDXGISurface2 *m_pIDXGISurface2;
  IDXGISurface1 *m_pIDXGISurface1;
  IDXGISurface *m_pIDXGISurface;
  IDXGIDeviceSubObject *m_pIDXGIDeviceSubObject;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGISurface2(T *pWrapped) {
    out() << "[CREATE] IDXGISurface2(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIDeviceSubObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDeviceSubObject), (void **)&m_pIDXGIDeviceSubObject);
    if (m_pIDXGIDeviceSubObject) {
      m_pIDXGIDeviceSubObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDeviceSubObject << " -> " << this << "\n";
    }
    m_pIDXGISurface = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISurface), (void **)&m_pIDXGISurface);
    if (m_pIDXGISurface) {
      m_pIDXGISurface->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISurface);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISurface)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISurface << " -> " << this << "\n";
    }
    m_pIDXGISurface1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISurface1), (void **)&m_pIDXGISurface1);
    if (m_pIDXGISurface1) {
      m_pIDXGISurface1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISurface1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISurface1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISurface1 << " -> " << this << "\n";
    }
    m_pIDXGISurface2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISurface2), (void **)&m_pIDXGISurface2);
    if (m_pIDXGISurface2) {
      m_pIDXGISurface2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISurface2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISurface2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISurface2 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall GetResource(
    const IID & riid, 
    void ** ppParentResource, 
    UINT * pSubresourceIndex
  ) override;
  HRESULT __stdcall GetDC(
    BOOL Discard, 
    HDC * phdc
  ) override;
  HRESULT __stdcall ReleaseDC(
    RECT * pDirtyRect
  ) override;
  HRESULT __stdcall GetDesc(
    DXGI_SURFACE_DESC * pDesc
  ) override;
  HRESULT __stdcall Map(
    DXGI_MAPPED_RECT * pLockedRect, 
    UINT MapFlags
  ) override;
  HRESULT __stdcall Unmap() override;
  HRESULT __stdcall GetDevice(
    const IID & riid, 
    void ** ppDevice
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIResource1
inherits:
  IDXGIResource
  IDXGIDeviceSubObject
  IDXGIObject
  IUnknown
methods:
  CreateSubresourceSurface
  CreateSharedHandle
end class
#endif
class WrappedIDXGIResource1 : public IDXGIResource1 {
private:
  IDXGIResource1 *m_pIDXGIResource1;
  IDXGIResource *m_pIDXGIResource;
  IDXGIDeviceSubObject *m_pIDXGIDeviceSubObject;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIResource1(T *pWrapped) {
    out() << "[CREATE] IDXGIResource1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIDeviceSubObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDeviceSubObject), (void **)&m_pIDXGIDeviceSubObject);
    if (m_pIDXGIDeviceSubObject) {
      m_pIDXGIDeviceSubObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDeviceSubObject << " -> " << this << "\n";
    }
    m_pIDXGIResource = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIResource), (void **)&m_pIDXGIResource);
    if (m_pIDXGIResource) {
      m_pIDXGIResource->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIResource);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIResource)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIResource << " -> " << this << "\n";
    }
    m_pIDXGIResource1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIResource1), (void **)&m_pIDXGIResource1);
    if (m_pIDXGIResource1) {
      m_pIDXGIResource1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIResource1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIResource1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIResource1 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CreateSubresourceSurface(
    UINT index, 
    IDXGISurface2 ** ppSurface
  ) override;
  HRESULT __stdcall CreateSharedHandle(
    const SECURITY_ATTRIBUTES * pAttributes, 
    DWORD dwAccess, 
    LPCWSTR lpName, 
    HANDLE * pHandle
  ) override;
  HRESULT __stdcall GetSharedHandle(
    HANDLE * pSharedHandle
  ) override;
  HRESULT __stdcall GetUsage(
    DXGI_USAGE * pUsage
  ) override;
  HRESULT __stdcall SetEvictionPriority(
    UINT EvictionPriority
  ) override;
  HRESULT __stdcall GetEvictionPriority(
    UINT * pEvictionPriority
  ) override;
  HRESULT __stdcall GetDevice(
    const IID & riid, 
    void ** ppDevice
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11BlendState1
inherits:
  ID3D11BlendState
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11BlendState1 : public ID3D11BlendState1 {
private:
  ID3D11BlendState1 *m_pID3D11BlendState1;
  ID3D11BlendState *m_pID3D11BlendState;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11BlendState1(T *pWrapped) {
    out() << "[CREATE] ID3D11BlendState1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11BlendState = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11BlendState), (void **)&m_pID3D11BlendState);
    if (m_pID3D11BlendState) {
      m_pID3D11BlendState->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11BlendState);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11BlendState)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11BlendState << " -> " << this << "\n";
    }
    m_pID3D11BlendState1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11BlendState1), (void **)&m_pID3D11BlendState1);
    if (m_pID3D11BlendState1) {
      m_pID3D11BlendState1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11BlendState1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11BlendState1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11BlendState1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_BLEND_DESC1 * pDesc
  ) override;
  void __stdcall GetDesc(
    D3D11_BLEND_DESC * pDesc
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoProcessorEnumerator1
inherits:
  ID3D11VideoProcessorEnumerator
  ID3D11DeviceChild
  IUnknown
methods:
  CheckVideoProcessorFormatConversion
end class
#endif
class WrappedID3D11VideoProcessorEnumerator1 : public ID3D11VideoProcessorEnumerator1 {
private:
  ID3D11VideoProcessorEnumerator1 *m_pID3D11VideoProcessorEnumerator1;
  ID3D11VideoProcessorEnumerator *m_pID3D11VideoProcessorEnumerator;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoProcessorEnumerator1(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoProcessorEnumerator1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11VideoProcessorEnumerator = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoProcessorEnumerator), (void **)&m_pID3D11VideoProcessorEnumerator);
    if (m_pID3D11VideoProcessorEnumerator) {
      m_pID3D11VideoProcessorEnumerator->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorEnumerator);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorEnumerator)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoProcessorEnumerator << " -> " << this << "\n";
    }
    m_pID3D11VideoProcessorEnumerator1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoProcessorEnumerator1), (void **)&m_pID3D11VideoProcessorEnumerator1);
    if (m_pID3D11VideoProcessorEnumerator1) {
      m_pID3D11VideoProcessorEnumerator1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorEnumerator1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoProcessorEnumerator1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoProcessorEnumerator1 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CheckVideoProcessorFormatConversion(
    DXGI_FORMAT InputFormat, 
    DXGI_COLOR_SPACE_TYPE InputColorSpace, 
    DXGI_FORMAT OutputFormat, 
    DXGI_COLOR_SPACE_TYPE OutputColorSpace, 
    BOOL * pSupported
  ) override;
  HRESULT __stdcall GetVideoProcessorContentDesc(
    D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc
  ) override;
  HRESULT __stdcall CheckVideoProcessorFormat(
    DXGI_FORMAT Format, 
    UINT * pFlags
  ) override;
  HRESULT __stdcall GetVideoProcessorCaps(
    D3D11_VIDEO_PROCESSOR_CAPS * pCaps
  ) override;
  HRESULT __stdcall GetVideoProcessorRateConversionCaps(
    UINT TypeIndex, 
    D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps
  ) override;
  HRESULT __stdcall GetVideoProcessorCustomRate(
    UINT TypeIndex, 
    UINT CustomRateIndex, 
    D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate
  ) override;
  HRESULT __stdcall GetVideoProcessorFilterRange(
    D3D11_VIDEO_PROCESSOR_FILTER Filter, 
    D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3DUserDefinedAnnotation
inherits:
  IUnknown
methods:
  BeginEvent
  EndEvent
  SetMarker
  GetStatus
end class
#endif
class WrappedID3DUserDefinedAnnotation : public ID3DUserDefinedAnnotation {
private:
  ID3DUserDefinedAnnotation *m_pID3DUserDefinedAnnotation;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3DUserDefinedAnnotation(T *pWrapped) {
    out() << "[CREATE] ID3DUserDefinedAnnotation(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3DUserDefinedAnnotation = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void **)&m_pID3DUserDefinedAnnotation);
    if (m_pID3DUserDefinedAnnotation) {
      m_pID3DUserDefinedAnnotation->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3DUserDefinedAnnotation);
      wt[reinterpret_cast<size_t>((void*)m_pID3DUserDefinedAnnotation)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3DUserDefinedAnnotation << " -> " << this << "\n";
    }
  }
  INT __stdcall BeginEvent(
    LPCWSTR Name
  ) override;
  INT __stdcall EndEvent() override;
  void __stdcall SetMarker(
    LPCWSTR Name
  ) override;
  BOOL __stdcall GetStatus() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIDecodeSwapChain
inherits:
  IUnknown
methods:
  PresentBuffer
  SetSourceRect
  SetTargetRect
  SetDestSize
  GetSourceRect
  GetTargetRect
  GetDestSize
  SetColorSpace
  GetColorSpace
end class
#endif
class WrappedIDXGIDecodeSwapChain : public IDXGIDecodeSwapChain {
private:
  IDXGIDecodeSwapChain *m_pIDXGIDecodeSwapChain;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIDecodeSwapChain(T *pWrapped) {
    out() << "[CREATE] IDXGIDecodeSwapChain(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIDecodeSwapChain = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDecodeSwapChain), (void **)&m_pIDXGIDecodeSwapChain);
    if (m_pIDXGIDecodeSwapChain) {
      m_pIDXGIDecodeSwapChain->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDecodeSwapChain);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDecodeSwapChain)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDecodeSwapChain << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall PresentBuffer(
    UINT BufferToPresent, 
    UINT SyncInterval, 
    UINT Flags
  ) override;
  HRESULT __stdcall SetSourceRect(
    const RECT * pRect
  ) override;
  HRESULT __stdcall SetTargetRect(
    const RECT * pRect
  ) override;
  HRESULT __stdcall SetDestSize(
    UINT Width, 
    UINT Height
  ) override;
  HRESULT __stdcall GetSourceRect(
    RECT * pRect
  ) override;
  HRESULT __stdcall GetTargetRect(
    RECT * pRect
  ) override;
  HRESULT __stdcall GetDestSize(
    UINT * pWidth, 
    UINT * pHeight
  ) override;
  HRESULT __stdcall SetColorSpace(
    DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS ColorSpace
  ) override;
  DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS __stdcall GetColorSpace() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIFactoryMedia
inherits:
  IUnknown
methods:
  CreateSwapChainForCompositionSurfaceHandle
  CreateDecodeSwapChainForCompositionSurfaceHandle
end class
#endif
class WrappedIDXGIFactoryMedia : public IDXGIFactoryMedia {
private:
  IDXGIFactoryMedia *m_pIDXGIFactoryMedia;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIFactoryMedia(T *pWrapped) {
    out() << "[CREATE] IDXGIFactoryMedia(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIFactoryMedia = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactoryMedia), (void **)&m_pIDXGIFactoryMedia);
    if (m_pIDXGIFactoryMedia) {
      m_pIDXGIFactoryMedia->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactoryMedia);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactoryMedia)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactoryMedia << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CreateSwapChainForCompositionSurfaceHandle(
    struct IUnknown * pDevice, 
    HANDLE hSurface, 
    const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
    IDXGIOutput * pRestrictToOutput, 
    IDXGISwapChain1 ** ppSwapChain
  ) override;
  HRESULT __stdcall CreateDecodeSwapChainForCompositionSurfaceHandle(
    struct IUnknown * pDevice, 
    HANDLE hSurface, 
    DXGI_DECODE_SWAP_CHAIN_DESC * pDesc, 
    IDXGIResource * pYuvDecodeBuffers, 
    IDXGIOutput * pRestrictToOutput, 
    IDXGIDecodeSwapChain ** ppSwapChain
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGISwapChainMedia
inherits:
  IUnknown
methods:
  GetFrameStatisticsMedia
  SetPresentDuration
  CheckPresentDurationSupport
end class
#endif
class WrappedIDXGISwapChainMedia : public IDXGISwapChainMedia {
private:
  IDXGISwapChainMedia *m_pIDXGISwapChainMedia;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGISwapChainMedia(T *pWrapped) {
    out() << "[CREATE] IDXGISwapChainMedia(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGISwapChainMedia = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChainMedia), (void **)&m_pIDXGISwapChainMedia);
    if (m_pIDXGISwapChainMedia) {
      m_pIDXGISwapChainMedia->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChainMedia);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChainMedia)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChainMedia << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall GetFrameStatisticsMedia(
    DXGI_FRAME_STATISTICS_MEDIA * pStats
  ) override;
  HRESULT __stdcall SetPresentDuration(
    UINT Duration
  ) override;
  HRESULT __stdcall CheckPresentDurationSupport(
    UINT DesiredPresentDuration, 
    UINT * pClosestSmallerPresentDuration, 
    UINT * pClosestLargerPresentDuration
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Texture2D1
inherits:
  ID3D11Texture2D
  ID3D11Resource
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11Texture2D1 : public ID3D11Texture2D1 {
private:
  ID3D11Texture2D1 *m_pID3D11Texture2D1;
  ID3D11Texture2D *m_pID3D11Texture2D;
  ID3D11Resource *m_pID3D11Resource;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Texture2D1(T *pWrapped) {
    out() << "[CREATE] ID3D11Texture2D1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Resource = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Resource), (void **)&m_pID3D11Resource);
    if (m_pID3D11Resource) {
      m_pID3D11Resource->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Resource);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Resource)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Resource << " -> " << this << "\n";
    }
    m_pID3D11Texture2D = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Texture2D), (void **)&m_pID3D11Texture2D);
    if (m_pID3D11Texture2D) {
      m_pID3D11Texture2D->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Texture2D);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Texture2D)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Texture2D << " -> " << this << "\n";
    }
    m_pID3D11Texture2D1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Texture2D1), (void **)&m_pID3D11Texture2D1);
    if (m_pID3D11Texture2D1) {
      m_pID3D11Texture2D1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Texture2D1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Texture2D1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Texture2D1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_TEXTURE2D_DESC1 * pDesc
  ) override;
  void __stdcall GetDesc(
    D3D11_TEXTURE2D_DESC * pDesc
  ) override;
  void __stdcall GetType(
    D3D11_RESOURCE_DIMENSION * pResourceDimension
  ) override;
  void __stdcall SetEvictionPriority(
    UINT EvictionPriority
  ) override;
  UINT __stdcall GetEvictionPriority() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Texture3D1
inherits:
  ID3D11Texture3D
  ID3D11Resource
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11Texture3D1 : public ID3D11Texture3D1 {
private:
  ID3D11Texture3D1 *m_pID3D11Texture3D1;
  ID3D11Texture3D *m_pID3D11Texture3D;
  ID3D11Resource *m_pID3D11Resource;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Texture3D1(T *pWrapped) {
    out() << "[CREATE] ID3D11Texture3D1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Resource = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Resource), (void **)&m_pID3D11Resource);
    if (m_pID3D11Resource) {
      m_pID3D11Resource->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Resource);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Resource)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Resource << " -> " << this << "\n";
    }
    m_pID3D11Texture3D = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Texture3D), (void **)&m_pID3D11Texture3D);
    if (m_pID3D11Texture3D) {
      m_pID3D11Texture3D->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Texture3D);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Texture3D)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Texture3D << " -> " << this << "\n";
    }
    m_pID3D11Texture3D1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Texture3D1), (void **)&m_pID3D11Texture3D1);
    if (m_pID3D11Texture3D1) {
      m_pID3D11Texture3D1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Texture3D1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Texture3D1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Texture3D1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_TEXTURE3D_DESC1 * pDesc
  ) override;
  void __stdcall GetDesc(
    D3D11_TEXTURE3D_DESC * pDesc
  ) override;
  void __stdcall GetType(
    D3D11_RESOURCE_DIMENSION * pResourceDimension
  ) override;
  void __stdcall SetEvictionPriority(
    UINT EvictionPriority
  ) override;
  UINT __stdcall GetEvictionPriority() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11RasterizerState2
inherits:
  ID3D11RasterizerState1
  ID3D11RasterizerState
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc2
end class
#endif
class WrappedID3D11RasterizerState2 : public ID3D11RasterizerState2 {
private:
  ID3D11RasterizerState2 *m_pID3D11RasterizerState2;
  ID3D11RasterizerState1 *m_pID3D11RasterizerState1;
  ID3D11RasterizerState *m_pID3D11RasterizerState;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11RasterizerState2(T *pWrapped) {
    out() << "[CREATE] ID3D11RasterizerState2(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11RasterizerState = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RasterizerState), (void **)&m_pID3D11RasterizerState);
    if (m_pID3D11RasterizerState) {
      m_pID3D11RasterizerState->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RasterizerState << " -> " << this << "\n";
    }
    m_pID3D11RasterizerState1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RasterizerState1), (void **)&m_pID3D11RasterizerState1);
    if (m_pID3D11RasterizerState1) {
      m_pID3D11RasterizerState1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RasterizerState1 << " -> " << this << "\n";
    }
    m_pID3D11RasterizerState2 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RasterizerState2), (void **)&m_pID3D11RasterizerState2);
    if (m_pID3D11RasterizerState2) {
      m_pID3D11RasterizerState2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState2);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RasterizerState2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RasterizerState2 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc2(
    D3D11_RASTERIZER_DESC2 * pDesc
  ) override;
  void __stdcall GetDesc1(
    D3D11_RASTERIZER_DESC1 * pDesc
  ) override;
  void __stdcall GetDesc(
    D3D11_RASTERIZER_DESC * pDesc
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11ShaderResourceView1
inherits:
  ID3D11ShaderResourceView
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11ShaderResourceView1 : public ID3D11ShaderResourceView1 {
private:
  ID3D11ShaderResourceView1 *m_pID3D11ShaderResourceView1;
  ID3D11ShaderResourceView *m_pID3D11ShaderResourceView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ShaderResourceView1(T *pWrapped) {
    out() << "[CREATE] ID3D11ShaderResourceView1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11ShaderResourceView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ShaderResourceView), (void **)&m_pID3D11ShaderResourceView);
    if (m_pID3D11ShaderResourceView) {
      m_pID3D11ShaderResourceView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ShaderResourceView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ShaderResourceView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ShaderResourceView << " -> " << this << "\n";
    }
    m_pID3D11ShaderResourceView1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ShaderResourceView1), (void **)&m_pID3D11ShaderResourceView1);
    if (m_pID3D11ShaderResourceView1) {
      m_pID3D11ShaderResourceView1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ShaderResourceView1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ShaderResourceView1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ShaderResourceView1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_SHADER_RESOURCE_VIEW_DESC1 * pDesc1
  ) override;
  void __stdcall GetDesc(
    D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11RenderTargetView1
inherits:
  ID3D11RenderTargetView
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11RenderTargetView1 : public ID3D11RenderTargetView1 {
private:
  ID3D11RenderTargetView1 *m_pID3D11RenderTargetView1;
  ID3D11RenderTargetView *m_pID3D11RenderTargetView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11RenderTargetView1(T *pWrapped) {
    out() << "[CREATE] ID3D11RenderTargetView1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11RenderTargetView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RenderTargetView), (void **)&m_pID3D11RenderTargetView);
    if (m_pID3D11RenderTargetView) {
      m_pID3D11RenderTargetView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RenderTargetView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RenderTargetView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RenderTargetView << " -> " << this << "\n";
    }
    m_pID3D11RenderTargetView1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11RenderTargetView1), (void **)&m_pID3D11RenderTargetView1);
    if (m_pID3D11RenderTargetView1) {
      m_pID3D11RenderTargetView1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11RenderTargetView1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11RenderTargetView1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11RenderTargetView1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_RENDER_TARGET_VIEW_DESC1 * pDesc1
  ) override;
  void __stdcall GetDesc(
    D3D11_RENDER_TARGET_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11UnorderedAccessView1
inherits:
  ID3D11UnorderedAccessView
  ID3D11View
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11UnorderedAccessView1 : public ID3D11UnorderedAccessView1 {
private:
  ID3D11UnorderedAccessView1 *m_pID3D11UnorderedAccessView1;
  ID3D11UnorderedAccessView *m_pID3D11UnorderedAccessView;
  ID3D11View *m_pID3D11View;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11UnorderedAccessView1(T *pWrapped) {
    out() << "[CREATE] ID3D11UnorderedAccessView1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11View = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11View), (void **)&m_pID3D11View);
    if (m_pID3D11View) {
      m_pID3D11View->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11View);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11View)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11View << " -> " << this << "\n";
    }
    m_pID3D11UnorderedAccessView = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11UnorderedAccessView), (void **)&m_pID3D11UnorderedAccessView);
    if (m_pID3D11UnorderedAccessView) {
      m_pID3D11UnorderedAccessView->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11UnorderedAccessView);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11UnorderedAccessView)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11UnorderedAccessView << " -> " << this << "\n";
    }
    m_pID3D11UnorderedAccessView1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11UnorderedAccessView1), (void **)&m_pID3D11UnorderedAccessView1);
    if (m_pID3D11UnorderedAccessView1) {
      m_pID3D11UnorderedAccessView1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11UnorderedAccessView1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11UnorderedAccessView1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11UnorderedAccessView1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_UNORDERED_ACCESS_VIEW_DESC1 * pDesc1
  ) override;
  void __stdcall GetDesc(
    D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc
  ) override;
  void __stdcall GetResource(
    ID3D11Resource ** ppResource
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Query1
inherits:
  ID3D11Query
  ID3D11Asynchronous
  ID3D11DeviceChild
  IUnknown
methods:
  GetDesc1
end class
#endif
class WrappedID3D11Query1 : public ID3D11Query1 {
private:
  ID3D11Query1 *m_pID3D11Query1;
  ID3D11Query *m_pID3D11Query;
  ID3D11Asynchronous *m_pID3D11Asynchronous;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Query1(T *pWrapped) {
    out() << "[CREATE] ID3D11Query1(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Asynchronous = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Asynchronous), (void **)&m_pID3D11Asynchronous);
    if (m_pID3D11Asynchronous) {
      m_pID3D11Asynchronous->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Asynchronous)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Asynchronous << " -> " << this << "\n";
    }
    m_pID3D11Query = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Query), (void **)&m_pID3D11Query);
    if (m_pID3D11Query) {
      m_pID3D11Query->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Query);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Query)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Query << " -> " << this << "\n";
    }
    m_pID3D11Query1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Query1), (void **)&m_pID3D11Query1);
    if (m_pID3D11Query1) {
      m_pID3D11Query1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Query1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Query1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Query1 << " -> " << this << "\n";
    }
  }
  void __stdcall GetDesc1(
    D3D11_QUERY_DESC1 * pDesc1
  ) override;
  void __stdcall GetDesc(
    D3D11_QUERY_DESC * pDesc
  ) override;
  UINT __stdcall GetDataSize() override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Fence
inherits:
  ID3D11DeviceChild
  IUnknown
methods:
  CreateSharedHandle
  GetCompletedValue
  SetEventOnCompletion
end class
#endif
class WrappedID3D11Fence : public ID3D11Fence {
private:
  ID3D11Fence *m_pID3D11Fence;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Fence(T *pWrapped) {
    out() << "[CREATE] ID3D11Fence(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11Fence = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Fence), (void **)&m_pID3D11Fence);
    if (m_pID3D11Fence) {
      m_pID3D11Fence->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Fence);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Fence)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Fence << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CreateSharedHandle(
    const SECURITY_ATTRIBUTES * pAttributes, 
    DWORD dwAccess, 
    LPCWSTR lpName, 
    HANDLE * pHandle
  ) override;
  UINT64 __stdcall GetCompletedValue() override;
  HRESULT __stdcall SetEventOnCompletion(
    UINT64 Value, 
    HANDLE hEvent
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11DeviceContext4
inherits:
  ID3D11DeviceContext3
  ID3D11DeviceContext2
  ID3D11DeviceContext1
  ID3D11DeviceContext
  ID3D11DeviceChild
  IUnknown
methods:
  Signal
  Wait
end class
#endif
class WrappedID3D11DeviceContext4 : public ID3D11DeviceContext4 {
private:
  ID3D11DeviceContext4 *m_pID3D11DeviceContext4;
  ID3D11DeviceContext3 *m_pID3D11DeviceContext3;
  ID3D11DeviceContext2 *m_pID3D11DeviceContext2;
  ID3D11DeviceContext1 *m_pID3D11DeviceContext1;
  ID3D11DeviceContext *m_pID3D11DeviceContext;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11DeviceContext4(T *pWrapped) {
    out() << "[CREATE] ID3D11DeviceContext4(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11DeviceContext = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceContext), (void **)&m_pID3D11DeviceContext);
    if (m_pID3D11DeviceContext) {
      m_pID3D11DeviceContext->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceContext << " -> " << this << "\n";
    }
    m_pID3D11DeviceContext1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&m_pID3D11DeviceContext1);
    if (m_pID3D11DeviceContext1) {
      m_pID3D11DeviceContext1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceContext1 << " -> " << this << "\n";
    }
    m_pID3D11DeviceContext2 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceContext2), (void **)&m_pID3D11DeviceContext2);
    if (m_pID3D11DeviceContext2) {
      m_pID3D11DeviceContext2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext2);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceContext2 << " -> " << this << "\n";
    }
    m_pID3D11DeviceContext3 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceContext3), (void **)&m_pID3D11DeviceContext3);
    if (m_pID3D11DeviceContext3) {
      m_pID3D11DeviceContext3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext3);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceContext3 << " -> " << this << "\n";
    }
    m_pID3D11DeviceContext4 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceContext4), (void **)&m_pID3D11DeviceContext4);
    if (m_pID3D11DeviceContext4) {
      m_pID3D11DeviceContext4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext4);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceContext4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceContext4 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall Signal(
    ID3D11Fence * pFence, 
    UINT64 Value
  ) override;
  HRESULT __stdcall Wait(
    ID3D11Fence * pFence, 
    UINT64 Value
  ) override;
  void __stdcall Flush1(
    D3D11_CONTEXT_TYPE ContextType, 
    HANDLE hEvent
  ) override;
  void __stdcall SetHardwareProtectionState(
    BOOL HwProtectionEnable
  ) override;
  void __stdcall GetHardwareProtectionState(
    BOOL * pHwProtectionEnable
  ) override;
  HRESULT __stdcall UpdateTileMappings(
    ID3D11Resource * pTiledResource, 
    UINT NumTiledResourceRegions, 
    const D3D11_TILED_RESOURCE_COORDINATE * pTiledResourceRegionStartCoordinates, 
    const D3D11_TILE_REGION_SIZE * pTiledResourceRegionSizes, 
    ID3D11Buffer * pTilePool, 
    UINT NumRanges, 
    const UINT * pRangeFlags, 
    const UINT * pTilePoolStartOffsets, 
    const UINT * pRangeTileCounts, 
    UINT Flags
  ) override;
  HRESULT __stdcall CopyTileMappings(
    ID3D11Resource * pDestTiledResource, 
    const D3D11_TILED_RESOURCE_COORDINATE * pDestRegionStartCoordinate, 
    ID3D11Resource * pSourceTiledResource, 
    const D3D11_TILED_RESOURCE_COORDINATE * pSourceRegionStartCoordinate, 
    const D3D11_TILE_REGION_SIZE * pTileRegionSize, 
    UINT Flags
  ) override;
  void __stdcall CopyTiles(
    ID3D11Resource * pTiledResource, 
    const D3D11_TILED_RESOURCE_COORDINATE * pTileRegionStartCoordinate, 
    const D3D11_TILE_REGION_SIZE * pTileRegionSize, 
    ID3D11Buffer * pBuffer, 
    UINT64 BufferStartOffsetInBytes, 
    UINT Flags
  ) override;
  void __stdcall UpdateTiles(
    ID3D11Resource * pDestTiledResource, 
    const D3D11_TILED_RESOURCE_COORDINATE * pDestTileRegionStartCoordinate, 
    const D3D11_TILE_REGION_SIZE * pDestTileRegionSize, 
    const void * pSourceTileData, 
    UINT Flags
  ) override;
  HRESULT __stdcall ResizeTilePool(
    ID3D11Buffer * pTilePool, 
    UINT64 NewSizeInBytes
  ) override;
  void __stdcall TiledResourceBarrier(
    struct ID3D11DeviceChild * pTiledResourceOrViewAccessBeforeBarrier, 
    struct ID3D11DeviceChild * pTiledResourceOrViewAccessAfterBarrier
  ) override;
  BOOL __stdcall IsAnnotationEnabled() override;
  void __stdcall SetMarkerInt(
    LPCWSTR pLabel, 
    INT Data
  ) override;
  void __stdcall BeginEventInt(
    LPCWSTR pLabel, 
    INT Data
  ) override;
  void __stdcall EndEvent() override;
  void __stdcall CopySubresourceRegion1(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    UINT DstX, 
    UINT DstY, 
    UINT DstZ, 
    ID3D11Resource * pSrcResource, 
    UINT SrcSubresource, 
    const D3D11_BOX * pSrcBox, 
    UINT CopyFlags
  ) override;
  void __stdcall UpdateSubresource1(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    const D3D11_BOX * pDstBox, 
    const void * pSrcData, 
    UINT SrcRowPitch, 
    UINT SrcDepthPitch, 
    UINT CopyFlags
  ) override;
  void __stdcall DiscardResource(
    ID3D11Resource * pResource
  ) override;
  void __stdcall DiscardView(
    ID3D11View * pResourceView
  ) override;
  void __stdcall VSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall HSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall DSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall GSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall PSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall CSSetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers, 
    const UINT * pFirstConstant, 
    const UINT * pNumConstants
  ) override;
  void __stdcall VSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall HSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall DSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall GSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall PSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall CSGetConstantBuffers1(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers, 
    UINT * pFirstConstant, 
    UINT * pNumConstants
  ) override;
  void __stdcall SwapDeviceContextState(
    ID3DDeviceContextState * pState, 
    ID3DDeviceContextState ** ppPreviousState
  ) override;
  void __stdcall ClearView(
    ID3D11View * pView, 
    const FLOAT * Color, 
    const D3D11_RECT * pRect, 
    UINT NumRects
  ) override;
  void __stdcall DiscardView1(
    ID3D11View * pResourceView, 
    const D3D11_RECT * pRects, 
    UINT NumRects
  ) override;
  void __stdcall VSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall PSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall PSSetShader(
    ID3D11PixelShader * pPixelShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall PSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall VSSetShader(
    ID3D11VertexShader * pVertexShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall DrawIndexed(
    UINT IndexCount, 
    UINT StartIndexLocation, 
    INT BaseVertexLocation
  ) override;
  void __stdcall Draw(
    UINT VertexCount, 
    UINT StartVertexLocation
  ) override;
  HRESULT __stdcall Map(
    ID3D11Resource * pResource, 
    UINT Subresource, 
    D3D11_MAP MapType, 
    UINT MapFlags, 
    D3D11_MAPPED_SUBRESOURCE * pMappedResource
  ) override;
  void __stdcall Unmap(
    ID3D11Resource * pResource, 
    UINT Subresource
  ) override;
  void __stdcall PSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall IASetInputLayout(
    ID3D11InputLayout * pInputLayout
  ) override;
  void __stdcall IASetVertexBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppVertexBuffers, 
    const UINT * pStrides, 
    const UINT * pOffsets
  ) override;
  void __stdcall IASetIndexBuffer(
    ID3D11Buffer * pIndexBuffer, 
    DXGI_FORMAT Format, 
    UINT Offset
  ) override;
  void __stdcall DrawIndexedInstanced(
    UINT IndexCountPerInstance, 
    UINT InstanceCount, 
    UINT StartIndexLocation, 
    INT BaseVertexLocation, 
    UINT StartInstanceLocation
  ) override;
  void __stdcall DrawInstanced(
    UINT VertexCountPerInstance, 
    UINT InstanceCount, 
    UINT StartVertexLocation, 
    UINT StartInstanceLocation
  ) override;
  void __stdcall GSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall GSSetShader(
    ID3D11GeometryShader * pShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY Topology
  ) override;
  void __stdcall VSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall VSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall Begin(
    ID3D11Asynchronous * pAsync
  ) override;
  void __stdcall End(
    ID3D11Asynchronous * pAsync
  ) override;
  HRESULT __stdcall GetData(
    ID3D11Asynchronous * pAsync, 
    void * pData, 
    UINT DataSize, 
    UINT GetDataFlags
  ) override;
  void __stdcall SetPredication(
    ID3D11Predicate * pPredicate, 
    BOOL PredicateValue
  ) override;
  void __stdcall GSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall GSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall OMSetRenderTargets(
    UINT NumViews, 
    ID3D11RenderTargetView *const * ppRenderTargetViews, 
    ID3D11DepthStencilView * pDepthStencilView
  ) override;
  void __stdcall OMSetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs, 
    ID3D11RenderTargetView *const * ppRenderTargetViews, 
    ID3D11DepthStencilView * pDepthStencilView, 
    UINT UAVStartSlot, 
    UINT NumUAVs, 
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, 
    const UINT * pUAVInitialCounts
  ) override;
  void __stdcall OMSetBlendState(
    ID3D11BlendState * pBlendState, 
    const FLOAT * BlendFactor, 
    UINT SampleMask
  ) override;
  void __stdcall OMSetDepthStencilState(
    ID3D11DepthStencilState * pDepthStencilState, 
    UINT StencilRef
  ) override;
  void __stdcall SOSetTargets(
    UINT NumBuffers, 
    ID3D11Buffer *const * ppSOTargets, 
    const UINT * pOffsets
  ) override;
  void __stdcall DrawAuto() override;
  void __stdcall DrawIndexedInstancedIndirect(
    ID3D11Buffer * pBufferForArgs, 
    UINT AlignedByteOffsetForArgs
  ) override;
  void __stdcall DrawInstancedIndirect(
    ID3D11Buffer * pBufferForArgs, 
    UINT AlignedByteOffsetForArgs
  ) override;
  void __stdcall Dispatch(
    UINT ThreadGroupCountX, 
    UINT ThreadGroupCountY, 
    UINT ThreadGroupCountZ
  ) override;
  void __stdcall DispatchIndirect(
    ID3D11Buffer * pBufferForArgs, 
    UINT AlignedByteOffsetForArgs
  ) override;
  void __stdcall RSSetState(
    ID3D11RasterizerState * pRasterizerState
  ) override;
  void __stdcall RSSetViewports(
    UINT NumViewports, 
    const D3D11_VIEWPORT * pViewports
  ) override;
  void __stdcall RSSetScissorRects(
    UINT NumRects, 
    const D3D11_RECT * pRects
  ) override;
  void __stdcall CopySubresourceRegion(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    UINT DstX, 
    UINT DstY, 
    UINT DstZ, 
    ID3D11Resource * pSrcResource, 
    UINT SrcSubresource, 
    const D3D11_BOX * pSrcBox
  ) override;
  void __stdcall CopyResource(
    ID3D11Resource * pDstResource, 
    ID3D11Resource * pSrcResource
  ) override;
  void __stdcall UpdateSubresource(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    const D3D11_BOX * pDstBox, 
    const void * pSrcData, 
    UINT SrcRowPitch, 
    UINT SrcDepthPitch
  ) override;
  void __stdcall CopyStructureCount(
    ID3D11Buffer * pDstBuffer, 
    UINT DstAlignedByteOffset, 
    ID3D11UnorderedAccessView * pSrcView
  ) override;
  void __stdcall ClearRenderTargetView(
    ID3D11RenderTargetView * pRenderTargetView, 
    const FLOAT * ColorRGBA
  ) override;
  void __stdcall ClearUnorderedAccessViewUint(
    ID3D11UnorderedAccessView * pUnorderedAccessView, 
    const UINT * Values
  ) override;
  void __stdcall ClearUnorderedAccessViewFloat(
    ID3D11UnorderedAccessView * pUnorderedAccessView, 
    const FLOAT * Values
  ) override;
  void __stdcall ClearDepthStencilView(
    ID3D11DepthStencilView * pDepthStencilView, 
    UINT ClearFlags, 
    FLOAT Depth, 
    UINT8 Stencil
  ) override;
  void __stdcall GenerateMips(
    ID3D11ShaderResourceView * pShaderResourceView
  ) override;
  void __stdcall SetResourceMinLOD(
    ID3D11Resource * pResource, 
    FLOAT MinLOD
  ) override;
  FLOAT __stdcall GetResourceMinLOD(
    ID3D11Resource * pResource
  ) override;
  void __stdcall ResolveSubresource(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    ID3D11Resource * pSrcResource, 
    UINT SrcSubresource, 
    DXGI_FORMAT Format
  ) override;
  void __stdcall ExecuteCommandList(
    ID3D11CommandList * pCommandList, 
    BOOL RestoreContextState
  ) override;
  void __stdcall HSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall HSSetShader(
    ID3D11HullShader * pHullShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall HSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall HSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall DSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall DSSetShader(
    ID3D11DomainShader * pDomainShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall DSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall DSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall CSSetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView *const * ppShaderResourceViews
  ) override;
  void __stdcall CSSetUnorderedAccessViews(
    UINT StartSlot, 
    UINT NumUAVs, 
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, 
    const UINT * pUAVInitialCounts
  ) override;
  void __stdcall CSSetShader(
    ID3D11ComputeShader * pComputeShader, 
    ID3D11ClassInstance *const * ppClassInstances, 
    UINT NumClassInstances
  ) override;
  void __stdcall CSSetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState *const * ppSamplers
  ) override;
  void __stdcall CSSetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer *const * ppConstantBuffers
  ) override;
  void __stdcall VSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall PSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall PSGetShader(
    ID3D11PixelShader ** ppPixelShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall PSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall VSGetShader(
    ID3D11VertexShader ** ppVertexShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall PSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall IAGetInputLayout(
    ID3D11InputLayout ** ppInputLayout
  ) override;
  void __stdcall IAGetVertexBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppVertexBuffers, 
    UINT * pStrides, 
    UINT * pOffsets
  ) override;
  void __stdcall IAGetIndexBuffer(
    ID3D11Buffer ** pIndexBuffer, 
    DXGI_FORMAT * Format, 
    UINT * Offset
  ) override;
  void __stdcall GSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall GSGetShader(
    ID3D11GeometryShader ** ppGeometryShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall IAGetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY * pTopology
  ) override;
  void __stdcall VSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall VSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall GetPredication(
    ID3D11Predicate ** ppPredicate, 
    BOOL * pPredicateValue
  ) override;
  void __stdcall GSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall GSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall OMGetRenderTargets(
    UINT NumViews, 
    ID3D11RenderTargetView ** ppRenderTargetViews, 
    ID3D11DepthStencilView ** ppDepthStencilView
  ) override;
  void __stdcall OMGetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs, 
    ID3D11RenderTargetView ** ppRenderTargetViews, 
    ID3D11DepthStencilView ** ppDepthStencilView, 
    UINT UAVStartSlot, 
    UINT NumUAVs, 
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews
  ) override;
  void __stdcall OMGetBlendState(
    ID3D11BlendState ** ppBlendState, 
    FLOAT * BlendFactor, 
    UINT * pSampleMask
  ) override;
  void __stdcall OMGetDepthStencilState(
    ID3D11DepthStencilState ** ppDepthStencilState, 
    UINT * pStencilRef
  ) override;
  void __stdcall SOGetTargets(
    UINT NumBuffers, 
    ID3D11Buffer ** ppSOTargets
  ) override;
  void __stdcall RSGetState(
    ID3D11RasterizerState ** ppRasterizerState
  ) override;
  void __stdcall RSGetViewports(
    UINT * pNumViewports, 
    D3D11_VIEWPORT * pViewports
  ) override;
  void __stdcall RSGetScissorRects(
    UINT * pNumRects, 
    D3D11_RECT * pRects
  ) override;
  void __stdcall HSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall HSGetShader(
    ID3D11HullShader ** ppHullShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall HSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall HSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall DSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall DSGetShader(
    ID3D11DomainShader ** ppDomainShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall DSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall DSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall CSGetShaderResources(
    UINT StartSlot, 
    UINT NumViews, 
    ID3D11ShaderResourceView ** ppShaderResourceViews
  ) override;
  void __stdcall CSGetUnorderedAccessViews(
    UINT StartSlot, 
    UINT NumUAVs, 
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews
  ) override;
  void __stdcall CSGetShader(
    ID3D11ComputeShader ** ppComputeShader, 
    ID3D11ClassInstance ** ppClassInstances, 
    UINT * pNumClassInstances
  ) override;
  void __stdcall CSGetSamplers(
    UINT StartSlot, 
    UINT NumSamplers, 
    ID3D11SamplerState ** ppSamplers
  ) override;
  void __stdcall CSGetConstantBuffers(
    UINT StartSlot, 
    UINT NumBuffers, 
    ID3D11Buffer ** ppConstantBuffers
  ) override;
  void __stdcall ClearState() override;
  void __stdcall Flush() override;
  D3D11_DEVICE_CONTEXT_TYPE __stdcall GetType() override;
  UINT __stdcall GetContextFlags() override;
  HRESULT __stdcall FinishCommandList(
    BOOL RestoreDeferredContextState, 
    ID3D11CommandList ** ppCommandList
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIAdapter3
inherits:
  IDXGIAdapter2
  IDXGIAdapter1
  IDXGIAdapter
  IDXGIObject
  IUnknown
methods:
  RegisterHardwareContentProtectionTeardownStatusEvent
  UnregisterHardwareContentProtectionTeardownStatus
  QueryVideoMemoryInfo
  SetVideoMemoryReservation
  RegisterVideoMemoryBudgetChangeNotificationEvent
  UnregisterVideoMemoryBudgetChangeNotification
end class
#endif
class WrappedIDXGIAdapter3 : public IDXGIAdapter3 {
private:
  IDXGIAdapter3 *m_pIDXGIAdapter3;
  IDXGIAdapter2 *m_pIDXGIAdapter2;
  IDXGIAdapter1 *m_pIDXGIAdapter1;
  IDXGIAdapter *m_pIDXGIAdapter;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIAdapter3(T *pWrapped) {
    out() << "[CREATE] IDXGIAdapter3(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIAdapter = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIAdapter), (void **)&m_pIDXGIAdapter);
    if (m_pIDXGIAdapter) {
      m_pIDXGIAdapter->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIAdapter);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIAdapter)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIAdapter << " -> " << this << "\n";
    }
    m_pIDXGIAdapter1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIAdapter1), (void **)&m_pIDXGIAdapter1);
    if (m_pIDXGIAdapter1) {
      m_pIDXGIAdapter1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIAdapter1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIAdapter1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIAdapter1 << " -> " << this << "\n";
    }
    m_pIDXGIAdapter2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIAdapter2), (void **)&m_pIDXGIAdapter2);
    if (m_pIDXGIAdapter2) {
      m_pIDXGIAdapter2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIAdapter2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIAdapter2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIAdapter2 << " -> " << this << "\n";
    }
    m_pIDXGIAdapter3 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIAdapter3), (void **)&m_pIDXGIAdapter3);
    if (m_pIDXGIAdapter3) {
      m_pIDXGIAdapter3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIAdapter3);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIAdapter3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIAdapter3 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall RegisterHardwareContentProtectionTeardownStatusEvent(
    HANDLE hEvent, 
    DWORD * pdwCookie
  ) override;
  void __stdcall UnregisterHardwareContentProtectionTeardownStatus(
    DWORD dwCookie
  ) override;
  HRESULT __stdcall QueryVideoMemoryInfo(
    UINT NodeIndex, 
    DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, 
    DXGI_QUERY_VIDEO_MEMORY_INFO * pVideoMemoryInfo
  ) override;
  HRESULT __stdcall SetVideoMemoryReservation(
    UINT NodeIndex, 
    DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, 
    UINT64 Reservation
  ) override;
  HRESULT __stdcall RegisterVideoMemoryBudgetChangeNotificationEvent(
    HANDLE hEvent, 
    DWORD * pdwCookie
  ) override;
  void __stdcall UnregisterVideoMemoryBudgetChangeNotification(
    DWORD dwCookie
  ) override;
  HRESULT __stdcall GetDesc2(
    DXGI_ADAPTER_DESC2 * pDesc
  ) override;
  HRESULT __stdcall GetDesc1(
    DXGI_ADAPTER_DESC1 * pDesc
  ) override;
  HRESULT __stdcall EnumOutputs(
    UINT Output, 
    IDXGIOutput ** ppOutput
  ) override;
  HRESULT __stdcall GetDesc(
    DXGI_ADAPTER_DESC * pDesc
  ) override;
  HRESULT __stdcall CheckInterfaceSupport(
    const GUID & InterfaceName, 
    LARGE_INTEGER * pUMDVersion
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIOutput5
inherits:
  IDXGIOutput4
  IDXGIOutput3
  IDXGIOutput2
  IDXGIOutput1
  IDXGIOutput
  IDXGIObject
  IUnknown
methods:
  DuplicateOutput1
end class
#endif
class WrappedIDXGIOutput5 : public IDXGIOutput5 {
private:
  IDXGIOutput5 *m_pIDXGIOutput5;
  IDXGIOutput4 *m_pIDXGIOutput4;
  IDXGIOutput3 *m_pIDXGIOutput3;
  IDXGIOutput2 *m_pIDXGIOutput2;
  IDXGIOutput1 *m_pIDXGIOutput1;
  IDXGIOutput *m_pIDXGIOutput;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIOutput5(T *pWrapped) {
    out() << "[CREATE] IDXGIOutput5(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIOutput = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput), (void **)&m_pIDXGIOutput);
    if (m_pIDXGIOutput) {
      m_pIDXGIOutput->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput << " -> " << this << "\n";
    }
    m_pIDXGIOutput1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput1), (void **)&m_pIDXGIOutput1);
    if (m_pIDXGIOutput1) {
      m_pIDXGIOutput1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput1 << " -> " << this << "\n";
    }
    m_pIDXGIOutput2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput2), (void **)&m_pIDXGIOutput2);
    if (m_pIDXGIOutput2) {
      m_pIDXGIOutput2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput2 << " -> " << this << "\n";
    }
    m_pIDXGIOutput3 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput3), (void **)&m_pIDXGIOutput3);
    if (m_pIDXGIOutput3) {
      m_pIDXGIOutput3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput3);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput3 << " -> " << this << "\n";
    }
    m_pIDXGIOutput4 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput4), (void **)&m_pIDXGIOutput4);
    if (m_pIDXGIOutput4) {
      m_pIDXGIOutput4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput4);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput4 << " -> " << this << "\n";
    }
    m_pIDXGIOutput5 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIOutput5), (void **)&m_pIDXGIOutput5);
    if (m_pIDXGIOutput5) {
      m_pIDXGIOutput5->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIOutput5);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIOutput5)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIOutput5 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall DuplicateOutput1(
    struct IUnknown * pDevice, 
    UINT Flags, 
    UINT SupportedFormatsCount, 
    const DXGI_FORMAT * pSupportedFormats, 
    IDXGIOutputDuplication ** ppOutputDuplication
  ) override;
  HRESULT __stdcall CheckOverlayColorSpaceSupport(
    DXGI_FORMAT Format, 
    DXGI_COLOR_SPACE_TYPE ColorSpace, 
    struct IUnknown * pConcernedDevice, 
    UINT * pFlags
  ) override;
  HRESULT __stdcall CheckOverlaySupport(
    DXGI_FORMAT EnumFormat, 
    struct IUnknown * pConcernedDevice, 
    UINT * pFlags
  ) override;
  BOOL __stdcall SupportsOverlays() override;
  HRESULT __stdcall GetDisplayModeList1(
    DXGI_FORMAT EnumFormat, 
    UINT Flags, 
    UINT * pNumModes, 
    DXGI_MODE_DESC1 * pDesc
  ) override;
  HRESULT __stdcall FindClosestMatchingMode1(
    const DXGI_MODE_DESC1 * pModeToMatch, 
    DXGI_MODE_DESC1 * pClosestMatch, 
    struct IUnknown * pConcernedDevice
  ) override;
  HRESULT __stdcall GetDisplaySurfaceData1(
    IDXGIResource * pDestination
  ) override;
  HRESULT __stdcall DuplicateOutput(
    struct IUnknown * pDevice, 
    IDXGIOutputDuplication ** ppOutputDuplication
  ) override;
  HRESULT __stdcall GetDesc(
    DXGI_OUTPUT_DESC * pDesc
  ) override;
  HRESULT __stdcall GetDisplayModeList(
    DXGI_FORMAT EnumFormat, 
    UINT Flags, 
    UINT * pNumModes, 
    DXGI_MODE_DESC * pDesc
  ) override;
  HRESULT __stdcall FindClosestMatchingMode(
    const DXGI_MODE_DESC * pModeToMatch, 
    DXGI_MODE_DESC * pClosestMatch, 
    struct IUnknown * pConcernedDevice
  ) override;
  HRESULT __stdcall WaitForVBlank() override;
  HRESULT __stdcall TakeOwnership(
    struct IUnknown * pDevice, 
    BOOL Exclusive
  ) override;
  void __stdcall ReleaseOwnership() override;
  HRESULT __stdcall GetGammaControlCapabilities(
    DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps
  ) override;
  HRESULT __stdcall SetGammaControl(
    const DXGI_GAMMA_CONTROL * pArray
  ) override;
  HRESULT __stdcall GetGammaControl(
    DXGI_GAMMA_CONTROL * pArray
  ) override;
  HRESULT __stdcall SetDisplaySurface(
    IDXGISurface * pScanoutSurface
  ) override;
  HRESULT __stdcall GetDisplaySurfaceData(
    IDXGISurface * pDestination
  ) override;
  HRESULT __stdcall GetFrameStatistics(
    DXGI_FRAME_STATISTICS * pStats
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGISwapChain4
inherits:
  IDXGISwapChain3
  IDXGISwapChain2
  IDXGISwapChain1
  IDXGISwapChain
  IDXGIDeviceSubObject
  IDXGIObject
  IUnknown
methods:
  SetHDRMetaData
end class
#endif
class WrappedIDXGISwapChain4 : public IDXGISwapChain4 {
private:
  IDXGISwapChain4 *m_pIDXGISwapChain4;
  IDXGISwapChain3 *m_pIDXGISwapChain3;
  IDXGISwapChain2 *m_pIDXGISwapChain2;
  IDXGISwapChain1 *m_pIDXGISwapChain1;
  IDXGISwapChain *m_pIDXGISwapChain;
  IDXGIDeviceSubObject *m_pIDXGIDeviceSubObject;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGISwapChain4(T *pWrapped) {
    out() << "[CREATE] IDXGISwapChain4(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIDeviceSubObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDeviceSubObject), (void **)&m_pIDXGIDeviceSubObject);
    if (m_pIDXGIDeviceSubObject) {
      m_pIDXGIDeviceSubObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDeviceSubObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDeviceSubObject << " -> " << this << "\n";
    }
    m_pIDXGISwapChain = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChain), (void **)&m_pIDXGISwapChain);
    if (m_pIDXGISwapChain) {
      m_pIDXGISwapChain->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChain);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChain)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChain << " -> " << this << "\n";
    }
    m_pIDXGISwapChain1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChain1), (void **)&m_pIDXGISwapChain1);
    if (m_pIDXGISwapChain1) {
      m_pIDXGISwapChain1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChain1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChain1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChain1 << " -> " << this << "\n";
    }
    m_pIDXGISwapChain2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChain2), (void **)&m_pIDXGISwapChain2);
    if (m_pIDXGISwapChain2) {
      m_pIDXGISwapChain2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChain2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChain2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChain2 << " -> " << this << "\n";
    }
    m_pIDXGISwapChain3 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChain3), (void **)&m_pIDXGISwapChain3);
    if (m_pIDXGISwapChain3) {
      m_pIDXGISwapChain3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChain3);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChain3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChain3 << " -> " << this << "\n";
    }
    m_pIDXGISwapChain4 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGISwapChain4), (void **)&m_pIDXGISwapChain4);
    if (m_pIDXGISwapChain4) {
      m_pIDXGISwapChain4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGISwapChain4);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGISwapChain4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGISwapChain4 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall SetHDRMetaData(
    DXGI_HDR_METADATA_TYPE Type, 
    UINT Size, 
    void * pMetaData
  ) override;
  UINT __stdcall GetCurrentBackBufferIndex() override;
  HRESULT __stdcall CheckColorSpaceSupport(
    DXGI_COLOR_SPACE_TYPE ColorSpace, 
    UINT * pColorSpaceSupport
  ) override;
  HRESULT __stdcall SetColorSpace1(
    DXGI_COLOR_SPACE_TYPE ColorSpace
  ) override;
  HRESULT __stdcall ResizeBuffers1(
    UINT BufferCount, 
    UINT Width, 
    UINT Height, 
    DXGI_FORMAT Format, 
    UINT SwapChainFlags, 
    const UINT * pCreationNodeMask, 
    struct IUnknown *const * ppPresentQueue
  ) override;
  HRESULT __stdcall SetSourceSize(
    UINT Width, 
    UINT Height
  ) override;
  HRESULT __stdcall GetSourceSize(
    UINT * pWidth, 
    UINT * pHeight
  ) override;
  HRESULT __stdcall SetMaximumFrameLatency(
    UINT MaxLatency
  ) override;
  HRESULT __stdcall GetMaximumFrameLatency(
    UINT * pMaxLatency
  ) override;
  HANDLE __stdcall GetFrameLatencyWaitableObject() override;
  HRESULT __stdcall SetMatrixTransform(
    const DXGI_MATRIX_3X2_F * pMatrix
  ) override;
  HRESULT __stdcall GetMatrixTransform(
    DXGI_MATRIX_3X2_F * pMatrix
  ) override;
  HRESULT __stdcall GetDesc1(
    DXGI_SWAP_CHAIN_DESC1 * pDesc
  ) override;
  HRESULT __stdcall GetFullscreenDesc(
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc
  ) override;
  HRESULT __stdcall GetHwnd(
    HWND * pHwnd
  ) override;
  HRESULT __stdcall GetCoreWindow(
    const IID & refiid, 
    void ** ppUnk
  ) override;
  HRESULT __stdcall Present1(
    UINT SyncInterval, 
    UINT PresentFlags, 
    const DXGI_PRESENT_PARAMETERS * pPresentParameters
  ) override;
  BOOL __stdcall IsTemporaryMonoSupported() override;
  HRESULT __stdcall GetRestrictToOutput(
    IDXGIOutput ** ppRestrictToOutput
  ) override;
  HRESULT __stdcall SetBackgroundColor(
    const DXGI_RGBA * pColor
  ) override;
  HRESULT __stdcall GetBackgroundColor(
    DXGI_RGBA * pColor
  ) override;
  HRESULT __stdcall SetRotation(
    DXGI_MODE_ROTATION Rotation
  ) override;
  HRESULT __stdcall GetRotation(
    DXGI_MODE_ROTATION * pRotation
  ) override;
  HRESULT __stdcall Present(
    UINT SyncInterval, 
    UINT Flags
  ) override;
  HRESULT __stdcall GetBuffer(
    UINT Buffer, 
    const IID & riid, 
    void ** ppSurface
  ) override;
  HRESULT __stdcall SetFullscreenState(
    BOOL Fullscreen, 
    IDXGIOutput * pTarget
  ) override;
  HRESULT __stdcall GetFullscreenState(
    BOOL * pFullscreen, 
    IDXGIOutput ** ppTarget
  ) override;
  HRESULT __stdcall GetDesc(
    DXGI_SWAP_CHAIN_DESC * pDesc
  ) override;
  HRESULT __stdcall ResizeBuffers(
    UINT BufferCount, 
    UINT Width, 
    UINT Height, 
    DXGI_FORMAT NewFormat, 
    UINT SwapChainFlags
  ) override;
  HRESULT __stdcall ResizeTarget(
    const DXGI_MODE_DESC * pNewTargetParameters
  ) override;
  HRESULT __stdcall GetContainingOutput(
    IDXGIOutput ** ppOutput
  ) override;
  HRESULT __stdcall GetFrameStatistics(
    DXGI_FRAME_STATISTICS * pStats
  ) override;
  HRESULT __stdcall GetLastPresentCount(
    UINT * pLastPresentCount
  ) override;
  HRESULT __stdcall GetDevice(
    const IID & riid, 
    void ** ppDevice
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIDevice4
inherits:
  IDXGIDevice3
  IDXGIDevice2
  IDXGIDevice1
  IDXGIDevice
  IDXGIObject
  IUnknown
methods:
  OfferResources1
  ReclaimResources1
end class
#endif
class WrappedIDXGIDevice4 : public IDXGIDevice4 {
private:
  IDXGIDevice4 *m_pIDXGIDevice4;
  IDXGIDevice3 *m_pIDXGIDevice3;
  IDXGIDevice2 *m_pIDXGIDevice2;
  IDXGIDevice1 *m_pIDXGIDevice1;
  IDXGIDevice *m_pIDXGIDevice;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIDevice4(T *pWrapped) {
    out() << "[CREATE] IDXGIDevice4(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIDevice = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_pIDXGIDevice);
    if (m_pIDXGIDevice) {
      m_pIDXGIDevice->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDevice);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDevice)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDevice << " -> " << this << "\n";
    }
    m_pIDXGIDevice1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDevice1), (void **)&m_pIDXGIDevice1);
    if (m_pIDXGIDevice1) {
      m_pIDXGIDevice1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDevice1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDevice1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDevice1 << " -> " << this << "\n";
    }
    m_pIDXGIDevice2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDevice2), (void **)&m_pIDXGIDevice2);
    if (m_pIDXGIDevice2) {
      m_pIDXGIDevice2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDevice2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDevice2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDevice2 << " -> " << this << "\n";
    }
    m_pIDXGIDevice3 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDevice3), (void **)&m_pIDXGIDevice3);
    if (m_pIDXGIDevice3) {
      m_pIDXGIDevice3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDevice3);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDevice3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDevice3 << " -> " << this << "\n";
    }
    m_pIDXGIDevice4 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIDevice4), (void **)&m_pIDXGIDevice4);
    if (m_pIDXGIDevice4) {
      m_pIDXGIDevice4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIDevice4);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIDevice4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIDevice4 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall OfferResources1(
    UINT NumResources, 
    IDXGIResource *const * ppResources, 
    DXGI_OFFER_RESOURCE_PRIORITY Priority, 
    UINT Flags
  ) override;
  HRESULT __stdcall ReclaimResources1(
    UINT NumResources, 
    IDXGIResource *const * ppResources, 
    DXGI_RECLAIM_RESOURCE_RESULTS * pResults
  ) override;
  void __stdcall Trim() override;
  HRESULT __stdcall OfferResources(
    UINT NumResources, 
    IDXGIResource *const * ppResources, 
    DXGI_OFFER_RESOURCE_PRIORITY Priority
  ) override;
  HRESULT __stdcall ReclaimResources(
    UINT NumResources, 
    IDXGIResource *const * ppResources, 
    BOOL * pDiscarded
  ) override;
  HRESULT __stdcall EnqueueSetEvent(
    HANDLE hEvent
  ) override;
  HRESULT __stdcall SetMaximumFrameLatency(
    UINT MaxLatency
  ) override;
  HRESULT __stdcall GetMaximumFrameLatency(
    UINT * pMaxLatency
  ) override;
  HRESULT __stdcall GetAdapter(
    IDXGIAdapter ** pAdapter
  ) override;
  HRESULT __stdcall CreateSurface(
    const DXGI_SURFACE_DESC * pDesc, 
    UINT NumSurfaces, 
    DXGI_USAGE Usage, 
    const DXGI_SHARED_RESOURCE * pSharedResource, 
    IDXGISurface ** ppSurface
  ) override;
  HRESULT __stdcall QueryResourceResidency(
    struct IUnknown *const * ppResources, 
    DXGI_RESIDENCY * pResidencyStatus, 
    UINT NumResources
  ) override;
  HRESULT __stdcall SetGPUThreadPriority(
    INT Priority
  ) override;
  HRESULT __stdcall GetGPUThreadPriority(
    INT * pPriority
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class IDXGIFactory5
inherits:
  IDXGIFactory4
  IDXGIFactory3
  IDXGIFactory2
  IDXGIFactory1
  IDXGIFactory
  IDXGIObject
  IUnknown
methods:
  CheckFeatureSupport
end class
#endif
class WrappedIDXGIFactory5 : public IDXGIFactory5 {
private:
  IDXGIFactory5 *m_pIDXGIFactory5;
  IDXGIFactory4 *m_pIDXGIFactory4;
  IDXGIFactory3 *m_pIDXGIFactory3;
  IDXGIFactory2 *m_pIDXGIFactory2;
  IDXGIFactory1 *m_pIDXGIFactory1;
  IDXGIFactory *m_pIDXGIFactory;
  IDXGIObject *m_pIDXGIObject;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedIDXGIFactory5(T *pWrapped) {
    out() << "[CREATE] IDXGIFactory5(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pIDXGIObject = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIObject), (void **)&m_pIDXGIObject);
    if (m_pIDXGIObject) {
      m_pIDXGIObject->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIObject);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIObject)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIObject << " -> " << this << "\n";
    }
    m_pIDXGIFactory = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory), (void **)&m_pIDXGIFactory);
    if (m_pIDXGIFactory) {
      m_pIDXGIFactory->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory << " -> " << this << "\n";
    }
    m_pIDXGIFactory1 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory1), (void **)&m_pIDXGIFactory1);
    if (m_pIDXGIFactory1) {
      m_pIDXGIFactory1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory1);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory1 << " -> " << this << "\n";
    }
    m_pIDXGIFactory2 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory2), (void **)&m_pIDXGIFactory2);
    if (m_pIDXGIFactory2) {
      m_pIDXGIFactory2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory2);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory2 << " -> " << this << "\n";
    }
    m_pIDXGIFactory3 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory3), (void **)&m_pIDXGIFactory3);
    if (m_pIDXGIFactory3) {
      m_pIDXGIFactory3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory3);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory3 << " -> " << this << "\n";
    }
    m_pIDXGIFactory4 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory4), (void **)&m_pIDXGIFactory4);
    if (m_pIDXGIFactory4) {
      m_pIDXGIFactory4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory4);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory4 << " -> " << this << "\n";
    }
    m_pIDXGIFactory5 = nullptr;
    pWrapped->QueryInterface(__uuidof(IDXGIFactory5), (void **)&m_pIDXGIFactory5);
    if (m_pIDXGIFactory5) {
      m_pIDXGIFactory5->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIDXGIFactory5);
      wt[reinterpret_cast<size_t>((void*)m_pIDXGIFactory5)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIDXGIFactory5 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CheckFeatureSupport(
    DXGI_FEATURE Feature, 
    void * pFeatureSupportData, 
    UINT FeatureSupportDataSize
  ) override;
  HRESULT __stdcall EnumAdapterByLuid(
    LUID AdapterLuid, 
    const IID & riid, 
    void ** ppvAdapter
  ) override;
  HRESULT __stdcall EnumWarpAdapter(
    const IID & riid, 
    void ** ppvAdapter
  ) override;
  UINT __stdcall GetCreationFlags() override;
  BOOL __stdcall IsWindowedStereoEnabled() override;
  HRESULT __stdcall CreateSwapChainForHwnd(
    struct IUnknown * pDevice, 
    HWND hWnd, 
    const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc, 
    IDXGIOutput * pRestrictToOutput, 
    IDXGISwapChain1 ** ppSwapChain
  ) override;
  HRESULT __stdcall CreateSwapChainForCoreWindow(
    struct IUnknown * pDevice, 
    struct IUnknown * pWindow, 
    const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
    IDXGIOutput * pRestrictToOutput, 
    IDXGISwapChain1 ** ppSwapChain
  ) override;
  HRESULT __stdcall GetSharedResourceAdapterLuid(
    HANDLE hResource, 
    LUID * pLuid
  ) override;
  HRESULT __stdcall RegisterStereoStatusWindow(
    HWND WindowHandle, 
    UINT wMsg, 
    DWORD * pdwCookie
  ) override;
  HRESULT __stdcall RegisterStereoStatusEvent(
    HANDLE hEvent, 
    DWORD * pdwCookie
  ) override;
  void __stdcall UnregisterStereoStatus(
    DWORD dwCookie
  ) override;
  HRESULT __stdcall RegisterOcclusionStatusWindow(
    HWND WindowHandle, 
    UINT wMsg, 
    DWORD * pdwCookie
  ) override;
  HRESULT __stdcall RegisterOcclusionStatusEvent(
    HANDLE hEvent, 
    DWORD * pdwCookie
  ) override;
  void __stdcall UnregisterOcclusionStatus(
    DWORD dwCookie
  ) override;
  HRESULT __stdcall CreateSwapChainForComposition(
    struct IUnknown * pDevice, 
    const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
    IDXGIOutput * pRestrictToOutput, 
    IDXGISwapChain1 ** ppSwapChain
  ) override;
  HRESULT __stdcall EnumAdapters1(
    UINT Adapter, 
    IDXGIAdapter1 ** ppAdapter
  ) override;
  BOOL __stdcall IsCurrent() override;
  HRESULT __stdcall EnumAdapters(
    UINT Adapter, 
    IDXGIAdapter ** ppAdapter
  ) override;
  HRESULT __stdcall MakeWindowAssociation(
    HWND WindowHandle, 
    UINT Flags
  ) override;
  HRESULT __stdcall GetWindowAssociation(
    HWND * pWindowHandle
  ) override;
  HRESULT __stdcall CreateSwapChain(
    struct IUnknown * pDevice, 
    DXGI_SWAP_CHAIN_DESC * pDesc, 
    IDXGISwapChain ** ppSwapChain
  ) override;
  HRESULT __stdcall CreateSoftwareAdapter(
    HMODULE Module, 
    IDXGIAdapter ** ppAdapter
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & Name, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & Name, 
    const struct IUnknown * pUnknown
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & Name, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetParent(
    const IID & riid, 
    void ** ppParent
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Device5
inherits:
  ID3D11Device4
  ID3D11Device3
  ID3D11Device2
  ID3D11Device1
  ID3D11Device
  IUnknown
methods:
  OpenSharedFence
  CreateFence
end class
#endif
class WrappedID3D11Device5 : public ID3D11Device5 {
private:
  ID3D11Device5 *m_pID3D11Device5;
  ID3D11Device4 *m_pID3D11Device4;
  ID3D11Device3 *m_pID3D11Device3;
  ID3D11Device2 *m_pID3D11Device2;
  ID3D11Device1 *m_pID3D11Device1;
  ID3D11Device *m_pID3D11Device;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Device5(T *pWrapped) {
    out() << "[CREATE] ID3D11Device5(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11Device = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device), (void **)&m_pID3D11Device);
    if (m_pID3D11Device) {
      m_pID3D11Device->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device << " -> " << this << "\n";
    }
    m_pID3D11Device1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device1), (void **)&m_pID3D11Device1);
    if (m_pID3D11Device1) {
      m_pID3D11Device1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device1 << " -> " << this << "\n";
    }
    m_pID3D11Device2 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device2), (void **)&m_pID3D11Device2);
    if (m_pID3D11Device2) {
      m_pID3D11Device2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device2);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device2 << " -> " << this << "\n";
    }
    m_pID3D11Device3 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device3), (void **)&m_pID3D11Device3);
    if (m_pID3D11Device3) {
      m_pID3D11Device3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device3);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device3 << " -> " << this << "\n";
    }
    m_pID3D11Device4 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device4), (void **)&m_pID3D11Device4);
    if (m_pID3D11Device4) {
      m_pID3D11Device4->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device4);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device4)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device4 << " -> " << this << "\n";
    }
    m_pID3D11Device5 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Device5), (void **)&m_pID3D11Device5);
    if (m_pID3D11Device5) {
      m_pID3D11Device5->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Device5);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Device5)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Device5 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall OpenSharedFence(
    HANDLE hFence, 
    const IID & ReturnedInterface, 
    void ** ppFence
  ) override;
  HRESULT __stdcall CreateFence(
    UINT64 InitialValue, 
    D3D11_FENCE_FLAG Flags, 
    const IID & ReturnedInterface, 
    void ** ppFence
  ) override;
  HRESULT __stdcall RegisterDeviceRemovedEvent(
    HANDLE hEvent, 
    DWORD * pdwCookie
  ) override;
  void __stdcall UnregisterDeviceRemoved(
    DWORD dwCookie
  ) override;
  HRESULT __stdcall CreateTexture2D1(
    const D3D11_TEXTURE2D_DESC1 * pDesc1, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Texture2D1 ** ppTexture2D
  ) override;
  HRESULT __stdcall CreateTexture3D1(
    const D3D11_TEXTURE3D_DESC1 * pDesc1, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Texture3D1 ** ppTexture3D
  ) override;
  HRESULT __stdcall CreateRasterizerState2(
    const D3D11_RASTERIZER_DESC2 * pRasterizerDesc, 
    ID3D11RasterizerState2 ** ppRasterizerState
  ) override;
  HRESULT __stdcall CreateShaderResourceView1(
    ID3D11Resource * pResource, 
    const D3D11_SHADER_RESOURCE_VIEW_DESC1 * pDesc1, 
    ID3D11ShaderResourceView1 ** ppSRView1
  ) override;
  HRESULT __stdcall CreateUnorderedAccessView1(
    ID3D11Resource * pResource, 
    const D3D11_UNORDERED_ACCESS_VIEW_DESC1 * pDesc1, 
    ID3D11UnorderedAccessView1 ** ppUAView1
  ) override;
  HRESULT __stdcall CreateRenderTargetView1(
    ID3D11Resource * pResource, 
    const D3D11_RENDER_TARGET_VIEW_DESC1 * pDesc1, 
    ID3D11RenderTargetView1 ** ppRTView1
  ) override;
  HRESULT __stdcall CreateQuery1(
    const D3D11_QUERY_DESC1 * pQueryDesc1, 
    ID3D11Query1 ** ppQuery1
  ) override;
  void __stdcall GetImmediateContext3(
    ID3D11DeviceContext3 ** ppImmediateContext
  ) override;
  HRESULT __stdcall CreateDeferredContext3(
    UINT ContextFlags, 
    ID3D11DeviceContext3 ** ppDeferredContext
  ) override;
  void __stdcall WriteToSubresource(
    ID3D11Resource * pDstResource, 
    UINT DstSubresource, 
    const D3D11_BOX * pDstBox, 
    const void * pSrcData, 
    UINT SrcRowPitch, 
    UINT SrcDepthPitch
  ) override;
  void __stdcall ReadFromSubresource(
    void * pDstData, 
    UINT DstRowPitch, 
    UINT DstDepthPitch, 
    ID3D11Resource * pSrcResource, 
    UINT SrcSubresource, 
    const D3D11_BOX * pSrcBox
  ) override;
  void __stdcall GetImmediateContext2(
    ID3D11DeviceContext2 ** ppImmediateContext
  ) override;
  HRESULT __stdcall CreateDeferredContext2(
    UINT ContextFlags, 
    ID3D11DeviceContext2 ** ppDeferredContext
  ) override;
  void __stdcall GetResourceTiling(
    ID3D11Resource * pTiledResource, 
    UINT * pNumTilesForEntireResource, 
    D3D11_PACKED_MIP_DESC * pPackedMipDesc, 
    D3D11_TILE_SHAPE * pStandardTileShapeForNonPackedMips, 
    UINT * pNumSubresourceTilings, 
    UINT FirstSubresourceTilingToGet, 
    D3D11_SUBRESOURCE_TILING * pSubresourceTilingsForNonPackedMips
  ) override;
  HRESULT __stdcall CheckMultisampleQualityLevels1(
    DXGI_FORMAT Format, 
    UINT SampleCount, 
    UINT Flags, 
    UINT * pNumQualityLevels
  ) override;
  void __stdcall GetImmediateContext1(
    ID3D11DeviceContext1 ** ppImmediateContext
  ) override;
  HRESULT __stdcall CreateDeferredContext1(
    UINT ContextFlags, 
    ID3D11DeviceContext1 ** ppDeferredContext
  ) override;
  HRESULT __stdcall CreateBlendState1(
    const D3D11_BLEND_DESC1 * pBlendStateDesc, 
    ID3D11BlendState1 ** ppBlendState
  ) override;
  HRESULT __stdcall CreateRasterizerState1(
    const D3D11_RASTERIZER_DESC1 * pRasterizerDesc, 
    ID3D11RasterizerState1 ** ppRasterizerState
  ) override;
  HRESULT __stdcall CreateDeviceContextState(
    UINT Flags, 
    const D3D_FEATURE_LEVEL * pFeatureLevels, 
    UINT FeatureLevels, 
    UINT SDKVersion, 
    const IID & EmulatedInterface, 
    D3D_FEATURE_LEVEL * pChosenFeatureLevel, 
    ID3DDeviceContextState ** ppContextState
  ) override;
  HRESULT __stdcall OpenSharedResource1(
    HANDLE hResource, 
    const IID & returnedInterface, 
    void ** ppResource
  ) override;
  HRESULT __stdcall OpenSharedResourceByName(
    LPCWSTR lpName, 
    DWORD dwDesiredAccess, 
    const IID & returnedInterface, 
    void ** ppResource
  ) override;
  HRESULT __stdcall CreateBuffer(
    const D3D11_BUFFER_DESC * pDesc, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Buffer ** ppBuffer
  ) override;
  HRESULT __stdcall CreateTexture1D(
    const D3D11_TEXTURE1D_DESC * pDesc, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Texture1D ** ppTexture1D
  ) override;
  HRESULT __stdcall CreateTexture2D(
    const D3D11_TEXTURE2D_DESC * pDesc, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Texture2D ** ppTexture2D
  ) override;
  HRESULT __stdcall CreateTexture3D(
    const D3D11_TEXTURE3D_DESC * pDesc, 
    const D3D11_SUBRESOURCE_DATA * pInitialData, 
    ID3D11Texture3D ** ppTexture3D
  ) override;
  HRESULT __stdcall CreateShaderResourceView(
    ID3D11Resource * pResource, 
    const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc, 
    ID3D11ShaderResourceView ** ppSRView
  ) override;
  HRESULT __stdcall CreateUnorderedAccessView(
    ID3D11Resource * pResource, 
    const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc, 
    ID3D11UnorderedAccessView ** ppUAView
  ) override;
  HRESULT __stdcall CreateRenderTargetView(
    ID3D11Resource * pResource, 
    const D3D11_RENDER_TARGET_VIEW_DESC * pDesc, 
    ID3D11RenderTargetView ** ppRTView
  ) override;
  HRESULT __stdcall CreateDepthStencilView(
    ID3D11Resource * pResource, 
    const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc, 
    ID3D11DepthStencilView ** ppDepthStencilView
  ) override;
  HRESULT __stdcall CreateInputLayout(
    const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs, 
    UINT NumElements, 
    const void * pShaderBytecodeWithInputSignature, 
    SIZE_T BytecodeLength, 
    ID3D11InputLayout ** ppInputLayout
  ) override;
  HRESULT __stdcall CreateVertexShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11VertexShader ** ppVertexShader
  ) override;
  HRESULT __stdcall CreateGeometryShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11GeometryShader ** ppGeometryShader
  ) override;
  HRESULT __stdcall CreateGeometryShaderWithStreamOutput(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    const D3D11_SO_DECLARATION_ENTRY * pSODeclaration, 
    UINT NumEntries, 
    const UINT * pBufferStrides, 
    UINT NumStrides, 
    UINT RasterizedStream, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11GeometryShader ** ppGeometryShader
  ) override;
  HRESULT __stdcall CreatePixelShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11PixelShader ** ppPixelShader
  ) override;
  HRESULT __stdcall CreateHullShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11HullShader ** ppHullShader
  ) override;
  HRESULT __stdcall CreateDomainShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11DomainShader ** ppDomainShader
  ) override;
  HRESULT __stdcall CreateComputeShader(
    const void * pShaderBytecode, 
    SIZE_T BytecodeLength, 
    ID3D11ClassLinkage * pClassLinkage, 
    ID3D11ComputeShader ** ppComputeShader
  ) override;
  HRESULT __stdcall CreateClassLinkage(
    ID3D11ClassLinkage ** ppLinkage
  ) override;
  HRESULT __stdcall CreateBlendState(
    const D3D11_BLEND_DESC * pBlendStateDesc, 
    ID3D11BlendState ** ppBlendState
  ) override;
  HRESULT __stdcall CreateDepthStencilState(
    const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc, 
    ID3D11DepthStencilState ** ppDepthStencilState
  ) override;
  HRESULT __stdcall CreateRasterizerState(
    const D3D11_RASTERIZER_DESC * pRasterizerDesc, 
    ID3D11RasterizerState ** ppRasterizerState
  ) override;
  HRESULT __stdcall CreateSamplerState(
    const D3D11_SAMPLER_DESC * pSamplerDesc, 
    ID3D11SamplerState ** ppSamplerState
  ) override;
  HRESULT __stdcall CreateQuery(
    const D3D11_QUERY_DESC * pQueryDesc, 
    ID3D11Query ** ppQuery
  ) override;
  HRESULT __stdcall CreatePredicate(
    const D3D11_QUERY_DESC * pPredicateDesc, 
    ID3D11Predicate ** ppPredicate
  ) override;
  HRESULT __stdcall CreateCounter(
    const D3D11_COUNTER_DESC * pCounterDesc, 
    ID3D11Counter ** ppCounter
  ) override;
  HRESULT __stdcall CreateDeferredContext(
    UINT ContextFlags, 
    ID3D11DeviceContext ** ppDeferredContext
  ) override;
  HRESULT __stdcall OpenSharedResource(
    HANDLE hResource, 
    const IID & ReturnedInterface, 
    void ** ppResource
  ) override;
  HRESULT __stdcall CheckFormatSupport(
    DXGI_FORMAT Format, 
    UINT * pFormatSupport
  ) override;
  HRESULT __stdcall CheckMultisampleQualityLevels(
    DXGI_FORMAT Format, 
    UINT SampleCount, 
    UINT * pNumQualityLevels
  ) override;
  void __stdcall CheckCounterInfo(
    D3D11_COUNTER_INFO * pCounterInfo
  ) override;
  HRESULT __stdcall CheckCounter(
    const D3D11_COUNTER_DESC * pDesc, 
    D3D11_COUNTER_TYPE * pType, 
    UINT * pActiveCounters, 
    LPSTR szName, 
    UINT * pNameLength, 
    LPSTR szUnits, 
    UINT * pUnitsLength, 
    LPSTR szDescription, 
    UINT * pDescriptionLength
  ) override;
  HRESULT __stdcall CheckFeatureSupport(
    D3D11_FEATURE Feature, 
    void * pFeatureSupportData, 
    UINT FeatureSupportDataSize
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  D3D_FEATURE_LEVEL __stdcall GetFeatureLevel() override;
  UINT __stdcall GetCreationFlags() override;
  HRESULT __stdcall GetDeviceRemovedReason() override;
  void __stdcall GetImmediateContext(
    ID3D11DeviceContext ** ppImmediateContext
  ) override;
  HRESULT __stdcall SetExceptionMode(
    UINT RaiseFlags
  ) override;
  UINT __stdcall GetExceptionMode() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11Multithread
inherits:
  IUnknown
methods:
  Enter
  Leave
  SetMultithreadProtected
  GetMultithreadProtected
end class
#endif
class WrappedID3D11Multithread : public ID3D11Multithread {
private:
  ID3D11Multithread *m_pID3D11Multithread;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Multithread(T *pWrapped) {
    out() << "[CREATE] ID3D11Multithread(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11Multithread = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Multithread), (void **)&m_pID3D11Multithread);
    if (m_pID3D11Multithread) {
      m_pID3D11Multithread->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Multithread);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Multithread)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Multithread << " -> " << this << "\n";
    }
  }
  void __stdcall Enter() override;
  void __stdcall Leave() override;
  BOOL __stdcall SetMultithreadProtected(
    BOOL bMTProtect
  ) override;
  BOOL __stdcall GetMultithreadProtected() override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoDevice2
inherits:
  ID3D11VideoDevice1
  ID3D11VideoDevice
  IUnknown
methods:
  CheckFeatureSupport
  NegotiateCryptoSessionKeyExchangeMT
end class
#endif
class WrappedID3D11VideoDevice2 : public ID3D11VideoDevice2 {
private:
  ID3D11VideoDevice2 *m_pID3D11VideoDevice2;
  ID3D11VideoDevice1 *m_pID3D11VideoDevice1;
  ID3D11VideoDevice *m_pID3D11VideoDevice;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoDevice2(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoDevice2(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11VideoDevice = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoDevice), (void **)&m_pID3D11VideoDevice);
    if (m_pID3D11VideoDevice) {
      m_pID3D11VideoDevice->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoDevice << " -> " << this << "\n";
    }
    m_pID3D11VideoDevice1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoDevice1), (void **)&m_pID3D11VideoDevice1);
    if (m_pID3D11VideoDevice1) {
      m_pID3D11VideoDevice1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoDevice1 << " -> " << this << "\n";
    }
    m_pID3D11VideoDevice2 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoDevice2), (void **)&m_pID3D11VideoDevice2);
    if (m_pID3D11VideoDevice2) {
      m_pID3D11VideoDevice2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice2);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoDevice2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoDevice2 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall CheckFeatureSupport(
    D3D11_FEATURE_VIDEO Feature, 
    void * pFeatureSupportData, 
    UINT FeatureSupportDataSize
  ) override;
  HRESULT __stdcall NegotiateCryptoSessionKeyExchangeMT(
    ID3D11CryptoSession * pCryptoSession, 
    D3D11_CRYPTO_SESSION_KEY_EXCHANGE_FLAGS flags, 
    UINT DataSize, 
    void * pData
  ) override;
  HRESULT __stdcall GetCryptoSessionPrivateDataSize(
    const GUID * pCryptoType, 
    const GUID * pDecoderProfile, 
    const GUID * pKeyExchangeType, 
    UINT * pPrivateInputSize, 
    UINT * pPrivateOutputSize
  ) override;
  HRESULT __stdcall GetVideoDecoderCaps(
    const GUID * pDecoderProfile, 
    UINT SampleWidth, 
    UINT SampleHeight, 
    const DXGI_RATIONAL * pFrameRate, 
    UINT BitRate, 
    const GUID * pCryptoType, 
    UINT * pDecoderCaps
  ) override;
  HRESULT __stdcall CheckVideoDecoderDownsampling(
    const D3D11_VIDEO_DECODER_DESC * pInputDesc, 
    DXGI_COLOR_SPACE_TYPE InputColorSpace, 
    const D3D11_VIDEO_DECODER_CONFIG * pInputConfig, 
    const DXGI_RATIONAL * pFrameRate, 
    const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc, 
    BOOL * pSupported, 
    BOOL * pRealTimeHint
  ) override;
  HRESULT __stdcall RecommendVideoDecoderDownsampleParameters(
    const D3D11_VIDEO_DECODER_DESC * pInputDesc, 
    DXGI_COLOR_SPACE_TYPE InputColorSpace, 
    const D3D11_VIDEO_DECODER_CONFIG * pInputConfig, 
    const DXGI_RATIONAL * pFrameRate, 
    D3D11_VIDEO_SAMPLE_DESC * pRecommendedOutputDesc
  ) override;
  HRESULT __stdcall CreateVideoDecoder(
    const D3D11_VIDEO_DECODER_DESC * pVideoDesc, 
    const D3D11_VIDEO_DECODER_CONFIG * pConfig, 
    ID3D11VideoDecoder ** ppDecoder
  ) override;
  HRESULT __stdcall CreateVideoProcessor(
    ID3D11VideoProcessorEnumerator * pEnum, 
    UINT RateConversionIndex, 
    ID3D11VideoProcessor ** ppVideoProcessor
  ) override;
  HRESULT __stdcall CreateAuthenticatedChannel(
    D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType, 
    ID3D11AuthenticatedChannel ** ppAuthenticatedChannel
  ) override;
  HRESULT __stdcall CreateCryptoSession(
    const GUID * pCryptoType, 
    const GUID * pDecoderProfile, 
    const GUID * pKeyExchangeType, 
    ID3D11CryptoSession ** ppCryptoSession
  ) override;
  HRESULT __stdcall CreateVideoDecoderOutputView(
    ID3D11Resource * pResource, 
    const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc, 
    ID3D11VideoDecoderOutputView ** ppVDOVView
  ) override;
  HRESULT __stdcall CreateVideoProcessorInputView(
    ID3D11Resource * pResource, 
    ID3D11VideoProcessorEnumerator * pEnum, 
    const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc, 
    ID3D11VideoProcessorInputView ** ppVPIView
  ) override;
  HRESULT __stdcall CreateVideoProcessorOutputView(
    ID3D11Resource * pResource, 
    ID3D11VideoProcessorEnumerator * pEnum, 
    const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc, 
    ID3D11VideoProcessorOutputView ** ppVPOView
  ) override;
  HRESULT __stdcall CreateVideoProcessorEnumerator(
    const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc, 
    ID3D11VideoProcessorEnumerator ** ppEnum
  ) override;
  UINT __stdcall GetVideoDecoderProfileCount() override;
  HRESULT __stdcall GetVideoDecoderProfile(
    UINT Index, 
    GUID * pDecoderProfile
  ) override;
  HRESULT __stdcall CheckVideoDecoderFormat(
    const GUID * pDecoderProfile, 
    DXGI_FORMAT Format, 
    BOOL * pSupported
  ) override;
  HRESULT __stdcall GetVideoDecoderConfigCount(
    const D3D11_VIDEO_DECODER_DESC * pDesc, 
    UINT * pCount
  ) override;
  HRESULT __stdcall GetVideoDecoderConfig(
    const D3D11_VIDEO_DECODER_DESC * pDesc, 
    UINT Index, 
    D3D11_VIDEO_DECODER_CONFIG * pConfig
  ) override;
  HRESULT __stdcall GetContentProtectionCaps(
    const GUID * pCryptoType, 
    const GUID * pDecoderProfile, 
    D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps
  ) override;
  HRESULT __stdcall CheckCryptoKeyExchange(
    const GUID * pCryptoType, 
    const GUID * pDecoderProfile, 
    UINT Index, 
    GUID * pKeyExchangeType
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11VideoContext3
inherits:
  ID3D11VideoContext2
  ID3D11VideoContext1
  ID3D11VideoContext
  ID3D11DeviceChild
  IUnknown
methods:
  DecoderBeginFrame1
  SubmitDecoderBuffers2
end class
#endif
class WrappedID3D11VideoContext3 : public ID3D11VideoContext3 {
private:
  ID3D11VideoContext3 *m_pID3D11VideoContext3;
  ID3D11VideoContext2 *m_pID3D11VideoContext2;
  ID3D11VideoContext1 *m_pID3D11VideoContext1;
  ID3D11VideoContext *m_pID3D11VideoContext;
  ID3D11DeviceChild *m_pID3D11DeviceChild;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11VideoContext3(T *pWrapped) {
    out() << "[CREATE] ID3D11VideoContext3(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11DeviceChild = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11DeviceChild), (void **)&m_pID3D11DeviceChild);
    if (m_pID3D11DeviceChild) {
      m_pID3D11DeviceChild->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11DeviceChild)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11DeviceChild << " -> " << this << "\n";
    }
    m_pID3D11VideoContext = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoContext), (void **)&m_pID3D11VideoContext);
    if (m_pID3D11VideoContext) {
      m_pID3D11VideoContext->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoContext);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoContext)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoContext << " -> " << this << "\n";
    }
    m_pID3D11VideoContext1 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoContext1), (void **)&m_pID3D11VideoContext1);
    if (m_pID3D11VideoContext1) {
      m_pID3D11VideoContext1->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoContext1);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoContext1)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoContext1 << " -> " << this << "\n";
    }
    m_pID3D11VideoContext2 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoContext2), (void **)&m_pID3D11VideoContext2);
    if (m_pID3D11VideoContext2) {
      m_pID3D11VideoContext2->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoContext2);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoContext2)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoContext2 << " -> " << this << "\n";
    }
    m_pID3D11VideoContext3 = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11VideoContext3), (void **)&m_pID3D11VideoContext3);
    if (m_pID3D11VideoContext3) {
      m_pID3D11VideoContext3->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11VideoContext3);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11VideoContext3)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11VideoContext3 << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall DecoderBeginFrame1(
    ID3D11VideoDecoder * pDecoder, 
    ID3D11VideoDecoderOutputView * pView, 
    UINT ContentKeySize, 
    const void * pContentKey, 
    UINT NumComponentHistograms, 
    const UINT * pHistogramOffsets, 
    ID3D11Buffer *const * ppHistogramBuffers
  ) override;
  HRESULT __stdcall SubmitDecoderBuffers2(
    ID3D11VideoDecoder * pDecoder, 
    UINT NumBuffers, 
    const D3D11_VIDEO_DECODER_BUFFER_DESC2 * pBufferDesc
  ) override;
  void __stdcall VideoProcessorSetOutputHDRMetaData(
    ID3D11VideoProcessor * pVideoProcessor, 
    DXGI_HDR_METADATA_TYPE Type, 
    UINT Size, 
    const void * pHDRMetaData
  ) override;
  void __stdcall VideoProcessorGetOutputHDRMetaData(
    ID3D11VideoProcessor * pVideoProcessor, 
    DXGI_HDR_METADATA_TYPE * pType, 
    UINT Size, 
    void * pMetaData
  ) override;
  void __stdcall VideoProcessorSetStreamHDRMetaData(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    DXGI_HDR_METADATA_TYPE Type, 
    UINT Size, 
    const void * pHDRMetaData
  ) override;
  void __stdcall VideoProcessorGetStreamHDRMetaData(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    DXGI_HDR_METADATA_TYPE * pType, 
    UINT Size, 
    void * pMetaData
  ) override;
  HRESULT __stdcall SubmitDecoderBuffers1(
    ID3D11VideoDecoder * pDecoder, 
    UINT NumBuffers, 
    const D3D11_VIDEO_DECODER_BUFFER_DESC1 * pBufferDesc
  ) override;
  HRESULT __stdcall GetDataForNewHardwareKey(
    ID3D11CryptoSession * pCryptoSession, 
    UINT PrivateInputSize, 
    const void * pPrivatInputData, 
    UINT64 * pPrivateOutputData
  ) override;
  HRESULT __stdcall CheckCryptoSessionStatus(
    ID3D11CryptoSession * pCryptoSession, 
    D3D11_CRYPTO_SESSION_STATUS * pStatus
  ) override;
  HRESULT __stdcall DecoderEnableDownsampling(
    ID3D11VideoDecoder * pDecoder, 
    DXGI_COLOR_SPACE_TYPE InputColorSpace, 
    const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc, 
    UINT ReferenceFrameCount
  ) override;
  HRESULT __stdcall DecoderUpdateDownsampling(
    ID3D11VideoDecoder * pDecoder, 
    const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc
  ) override;
  void __stdcall VideoProcessorSetOutputColorSpace1(
    ID3D11VideoProcessor * pVideoProcessor, 
    DXGI_COLOR_SPACE_TYPE ColorSpace
  ) override;
  void __stdcall VideoProcessorSetOutputShaderUsage(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL ShaderUsage
  ) override;
  void __stdcall VideoProcessorGetOutputColorSpace1(
    ID3D11VideoProcessor * pVideoProcessor, 
    DXGI_COLOR_SPACE_TYPE * pColorSpace
  ) override;
  void __stdcall VideoProcessorGetOutputShaderUsage(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL * pShaderUsage
  ) override;
  void __stdcall VideoProcessorSetStreamColorSpace1(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    DXGI_COLOR_SPACE_TYPE ColorSpace
  ) override;
  void __stdcall VideoProcessorSetStreamMirror(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    BOOL FlipHorizontal, 
    BOOL FlipVertical
  ) override;
  void __stdcall VideoProcessorGetStreamColorSpace1(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    DXGI_COLOR_SPACE_TYPE * pColorSpace
  ) override;
  void __stdcall VideoProcessorGetStreamMirror(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnable, 
    BOOL * pFlipHorizontal, 
    BOOL * pFlipVertical
  ) override;
  HRESULT __stdcall VideoProcessorGetBehaviorHints(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT OutputWidth, 
    UINT OutputHeight, 
    DXGI_FORMAT OutputFormat, 
    UINT StreamCount, 
    const D3D11_VIDEO_PROCESSOR_STREAM_BEHAVIOR_HINT * pStreams, 
    UINT * pBehaviorHints
  ) override;
  HRESULT __stdcall GetDecoderBuffer(
    ID3D11VideoDecoder * pDecoder, 
    D3D11_VIDEO_DECODER_BUFFER_TYPE Type, 
    UINT * pBufferSize, 
    void ** ppBuffer
  ) override;
  HRESULT __stdcall ReleaseDecoderBuffer(
    ID3D11VideoDecoder * pDecoder, 
    D3D11_VIDEO_DECODER_BUFFER_TYPE Type
  ) override;
  HRESULT __stdcall DecoderBeginFrame(
    ID3D11VideoDecoder * pDecoder, 
    ID3D11VideoDecoderOutputView * pView, 
    UINT ContentKeySize, 
    const void * pContentKey
  ) override;
  HRESULT __stdcall DecoderEndFrame(
    ID3D11VideoDecoder * pDecoder
  ) override;
  HRESULT __stdcall SubmitDecoderBuffers(
    ID3D11VideoDecoder * pDecoder, 
    UINT NumBuffers, 
    const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc
  ) override;
  APP_DEPRECATED_HRESULT __stdcall DecoderExtension(
    ID3D11VideoDecoder * pDecoder, 
    const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData
  ) override;
  void __stdcall VideoProcessorSetOutputTargetRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL Enable, 
    const RECT * pRect
  ) override;
  void __stdcall VideoProcessorSetOutputBackgroundColor(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL YCbCr, 
    const D3D11_VIDEO_COLOR * pColor
  ) override;
  void __stdcall VideoProcessorSetOutputColorSpace(
    ID3D11VideoProcessor * pVideoProcessor, 
    const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
  ) override;
  void __stdcall VideoProcessorSetOutputAlphaFillMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode, 
    UINT StreamIndex
  ) override;
  void __stdcall VideoProcessorSetOutputConstriction(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL Enable, 
    SIZE Size
  ) override;
  void __stdcall VideoProcessorSetOutputStereoMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL Enable
  ) override;
  APP_DEPRECATED_HRESULT __stdcall VideoProcessorSetOutputExtension(
    ID3D11VideoProcessor * pVideoProcessor, 
    const GUID * pExtensionGuid, 
    UINT DataSize, 
    void * pData
  ) override;
  void __stdcall VideoProcessorGetOutputTargetRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL * Enabled, 
    RECT * pRect
  ) override;
  void __stdcall VideoProcessorGetOutputBackgroundColor(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL * pYCbCr, 
    D3D11_VIDEO_COLOR * pColor
  ) override;
  void __stdcall VideoProcessorGetOutputColorSpace(
    ID3D11VideoProcessor * pVideoProcessor, 
    D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
  ) override;
  void __stdcall VideoProcessorGetOutputAlphaFillMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode, 
    UINT * pStreamIndex
  ) override;
  void __stdcall VideoProcessorGetOutputConstriction(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL * pEnabled, 
    SIZE * pSize
  ) override;
  void __stdcall VideoProcessorGetOutputStereoMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    BOOL * pEnabled
  ) override;
  APP_DEPRECATED_HRESULT __stdcall VideoProcessorGetOutputExtension(
    ID3D11VideoProcessor * pVideoProcessor, 
    const GUID * pExtensionGuid, 
    UINT DataSize, 
    void * pData
  ) override;
  void __stdcall VideoProcessorSetStreamFrameFormat(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_FRAME_FORMAT FrameFormat
  ) override;
  void __stdcall VideoProcessorSetStreamColorSpace(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
  ) override;
  void __stdcall VideoProcessorSetStreamOutputRate(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate, 
    BOOL RepeatFrame, 
    const DXGI_RATIONAL * pCustomRate
  ) override;
  void __stdcall VideoProcessorSetStreamSourceRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    const RECT * pRect
  ) override;
  void __stdcall VideoProcessorSetStreamDestRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    const RECT * pRect
  ) override;
  void __stdcall VideoProcessorSetStreamAlpha(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    FLOAT Alpha
  ) override;
  void __stdcall VideoProcessorSetStreamPalette(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    UINT Count, 
    const UINT * pEntries
  ) override;
  void __stdcall VideoProcessorSetStreamPixelAspectRatio(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    const DXGI_RATIONAL * pSourceAspectRatio, 
    const DXGI_RATIONAL * pDestinationAspectRatio
  ) override;
  void __stdcall VideoProcessorSetStreamLumaKey(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    FLOAT Lower, 
    FLOAT Upper
  ) override;
  void __stdcall VideoProcessorSetStreamStereoFormat(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format, 
    BOOL LeftViewFrame0, 
    BOOL BaseViewFrame0, 
    D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode, 
    int MonoOffset
  ) override;
  void __stdcall VideoProcessorSetStreamAutoProcessingMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable
  ) override;
  void __stdcall VideoProcessorSetStreamFilter(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_PROCESSOR_FILTER Filter, 
    BOOL Enable, 
    int Level
  ) override;
  APP_DEPRECATED_HRESULT __stdcall VideoProcessorSetStreamExtension(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    const GUID * pExtensionGuid, 
    UINT DataSize, 
    void * pData
  ) override;
  void __stdcall VideoProcessorGetStreamFrameFormat(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_FRAME_FORMAT * pFrameFormat
  ) override;
  void __stdcall VideoProcessorGetStreamColorSpace(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
  ) override;
  void __stdcall VideoProcessorGetStreamOutputRate(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate, 
    BOOL * pRepeatFrame, 
    DXGI_RATIONAL * pCustomRate
  ) override;
  void __stdcall VideoProcessorGetStreamSourceRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled, 
    RECT * pRect
  ) override;
  void __stdcall VideoProcessorGetStreamDestRect(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled, 
    RECT * pRect
  ) override;
  void __stdcall VideoProcessorGetStreamAlpha(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled, 
    FLOAT * pAlpha
  ) override;
  void __stdcall VideoProcessorGetStreamPalette(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    UINT Count, 
    UINT * pEntries
  ) override;
  void __stdcall VideoProcessorGetStreamPixelAspectRatio(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled, 
    DXGI_RATIONAL * pSourceAspectRatio, 
    DXGI_RATIONAL * pDestinationAspectRatio
  ) override;
  void __stdcall VideoProcessorGetStreamLumaKey(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled, 
    FLOAT * pLower, 
    FLOAT * pUpper
  ) override;
  void __stdcall VideoProcessorGetStreamStereoFormat(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnable, 
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat, 
    BOOL * pLeftViewFrame0, 
    BOOL * pBaseViewFrame0, 
    D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode, 
    int * MonoOffset
  ) override;
  void __stdcall VideoProcessorGetStreamAutoProcessingMode(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnabled
  ) override;
  void __stdcall VideoProcessorGetStreamFilter(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    D3D11_VIDEO_PROCESSOR_FILTER Filter, 
    BOOL * pEnabled, 
    int * pLevel
  ) override;
  APP_DEPRECATED_HRESULT __stdcall VideoProcessorGetStreamExtension(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    const GUID * pExtensionGuid, 
    UINT DataSize, 
    void * pData
  ) override;
  HRESULT __stdcall VideoProcessorBlt(
    ID3D11VideoProcessor * pVideoProcessor, 
    ID3D11VideoProcessorOutputView * pView, 
    UINT OutputFrame, 
    UINT StreamCount, 
    const D3D11_VIDEO_PROCESSOR_STREAM * pStreams
  ) override;
  HRESULT __stdcall NegotiateCryptoSessionKeyExchange(
    ID3D11CryptoSession * pCryptoSession, 
    UINT DataSize, 
    void * pData
  ) override;
  void __stdcall EncryptionBlt(
    ID3D11CryptoSession * pCryptoSession, 
    ID3D11Texture2D * pSrcSurface, 
    ID3D11Texture2D * pDstSurface, 
    UINT IVSize, 
    void * pIV
  ) override;
  void __stdcall DecryptionBlt(
    ID3D11CryptoSession * pCryptoSession, 
    ID3D11Texture2D * pSrcSurface, 
    ID3D11Texture2D * pDstSurface, 
    D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, 
    UINT ContentKeySize, 
    const void * pContentKey, 
    UINT IVSize, 
    void * pIV
  ) override;
  void __stdcall StartSessionKeyRefresh(
    ID3D11CryptoSession * pCryptoSession, 
    UINT RandomNumberSize, 
    void * pRandomNumber
  ) override;
  void __stdcall FinishSessionKeyRefresh(
    ID3D11CryptoSession * pCryptoSession
  ) override;
  HRESULT __stdcall GetEncryptionBltKey(
    ID3D11CryptoSession * pCryptoSession, 
    UINT KeySize, 
    void * pReadbackKey
  ) override;
  HRESULT __stdcall NegotiateAuthenticatedChannelKeyExchange(
    ID3D11AuthenticatedChannel * pChannel, 
    UINT DataSize, 
    void * pData
  ) override;
  HRESULT __stdcall QueryAuthenticatedChannel(
    ID3D11AuthenticatedChannel * pChannel, 
    UINT InputSize, 
    const void * pInput, 
    UINT OutputSize, 
    void * pOutput
  ) override;
  HRESULT __stdcall ConfigureAuthenticatedChannel(
    ID3D11AuthenticatedChannel * pChannel, 
    UINT InputSize, 
    const void * pInput, 
    D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput
  ) override;
  void __stdcall VideoProcessorSetStreamRotation(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL Enable, 
    D3D11_VIDEO_PROCESSOR_ROTATION Rotation
  ) override;
  void __stdcall VideoProcessorGetStreamRotation(
    ID3D11VideoProcessor * pVideoProcessor, 
    UINT StreamIndex, 
    BOOL * pEnable, 
    D3D11_VIDEO_PROCESSOR_ROTATION * pRotation
  ) override;
  void __stdcall GetDevice(
    ID3D11Device ** ppDevice
  ) override;
  HRESULT __stdcall GetPrivateData(
    const GUID & guid, 
    UINT * pDataSize, 
    void * pData
  ) override;
  HRESULT __stdcall SetPrivateData(
    const GUID & guid, 
    UINT DataSize, 
    const void * pData
  ) override;
  HRESULT __stdcall SetPrivateDataInterface(
    const GUID & guid, 
    const struct IUnknown * pData
  ) override;
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11ShaderReflectionType
inherits:
methods:
  GetDesc
  GetMemberTypeByIndex
  GetMemberTypeByName
  GetMemberTypeName
  IsEqual
  GetSubType
  GetBaseClass
  GetNumInterfaces
  GetInterfaceByIndex
  IsOfType
  ImplementsInterface
end class
#endif
class WrappedID3D11ShaderReflectionType : public ID3D11ShaderReflectionType {
private:
  ID3D11ShaderReflectionType *m_pID3D11ShaderReflectionType;
public:
  template<typename T>
  WrappedID3D11ShaderReflectionType(T *pWrapped) {
    out() << "[CREATE] ID3D11ShaderReflectionType(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D11ShaderReflectionType = nullptr;
    m_pID3D11ShaderReflectionType = (ID3D11ShaderReflectionType*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D11_SHADER_TYPE_DESC * pDesc
  ) override;
  struct ID3D11ShaderReflectionType * __stdcall GetMemberTypeByIndex(
    UINT Index
  ) override;
  struct ID3D11ShaderReflectionType * __stdcall GetMemberTypeByName(
    LPCSTR Name
  ) override;
  LPCSTR __stdcall GetMemberTypeName(
    UINT Index
  ) override;
  HRESULT __stdcall IsEqual(
    struct ID3D11ShaderReflectionType * pType
  ) override;
  struct ID3D11ShaderReflectionType * __stdcall GetSubType() override;
  struct ID3D11ShaderReflectionType * __stdcall GetBaseClass() override;
  UINT __stdcall GetNumInterfaces() override;
  struct ID3D11ShaderReflectionType * __stdcall GetInterfaceByIndex(
    UINT uIndex
  ) override;
  HRESULT __stdcall IsOfType(
    struct ID3D11ShaderReflectionType * pType
  ) override;
  HRESULT __stdcall ImplementsInterface(
    struct ID3D11ShaderReflectionType * pBase
  ) override;
};
#if 0
begin class ID3D11ShaderReflectionVariable
inherits:
methods:
  GetDesc
  GetType
  GetBuffer
  GetInterfaceSlot
end class
#endif
class WrappedID3D11ShaderReflectionVariable : public ID3D11ShaderReflectionVariable {
private:
  ID3D11ShaderReflectionVariable *m_pID3D11ShaderReflectionVariable;
public:
  template<typename T>
  WrappedID3D11ShaderReflectionVariable(T *pWrapped) {
    out() << "[CREATE] ID3D11ShaderReflectionVariable(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D11ShaderReflectionVariable = nullptr;
    m_pID3D11ShaderReflectionVariable = (ID3D11ShaderReflectionVariable*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D11_SHADER_VARIABLE_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionType * __stdcall GetType() override;
  ID3D11ShaderReflectionConstantBuffer * __stdcall GetBuffer() override;
  UINT __stdcall GetInterfaceSlot(
    UINT uArrayIndex
  ) override;
};
#if 0
begin class ID3D11ShaderReflectionConstantBuffer
inherits:
methods:
  GetDesc
  GetVariableByIndex
  GetVariableByName
end class
#endif
class WrappedID3D11ShaderReflectionConstantBuffer : public ID3D11ShaderReflectionConstantBuffer {
private:
  ID3D11ShaderReflectionConstantBuffer *m_pID3D11ShaderReflectionConstantBuffer;
public:
  template<typename T>
  WrappedID3D11ShaderReflectionConstantBuffer(T *pWrapped) {
    out() << "[CREATE] ID3D11ShaderReflectionConstantBuffer(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D11ShaderReflectionConstantBuffer = nullptr;
    m_pID3D11ShaderReflectionConstantBuffer = (ID3D11ShaderReflectionConstantBuffer*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D11_SHADER_BUFFER_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionVariable * __stdcall GetVariableByIndex(
    UINT Index
  ) override;
  ID3D11ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
};
#if 0
begin class ID3D11ShaderReflection
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  GetDesc
  GetConstantBufferByIndex
  GetConstantBufferByName
  GetResourceBindingDesc
  GetInputParameterDesc
  GetOutputParameterDesc
  GetPatchConstantParameterDesc
  GetVariableByName
  GetResourceBindingDescByName
  GetMovInstructionCount
  GetMovcInstructionCount
  GetConversionInstructionCount
  GetBitwiseInstructionCount
  GetGSInputPrimitive
  IsSampleFrequencyShader
  GetNumInterfaceSlots
  GetMinFeatureLevel
  GetThreadGroupSize
  GetRequiresFlags
end class
#endif
class WrappedID3D11ShaderReflection : public ID3D11ShaderReflection {
private:
  ID3D11ShaderReflection *m_pID3D11ShaderReflection;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ShaderReflection(T *pWrapped) {
    out() << "[CREATE] ID3D11ShaderReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11ShaderReflection = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ShaderReflection), (void **)&m_pID3D11ShaderReflection);
    if (m_pID3D11ShaderReflection) {
      m_pID3D11ShaderReflection->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ShaderReflection);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ShaderReflection)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ShaderReflection << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall GetDesc(
    D3D11_SHADER_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByIndex(
    UINT Index
  ) override;
  ID3D11ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDesc(
    UINT ResourceIndex, 
    D3D11_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  HRESULT __stdcall GetInputParameterDesc(
    UINT ParameterIndex, 
    D3D11_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  HRESULT __stdcall GetOutputParameterDesc(
    UINT ParameterIndex, 
    D3D11_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  HRESULT __stdcall GetPatchConstantParameterDesc(
    UINT ParameterIndex, 
    D3D11_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDescByName(
    LPCSTR Name, 
    D3D11_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  UINT __stdcall GetMovInstructionCount() override;
  UINT __stdcall GetMovcInstructionCount() override;
  UINT __stdcall GetConversionInstructionCount() override;
  UINT __stdcall GetBitwiseInstructionCount() override;
  D3D_PRIMITIVE __stdcall GetGSInputPrimitive() override;
  BOOL __stdcall IsSampleFrequencyShader() override;
  UINT __stdcall GetNumInterfaceSlots() override;
  HRESULT __stdcall GetMinFeatureLevel(
    enum D3D_FEATURE_LEVEL * pLevel
  ) override;
  UINT __stdcall GetThreadGroupSize(
    UINT * pSizeX, 
    UINT * pSizeY, 
    UINT * pSizeZ
  ) override;
  UINT64 __stdcall GetRequiresFlags() override;
};
#if 0
begin class ID3D11LibraryReflection
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  GetDesc
  GetFunctionByIndex
end class
#endif
class WrappedID3D11LibraryReflection : public ID3D11LibraryReflection {
private:
  ID3D11LibraryReflection *m_pID3D11LibraryReflection;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11LibraryReflection(T *pWrapped) {
    out() << "[CREATE] ID3D11LibraryReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11LibraryReflection = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11LibraryReflection), (void **)&m_pID3D11LibraryReflection);
    if (m_pID3D11LibraryReflection) {
      m_pID3D11LibraryReflection->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11LibraryReflection);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11LibraryReflection)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11LibraryReflection << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall GetDesc(
    D3D11_LIBRARY_DESC * pDesc
  ) override;
  ID3D11FunctionReflection * __stdcall GetFunctionByIndex(
    INT FunctionIndex
  ) override;
};
#if 0
begin class ID3D11FunctionReflection
inherits:
methods:
  GetDesc
  GetConstantBufferByIndex
  GetConstantBufferByName
  GetResourceBindingDesc
  GetVariableByName
  GetResourceBindingDescByName
  GetFunctionParameter
end class
#endif
class WrappedID3D11FunctionReflection : public ID3D11FunctionReflection {
private:
  ID3D11FunctionReflection *m_pID3D11FunctionReflection;
public:
  template<typename T>
  WrappedID3D11FunctionReflection(T *pWrapped) {
    out() << "[CREATE] ID3D11FunctionReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D11FunctionReflection = nullptr;
    m_pID3D11FunctionReflection = (ID3D11FunctionReflection*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D11_FUNCTION_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByIndex(
    UINT BufferIndex
  ) override;
  ID3D11ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDesc(
    UINT ResourceIndex, 
    D3D11_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  ID3D11ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDescByName(
    LPCSTR Name, 
    D3D11_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  ID3D11FunctionParameterReflection * __stdcall GetFunctionParameter(
    INT ParameterIndex
  ) override;
};
#if 0
begin class ID3D11FunctionParameterReflection
inherits:
methods:
  GetDesc
end class
#endif
class WrappedID3D11FunctionParameterReflection : public ID3D11FunctionParameterReflection {
private:
  ID3D11FunctionParameterReflection *m_pID3D11FunctionParameterReflection;
public:
  template<typename T>
  WrappedID3D11FunctionParameterReflection(T *pWrapped) {
    out() << "[CREATE] ID3D11FunctionParameterReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D11FunctionParameterReflection = nullptr;
    m_pID3D11FunctionParameterReflection = (ID3D11FunctionParameterReflection*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D11_PARAMETER_DESC * pDesc
  ) override;
};
#if 0
begin class ID3D11Module
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  CreateInstance
end class
#endif
class WrappedID3D11Module : public ID3D11Module {
private:
  ID3D11Module *m_pID3D11Module;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Module(T *pWrapped) {
    out() << "[CREATE] ID3D11Module(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11Module = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Module), (void **)&m_pID3D11Module);
    if (m_pID3D11Module) {
      m_pID3D11Module->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Module);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Module)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Module << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall CreateInstance(
    LPCSTR pNamespace, 
    struct ID3D11ModuleInstance ** ppModuleInstance
  ) override;
};
#if 0
begin class ID3D11ModuleInstance
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  BindConstantBuffer
  BindConstantBufferByName
  BindResource
  BindResourceByName
  BindSampler
  BindSamplerByName
  BindUnorderedAccessView
  BindUnorderedAccessViewByName
  BindResourceAsUnorderedAccessView
  BindResourceAsUnorderedAccessViewByName
end class
#endif
class WrappedID3D11ModuleInstance : public ID3D11ModuleInstance {
private:
  ID3D11ModuleInstance *m_pID3D11ModuleInstance;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11ModuleInstance(T *pWrapped) {
    out() << "[CREATE] ID3D11ModuleInstance(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11ModuleInstance = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11ModuleInstance), (void **)&m_pID3D11ModuleInstance);
    if (m_pID3D11ModuleInstance) {
      m_pID3D11ModuleInstance->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11ModuleInstance);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11ModuleInstance)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11ModuleInstance << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall BindConstantBuffer(
    UINT uSrcSlot, 
    UINT uDstSlot, 
    UINT cbDstOffset
  ) override;
  HRESULT __stdcall BindConstantBufferByName(
    LPCSTR pName, 
    UINT uDstSlot, 
    UINT cbDstOffset
  ) override;
  HRESULT __stdcall BindResource(
    UINT uSrcSlot, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindResourceByName(
    LPCSTR pName, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindSampler(
    UINT uSrcSlot, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindSamplerByName(
    LPCSTR pName, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindUnorderedAccessView(
    UINT uSrcSlot, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindUnorderedAccessViewByName(
    LPCSTR pName, 
    UINT uDstSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindResourceAsUnorderedAccessView(
    UINT uSrcSrvSlot, 
    UINT uDstUavSlot, 
    UINT uCount
  ) override;
  HRESULT __stdcall BindResourceAsUnorderedAccessViewByName(
    LPCSTR pSrvName, 
    UINT uDstUavSlot, 
    UINT uCount
  ) override;
};
#if 0
begin class ID3D11Linker
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  Link
  UseLibrary
  AddClipPlaneFromCBuffer
end class
#endif
class WrappedID3D11Linker : public ID3D11Linker {
private:
  ID3D11Linker *m_pID3D11Linker;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11Linker(T *pWrapped) {
    out() << "[CREATE] ID3D11Linker(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11Linker = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11Linker), (void **)&m_pID3D11Linker);
    if (m_pID3D11Linker) {
      m_pID3D11Linker->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11Linker);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11Linker)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11Linker << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall Link(
    struct ID3D11ModuleInstance * pEntry, 
    LPCSTR pEntryName, 
    LPCSTR pTargetName, 
    UINT uFlags, 
    ID3DBlob ** ppShaderBlob, 
    ID3DBlob ** ppErrorBuffer
  ) override;
  HRESULT __stdcall UseLibrary(
    struct ID3D11ModuleInstance * pLibraryMI
  ) override;
  HRESULT __stdcall AddClipPlaneFromCBuffer(
    UINT uCBufferSlot, 
    UINT uCBufferEntry
  ) override;
};
#if 0
begin class ID3D11LinkingNode
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
end class
#endif
class WrappedID3D11LinkingNode : public ID3D11LinkingNode {
private:
  ID3D11LinkingNode *m_pID3D11LinkingNode;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11LinkingNode(T *pWrapped) {
    out() << "[CREATE] ID3D11LinkingNode(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11LinkingNode = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11LinkingNode), (void **)&m_pID3D11LinkingNode);
    if (m_pID3D11LinkingNode) {
      m_pID3D11LinkingNode->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11LinkingNode);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11LinkingNode)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11LinkingNode << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
};
#if 0
begin class ID3D11FunctionLinkingGraph
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  CreateModuleInstance
  SetInputSignature
  SetOutputSignature
  CallFunction
  PassValue
  PassValueWithSwizzle
  GetLastError
  GenerateHlsl
end class
#endif
class WrappedID3D11FunctionLinkingGraph : public ID3D11FunctionLinkingGraph {
private:
  ID3D11FunctionLinkingGraph *m_pID3D11FunctionLinkingGraph;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D11FunctionLinkingGraph(T *pWrapped) {
    out() << "[CREATE] ID3D11FunctionLinkingGraph(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D11FunctionLinkingGraph = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D11FunctionLinkingGraph), (void **)&m_pID3D11FunctionLinkingGraph);
    if (m_pID3D11FunctionLinkingGraph) {
      m_pID3D11FunctionLinkingGraph->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D11FunctionLinkingGraph);
      wt[reinterpret_cast<size_t>((void*)m_pID3D11FunctionLinkingGraph)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D11FunctionLinkingGraph << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall CreateModuleInstance(
    struct ID3D11ModuleInstance ** ppModuleInstance, 
    ID3DBlob ** ppErrorBuffer
  ) override;
  HRESULT __stdcall SetInputSignature(
    const D3D11_PARAMETER_DESC * pInputParameters, 
    UINT cInputParameters, 
    struct ID3D11LinkingNode ** ppInputNode
  ) override;
  HRESULT __stdcall SetOutputSignature(
    const D3D11_PARAMETER_DESC * pOutputParameters, 
    UINT cOutputParameters, 
    struct ID3D11LinkingNode ** ppOutputNode
  ) override;
  HRESULT __stdcall CallFunction(
    LPCSTR pModuleInstanceNamespace, 
    struct ID3D11Module * pModuleWithFunctionPrototype, 
    LPCSTR pFunctionName, 
    struct ID3D11LinkingNode ** ppCallNode
  ) override;
  HRESULT __stdcall PassValue(
    struct ID3D11LinkingNode * pSrcNode, 
    INT SrcParameterIndex, 
    struct ID3D11LinkingNode * pDstNode, 
    INT DstParameterIndex
  ) override;
  HRESULT __stdcall PassValueWithSwizzle(
    struct ID3D11LinkingNode * pSrcNode, 
    INT SrcParameterIndex, 
    LPCSTR pSrcSwizzle, 
    struct ID3D11LinkingNode * pDstNode, 
    INT DstParameterIndex, 
    LPCSTR pDstSwizzle
  ) override;
  HRESULT __stdcall GetLastError(
    ID3DBlob ** ppErrorBuffer
  ) override;
  HRESULT __stdcall GenerateHlsl(
    UINT uFlags, 
    ID3DBlob ** ppBuffer
  ) override;
};
#if 0
begin class ID3D12ShaderReflectionType
inherits:
methods:
  GetDesc
  GetMemberTypeByIndex
  GetMemberTypeByName
  GetMemberTypeName
  IsEqual
  GetSubType
  GetBaseClass
  GetNumInterfaces
  GetInterfaceByIndex
  IsOfType
  ImplementsInterface
end class
#endif
class WrappedID3D12ShaderReflectionType : public ID3D12ShaderReflectionType {
private:
  ID3D12ShaderReflectionType *m_pID3D12ShaderReflectionType;
public:
  template<typename T>
  WrappedID3D12ShaderReflectionType(T *pWrapped) {
    out() << "[CREATE] ID3D12ShaderReflectionType(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D12ShaderReflectionType = nullptr;
    m_pID3D12ShaderReflectionType = (ID3D12ShaderReflectionType*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D12_SHADER_TYPE_DESC * pDesc
  ) override;
  struct ID3D12ShaderReflectionType * __stdcall GetMemberTypeByIndex(
    UINT Index
  ) override;
  struct ID3D12ShaderReflectionType * __stdcall GetMemberTypeByName(
    LPCSTR Name
  ) override;
  LPCSTR __stdcall GetMemberTypeName(
    UINT Index
  ) override;
  HRESULT __stdcall IsEqual(
    struct ID3D12ShaderReflectionType * pType
  ) override;
  struct ID3D12ShaderReflectionType * __stdcall GetSubType() override;
  struct ID3D12ShaderReflectionType * __stdcall GetBaseClass() override;
  UINT __stdcall GetNumInterfaces() override;
  struct ID3D12ShaderReflectionType * __stdcall GetInterfaceByIndex(
    UINT uIndex
  ) override;
  HRESULT __stdcall IsOfType(
    struct ID3D12ShaderReflectionType * pType
  ) override;
  HRESULT __stdcall ImplementsInterface(
    struct ID3D12ShaderReflectionType * pBase
  ) override;
};
#if 0
begin class ID3D12ShaderReflectionVariable
inherits:
methods:
  GetDesc
  GetType
  GetBuffer
  GetInterfaceSlot
end class
#endif
class WrappedID3D12ShaderReflectionVariable : public ID3D12ShaderReflectionVariable {
private:
  ID3D12ShaderReflectionVariable *m_pID3D12ShaderReflectionVariable;
public:
  template<typename T>
  WrappedID3D12ShaderReflectionVariable(T *pWrapped) {
    out() << "[CREATE] ID3D12ShaderReflectionVariable(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D12ShaderReflectionVariable = nullptr;
    m_pID3D12ShaderReflectionVariable = (ID3D12ShaderReflectionVariable*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D12_SHADER_VARIABLE_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionType * __stdcall GetType() override;
  ID3D12ShaderReflectionConstantBuffer * __stdcall GetBuffer() override;
  UINT __stdcall GetInterfaceSlot(
    UINT uArrayIndex
  ) override;
};
#if 0
begin class ID3D12ShaderReflectionConstantBuffer
inherits:
methods:
  GetDesc
  GetVariableByIndex
  GetVariableByName
end class
#endif
class WrappedID3D12ShaderReflectionConstantBuffer : public ID3D12ShaderReflectionConstantBuffer {
private:
  ID3D12ShaderReflectionConstantBuffer *m_pID3D12ShaderReflectionConstantBuffer;
public:
  template<typename T>
  WrappedID3D12ShaderReflectionConstantBuffer(T *pWrapped) {
    out() << "[CREATE] ID3D12ShaderReflectionConstantBuffer(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D12ShaderReflectionConstantBuffer = nullptr;
    m_pID3D12ShaderReflectionConstantBuffer = (ID3D12ShaderReflectionConstantBuffer*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D12_SHADER_BUFFER_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionVariable * __stdcall GetVariableByIndex(
    UINT Index
  ) override;
  ID3D12ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
};
#if 0
begin class ID3D12ShaderReflection
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  GetDesc
  GetConstantBufferByIndex
  GetConstantBufferByName
  GetResourceBindingDesc
  GetInputParameterDesc
  GetOutputParameterDesc
  GetPatchConstantParameterDesc
  GetVariableByName
  GetResourceBindingDescByName
  GetMovInstructionCount
  GetMovcInstructionCount
  GetConversionInstructionCount
  GetBitwiseInstructionCount
  GetGSInputPrimitive
  IsSampleFrequencyShader
  GetNumInterfaceSlots
  GetMinFeatureLevel
  GetThreadGroupSize
  GetRequiresFlags
end class
#endif
class WrappedID3D12ShaderReflection : public ID3D12ShaderReflection {
private:
  ID3D12ShaderReflection *m_pID3D12ShaderReflection;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D12ShaderReflection(T *pWrapped) {
    out() << "[CREATE] ID3D12ShaderReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D12ShaderReflection = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D12ShaderReflection), (void **)&m_pID3D12ShaderReflection);
    if (m_pID3D12ShaderReflection) {
      m_pID3D12ShaderReflection->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D12ShaderReflection);
      wt[reinterpret_cast<size_t>((void*)m_pID3D12ShaderReflection)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D12ShaderReflection << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall GetDesc(
    D3D12_SHADER_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByIndex(
    UINT Index
  ) override;
  ID3D12ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDesc(
    UINT ResourceIndex, 
    D3D12_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  HRESULT __stdcall GetInputParameterDesc(
    UINT ParameterIndex, 
    D3D12_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  HRESULT __stdcall GetOutputParameterDesc(
    UINT ParameterIndex, 
    D3D12_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  HRESULT __stdcall GetPatchConstantParameterDesc(
    UINT ParameterIndex, 
    D3D12_SIGNATURE_PARAMETER_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDescByName(
    LPCSTR Name, 
    D3D12_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  UINT __stdcall GetMovInstructionCount() override;
  UINT __stdcall GetMovcInstructionCount() override;
  UINT __stdcall GetConversionInstructionCount() override;
  UINT __stdcall GetBitwiseInstructionCount() override;
  D3D_PRIMITIVE __stdcall GetGSInputPrimitive() override;
  BOOL __stdcall IsSampleFrequencyShader() override;
  UINT __stdcall GetNumInterfaceSlots() override;
  HRESULT __stdcall GetMinFeatureLevel(
    enum D3D_FEATURE_LEVEL * pLevel
  ) override;
  UINT __stdcall GetThreadGroupSize(
    UINT * pSizeX, 
    UINT * pSizeY, 
    UINT * pSizeZ
  ) override;
  UINT64 __stdcall GetRequiresFlags() override;
};
#if 0
begin class ID3D12LibraryReflection
inherits:
  IUnknown
methods:
  QueryInterface
  AddRef
  Release
  GetDesc
  GetFunctionByIndex
end class
#endif
class WrappedID3D12LibraryReflection : public ID3D12LibraryReflection {
private:
  ID3D12LibraryReflection *m_pID3D12LibraryReflection;
  IUnknown *m_pIUnknown;
public:
  template<typename T>
  WrappedID3D12LibraryReflection(T *pWrapped) {
    out() << "[CREATE] ID3D12LibraryReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pIUnknown = nullptr;
    pWrapped->QueryInterface(__uuidof(IUnknown), (void **)&m_pIUnknown);
    if (m_pIUnknown) {
      m_pIUnknown->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pIUnknown);
      wt[reinterpret_cast<size_t>((void*)m_pIUnknown)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pIUnknown << " -> " << this << "\n";
    }
    m_pID3D12LibraryReflection = nullptr;
    pWrapped->QueryInterface(__uuidof(ID3D12LibraryReflection), (void **)&m_pID3D12LibraryReflection);
    if (m_pID3D12LibraryReflection) {
      m_pID3D12LibraryReflection->Release();
      uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_pID3D12LibraryReflection);
      wt[reinterpret_cast<size_t>((void*)m_pID3D12LibraryReflection)] = reinterpret_cast<size_t>((void*)this);
      out() << "[MAP] " << m_pID3D12LibraryReflection << " -> " << this << "\n";
    }
  }
  HRESULT __stdcall QueryInterface(
    const IID & riid, 
    void ** ppvObject
  ) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;
  HRESULT __stdcall GetDesc(
    D3D12_LIBRARY_DESC * pDesc
  ) override;
  ID3D12FunctionReflection * __stdcall GetFunctionByIndex(
    INT FunctionIndex
  ) override;
};
#if 0
begin class ID3D12FunctionReflection
inherits:
methods:
  GetDesc
  GetConstantBufferByIndex
  GetConstantBufferByName
  GetResourceBindingDesc
  GetVariableByName
  GetResourceBindingDescByName
  GetFunctionParameter
end class
#endif
class WrappedID3D12FunctionReflection : public ID3D12FunctionReflection {
private:
  ID3D12FunctionReflection *m_pID3D12FunctionReflection;
public:
  template<typename T>
  WrappedID3D12FunctionReflection(T *pWrapped) {
    out() << "[CREATE] ID3D12FunctionReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D12FunctionReflection = nullptr;
    m_pID3D12FunctionReflection = (ID3D12FunctionReflection*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D12_FUNCTION_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByIndex(
    UINT BufferIndex
  ) override;
  ID3D12ShaderReflectionConstantBuffer * __stdcall GetConstantBufferByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDesc(
    UINT ResourceIndex, 
    D3D12_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  ID3D12ShaderReflectionVariable * __stdcall GetVariableByName(
    LPCSTR Name
  ) override;
  HRESULT __stdcall GetResourceBindingDescByName(
    LPCSTR Name, 
    D3D12_SHADER_INPUT_BIND_DESC * pDesc
  ) override;
  ID3D12FunctionParameterReflection * __stdcall GetFunctionParameter(
    INT ParameterIndex
  ) override;
};
#if 0
begin class ID3D12FunctionParameterReflection
inherits:
methods:
  GetDesc
end class
#endif
class WrappedID3D12FunctionParameterReflection : public ID3D12FunctionParameterReflection {
private:
  ID3D12FunctionParameterReflection *m_pID3D12FunctionParameterReflection;
public:
  template<typename T>
  WrappedID3D12FunctionParameterReflection(T *pWrapped) {
    out() << "[CREATE] ID3D12FunctionParameterReflection(" << pWrapped << ")\n";
    assert(pWrapped);
    auto &uwt = getUnwrapTable();
    auto &wt = getWrapTable();
    m_pID3D12FunctionParameterReflection = nullptr;
    m_pID3D12FunctionParameterReflection = (ID3D12FunctionParameterReflection*)pWrapped;
  }
  HRESULT __stdcall GetDesc(
    D3D12_PARAMETER_DESC * pDesc
  ) override;
};
typedef WrappedID3D10Blob WrappedID3DBlob;
HRESULT __stdcall WrappedIDXGIKeyedMutex::AcquireSync(
  UINT64 Key, 
  DWORD dwMilliseconds
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIKeyedMutex << ")::AcquireSync\n";
  auto ret = m_pIDXGIKeyedMutex->AcquireSync(
    Key, 
    dwMilliseconds
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::ReleaseSync(
  UINT64 Key
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIKeyedMutex << ")::ReleaseSync\n";
  auto ret = m_pIDXGIKeyedMutex->ReleaseSync(
    Key
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::GetDevice(
  const IID & riid, 
  void ** ppDevice
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIDeviceSubObject << ")::GetDevice\n";
  auto ret = m_pIDXGIDeviceSubObject->GetDevice(
    riid, 
    ppDevice
  );
  out() << "\treturned " << ret << "\n";
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIKeyedMutex(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIKeyedMutex::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIKeyedMutex(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIKeyedMutex)) {
      *ppvObject = (IDXGIKeyedMutex*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDeviceSubObject)) {
      *ppvObject = (IDXGIDeviceSubObject*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIKeyedMutex::AddRef() {
  out() << "IDXGIKeyedMutex(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIKeyedMutex::Release() {
  out() << "IDXGIKeyedMutex(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
LPVOID __stdcall WrappedID3D10Blob::GetBufferPointer() {
  out() << "ID3D10Blob(" << m_pID3D10Blob << ")::GetBufferPointer\n";
  auto ret = m_pID3D10Blob->GetBufferPointer();
  out() << "\treturned " << ret << "\n";
  return ret;
}
SIZE_T __stdcall WrappedID3D10Blob::GetBufferSize() {
  out() << "ID3D10Blob(" << m_pID3D10Blob << ")::GetBufferSize\n";
  auto ret = m_pID3D10Blob->GetBufferSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D10Blob::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D10Blob(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D10Blob)) {
      *ppvObject = (ID3D10Blob*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D10Blob::AddRef() {
  out() << "ID3D10Blob(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D10Blob::Release() {
  out() << "ID3D10Blob(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDestructionNotifier::RegisterDestructionCallback(
  PFN_DESTRUCTION_CALLBACK callbackFn, 
  void * pData, 
  UINT * pCallbackID
) {
  out() << "ID3DDestructionNotifier(" << m_pID3DDestructionNotifier << ")::RegisterDestructionCallback\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3DDestructionNotifier->RegisterDestructionCallback(
    callbackFn, 
    unwrapped_pData, 
    pCallbackID
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDestructionNotifier::UnregisterDestructionCallback(
  UINT callbackID
) {
  out() << "ID3DDestructionNotifier(" << m_pID3DDestructionNotifier << ")::UnregisterDestructionCallback\n";
  auto ret = m_pID3DDestructionNotifier->UnregisterDestructionCallback(
    callbackID
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDestructionNotifier::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3DDestructionNotifier(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3DDestructionNotifier)) {
      *ppvObject = (ID3DDestructionNotifier*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3DDestructionNotifier::AddRef() {
  out() << "ID3DDestructionNotifier(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3DDestructionNotifier::Release() {
  out() << "ID3DDestructionNotifier(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DInclude::Open(
  D3D_INCLUDE_TYPE IncludeType, 
  LPCSTR pFileName, 
  LPCVOID pParentData, 
  LPCVOID * ppData, 
  UINT * pBytes
) {
  out() << "ID3DInclude(" << m_pID3DInclude << ")::Open\n";
  auto unwrapped_ppData = unwrap(ppData);
  auto unwrapped_pBytes = unwrap(pBytes);
  auto ret = m_pID3DInclude->Open(
    IncludeType, 
    pFileName, 
    pParentData, 
    unwrapped_ppData, 
    unwrapped_pBytes
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DInclude::Close(
  LPCVOID pData
) {
  out() << "ID3DInclude(" << m_pID3DInclude << ")::Close\n";
  auto ret = m_pID3DInclude->Close(
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DepthStencilState::GetDesc(
  D3D11_DEPTH_STENCIL_DESC * pDesc
) {
  out() << "ID3D11DepthStencilState(" << m_pID3D11DepthStencilState << ")::GetDesc\n";
  m_pID3D11DepthStencilState->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11DepthStencilState::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11DepthStencilState(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11DepthStencilState::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11DepthStencilState(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilState::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11DepthStencilState(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilState::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11DepthStencilState(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilState::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11DepthStencilState(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11DepthStencilState)) {
      *ppvObject = (ID3D11DepthStencilState*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11DepthStencilState::AddRef() {
  out() << "ID3D11DepthStencilState(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11DepthStencilState::Release() {
  out() << "ID3D11DepthStencilState(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Buffer::GetDesc(
  D3D11_BUFFER_DESC * pDesc
) {
  out() << "ID3D11Buffer(" << m_pID3D11Buffer << ")::GetDesc\n";
  m_pID3D11Buffer->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11Buffer::GetType(
  D3D11_RESOURCE_DIMENSION * pResourceDimension
) {
  out() << "ID3D11Buffer(" << m_pID3D11Resource << ")::GetType\n";
  m_pID3D11Resource->GetType(
    pResourceDimension
  );
}
void __stdcall WrappedID3D11Buffer::SetEvictionPriority(
  UINT EvictionPriority
) {
  out() << "ID3D11Buffer(" << m_pID3D11Resource << ")::SetEvictionPriority\n";
  m_pID3D11Resource->SetEvictionPriority(
    EvictionPriority
  );
}
UINT __stdcall WrappedID3D11Buffer::GetEvictionPriority() {
  out() << "ID3D11Buffer(" << m_pID3D11Resource << ")::GetEvictionPriority\n";
  auto ret = m_pID3D11Resource->GetEvictionPriority();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Buffer::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Buffer(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Buffer::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Buffer(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Buffer::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Buffer(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Buffer::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Buffer(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Buffer::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Buffer(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Buffer)) {
      *ppvObject = (ID3D11Buffer*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Resource)) {
      *ppvObject = (ID3D11Resource*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Buffer::AddRef() {
  out() << "ID3D11Buffer(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Buffer::Release() {
  out() << "ID3D11Buffer(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture1D::GetDesc(
  D3D11_TEXTURE1D_DESC * pDesc
) {
  out() << "ID3D11Texture1D(" << m_pID3D11Texture1D << ")::GetDesc\n";
  m_pID3D11Texture1D->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11Texture1D::GetType(
  D3D11_RESOURCE_DIMENSION * pResourceDimension
) {
  out() << "ID3D11Texture1D(" << m_pID3D11Resource << ")::GetType\n";
  m_pID3D11Resource->GetType(
    pResourceDimension
  );
}
void __stdcall WrappedID3D11Texture1D::SetEvictionPriority(
  UINT EvictionPriority
) {
  out() << "ID3D11Texture1D(" << m_pID3D11Resource << ")::SetEvictionPriority\n";
  m_pID3D11Resource->SetEvictionPriority(
    EvictionPriority
  );
}
UINT __stdcall WrappedID3D11Texture1D::GetEvictionPriority() {
  out() << "ID3D11Texture1D(" << m_pID3D11Resource << ")::GetEvictionPriority\n";
  auto ret = m_pID3D11Resource->GetEvictionPriority();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture1D::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Texture1D(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Texture1D::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Texture1D(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture1D::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Texture1D(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture1D::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Texture1D(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture1D::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Texture1D(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Texture1D)) {
      *ppvObject = (ID3D11Texture1D*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Resource)) {
      *ppvObject = (ID3D11Resource*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Texture1D::AddRef() {
  out() << "ID3D11Texture1D(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Texture1D::Release() {
  out() << "ID3D11Texture1D(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DepthStencilView::GetDesc(
  D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11DepthStencilView << ")::GetDesc\n";
  m_pID3D11DepthStencilView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11DepthStencilView::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11DepthStencilView::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11DepthStencilView::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilView::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilView::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11DepthStencilView(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DepthStencilView::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11DepthStencilView(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11DepthStencilView)) {
      *ppvObject = (ID3D11DepthStencilView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11DepthStencilView::AddRef() {
  out() << "ID3D11DepthStencilView(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11DepthStencilView::Release() {
  out() << "ID3D11DepthStencilView(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VertexShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VertexShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VertexShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VertexShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VertexShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VertexShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VertexShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VertexShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VertexShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VertexShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VertexShader)) {
      *ppvObject = (ID3D11VertexShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VertexShader::AddRef() {
  out() << "ID3D11VertexShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VertexShader::Release() {
  out() << "ID3D11VertexShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11HullShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11HullShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11HullShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11HullShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11HullShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11HullShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11HullShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11HullShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11HullShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11HullShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11HullShader)) {
      *ppvObject = (ID3D11HullShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11HullShader::AddRef() {
  out() << "ID3D11HullShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11HullShader::Release() {
  out() << "ID3D11HullShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DomainShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11DomainShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11DomainShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11DomainShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DomainShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11DomainShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DomainShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11DomainShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DomainShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11DomainShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11DomainShader)) {
      *ppvObject = (ID3D11DomainShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11DomainShader::AddRef() {
  out() << "ID3D11DomainShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11DomainShader::Release() {
  out() << "ID3D11DomainShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11GeometryShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11GeometryShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11GeometryShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11GeometryShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11GeometryShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11GeometryShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11GeometryShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11GeometryShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11GeometryShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11GeometryShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11GeometryShader)) {
      *ppvObject = (ID3D11GeometryShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11GeometryShader::AddRef() {
  out() << "ID3D11GeometryShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11GeometryShader::Release() {
  out() << "ID3D11GeometryShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11PixelShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11PixelShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11PixelShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11PixelShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11PixelShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11PixelShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11PixelShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11PixelShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11PixelShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11PixelShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11PixelShader)) {
      *ppvObject = (ID3D11PixelShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11PixelShader::AddRef() {
  out() << "ID3D11PixelShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11PixelShader::Release() {
  out() << "ID3D11PixelShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11ComputeShader::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11ComputeShader(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11ComputeShader::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11ComputeShader(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ComputeShader::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11ComputeShader(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ComputeShader::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11ComputeShader(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ComputeShader::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ComputeShader(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ComputeShader)) {
      *ppvObject = (ID3D11ComputeShader*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ComputeShader::AddRef() {
  out() << "ID3D11ComputeShader(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ComputeShader::Release() {
  out() << "ID3D11ComputeShader(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InputLayout::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11InputLayout(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11InputLayout::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11InputLayout(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InputLayout::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11InputLayout(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InputLayout::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11InputLayout(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InputLayout::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11InputLayout(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11InputLayout)) {
      *ppvObject = (ID3D11InputLayout*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11InputLayout::AddRef() {
  out() << "ID3D11InputLayout(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11InputLayout::Release() {
  out() << "ID3D11InputLayout(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11SamplerState::GetDesc(
  D3D11_SAMPLER_DESC * pDesc
) {
  out() << "ID3D11SamplerState(" << m_pID3D11SamplerState << ")::GetDesc\n";
  m_pID3D11SamplerState->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11SamplerState::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11SamplerState(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11SamplerState::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11SamplerState(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11SamplerState::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11SamplerState(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11SamplerState::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11SamplerState(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11SamplerState::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11SamplerState(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11SamplerState)) {
      *ppvObject = (ID3D11SamplerState*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11SamplerState::AddRef() {
  out() << "ID3D11SamplerState(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11SamplerState::Release() {
  out() << "ID3D11SamplerState(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Predicate::GetDesc(
  D3D11_QUERY_DESC * pDesc
) {
  out() << "ID3D11Predicate(" << m_pID3D11Query << ")::GetDesc\n";
  m_pID3D11Query->GetDesc(
    pDesc
  );
}
UINT __stdcall WrappedID3D11Predicate::GetDataSize() {
  out() << "ID3D11Predicate(" << m_pID3D11Asynchronous << ")::GetDataSize\n";
  auto ret = m_pID3D11Asynchronous->GetDataSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Predicate::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Predicate(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Predicate::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Predicate(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Predicate::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Predicate(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Predicate::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Predicate(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Predicate::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Predicate(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Predicate)) {
      *ppvObject = (ID3D11Predicate*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Query)) {
      *ppvObject = (ID3D11Query*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Asynchronous)) {
      *ppvObject = (ID3D11Asynchronous*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Predicate::AddRef() {
  out() << "ID3D11Predicate(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Predicate::Release() {
  out() << "ID3D11Predicate(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Counter::GetDesc(
  D3D11_COUNTER_DESC * pDesc
) {
  out() << "ID3D11Counter(" << m_pID3D11Counter << ")::GetDesc\n";
  m_pID3D11Counter->GetDesc(
    pDesc
  );
}
UINT __stdcall WrappedID3D11Counter::GetDataSize() {
  out() << "ID3D11Counter(" << m_pID3D11Asynchronous << ")::GetDataSize\n";
  auto ret = m_pID3D11Asynchronous->GetDataSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Counter::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Counter(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Counter::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Counter(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Counter::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Counter(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Counter::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Counter(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Counter::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Counter(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Counter)) {
      *ppvObject = (ID3D11Counter*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Asynchronous)) {
      *ppvObject = (ID3D11Asynchronous*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Counter::AddRef() {
  out() << "ID3D11Counter(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Counter::Release() {
  out() << "ID3D11Counter(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11ClassInstance::GetClassLinkage(
  ID3D11ClassLinkage ** ppLinkage
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11ClassInstance << ")::GetClassLinkage\n";
  m_pID3D11ClassInstance->GetClassLinkage(
    ppLinkage
  );
    if (true && ppLinkage && *ppLinkage ) *ppLinkage = getWrapper<ID3D11ClassLinkage, WrappedID3D11ClassLinkage>(*ppLinkage);
}
void __stdcall WrappedID3D11ClassInstance::GetDesc(
  D3D11_CLASS_INSTANCE_DESC * pDesc
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11ClassInstance << ")::GetDesc\n";
  m_pID3D11ClassInstance->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11ClassInstance::GetInstanceName(
  LPSTR pInstanceName, 
  SIZE_T * pBufferLength
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11ClassInstance << ")::GetInstanceName\n";
  auto unwrapped_pBufferLength = unwrap(pBufferLength);
  m_pID3D11ClassInstance->GetInstanceName(
    pInstanceName, 
    unwrapped_pBufferLength
  );
}
void __stdcall WrappedID3D11ClassInstance::GetTypeName(
  LPSTR pTypeName, 
  SIZE_T * pBufferLength
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11ClassInstance << ")::GetTypeName\n";
  auto unwrapped_pBufferLength = unwrap(pBufferLength);
  m_pID3D11ClassInstance->GetTypeName(
    pTypeName, 
    unwrapped_pBufferLength
  );
}
void __stdcall WrappedID3D11ClassInstance::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11ClassInstance::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassInstance::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassInstance::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11ClassInstance(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassInstance::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ClassInstance(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ClassInstance)) {
      *ppvObject = (ID3D11ClassInstance*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ClassInstance::AddRef() {
  out() << "ID3D11ClassInstance(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ClassInstance::Release() {
  out() << "ID3D11ClassInstance(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassLinkage::GetClassInstance(
  LPCSTR pClassInstanceName, 
  UINT InstanceIndex, 
  ID3D11ClassInstance ** ppInstance
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11ClassLinkage << ")::GetClassInstance\n";
  auto ret = m_pID3D11ClassLinkage->GetClassInstance(
    pClassInstanceName, 
    InstanceIndex, 
    ppInstance
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppInstance && *ppInstance ) *ppInstance = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(*ppInstance);
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassLinkage::CreateClassInstance(
  LPCSTR pClassTypeName, 
  UINT ConstantBufferOffset, 
  UINT ConstantVectorOffset, 
  UINT TextureOffset, 
  UINT SamplerOffset, 
  ID3D11ClassInstance ** ppInstance
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11ClassLinkage << ")::CreateClassInstance\n";
  auto ret = m_pID3D11ClassLinkage->CreateClassInstance(
    pClassTypeName, 
    ConstantBufferOffset, 
    ConstantVectorOffset, 
    TextureOffset, 
    SamplerOffset, 
    ppInstance
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppInstance && *ppInstance ) *ppInstance = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(*ppInstance);
  return ret;
}
void __stdcall WrappedID3D11ClassLinkage::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11ClassLinkage::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassLinkage::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassLinkage::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11ClassLinkage(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ClassLinkage::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ClassLinkage(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ClassLinkage)) {
      *ppvObject = (ID3D11ClassLinkage*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ClassLinkage::AddRef() {
  out() << "ID3D11ClassLinkage(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ClassLinkage::Release() {
  out() << "ID3D11ClassLinkage(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11CommandList::GetContextFlags() {
  out() << "ID3D11CommandList(" << m_pID3D11CommandList << ")::GetContextFlags\n";
  auto ret = m_pID3D11CommandList->GetContextFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11CommandList::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11CommandList(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11CommandList::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11CommandList(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CommandList::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11CommandList(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CommandList::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11CommandList(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CommandList::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11CommandList(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11CommandList)) {
      *ppvObject = (ID3D11CommandList*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11CommandList::AddRef() {
  out() << "ID3D11CommandList(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11CommandList::Release() {
  out() << "ID3D11CommandList(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoder::GetCreationParameters(
  D3D11_VIDEO_DECODER_DESC * pVideoDesc, 
  D3D11_VIDEO_DECODER_CONFIG * pConfig
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11VideoDecoder << ")::GetCreationParameters\n";
  auto ret = m_pID3D11VideoDecoder->GetCreationParameters(
    pVideoDesc, 
    pConfig
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoder::GetDriverHandle(
  HANDLE * pDriverHandle
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11VideoDecoder << ")::GetDriverHandle\n";
  auto ret = m_pID3D11VideoDecoder->GetDriverHandle(
    pDriverHandle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoDecoder::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoDecoder::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoder::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoder::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoDecoder(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoder::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoDecoder(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoDecoder)) {
      *ppvObject = (ID3D11VideoDecoder*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDecoder::AddRef() {
  out() << "ID3D11VideoDecoder(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDecoder::Release() {
  out() << "ID3D11VideoDecoder(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoProcessor::GetContentDesc(
  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11VideoProcessor << ")::GetContentDesc\n";
  m_pID3D11VideoProcessor->GetContentDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11VideoProcessor::GetRateConversionCaps(
  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11VideoProcessor << ")::GetRateConversionCaps\n";
  m_pID3D11VideoProcessor->GetRateConversionCaps(
    pCaps
  );
}
void __stdcall WrappedID3D11VideoProcessor::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoProcessor::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessor::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessor::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoProcessor(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessor::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoProcessor(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoProcessor)) {
      *ppvObject = (ID3D11VideoProcessor*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessor::AddRef() {
  out() << "ID3D11VideoProcessor(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessor::Release() {
  out() << "ID3D11VideoProcessor(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::GetCertificateSize(
  UINT * pCertificateSize
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11AuthenticatedChannel << ")::GetCertificateSize\n";
  auto ret = m_pID3D11AuthenticatedChannel->GetCertificateSize(
    pCertificateSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::GetCertificate(
  UINT CertificateSize, 
  BYTE * pCertificate
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11AuthenticatedChannel << ")::GetCertificate\n";
  auto ret = m_pID3D11AuthenticatedChannel->GetCertificate(
    CertificateSize, 
    pCertificate
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11AuthenticatedChannel::GetChannelHandle(
  HANDLE * pChannelHandle
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11AuthenticatedChannel << ")::GetChannelHandle\n";
  m_pID3D11AuthenticatedChannel->GetChannelHandle(
    pChannelHandle
  );
}
void __stdcall WrappedID3D11AuthenticatedChannel::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11AuthenticatedChannel::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11AuthenticatedChannel(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11AuthenticatedChannel)) {
      *ppvObject = (ID3D11AuthenticatedChannel*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11AuthenticatedChannel::AddRef() {
  out() << "ID3D11AuthenticatedChannel(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11AuthenticatedChannel::Release() {
  out() << "ID3D11AuthenticatedChannel(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11CryptoSession::GetCryptoType(
  GUID * pCryptoType
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11CryptoSession << ")::GetCryptoType\n";
  m_pID3D11CryptoSession->GetCryptoType(
    pCryptoType
  );
}
void __stdcall WrappedID3D11CryptoSession::GetDecoderProfile(
  GUID * pDecoderProfile
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11CryptoSession << ")::GetDecoderProfile\n";
  m_pID3D11CryptoSession->GetDecoderProfile(
    pDecoderProfile
  );
}
HRESULT __stdcall WrappedID3D11CryptoSession::GetCertificateSize(
  UINT * pCertificateSize
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11CryptoSession << ")::GetCertificateSize\n";
  auto ret = m_pID3D11CryptoSession->GetCertificateSize(
    pCertificateSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CryptoSession::GetCertificate(
  UINT CertificateSize, 
  BYTE * pCertificate
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11CryptoSession << ")::GetCertificate\n";
  auto ret = m_pID3D11CryptoSession->GetCertificate(
    CertificateSize, 
    pCertificate
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11CryptoSession::GetCryptoSessionHandle(
  HANDLE * pCryptoSessionHandle
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11CryptoSession << ")::GetCryptoSessionHandle\n";
  m_pID3D11CryptoSession->GetCryptoSessionHandle(
    pCryptoSessionHandle
  );
}
void __stdcall WrappedID3D11CryptoSession::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11CryptoSession::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CryptoSession::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CryptoSession::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11CryptoSession(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11CryptoSession::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11CryptoSession(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11CryptoSession)) {
      *ppvObject = (ID3D11CryptoSession*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11CryptoSession::AddRef() {
  out() << "ID3D11CryptoSession(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11CryptoSession::Release() {
  out() << "ID3D11CryptoSession(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoDecoderOutputView::GetDesc(
  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11VideoDecoderOutputView << ")::GetDesc\n";
  m_pID3D11VideoDecoderOutputView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11VideoDecoderOutputView::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11VideoDecoderOutputView::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoDecoderOutputView::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoderOutputView::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoderOutputView::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDecoderOutputView::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoDecoderOutputView(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoDecoderOutputView)) {
      *ppvObject = (ID3D11VideoDecoderOutputView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDecoderOutputView::AddRef() {
  out() << "ID3D11VideoDecoderOutputView(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDecoderOutputView::Release() {
  out() << "ID3D11VideoDecoderOutputView(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoProcessorInputView::GetDesc(
  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11VideoProcessorInputView << ")::GetDesc\n";
  m_pID3D11VideoProcessorInputView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11VideoProcessorInputView::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11VideoProcessorInputView::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoProcessorInputView::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorInputView::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorInputView::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorInputView::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoProcessorInputView(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoProcessorInputView)) {
      *ppvObject = (ID3D11VideoProcessorInputView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorInputView::AddRef() {
  out() << "ID3D11VideoProcessorInputView(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorInputView::Release() {
  out() << "ID3D11VideoProcessorInputView(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoProcessorOutputView::GetDesc(
  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11VideoProcessorOutputView << ")::GetDesc\n";
  m_pID3D11VideoProcessorOutputView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11VideoProcessorOutputView::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11VideoProcessorOutputView::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoProcessorOutputView::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorOutputView::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorOutputView::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorOutputView::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoProcessorOutputView(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoProcessorOutputView)) {
      *ppvObject = (ID3D11VideoProcessorOutputView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorOutputView::AddRef() {
  out() << "ID3D11VideoProcessorOutputView(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorOutputView::Release() {
  out() << "ID3D11VideoProcessorOutputView(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::SetFeatureMask(
  UINT Mask
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::SetFeatureMask\n";
  auto ret = m_pID3D11Debug->SetFeatureMask(
    Mask
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11Debug::GetFeatureMask() {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::GetFeatureMask\n";
  auto ret = m_pID3D11Debug->GetFeatureMask();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::SetPresentPerRenderOpDelay(
  UINT Milliseconds
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::SetPresentPerRenderOpDelay\n";
  auto ret = m_pID3D11Debug->SetPresentPerRenderOpDelay(
    Milliseconds
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11Debug::GetPresentPerRenderOpDelay() {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::GetPresentPerRenderOpDelay\n";
  auto ret = m_pID3D11Debug->GetPresentPerRenderOpDelay();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::SetSwapChain(
  IDXGISwapChain * pSwapChain
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::SetSwapChain\n";
  auto unwrapped_pSwapChain = unwrap(pSwapChain);
  auto ret = m_pID3D11Debug->SetSwapChain(
    unwrapped_pSwapChain
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::GetSwapChain(
  IDXGISwapChain ** ppSwapChain
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::GetSwapChain\n";
  auto ret = m_pID3D11Debug->GetSwapChain(
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::ValidateContext(
  ID3D11DeviceContext * pContext
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::ValidateContext\n";
  auto unwrapped_pContext = unwrap(pContext);
  auto ret = m_pID3D11Debug->ValidateContext(
    unwrapped_pContext
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::ReportLiveDeviceObjects(
  D3D11_RLDO_FLAGS Flags
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::ReportLiveDeviceObjects\n";
  auto ret = m_pID3D11Debug->ReportLiveDeviceObjects(
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::ValidateContextForDispatch(
  ID3D11DeviceContext * pContext
) {
  out() << "ID3D11Debug(" << m_pID3D11Debug << ")::ValidateContextForDispatch\n";
  auto unwrapped_pContext = unwrap(pContext);
  auto ret = m_pID3D11Debug->ValidateContextForDispatch(
    unwrapped_pContext
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Debug::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Debug(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Debug)) {
      *ppvObject = (ID3D11Debug*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Debug::AddRef() {
  out() << "ID3D11Debug(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Debug::Release() {
  out() << "ID3D11Debug(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11SwitchToRef::SetUseRef(
  BOOL UseRef
) {
  out() << "ID3D11SwitchToRef(" << m_pID3D11SwitchToRef << ")::SetUseRef\n";
  auto ret = m_pID3D11SwitchToRef->SetUseRef(
    UseRef
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11SwitchToRef::GetUseRef() {
  out() << "ID3D11SwitchToRef(" << m_pID3D11SwitchToRef << ")::GetUseRef\n";
  auto ret = m_pID3D11SwitchToRef->GetUseRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11SwitchToRef::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11SwitchToRef(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11SwitchToRef)) {
      *ppvObject = (ID3D11SwitchToRef*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11SwitchToRef::AddRef() {
  out() << "ID3D11SwitchToRef(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11SwitchToRef::Release() {
  out() << "ID3D11SwitchToRef(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11TracingDevice::SetShaderTrackingOptionsByType(
  UINT ResourceTypeFlags, 
  UINT Options
) {
  out() << "ID3D11TracingDevice(" << m_pID3D11TracingDevice << ")::SetShaderTrackingOptionsByType\n";
  auto ret = m_pID3D11TracingDevice->SetShaderTrackingOptionsByType(
    ResourceTypeFlags, 
    Options
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11TracingDevice::SetShaderTrackingOptions(
  struct IUnknown * pShader, 
  UINT Options
) {
  out() << "ID3D11TracingDevice(" << m_pID3D11TracingDevice << ")::SetShaderTrackingOptions\n";
  auto unwrapped_pShader = unwrap(pShader);
  auto ret = m_pID3D11TracingDevice->SetShaderTrackingOptions(
    unwrapped_pShader, 
    Options
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11TracingDevice::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11TracingDevice(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11TracingDevice)) {
      *ppvObject = (ID3D11TracingDevice*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11TracingDevice::AddRef() {
  out() << "ID3D11TracingDevice(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11TracingDevice::Release() {
  out() << "ID3D11TracingDevice(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RefTrackingOptions::SetTrackingOptions(
  UINT uOptions
) {
  out() << "ID3D11RefTrackingOptions(" << m_pID3D11RefTrackingOptions << ")::SetTrackingOptions\n";
  auto ret = m_pID3D11RefTrackingOptions->SetTrackingOptions(
    uOptions
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RefTrackingOptions::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11RefTrackingOptions(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11RefTrackingOptions)) {
      *ppvObject = (ID3D11RefTrackingOptions*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11RefTrackingOptions::AddRef() {
  out() << "ID3D11RefTrackingOptions(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11RefTrackingOptions::Release() {
  out() << "ID3D11RefTrackingOptions(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RefDefaultTrackingOptions::SetTrackingOptions(
  UINT ResourceTypeFlags, 
  UINT Options
) {
  out() << "ID3D11RefDefaultTrackingOptions(" << m_pID3D11RefDefaultTrackingOptions << ")::SetTrackingOptions\n";
  auto ret = m_pID3D11RefDefaultTrackingOptions->SetTrackingOptions(
    ResourceTypeFlags, 
    Options
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RefDefaultTrackingOptions::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11RefDefaultTrackingOptions(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11RefDefaultTrackingOptions)) {
      *ppvObject = (ID3D11RefDefaultTrackingOptions*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11RefDefaultTrackingOptions::AddRef() {
  out() << "ID3D11RefDefaultTrackingOptions(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11RefDefaultTrackingOptions::Release() {
  out() << "ID3D11RefDefaultTrackingOptions(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::SetMessageCountLimit(
  UINT64 MessageCountLimit
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::SetMessageCountLimit\n";
  auto ret = m_pID3D11InfoQueue->SetMessageCountLimit(
    MessageCountLimit
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::ClearStoredMessages() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::ClearStoredMessages\n";
  return m_pID3D11InfoQueue->ClearStoredMessages();
}
HRESULT __stdcall WrappedID3D11InfoQueue::GetMessageW(
  UINT64 MessageIndex, 
  D3D11_MESSAGE * pMessage, 
  SIZE_T * pMessageByteLength
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetMessageW\n";
  auto unwrapped_pMessageByteLength = unwrap(pMessageByteLength);
  auto ret = m_pID3D11InfoQueue->GetMessageW(
    MessageIndex, 
    pMessage, 
    unwrapped_pMessageByteLength
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetNumMessagesAllowedByStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetNumMessagesAllowedByStorageFilter\n";
  auto ret = m_pID3D11InfoQueue->GetNumMessagesAllowedByStorageFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetNumMessagesDeniedByStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetNumMessagesDeniedByStorageFilter\n";
  auto ret = m_pID3D11InfoQueue->GetNumMessagesDeniedByStorageFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetNumStoredMessages() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetNumStoredMessages\n";
  auto ret = m_pID3D11InfoQueue->GetNumStoredMessages();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetNumStoredMessagesAllowedByRetrievalFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetNumStoredMessagesAllowedByRetrievalFilter\n";
  auto ret = m_pID3D11InfoQueue->GetNumStoredMessagesAllowedByRetrievalFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetNumMessagesDiscardedByMessageCountLimit() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetNumMessagesDiscardedByMessageCountLimit\n";
  auto ret = m_pID3D11InfoQueue->GetNumMessagesDiscardedByMessageCountLimit();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11InfoQueue::GetMessageCountLimit() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetMessageCountLimit\n";
  auto ret = m_pID3D11InfoQueue->GetMessageCountLimit();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::AddStorageFilterEntries(
  D3D11_INFO_QUEUE_FILTER * pFilter
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::AddStorageFilterEntries\n";
  auto unwrapped_pFilter = unwrap(pFilter);
  auto ret = m_pID3D11InfoQueue->AddStorageFilterEntries(
    unwrapped_pFilter
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::GetStorageFilter(
  D3D11_INFO_QUEUE_FILTER * pFilter, 
  SIZE_T * pFilterByteLength
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetStorageFilter\n";
  auto unwrapped_pFilterByteLength = unwrap(pFilterByteLength);
  auto ret = m_pID3D11InfoQueue->GetStorageFilter(
    pFilter, 
    unwrapped_pFilterByteLength
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::ClearStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::ClearStorageFilter\n";
  return m_pID3D11InfoQueue->ClearStorageFilter();
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushEmptyStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushEmptyStorageFilter\n";
  auto ret = m_pID3D11InfoQueue->PushEmptyStorageFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushCopyOfStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushCopyOfStorageFilter\n";
  auto ret = m_pID3D11InfoQueue->PushCopyOfStorageFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushStorageFilter(
  D3D11_INFO_QUEUE_FILTER * pFilter
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushStorageFilter\n";
  auto unwrapped_pFilter = unwrap(pFilter);
  auto ret = m_pID3D11InfoQueue->PushStorageFilter(
    unwrapped_pFilter
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::PopStorageFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PopStorageFilter\n";
  return m_pID3D11InfoQueue->PopStorageFilter();
}
UINT __stdcall WrappedID3D11InfoQueue::GetStorageFilterStackSize() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetStorageFilterStackSize\n";
  auto ret = m_pID3D11InfoQueue->GetStorageFilterStackSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::AddRetrievalFilterEntries(
  D3D11_INFO_QUEUE_FILTER * pFilter
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::AddRetrievalFilterEntries\n";
  auto unwrapped_pFilter = unwrap(pFilter);
  auto ret = m_pID3D11InfoQueue->AddRetrievalFilterEntries(
    unwrapped_pFilter
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::GetRetrievalFilter(
  D3D11_INFO_QUEUE_FILTER * pFilter, 
  SIZE_T * pFilterByteLength
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetRetrievalFilter\n";
  auto unwrapped_pFilterByteLength = unwrap(pFilterByteLength);
  auto ret = m_pID3D11InfoQueue->GetRetrievalFilter(
    pFilter, 
    unwrapped_pFilterByteLength
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::ClearRetrievalFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::ClearRetrievalFilter\n";
  return m_pID3D11InfoQueue->ClearRetrievalFilter();
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushEmptyRetrievalFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushEmptyRetrievalFilter\n";
  auto ret = m_pID3D11InfoQueue->PushEmptyRetrievalFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushCopyOfRetrievalFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushCopyOfRetrievalFilter\n";
  auto ret = m_pID3D11InfoQueue->PushCopyOfRetrievalFilter();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::PushRetrievalFilter(
  D3D11_INFO_QUEUE_FILTER * pFilter
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PushRetrievalFilter\n";
  auto unwrapped_pFilter = unwrap(pFilter);
  auto ret = m_pID3D11InfoQueue->PushRetrievalFilter(
    unwrapped_pFilter
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::PopRetrievalFilter() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::PopRetrievalFilter\n";
  return m_pID3D11InfoQueue->PopRetrievalFilter();
}
UINT __stdcall WrappedID3D11InfoQueue::GetRetrievalFilterStackSize() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetRetrievalFilterStackSize\n";
  auto ret = m_pID3D11InfoQueue->GetRetrievalFilterStackSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::AddMessage(
  D3D11_MESSAGE_CATEGORY Category, 
  D3D11_MESSAGE_SEVERITY Severity, 
  D3D11_MESSAGE_ID ID, 
  LPCSTR pDescription
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::AddMessage\n";
  auto ret = m_pID3D11InfoQueue->AddMessage(
    Category, 
    Severity, 
    ID, 
    pDescription
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::AddApplicationMessage(
  D3D11_MESSAGE_SEVERITY Severity, 
  LPCSTR pDescription
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::AddApplicationMessage\n";
  auto ret = m_pID3D11InfoQueue->AddApplicationMessage(
    Severity, 
    pDescription
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::SetBreakOnCategory(
  D3D11_MESSAGE_CATEGORY Category, 
  BOOL bEnable
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::SetBreakOnCategory\n";
  auto ret = m_pID3D11InfoQueue->SetBreakOnCategory(
    Category, 
    bEnable
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::SetBreakOnSeverity(
  D3D11_MESSAGE_SEVERITY Severity, 
  BOOL bEnable
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::SetBreakOnSeverity\n";
  auto ret = m_pID3D11InfoQueue->SetBreakOnSeverity(
    Severity, 
    bEnable
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::SetBreakOnID(
  D3D11_MESSAGE_ID ID, 
  BOOL bEnable
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::SetBreakOnID\n";
  auto ret = m_pID3D11InfoQueue->SetBreakOnID(
    ID, 
    bEnable
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11InfoQueue::GetBreakOnCategory(
  D3D11_MESSAGE_CATEGORY Category
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetBreakOnCategory\n";
  auto ret = m_pID3D11InfoQueue->GetBreakOnCategory(
    Category
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11InfoQueue::GetBreakOnSeverity(
  D3D11_MESSAGE_SEVERITY Severity
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetBreakOnSeverity\n";
  auto ret = m_pID3D11InfoQueue->GetBreakOnSeverity(
    Severity
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11InfoQueue::GetBreakOnID(
  D3D11_MESSAGE_ID ID
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetBreakOnID\n";
  auto ret = m_pID3D11InfoQueue->GetBreakOnID(
    ID
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11InfoQueue::SetMuteDebugOutput(
  BOOL bMute
) {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::SetMuteDebugOutput\n";
  m_pID3D11InfoQueue->SetMuteDebugOutput(
    bMute
  );
}
BOOL __stdcall WrappedID3D11InfoQueue::GetMuteDebugOutput() {
  out() << "ID3D11InfoQueue(" << m_pID3D11InfoQueue << ")::GetMuteDebugOutput\n";
  auto ret = m_pID3D11InfoQueue->GetMuteDebugOutput();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11InfoQueue::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11InfoQueue(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11InfoQueue)) {
      *ppvObject = (ID3D11InfoQueue*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11InfoQueue::AddRef() {
  out() << "ID3D11InfoQueue(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11InfoQueue::Release() {
  out() << "ID3D11InfoQueue(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3DDeviceContextState::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3DDeviceContextState(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3DDeviceContextState::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3DDeviceContextState(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDeviceContextState::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3DDeviceContextState(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDeviceContextState::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3DDeviceContextState(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DDeviceContextState::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3DDeviceContextState(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3DDeviceContextState)) {
      *ppvObject = (ID3DDeviceContextState*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3DDeviceContextState::AddRef() {
  out() << "ID3DDeviceContextState(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3DDeviceContextState::Release() {
  out() << "ID3DDeviceContextState(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedIDXGIDisplayControl::IsStereoEnabled() {
  out() << "IDXGIDisplayControl(" << m_pIDXGIDisplayControl << ")::IsStereoEnabled\n";
  auto ret = m_pIDXGIDisplayControl->IsStereoEnabled();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIDisplayControl::SetStereoEnabled(
  BOOL enabled
) {
  out() << "IDXGIDisplayControl(" << m_pIDXGIDisplayControl << ")::SetStereoEnabled\n";
  m_pIDXGIDisplayControl->SetStereoEnabled(
    enabled
  );
}
HRESULT __stdcall WrappedIDXGIDisplayControl::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIDisplayControl(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIDisplayControl)) {
      *ppvObject = (IDXGIDisplayControl*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIDisplayControl::AddRef() {
  out() << "IDXGIDisplayControl(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIDisplayControl::Release() {
  out() << "IDXGIDisplayControl(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIOutputDuplication::GetDesc(
  DXGI_OUTDUPL_DESC * pDesc
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::GetDesc\n";
  m_pIDXGIOutputDuplication->GetDesc(
    pDesc
  );
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::AcquireNextFrame(
  UINT TimeoutInMilliseconds, 
  DXGI_OUTDUPL_FRAME_INFO * pFrameInfo, 
  IDXGIResource ** ppDesktopResource
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::AcquireNextFrame\n";
  auto ret = m_pIDXGIOutputDuplication->AcquireNextFrame(
    TimeoutInMilliseconds, 
    pFrameInfo, 
    ppDesktopResource
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDesktopResource && *ppDesktopResource ) *ppDesktopResource = getWrapper<IDXGIResource, WrappedIDXGIResource1>(*ppDesktopResource);
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::GetFrameDirtyRects(
  UINT DirtyRectsBufferSize, 
  RECT * pDirtyRectsBuffer, 
  UINT * pDirtyRectsBufferSizeRequired
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::GetFrameDirtyRects\n";
  auto unwrapped_pDirtyRectsBuffer = unwrap(pDirtyRectsBuffer);
  auto ret = m_pIDXGIOutputDuplication->GetFrameDirtyRects(
    DirtyRectsBufferSize, 
    unwrapped_pDirtyRectsBuffer, 
    pDirtyRectsBufferSizeRequired
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::GetFrameMoveRects(
  UINT MoveRectsBufferSize, 
  DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer, 
  UINT * pMoveRectsBufferSizeRequired
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::GetFrameMoveRects\n";
  auto unwrapped_pMoveRectBuffer = unwrap(pMoveRectBuffer);
  auto ret = m_pIDXGIOutputDuplication->GetFrameMoveRects(
    MoveRectsBufferSize, 
    unwrapped_pMoveRectBuffer, 
    pMoveRectsBufferSizeRequired
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::GetFramePointerShape(
  UINT PointerShapeBufferSize, 
  void * pPointerShapeBuffer, 
  UINT * pPointerShapeBufferSizeRequired, 
  DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::GetFramePointerShape\n";
  auto unwrapped_pPointerShapeBuffer = unwrap(pPointerShapeBuffer);
  auto ret = m_pIDXGIOutputDuplication->GetFramePointerShape(
    PointerShapeBufferSize, 
    unwrapped_pPointerShapeBuffer, 
    pPointerShapeBufferSizeRequired, 
    pPointerShapeInfo
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::MapDesktopSurface(
  DXGI_MAPPED_RECT * pLockedRect
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::MapDesktopSurface\n";
  auto ret = m_pIDXGIOutputDuplication->MapDesktopSurface(
    pLockedRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::UnMapDesktopSurface() {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::UnMapDesktopSurface\n";
  auto ret = m_pIDXGIOutputDuplication->UnMapDesktopSurface();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::ReleaseFrame() {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIOutputDuplication << ")::ReleaseFrame\n";
  auto ret = m_pIDXGIOutputDuplication->ReleaseFrame();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIOutputDuplication(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutputDuplication::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIOutputDuplication(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIOutputDuplication)) {
      *ppvObject = (IDXGIOutputDuplication*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIOutputDuplication::AddRef() {
  out() << "IDXGIOutputDuplication(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIOutputDuplication::Release() {
  out() << "IDXGIOutputDuplication(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetResource(
  const IID & riid, 
  void ** ppParentResource, 
  UINT * pSubresourceIndex
) {
  out() << "IDXGISurface2(" << m_pIDXGISurface2 << ")::GetResource\n";
  auto ret = m_pIDXGISurface2->GetResource(
    riid, 
    ppParentResource, 
    pSubresourceIndex
  );
  out() << "\treturned " << ret << "\n";
  if(*pSubresourceIndex)
    HandleWrap(__uuidof(ID3D11Resource), (void**)pSubresourceIndex);
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetDC(
  BOOL Discard, 
  HDC * phdc
) {
  out() << "IDXGISurface2(" << m_pIDXGISurface1 << ")::GetDC\n";
  auto ret = m_pIDXGISurface1->GetDC(
    Discard, 
    phdc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::ReleaseDC(
  RECT * pDirtyRect
) {
  out() << "IDXGISurface2(" << m_pIDXGISurface1 << ")::ReleaseDC\n";
  auto unwrapped_pDirtyRect = unwrap(pDirtyRect);
  auto ret = m_pIDXGISurface1->ReleaseDC(
    unwrapped_pDirtyRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetDesc(
  DXGI_SURFACE_DESC * pDesc
) {
  out() << "IDXGISurface2(" << m_pIDXGISurface << ")::GetDesc\n";
  auto ret = m_pIDXGISurface->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::Map(
  DXGI_MAPPED_RECT * pLockedRect, 
  UINT MapFlags
) {
  out() << "IDXGISurface2(" << m_pIDXGISurface << ")::Map\n";
  auto ret = m_pIDXGISurface->Map(
    pLockedRect, 
    MapFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::Unmap() {
  out() << "IDXGISurface2(" << m_pIDXGISurface << ")::Unmap\n";
  auto ret = m_pIDXGISurface->Unmap();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetDevice(
  const IID & riid, 
  void ** ppDevice
) {
  out() << "IDXGISurface2(" << m_pIDXGIDeviceSubObject << ")::GetDevice\n";
  auto ret = m_pIDXGIDeviceSubObject->GetDevice(
    riid, 
    ppDevice
  );
  out() << "\treturned " << ret << "\n";
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGISurface2(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGISurface2(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGISurface2(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGISurface2(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGISurface2::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGISurface2(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGISurface2)) {
      *ppvObject = (IDXGISurface2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISurface1)) {
      *ppvObject = (IDXGISurface1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISurface)) {
      *ppvObject = (IDXGISurface*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDeviceSubObject)) {
      *ppvObject = (IDXGIDeviceSubObject*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGISurface2::AddRef() {
  out() << "IDXGISurface2(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGISurface2::Release() {
  out() << "IDXGISurface2(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::CreateSubresourceSurface(
  UINT index, 
  IDXGISurface2 ** ppSurface
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource1 << ")::CreateSubresourceSurface\n";
  auto ret = m_pIDXGIResource1->CreateSubresourceSurface(
    index, 
    ppSurface
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSurface && *ppSurface ) *ppSurface = getWrapper<IDXGISurface2, WrappedIDXGISurface2>(*ppSurface);
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::CreateSharedHandle(
  const SECURITY_ATTRIBUTES * pAttributes, 
  DWORD dwAccess, 
  LPCWSTR lpName, 
  HANDLE * pHandle
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource1 << ")::CreateSharedHandle\n";
  auto unwrapped_pAttributes = unwrap(pAttributes);
  auto ret = m_pIDXGIResource1->CreateSharedHandle(
    unwrapped_pAttributes, 
    dwAccess, 
    lpName, 
    pHandle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetSharedHandle(
  HANDLE * pSharedHandle
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource << ")::GetSharedHandle\n";
  auto ret = m_pIDXGIResource->GetSharedHandle(
    pSharedHandle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetUsage(
  DXGI_USAGE * pUsage
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource << ")::GetUsage\n";
  auto unwrapped_pUsage = unwrap(pUsage);
  auto ret = m_pIDXGIResource->GetUsage(
    unwrapped_pUsage
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::SetEvictionPriority(
  UINT EvictionPriority
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource << ")::SetEvictionPriority\n";
  auto ret = m_pIDXGIResource->SetEvictionPriority(
    EvictionPriority
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetEvictionPriority(
  UINT * pEvictionPriority
) {
  out() << "IDXGIResource1(" << m_pIDXGIResource << ")::GetEvictionPriority\n";
  auto ret = m_pIDXGIResource->GetEvictionPriority(
    pEvictionPriority
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetDevice(
  const IID & riid, 
  void ** ppDevice
) {
  out() << "IDXGIResource1(" << m_pIDXGIDeviceSubObject << ")::GetDevice\n";
  auto ret = m_pIDXGIDeviceSubObject->GetDevice(
    riid, 
    ppDevice
  );
  out() << "\treturned " << ret << "\n";
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIResource1(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIResource1(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIResource1(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIResource1(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIResource1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIResource1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIResource1)) {
      *ppvObject = (IDXGIResource1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIResource)) {
      *ppvObject = (IDXGIResource*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDeviceSubObject)) {
      *ppvObject = (IDXGIDeviceSubObject*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIResource1::AddRef() {
  out() << "IDXGIResource1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIResource1::Release() {
  out() << "IDXGIResource1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11BlendState1::GetDesc1(
  D3D11_BLEND_DESC1 * pDesc
) {
  out() << "ID3D11BlendState1(" << m_pID3D11BlendState1 << ")::GetDesc1\n";
  m_pID3D11BlendState1->GetDesc1(
    pDesc
  );
}
void __stdcall WrappedID3D11BlendState1::GetDesc(
  D3D11_BLEND_DESC * pDesc
) {
  out() << "ID3D11BlendState1(" << m_pID3D11BlendState << ")::GetDesc\n";
  m_pID3D11BlendState->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11BlendState1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11BlendState1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11BlendState1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11BlendState1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11BlendState1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11BlendState1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11BlendState1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11BlendState1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11BlendState1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11BlendState1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11BlendState1)) {
      *ppvObject = (ID3D11BlendState1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11BlendState)) {
      *ppvObject = (ID3D11BlendState*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11BlendState1::AddRef() {
  out() << "ID3D11BlendState1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11BlendState1::Release() {
  out() << "ID3D11BlendState1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::CheckVideoProcessorFormatConversion(
  DXGI_FORMAT InputFormat, 
  DXGI_COLOR_SPACE_TYPE InputColorSpace, 
  DXGI_FORMAT OutputFormat, 
  DXGI_COLOR_SPACE_TYPE OutputColorSpace, 
  BOOL * pSupported
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator1 << ")::CheckVideoProcessorFormatConversion\n";
  auto ret = m_pID3D11VideoProcessorEnumerator1->CheckVideoProcessorFormatConversion(
    InputFormat, 
    InputColorSpace, 
    OutputFormat, 
    OutputColorSpace, 
    pSupported
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetVideoProcessorContentDesc(
  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::GetVideoProcessorContentDesc\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->GetVideoProcessorContentDesc(
    pContentDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::CheckVideoProcessorFormat(
  DXGI_FORMAT Format, 
  UINT * pFlags
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::CheckVideoProcessorFormat\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->CheckVideoProcessorFormat(
    Format, 
    pFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetVideoProcessorCaps(
  D3D11_VIDEO_PROCESSOR_CAPS * pCaps
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::GetVideoProcessorCaps\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->GetVideoProcessorCaps(
    pCaps
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetVideoProcessorRateConversionCaps(
  UINT TypeIndex, 
  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::GetVideoProcessorRateConversionCaps\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->GetVideoProcessorRateConversionCaps(
    TypeIndex, 
    pCaps
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetVideoProcessorCustomRate(
  UINT TypeIndex, 
  UINT CustomRateIndex, 
  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::GetVideoProcessorCustomRate\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->GetVideoProcessorCustomRate(
    TypeIndex, 
    CustomRateIndex, 
    pRate
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetVideoProcessorFilterRange(
  D3D11_VIDEO_PROCESSOR_FILTER Filter, 
  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11VideoProcessorEnumerator << ")::GetVideoProcessorFilterRange\n";
  auto ret = m_pID3D11VideoProcessorEnumerator->GetVideoProcessorFilterRange(
    Filter, 
    pRange
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoProcessorEnumerator1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoProcessorEnumerator1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoProcessorEnumerator1)) {
      *ppvObject = (ID3D11VideoProcessorEnumerator1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoProcessorEnumerator)) {
      *ppvObject = (ID3D11VideoProcessorEnumerator*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorEnumerator1::AddRef() {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoProcessorEnumerator1::Release() {
  out() << "ID3D11VideoProcessorEnumerator1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
INT __stdcall WrappedID3DUserDefinedAnnotation::BeginEvent(
  LPCWSTR Name
) {
  out() << "ID3DUserDefinedAnnotation(" << m_pID3DUserDefinedAnnotation << ")::BeginEvent\n";
  auto ret = m_pID3DUserDefinedAnnotation->BeginEvent(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
INT __stdcall WrappedID3DUserDefinedAnnotation::EndEvent() {
  out() << "ID3DUserDefinedAnnotation(" << m_pID3DUserDefinedAnnotation << ")::EndEvent\n";
  auto ret = m_pID3DUserDefinedAnnotation->EndEvent();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3DUserDefinedAnnotation::SetMarker(
  LPCWSTR Name
) {
  out() << "ID3DUserDefinedAnnotation(" << m_pID3DUserDefinedAnnotation << ")::SetMarker\n";
  m_pID3DUserDefinedAnnotation->SetMarker(
    Name
  );
}
BOOL __stdcall WrappedID3DUserDefinedAnnotation::GetStatus() {
  out() << "ID3DUserDefinedAnnotation(" << m_pID3DUserDefinedAnnotation << ")::GetStatus\n";
  auto ret = m_pID3DUserDefinedAnnotation->GetStatus();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3DUserDefinedAnnotation::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3DUserDefinedAnnotation(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3DUserDefinedAnnotation)) {
      *ppvObject = (ID3DUserDefinedAnnotation*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3DUserDefinedAnnotation::AddRef() {
  out() << "ID3DUserDefinedAnnotation(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3DUserDefinedAnnotation::Release() {
  out() << "ID3DUserDefinedAnnotation(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::PresentBuffer(
  UINT BufferToPresent, 
  UINT SyncInterval, 
  UINT Flags
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::PresentBuffer\n";
  auto ret = m_pIDXGIDecodeSwapChain->PresentBuffer(
    BufferToPresent, 
    SyncInterval, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::SetSourceRect(
  const RECT * pRect
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::SetSourceRect\n";
  auto unwrapped_pRect = unwrap(pRect);
  auto ret = m_pIDXGIDecodeSwapChain->SetSourceRect(
    unwrapped_pRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::SetTargetRect(
  const RECT * pRect
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::SetTargetRect\n";
  auto unwrapped_pRect = unwrap(pRect);
  auto ret = m_pIDXGIDecodeSwapChain->SetTargetRect(
    unwrapped_pRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::SetDestSize(
  UINT Width, 
  UINT Height
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::SetDestSize\n";
  auto ret = m_pIDXGIDecodeSwapChain->SetDestSize(
    Width, 
    Height
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::GetSourceRect(
  RECT * pRect
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::GetSourceRect\n";
  auto ret = m_pIDXGIDecodeSwapChain->GetSourceRect(
    pRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::GetTargetRect(
  RECT * pRect
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::GetTargetRect\n";
  auto ret = m_pIDXGIDecodeSwapChain->GetTargetRect(
    pRect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::GetDestSize(
  UINT * pWidth, 
  UINT * pHeight
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::GetDestSize\n";
  auto ret = m_pIDXGIDecodeSwapChain->GetDestSize(
    pWidth, 
    pHeight
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::SetColorSpace(
  DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS ColorSpace
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::SetColorSpace\n";
  auto ret = m_pIDXGIDecodeSwapChain->SetColorSpace(
    ColorSpace
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS __stdcall WrappedIDXGIDecodeSwapChain::GetColorSpace() {
  out() << "IDXGIDecodeSwapChain(" << m_pIDXGIDecodeSwapChain << ")::GetColorSpace\n";
  auto ret = m_pIDXGIDecodeSwapChain->GetColorSpace();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDecodeSwapChain::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIDecodeSwapChain(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIDecodeSwapChain)) {
      *ppvObject = (IDXGIDecodeSwapChain*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIDecodeSwapChain::AddRef() {
  out() << "IDXGIDecodeSwapChain(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIDecodeSwapChain::Release() {
  out() << "IDXGIDecodeSwapChain(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactoryMedia::CreateSwapChainForCompositionSurfaceHandle(
  struct IUnknown * pDevice, 
  HANDLE hSurface, 
  const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
  IDXGIOutput * pRestrictToOutput, 
  IDXGISwapChain1 ** ppSwapChain
) {
  out() << "IDXGIFactoryMedia(" << m_pIDXGIFactoryMedia << ")::CreateSwapChainForCompositionSurfaceHandle\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pRestrictToOutput = unwrap(pRestrictToOutput);
  auto ret = m_pIDXGIFactoryMedia->CreateSwapChainForCompositionSurfaceHandle(
    unwrapped_pDevice, 
    hSurface, 
    unwrapped_pDesc, 
    unwrapped_pRestrictToOutput, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain1, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactoryMedia::CreateDecodeSwapChainForCompositionSurfaceHandle(
  struct IUnknown * pDevice, 
  HANDLE hSurface, 
  DXGI_DECODE_SWAP_CHAIN_DESC * pDesc, 
  IDXGIResource * pYuvDecodeBuffers, 
  IDXGIOutput * pRestrictToOutput, 
  IDXGIDecodeSwapChain ** ppSwapChain
) {
  out() << "IDXGIFactoryMedia(" << m_pIDXGIFactoryMedia << ")::CreateDecodeSwapChainForCompositionSurfaceHandle\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pYuvDecodeBuffers = unwrap(pYuvDecodeBuffers);
  auto unwrapped_pRestrictToOutput = unwrap(pRestrictToOutput);
  auto ret = m_pIDXGIFactoryMedia->CreateDecodeSwapChainForCompositionSurfaceHandle(
    unwrapped_pDevice, 
    hSurface, 
    unwrapped_pDesc, 
    unwrapped_pYuvDecodeBuffers, 
    unwrapped_pRestrictToOutput, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGIDecodeSwapChain, WrappedIDXGIDecodeSwapChain>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactoryMedia::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIFactoryMedia(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIFactoryMedia)) {
      *ppvObject = (IDXGIFactoryMedia*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIFactoryMedia::AddRef() {
  out() << "IDXGIFactoryMedia(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIFactoryMedia::Release() {
  out() << "IDXGIFactoryMedia(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChainMedia::GetFrameStatisticsMedia(
  DXGI_FRAME_STATISTICS_MEDIA * pStats
) {
  out() << "IDXGISwapChainMedia(" << m_pIDXGISwapChainMedia << ")::GetFrameStatisticsMedia\n";
  auto ret = m_pIDXGISwapChainMedia->GetFrameStatisticsMedia(
    pStats
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChainMedia::SetPresentDuration(
  UINT Duration
) {
  out() << "IDXGISwapChainMedia(" << m_pIDXGISwapChainMedia << ")::SetPresentDuration\n";
  auto ret = m_pIDXGISwapChainMedia->SetPresentDuration(
    Duration
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChainMedia::CheckPresentDurationSupport(
  UINT DesiredPresentDuration, 
  UINT * pClosestSmallerPresentDuration, 
  UINT * pClosestLargerPresentDuration
) {
  out() << "IDXGISwapChainMedia(" << m_pIDXGISwapChainMedia << ")::CheckPresentDurationSupport\n";
  auto ret = m_pIDXGISwapChainMedia->CheckPresentDurationSupport(
    DesiredPresentDuration, 
    pClosestSmallerPresentDuration, 
    pClosestLargerPresentDuration
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChainMedia::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGISwapChainMedia(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGISwapChainMedia)) {
      *ppvObject = (IDXGISwapChainMedia*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGISwapChainMedia::AddRef() {
  out() << "IDXGISwapChainMedia(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGISwapChainMedia::Release() {
  out() << "IDXGISwapChainMedia(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture2D1::GetDesc1(
  D3D11_TEXTURE2D_DESC1 * pDesc
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11Texture2D1 << ")::GetDesc1\n";
  m_pID3D11Texture2D1->GetDesc1(
    pDesc
  );
}
void __stdcall WrappedID3D11Texture2D1::GetDesc(
  D3D11_TEXTURE2D_DESC * pDesc
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11Texture2D << ")::GetDesc\n";
  m_pID3D11Texture2D->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11Texture2D1::GetType(
  D3D11_RESOURCE_DIMENSION * pResourceDimension
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11Resource << ")::GetType\n";
  m_pID3D11Resource->GetType(
    pResourceDimension
  );
}
void __stdcall WrappedID3D11Texture2D1::SetEvictionPriority(
  UINT EvictionPriority
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11Resource << ")::SetEvictionPriority\n";
  m_pID3D11Resource->SetEvictionPriority(
    EvictionPriority
  );
}
UINT __stdcall WrappedID3D11Texture2D1::GetEvictionPriority() {
  out() << "ID3D11Texture2D1(" << m_pID3D11Resource << ")::GetEvictionPriority\n";
  auto ret = m_pID3D11Resource->GetEvictionPriority();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture2D1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Texture2D1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture2D1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture2D1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Texture2D1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture2D1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Texture2D1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Texture2D1)) {
      *ppvObject = (ID3D11Texture2D1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Texture2D)) {
      *ppvObject = (ID3D11Texture2D*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Resource)) {
      *ppvObject = (ID3D11Resource*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Texture2D1::AddRef() {
  out() << "ID3D11Texture2D1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Texture2D1::Release() {
  out() << "ID3D11Texture2D1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture3D1::GetDesc1(
  D3D11_TEXTURE3D_DESC1 * pDesc
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11Texture3D1 << ")::GetDesc1\n";
  m_pID3D11Texture3D1->GetDesc1(
    pDesc
  );
}
void __stdcall WrappedID3D11Texture3D1::GetDesc(
  D3D11_TEXTURE3D_DESC * pDesc
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11Texture3D << ")::GetDesc\n";
  m_pID3D11Texture3D->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11Texture3D1::GetType(
  D3D11_RESOURCE_DIMENSION * pResourceDimension
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11Resource << ")::GetType\n";
  m_pID3D11Resource->GetType(
    pResourceDimension
  );
}
void __stdcall WrappedID3D11Texture3D1::SetEvictionPriority(
  UINT EvictionPriority
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11Resource << ")::SetEvictionPriority\n";
  m_pID3D11Resource->SetEvictionPriority(
    EvictionPriority
  );
}
UINT __stdcall WrappedID3D11Texture3D1::GetEvictionPriority() {
  out() << "ID3D11Texture3D1(" << m_pID3D11Resource << ")::GetEvictionPriority\n";
  auto ret = m_pID3D11Resource->GetEvictionPriority();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Texture3D1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Texture3D1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture3D1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture3D1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Texture3D1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Texture3D1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Texture3D1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Texture3D1)) {
      *ppvObject = (ID3D11Texture3D1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Texture3D)) {
      *ppvObject = (ID3D11Texture3D*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Resource)) {
      *ppvObject = (ID3D11Resource*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Texture3D1::AddRef() {
  out() << "ID3D11Texture3D1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Texture3D1::Release() {
  out() << "ID3D11Texture3D1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11RasterizerState2::GetDesc2(
  D3D11_RASTERIZER_DESC2 * pDesc
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11RasterizerState2 << ")::GetDesc2\n";
  m_pID3D11RasterizerState2->GetDesc2(
    pDesc
  );
}
void __stdcall WrappedID3D11RasterizerState2::GetDesc1(
  D3D11_RASTERIZER_DESC1 * pDesc
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11RasterizerState1 << ")::GetDesc1\n";
  m_pID3D11RasterizerState1->GetDesc1(
    pDesc
  );
}
void __stdcall WrappedID3D11RasterizerState2::GetDesc(
  D3D11_RASTERIZER_DESC * pDesc
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11RasterizerState << ")::GetDesc\n";
  m_pID3D11RasterizerState->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11RasterizerState2::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11RasterizerState2::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RasterizerState2::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RasterizerState2::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11RasterizerState2(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RasterizerState2::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11RasterizerState2(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11RasterizerState2)) {
      *ppvObject = (ID3D11RasterizerState2*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11RasterizerState1)) {
      *ppvObject = (ID3D11RasterizerState1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11RasterizerState)) {
      *ppvObject = (ID3D11RasterizerState*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11RasterizerState2::AddRef() {
  out() << "ID3D11RasterizerState2(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11RasterizerState2::Release() {
  out() << "ID3D11RasterizerState2(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11ShaderResourceView1::GetDesc1(
  D3D11_SHADER_RESOURCE_VIEW_DESC1 * pDesc1
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11ShaderResourceView1 << ")::GetDesc1\n";
  m_pID3D11ShaderResourceView1->GetDesc1(
    pDesc1
  );
}
void __stdcall WrappedID3D11ShaderResourceView1::GetDesc(
  D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11ShaderResourceView << ")::GetDesc\n";
  m_pID3D11ShaderResourceView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11ShaderResourceView1::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11ShaderResourceView1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11ShaderResourceView1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderResourceView1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderResourceView1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11ShaderResourceView1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderResourceView1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ShaderResourceView1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ShaderResourceView1)) {
      *ppvObject = (ID3D11ShaderResourceView1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11ShaderResourceView)) {
      *ppvObject = (ID3D11ShaderResourceView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ShaderResourceView1::AddRef() {
  out() << "ID3D11ShaderResourceView1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ShaderResourceView1::Release() {
  out() << "ID3D11ShaderResourceView1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11RenderTargetView1::GetDesc1(
  D3D11_RENDER_TARGET_VIEW_DESC1 * pDesc1
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11RenderTargetView1 << ")::GetDesc1\n";
  m_pID3D11RenderTargetView1->GetDesc1(
    pDesc1
  );
}
void __stdcall WrappedID3D11RenderTargetView1::GetDesc(
  D3D11_RENDER_TARGET_VIEW_DESC * pDesc
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11RenderTargetView << ")::GetDesc\n";
  m_pID3D11RenderTargetView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11RenderTargetView1::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11RenderTargetView1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11RenderTargetView1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RenderTargetView1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RenderTargetView1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11RenderTargetView1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11RenderTargetView1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11RenderTargetView1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11RenderTargetView1)) {
      *ppvObject = (ID3D11RenderTargetView1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11RenderTargetView)) {
      *ppvObject = (ID3D11RenderTargetView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11RenderTargetView1::AddRef() {
  out() << "ID3D11RenderTargetView1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11RenderTargetView1::Release() {
  out() << "ID3D11RenderTargetView1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11UnorderedAccessView1::GetDesc1(
  D3D11_UNORDERED_ACCESS_VIEW_DESC1 * pDesc1
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11UnorderedAccessView1 << ")::GetDesc1\n";
  m_pID3D11UnorderedAccessView1->GetDesc1(
    pDesc1
  );
}
void __stdcall WrappedID3D11UnorderedAccessView1::GetDesc(
  D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11UnorderedAccessView << ")::GetDesc\n";
  m_pID3D11UnorderedAccessView->GetDesc(
    pDesc
  );
}
void __stdcall WrappedID3D11UnorderedAccessView1::GetResource(
  ID3D11Resource ** ppResource
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11View << ")::GetResource\n";
  m_pID3D11View->GetResource(
    ppResource
  );
  if(*ppResource)
    HandleWrap(__uuidof(ID3D11Resource), (void**)ppResource);
}
void __stdcall WrappedID3D11UnorderedAccessView1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11UnorderedAccessView1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11UnorderedAccessView1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11UnorderedAccessView1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11UnorderedAccessView1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11UnorderedAccessView1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11UnorderedAccessView1)) {
      *ppvObject = (ID3D11UnorderedAccessView1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11UnorderedAccessView)) {
      *ppvObject = (ID3D11UnorderedAccessView*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11View)) {
      *ppvObject = (ID3D11View*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11UnorderedAccessView1::AddRef() {
  out() << "ID3D11UnorderedAccessView1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11UnorderedAccessView1::Release() {
  out() << "ID3D11UnorderedAccessView1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Query1::GetDesc1(
  D3D11_QUERY_DESC1 * pDesc1
) {
  out() << "ID3D11Query1(" << m_pID3D11Query1 << ")::GetDesc1\n";
  m_pID3D11Query1->GetDesc1(
    pDesc1
  );
}
void __stdcall WrappedID3D11Query1::GetDesc(
  D3D11_QUERY_DESC * pDesc
) {
  out() << "ID3D11Query1(" << m_pID3D11Query << ")::GetDesc\n";
  m_pID3D11Query->GetDesc(
    pDesc
  );
}
UINT __stdcall WrappedID3D11Query1::GetDataSize() {
  out() << "ID3D11Query1(" << m_pID3D11Asynchronous << ")::GetDataSize\n";
  auto ret = m_pID3D11Asynchronous->GetDataSize();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Query1::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Query1(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Query1::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Query1(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Query1::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Query1(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Query1::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Query1(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Query1::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Query1(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Query1)) {
      *ppvObject = (ID3D11Query1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Query)) {
      *ppvObject = (ID3D11Query*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Asynchronous)) {
      *ppvObject = (ID3D11Asynchronous*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Query1::AddRef() {
  out() << "ID3D11Query1(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Query1::Release() {
  out() << "ID3D11Query1(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Fence::CreateSharedHandle(
  const SECURITY_ATTRIBUTES * pAttributes, 
  DWORD dwAccess, 
  LPCWSTR lpName, 
  HANDLE * pHandle
) {
  out() << "ID3D11Fence(" << m_pID3D11Fence << ")::CreateSharedHandle\n";
  auto unwrapped_pAttributes = unwrap(pAttributes);
  auto ret = m_pID3D11Fence->CreateSharedHandle(
    unwrapped_pAttributes, 
    dwAccess, 
    lpName, 
    pHandle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11Fence::GetCompletedValue() {
  out() << "ID3D11Fence(" << m_pID3D11Fence << ")::GetCompletedValue\n";
  auto ret = m_pID3D11Fence->GetCompletedValue();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Fence::SetEventOnCompletion(
  UINT64 Value, 
  HANDLE hEvent
) {
  out() << "ID3D11Fence(" << m_pID3D11Fence << ")::SetEventOnCompletion\n";
  auto ret = m_pID3D11Fence->SetEventOnCompletion(
    Value, 
    hEvent
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Fence::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11Fence(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11Fence::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Fence(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Fence::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Fence(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Fence::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Fence(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Fence::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Fence(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Fence)) {
      *ppvObject = (ID3D11Fence*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Fence::AddRef() {
  out() << "ID3D11Fence(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Fence::Release() {
  out() << "ID3D11Fence(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::Signal(
  ID3D11Fence * pFence, 
  UINT64 Value
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext4 << ")::Signal\n";
  auto unwrapped_pFence = unwrap(pFence);
  auto ret = m_pID3D11DeviceContext4->Signal(
    unwrapped_pFence, 
    Value
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::Wait(
  ID3D11Fence * pFence, 
  UINT64 Value
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext4 << ")::Wait\n";
  auto unwrapped_pFence = unwrap(pFence);
  auto ret = m_pID3D11DeviceContext4->Wait(
    unwrapped_pFence, 
    Value
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::Flush1(
  D3D11_CONTEXT_TYPE ContextType, 
  HANDLE hEvent
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext3 << ")::Flush1\n";
  m_pID3D11DeviceContext3->Flush1(
    ContextType, 
    hEvent
  );
}
void __stdcall WrappedID3D11DeviceContext4::SetHardwareProtectionState(
  BOOL HwProtectionEnable
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext3 << ")::SetHardwareProtectionState\n";
  m_pID3D11DeviceContext3->SetHardwareProtectionState(
    HwProtectionEnable
  );
}
void __stdcall WrappedID3D11DeviceContext4::GetHardwareProtectionState(
  BOOL * pHwProtectionEnable
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext3 << ")::GetHardwareProtectionState\n";
  m_pID3D11DeviceContext3->GetHardwareProtectionState(
    pHwProtectionEnable
  );
}
HRESULT __stdcall WrappedID3D11DeviceContext4::UpdateTileMappings(
  ID3D11Resource * pTiledResource, 
  UINT NumTiledResourceRegions, 
  const D3D11_TILED_RESOURCE_COORDINATE * pTiledResourceRegionStartCoordinates, 
  const D3D11_TILE_REGION_SIZE * pTiledResourceRegionSizes, 
  ID3D11Buffer * pTilePool, 
  UINT NumRanges, 
  const UINT * pRangeFlags, 
  const UINT * pTilePoolStartOffsets, 
  const UINT * pRangeTileCounts, 
  UINT Flags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::UpdateTileMappings\n";
  auto unwrapped_pTiledResource = unwrap(pTiledResource);
  auto unwrapped_pTilePool = unwrap(pTilePool);
  auto ret = m_pID3D11DeviceContext2->UpdateTileMappings(
    unwrapped_pTiledResource, 
    NumTiledResourceRegions, 
    pTiledResourceRegionStartCoordinates, 
    pTiledResourceRegionSizes, 
    unwrapped_pTilePool, 
    NumRanges, 
    pRangeFlags, 
    pTilePoolStartOffsets, 
    pRangeTileCounts, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::CopyTileMappings(
  ID3D11Resource * pDestTiledResource, 
  const D3D11_TILED_RESOURCE_COORDINATE * pDestRegionStartCoordinate, 
  ID3D11Resource * pSourceTiledResource, 
  const D3D11_TILED_RESOURCE_COORDINATE * pSourceRegionStartCoordinate, 
  const D3D11_TILE_REGION_SIZE * pTileRegionSize, 
  UINT Flags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::CopyTileMappings\n";
  auto unwrapped_pDestTiledResource = unwrap(pDestTiledResource);
  auto unwrapped_pDestRegionStartCoordinate = unwrap(pDestRegionStartCoordinate);
  auto unwrapped_pSourceTiledResource = unwrap(pSourceTiledResource);
  auto unwrapped_pSourceRegionStartCoordinate = unwrap(pSourceRegionStartCoordinate);
  auto unwrapped_pTileRegionSize = unwrap(pTileRegionSize);
  auto ret = m_pID3D11DeviceContext2->CopyTileMappings(
    unwrapped_pDestTiledResource, 
    unwrapped_pDestRegionStartCoordinate, 
    unwrapped_pSourceTiledResource, 
    unwrapped_pSourceRegionStartCoordinate, 
    unwrapped_pTileRegionSize, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::CopyTiles(
  ID3D11Resource * pTiledResource, 
  const D3D11_TILED_RESOURCE_COORDINATE * pTileRegionStartCoordinate, 
  const D3D11_TILE_REGION_SIZE * pTileRegionSize, 
  ID3D11Buffer * pBuffer, 
  UINT64 BufferStartOffsetInBytes, 
  UINT Flags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::CopyTiles\n";
  auto unwrapped_pTiledResource = unwrap(pTiledResource);
  auto unwrapped_pTileRegionStartCoordinate = unwrap(pTileRegionStartCoordinate);
  auto unwrapped_pTileRegionSize = unwrap(pTileRegionSize);
  auto unwrapped_pBuffer = unwrap(pBuffer);
  m_pID3D11DeviceContext2->CopyTiles(
    unwrapped_pTiledResource, 
    unwrapped_pTileRegionStartCoordinate, 
    unwrapped_pTileRegionSize, 
    unwrapped_pBuffer, 
    BufferStartOffsetInBytes, 
    Flags
  );
}
void __stdcall WrappedID3D11DeviceContext4::UpdateTiles(
  ID3D11Resource * pDestTiledResource, 
  const D3D11_TILED_RESOURCE_COORDINATE * pDestTileRegionStartCoordinate, 
  const D3D11_TILE_REGION_SIZE * pDestTileRegionSize, 
  const void * pSourceTileData, 
  UINT Flags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::UpdateTiles\n";
  auto unwrapped_pDestTiledResource = unwrap(pDestTiledResource);
  auto unwrapped_pDestTileRegionStartCoordinate = unwrap(pDestTileRegionStartCoordinate);
  auto unwrapped_pDestTileRegionSize = unwrap(pDestTileRegionSize);
  auto unwrapped_pSourceTileData = unwrap(pSourceTileData);
  m_pID3D11DeviceContext2->UpdateTiles(
    unwrapped_pDestTiledResource, 
    unwrapped_pDestTileRegionStartCoordinate, 
    unwrapped_pDestTileRegionSize, 
    unwrapped_pSourceTileData, 
    Flags
  );
}
HRESULT __stdcall WrappedID3D11DeviceContext4::ResizeTilePool(
  ID3D11Buffer * pTilePool, 
  UINT64 NewSizeInBytes
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::ResizeTilePool\n";
  auto unwrapped_pTilePool = unwrap(pTilePool);
  auto ret = m_pID3D11DeviceContext2->ResizeTilePool(
    unwrapped_pTilePool, 
    NewSizeInBytes
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::TiledResourceBarrier(
  struct ID3D11DeviceChild * pTiledResourceOrViewAccessBeforeBarrier, 
  struct ID3D11DeviceChild * pTiledResourceOrViewAccessAfterBarrier
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::TiledResourceBarrier\n";
  auto unwrapped_pTiledResourceOrViewAccessBeforeBarrier = unwrap(pTiledResourceOrViewAccessBeforeBarrier);
  auto unwrapped_pTiledResourceOrViewAccessAfterBarrier = unwrap(pTiledResourceOrViewAccessAfterBarrier);
  m_pID3D11DeviceContext2->TiledResourceBarrier(
    unwrapped_pTiledResourceOrViewAccessBeforeBarrier, 
    unwrapped_pTiledResourceOrViewAccessAfterBarrier
  );
}
BOOL __stdcall WrappedID3D11DeviceContext4::IsAnnotationEnabled() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::IsAnnotationEnabled\n";
  auto ret = m_pID3D11DeviceContext2->IsAnnotationEnabled();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::SetMarkerInt(
  LPCWSTR pLabel, 
  INT Data
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::SetMarkerInt\n";
  m_pID3D11DeviceContext2->SetMarkerInt(
    pLabel, 
    Data
  );
}
void __stdcall WrappedID3D11DeviceContext4::BeginEventInt(
  LPCWSTR pLabel, 
  INT Data
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::BeginEventInt\n";
  m_pID3D11DeviceContext2->BeginEventInt(
    pLabel, 
    Data
  );
}
void __stdcall WrappedID3D11DeviceContext4::EndEvent() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext2 << ")::EndEvent\n";
  return m_pID3D11DeviceContext2->EndEvent();
}
void __stdcall WrappedID3D11DeviceContext4::CopySubresourceRegion1(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  UINT DstX, 
  UINT DstY, 
  UINT DstZ, 
  ID3D11Resource * pSrcResource, 
  UINT SrcSubresource, 
  const D3D11_BOX * pSrcBox, 
  UINT CopyFlags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::CopySubresourceRegion1\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pSrcResource = unwrap(pSrcResource);
  auto unwrapped_pSrcBox = unwrap(pSrcBox);
  m_pID3D11DeviceContext1->CopySubresourceRegion1(
    unwrapped_pDstResource, 
    DstSubresource, 
    DstX, 
    DstY, 
    DstZ, 
    unwrapped_pSrcResource, 
    SrcSubresource, 
    unwrapped_pSrcBox, 
    CopyFlags
  );
}
void __stdcall WrappedID3D11DeviceContext4::UpdateSubresource1(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  const D3D11_BOX * pDstBox, 
  const void * pSrcData, 
  UINT SrcRowPitch, 
  UINT SrcDepthPitch, 
  UINT CopyFlags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::UpdateSubresource1\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pDstBox = unwrap(pDstBox);
  auto unwrapped_pSrcData = unwrap(pSrcData);
  m_pID3D11DeviceContext1->UpdateSubresource1(
    unwrapped_pDstResource, 
    DstSubresource, 
    unwrapped_pDstBox, 
    unwrapped_pSrcData, 
    SrcRowPitch, 
    SrcDepthPitch, 
    CopyFlags
  );
}
void __stdcall WrappedID3D11DeviceContext4::DiscardResource(
  ID3D11Resource * pResource
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::DiscardResource\n";
  auto unwrapped_pResource = unwrap(pResource);
  m_pID3D11DeviceContext1->DiscardResource(
    unwrapped_pResource
  );
}
void __stdcall WrappedID3D11DeviceContext4::DiscardView(
  ID3D11View * pResourceView
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::DiscardView\n";
  auto unwrapped_pResourceView = unwrap(pResourceView);
  m_pID3D11DeviceContext1->DiscardView(
    unwrapped_pResourceView
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::VSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->VSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::HSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->HSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::DSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::DSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->DSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::GSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::GSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->GSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::PSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::PSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->PSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers, 
  const UINT * pFirstConstant, 
  const UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::CSSetConstantBuffers1\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext1->CSSetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::VSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->VSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::HSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::HSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->HSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::DSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::DSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->DSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::GSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::GSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->GSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::PSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::PSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->PSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetConstantBuffers1(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers, 
  UINT * pFirstConstant, 
  UINT * pNumConstants
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::CSGetConstantBuffers1\n";
  m_pID3D11DeviceContext1->CSGetConstantBuffers1(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers, 
    pFirstConstant, 
    pNumConstants
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::SwapDeviceContextState(
  ID3DDeviceContextState * pState, 
  ID3DDeviceContextState ** ppPreviousState
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::SwapDeviceContextState\n";
  auto unwrapped_pState = unwrap(pState);
  m_pID3D11DeviceContext1->SwapDeviceContextState(
    unwrapped_pState, 
    ppPreviousState
  );
    if (true && ppPreviousState && *ppPreviousState ) *ppPreviousState = getWrapper<ID3DDeviceContextState, WrappedID3DDeviceContextState>(*ppPreviousState);
}
void __stdcall WrappedID3D11DeviceContext4::ClearView(
  ID3D11View * pView, 
  const FLOAT * Color, 
  const D3D11_RECT * pRect, 
  UINT NumRects
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::ClearView\n";
  auto unwrapped_pView = unwrap(pView);
  auto unwrapped_Color = unwrap(Color);
  m_pID3D11DeviceContext1->ClearView(
    unwrapped_pView, 
    unwrapped_Color, 
    pRect, 
    NumRects
  );
}
void __stdcall WrappedID3D11DeviceContext4::DiscardView1(
  ID3D11View * pResourceView, 
  const D3D11_RECT * pRects, 
  UINT NumRects
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext1 << ")::DiscardView1\n";
  auto unwrapped_pResourceView = unwrap(pResourceView);
  m_pID3D11DeviceContext1->DiscardView1(
    unwrapped_pResourceView, 
    pRects, 
    NumRects
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->VSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::PSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->PSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::PSSetShader(
  ID3D11PixelShader * pPixelShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSSetShader\n";
  auto unwrapped_pPixelShader = unwrap(pPixelShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->PSSetShader(
    unwrapped_pPixelShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::PSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->PSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSSetShader(
  ID3D11VertexShader * pVertexShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSSetShader\n";
  auto unwrapped_pVertexShader = unwrap(pVertexShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->VSSetShader(
    unwrapped_pVertexShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::DrawIndexed(
  UINT IndexCount, 
  UINT StartIndexLocation, 
  INT BaseVertexLocation
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawIndexed\n";
  m_pID3D11DeviceContext->DrawIndexed(
    IndexCount, 
    StartIndexLocation, 
    BaseVertexLocation
  );
}
void __stdcall WrappedID3D11DeviceContext4::Draw(
  UINT VertexCount, 
  UINT StartVertexLocation
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Draw\n";
  m_pID3D11DeviceContext->Draw(
    VertexCount, 
    StartVertexLocation
  );
}
HRESULT __stdcall WrappedID3D11DeviceContext4::Map(
  ID3D11Resource * pResource, 
  UINT Subresource, 
  D3D11_MAP MapType, 
  UINT MapFlags, 
  D3D11_MAPPED_SUBRESOURCE * pMappedResource
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Map\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto ret = m_pID3D11DeviceContext->Map(
    unwrapped_pResource, 
    Subresource, 
    MapType, 
    MapFlags, 
    pMappedResource
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::Unmap(
  ID3D11Resource * pResource, 
  UINT Subresource
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Unmap\n";
  auto unwrapped_pResource = unwrap(pResource);
  m_pID3D11DeviceContext->Unmap(
    unwrapped_pResource, 
    Subresource
  );
}
void __stdcall WrappedID3D11DeviceContext4::PSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->PSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::IASetInputLayout(
  ID3D11InputLayout * pInputLayout
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IASetInputLayout\n";
  auto unwrapped_pInputLayout = unwrap(pInputLayout);
  m_pID3D11DeviceContext->IASetInputLayout(
    unwrapped_pInputLayout
  );
}
void __stdcall WrappedID3D11DeviceContext4::IASetVertexBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppVertexBuffers, 
  const UINT * pStrides, 
  const UINT * pOffsets
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IASetVertexBuffers\n";
  ID3D11Buffer  *tmp_ppVertexBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppVertexBuffers[i] = unwrap(ppVertexBuffers[i]);
  m_pID3D11DeviceContext->IASetVertexBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppVertexBuffers, 
    pStrides, 
    pOffsets
  );
}
void __stdcall WrappedID3D11DeviceContext4::IASetIndexBuffer(
  ID3D11Buffer * pIndexBuffer, 
  DXGI_FORMAT Format, 
  UINT Offset
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IASetIndexBuffer\n";
  auto unwrapped_pIndexBuffer = unwrap(pIndexBuffer);
  m_pID3D11DeviceContext->IASetIndexBuffer(
    unwrapped_pIndexBuffer, 
    Format, 
    Offset
  );
}
void __stdcall WrappedID3D11DeviceContext4::DrawIndexedInstanced(
  UINT IndexCountPerInstance, 
  UINT InstanceCount, 
  UINT StartIndexLocation, 
  INT BaseVertexLocation, 
  UINT StartInstanceLocation
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawIndexedInstanced\n";
  m_pID3D11DeviceContext->DrawIndexedInstanced(
    IndexCountPerInstance, 
    InstanceCount, 
    StartIndexLocation, 
    BaseVertexLocation, 
    StartInstanceLocation
  );
}
void __stdcall WrappedID3D11DeviceContext4::DrawInstanced(
  UINT VertexCountPerInstance, 
  UINT InstanceCount, 
  UINT StartVertexLocation, 
  UINT StartInstanceLocation
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawInstanced\n";
  m_pID3D11DeviceContext->DrawInstanced(
    VertexCountPerInstance, 
    InstanceCount, 
    StartVertexLocation, 
    StartInstanceLocation
  );
}
void __stdcall WrappedID3D11DeviceContext4::GSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->GSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::GSSetShader(
  ID3D11GeometryShader * pShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSSetShader\n";
  auto unwrapped_pShader = unwrap(pShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->GSSetShader(
    unwrapped_pShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::IASetPrimitiveTopology(
  D3D11_PRIMITIVE_TOPOLOGY Topology
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IASetPrimitiveTopology\n";
  m_pID3D11DeviceContext->IASetPrimitiveTopology(
    Topology
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->VSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->VSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::Begin(
  ID3D11Asynchronous * pAsync
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Begin\n";
  auto unwrapped_pAsync = unwrap(pAsync);
  m_pID3D11DeviceContext->Begin(
    unwrapped_pAsync
  );
}
void __stdcall WrappedID3D11DeviceContext4::End(
  ID3D11Asynchronous * pAsync
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::End\n";
  auto unwrapped_pAsync = unwrap(pAsync);
  m_pID3D11DeviceContext->End(
    unwrapped_pAsync
  );
}
HRESULT __stdcall WrappedID3D11DeviceContext4::GetData(
  ID3D11Asynchronous * pAsync, 
  void * pData, 
  UINT DataSize, 
  UINT GetDataFlags
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GetData\n";
  auto unwrapped_pAsync = unwrap(pAsync);
  auto ret = m_pID3D11DeviceContext->GetData(
    unwrapped_pAsync, 
    pData, 
    DataSize, 
    GetDataFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::SetPredication(
  ID3D11Predicate * pPredicate, 
  BOOL PredicateValue
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::SetPredication\n";
  auto unwrapped_pPredicate = unwrap(pPredicate);
  m_pID3D11DeviceContext->SetPredication(
    unwrapped_pPredicate, 
    PredicateValue
  );
}
void __stdcall WrappedID3D11DeviceContext4::GSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->GSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::GSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->GSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::OMSetRenderTargets(
  UINT NumViews, 
  ID3D11RenderTargetView *const * ppRenderTargetViews, 
  ID3D11DepthStencilView * pDepthStencilView
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMSetRenderTargets\n";
  ID3D11RenderTargetView  *tmp_ppRenderTargetViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppRenderTargetViews[i] = unwrap(ppRenderTargetViews[i]);
  auto unwrapped_pDepthStencilView = unwrap(pDepthStencilView);
  m_pID3D11DeviceContext->OMSetRenderTargets(
    NumViews, 
    tmp_ppRenderTargetViews, 
    unwrapped_pDepthStencilView
  );
}
void __stdcall WrappedID3D11DeviceContext4::OMSetRenderTargetsAndUnorderedAccessViews(
  UINT NumRTVs, 
  ID3D11RenderTargetView *const * ppRenderTargetViews, 
  ID3D11DepthStencilView * pDepthStencilView, 
  UINT UAVStartSlot, 
  UINT NumUAVs, 
  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, 
  const UINT * pUAVInitialCounts
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMSetRenderTargetsAndUnorderedAccessViews\n";
  ID3D11RenderTargetView  *tmp_ppRenderTargetViews[32];
  for (uint32_t i = 0; i < NumRTVs; i++) tmp_ppRenderTargetViews[i] = unwrap(ppRenderTargetViews[i]);
  auto unwrapped_pDepthStencilView = unwrap(pDepthStencilView);
  ID3D11UnorderedAccessView  *tmp_ppUnorderedAccessViews[32];
  for (uint32_t i = 0; i < NumUAVs; i++) tmp_ppUnorderedAccessViews[i] = unwrap(ppUnorderedAccessViews[i]);
  m_pID3D11DeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
    NumRTVs, 
    tmp_ppRenderTargetViews, 
    unwrapped_pDepthStencilView, 
    UAVStartSlot, 
    NumUAVs, 
    tmp_ppUnorderedAccessViews, 
    pUAVInitialCounts
  );
}
void __stdcall WrappedID3D11DeviceContext4::OMSetBlendState(
  ID3D11BlendState * pBlendState, 
  const FLOAT * BlendFactor, 
  UINT SampleMask
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMSetBlendState\n";
  auto unwrapped_pBlendState = unwrap(pBlendState);
  auto unwrapped_BlendFactor = unwrap(BlendFactor);
  m_pID3D11DeviceContext->OMSetBlendState(
    unwrapped_pBlendState, 
    unwrapped_BlendFactor, 
    SampleMask
  );
}
void __stdcall WrappedID3D11DeviceContext4::OMSetDepthStencilState(
  ID3D11DepthStencilState * pDepthStencilState, 
  UINT StencilRef
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMSetDepthStencilState\n";
  auto unwrapped_pDepthStencilState = unwrap(pDepthStencilState);
  m_pID3D11DeviceContext->OMSetDepthStencilState(
    unwrapped_pDepthStencilState, 
    StencilRef
  );
}
void __stdcall WrappedID3D11DeviceContext4::SOSetTargets(
  UINT NumBuffers, 
  ID3D11Buffer *const * ppSOTargets, 
  const UINT * pOffsets
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::SOSetTargets\n";
  ID3D11Buffer  *tmp_ppSOTargets[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppSOTargets[i] = unwrap(ppSOTargets[i]);
  m_pID3D11DeviceContext->SOSetTargets(
    NumBuffers, 
    tmp_ppSOTargets, 
    pOffsets
  );
}
void __stdcall WrappedID3D11DeviceContext4::DrawAuto() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawAuto\n";
  return m_pID3D11DeviceContext->DrawAuto();
}
void __stdcall WrappedID3D11DeviceContext4::DrawIndexedInstancedIndirect(
  ID3D11Buffer * pBufferForArgs, 
  UINT AlignedByteOffsetForArgs
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawIndexedInstancedIndirect\n";
  auto unwrapped_pBufferForArgs = unwrap(pBufferForArgs);
  m_pID3D11DeviceContext->DrawIndexedInstancedIndirect(
    unwrapped_pBufferForArgs, 
    AlignedByteOffsetForArgs
  );
}
void __stdcall WrappedID3D11DeviceContext4::DrawInstancedIndirect(
  ID3D11Buffer * pBufferForArgs, 
  UINT AlignedByteOffsetForArgs
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DrawInstancedIndirect\n";
  auto unwrapped_pBufferForArgs = unwrap(pBufferForArgs);
  m_pID3D11DeviceContext->DrawInstancedIndirect(
    unwrapped_pBufferForArgs, 
    AlignedByteOffsetForArgs
  );
}
void __stdcall WrappedID3D11DeviceContext4::Dispatch(
  UINT ThreadGroupCountX, 
  UINT ThreadGroupCountY, 
  UINT ThreadGroupCountZ
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Dispatch\n";
  m_pID3D11DeviceContext->Dispatch(
    ThreadGroupCountX, 
    ThreadGroupCountY, 
    ThreadGroupCountZ
  );
}
void __stdcall WrappedID3D11DeviceContext4::DispatchIndirect(
  ID3D11Buffer * pBufferForArgs, 
  UINT AlignedByteOffsetForArgs
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DispatchIndirect\n";
  auto unwrapped_pBufferForArgs = unwrap(pBufferForArgs);
  m_pID3D11DeviceContext->DispatchIndirect(
    unwrapped_pBufferForArgs, 
    AlignedByteOffsetForArgs
  );
}
void __stdcall WrappedID3D11DeviceContext4::RSSetState(
  ID3D11RasterizerState * pRasterizerState
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSSetState\n";
  auto unwrapped_pRasterizerState = unwrap(pRasterizerState);
  m_pID3D11DeviceContext->RSSetState(
    unwrapped_pRasterizerState
  );
}
void __stdcall WrappedID3D11DeviceContext4::RSSetViewports(
  UINT NumViewports, 
  const D3D11_VIEWPORT * pViewports
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSSetViewports\n";
  m_pID3D11DeviceContext->RSSetViewports(
    NumViewports, 
    pViewports
  );
}
void __stdcall WrappedID3D11DeviceContext4::RSSetScissorRects(
  UINT NumRects, 
  const D3D11_RECT * pRects
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSSetScissorRects\n";
  m_pID3D11DeviceContext->RSSetScissorRects(
    NumRects, 
    pRects
  );
}
void __stdcall WrappedID3D11DeviceContext4::CopySubresourceRegion(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  UINT DstX, 
  UINT DstY, 
  UINT DstZ, 
  ID3D11Resource * pSrcResource, 
  UINT SrcSubresource, 
  const D3D11_BOX * pSrcBox
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CopySubresourceRegion\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pSrcResource = unwrap(pSrcResource);
  auto unwrapped_pSrcBox = unwrap(pSrcBox);
  m_pID3D11DeviceContext->CopySubresourceRegion(
    unwrapped_pDstResource, 
    DstSubresource, 
    DstX, 
    DstY, 
    DstZ, 
    unwrapped_pSrcResource, 
    SrcSubresource, 
    unwrapped_pSrcBox
  );
}
void __stdcall WrappedID3D11DeviceContext4::CopyResource(
  ID3D11Resource * pDstResource, 
  ID3D11Resource * pSrcResource
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CopyResource\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pSrcResource = unwrap(pSrcResource);
  m_pID3D11DeviceContext->CopyResource(
    unwrapped_pDstResource, 
    unwrapped_pSrcResource
  );
}
void __stdcall WrappedID3D11DeviceContext4::UpdateSubresource(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  const D3D11_BOX * pDstBox, 
  const void * pSrcData, 
  UINT SrcRowPitch, 
  UINT SrcDepthPitch
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::UpdateSubresource\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pDstBox = unwrap(pDstBox);
  auto unwrapped_pSrcData = unwrap(pSrcData);
  m_pID3D11DeviceContext->UpdateSubresource(
    unwrapped_pDstResource, 
    DstSubresource, 
    unwrapped_pDstBox, 
    unwrapped_pSrcData, 
    SrcRowPitch, 
    SrcDepthPitch
  );
}
void __stdcall WrappedID3D11DeviceContext4::CopyStructureCount(
  ID3D11Buffer * pDstBuffer, 
  UINT DstAlignedByteOffset, 
  ID3D11UnorderedAccessView * pSrcView
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CopyStructureCount\n";
  auto unwrapped_pDstBuffer = unwrap(pDstBuffer);
  auto unwrapped_pSrcView = unwrap(pSrcView);
  m_pID3D11DeviceContext->CopyStructureCount(
    unwrapped_pDstBuffer, 
    DstAlignedByteOffset, 
    unwrapped_pSrcView
  );
}
void __stdcall WrappedID3D11DeviceContext4::ClearRenderTargetView(
  ID3D11RenderTargetView * pRenderTargetView, 
  const FLOAT * ColorRGBA
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ClearRenderTargetView\n";
  auto unwrapped_pRenderTargetView = unwrap(pRenderTargetView);
  auto unwrapped_ColorRGBA = unwrap(ColorRGBA);
  m_pID3D11DeviceContext->ClearRenderTargetView(
    unwrapped_pRenderTargetView, 
    unwrapped_ColorRGBA
  );
}
void __stdcall WrappedID3D11DeviceContext4::ClearUnorderedAccessViewUint(
  ID3D11UnorderedAccessView * pUnorderedAccessView, 
  const UINT * Values
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ClearUnorderedAccessViewUint\n";
  auto unwrapped_pUnorderedAccessView = unwrap(pUnorderedAccessView);
  auto unwrapped_Values = unwrap(Values);
  m_pID3D11DeviceContext->ClearUnorderedAccessViewUint(
    unwrapped_pUnorderedAccessView, 
    unwrapped_Values
  );
}
void __stdcall WrappedID3D11DeviceContext4::ClearUnorderedAccessViewFloat(
  ID3D11UnorderedAccessView * pUnorderedAccessView, 
  const FLOAT * Values
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ClearUnorderedAccessViewFloat\n";
  auto unwrapped_pUnorderedAccessView = unwrap(pUnorderedAccessView);
  auto unwrapped_Values = unwrap(Values);
  m_pID3D11DeviceContext->ClearUnorderedAccessViewFloat(
    unwrapped_pUnorderedAccessView, 
    unwrapped_Values
  );
}
void __stdcall WrappedID3D11DeviceContext4::ClearDepthStencilView(
  ID3D11DepthStencilView * pDepthStencilView, 
  UINT ClearFlags, 
  FLOAT Depth, 
  UINT8 Stencil
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ClearDepthStencilView\n";
  auto unwrapped_pDepthStencilView = unwrap(pDepthStencilView);
  m_pID3D11DeviceContext->ClearDepthStencilView(
    unwrapped_pDepthStencilView, 
    ClearFlags, 
    Depth, 
    Stencil
  );
}
void __stdcall WrappedID3D11DeviceContext4::GenerateMips(
  ID3D11ShaderResourceView * pShaderResourceView
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GenerateMips\n";
  auto unwrapped_pShaderResourceView = unwrap(pShaderResourceView);
  m_pID3D11DeviceContext->GenerateMips(
    unwrapped_pShaderResourceView
  );
}
void __stdcall WrappedID3D11DeviceContext4::SetResourceMinLOD(
  ID3D11Resource * pResource, 
  FLOAT MinLOD
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::SetResourceMinLOD\n";
  auto unwrapped_pResource = unwrap(pResource);
  m_pID3D11DeviceContext->SetResourceMinLOD(
    unwrapped_pResource, 
    MinLOD
  );
}
FLOAT __stdcall WrappedID3D11DeviceContext4::GetResourceMinLOD(
  ID3D11Resource * pResource
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GetResourceMinLOD\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto ret = m_pID3D11DeviceContext->GetResourceMinLOD(
    unwrapped_pResource
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::ResolveSubresource(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  ID3D11Resource * pSrcResource, 
  UINT SrcSubresource, 
  DXGI_FORMAT Format
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ResolveSubresource\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pSrcResource = unwrap(pSrcResource);
  m_pID3D11DeviceContext->ResolveSubresource(
    unwrapped_pDstResource, 
    DstSubresource, 
    unwrapped_pSrcResource, 
    SrcSubresource, 
    Format
  );
}
void __stdcall WrappedID3D11DeviceContext4::ExecuteCommandList(
  ID3D11CommandList * pCommandList, 
  BOOL RestoreContextState
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ExecuteCommandList\n";
  auto unwrapped_pCommandList = unwrap(pCommandList);
  m_pID3D11DeviceContext->ExecuteCommandList(
    unwrapped_pCommandList, 
    RestoreContextState
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->HSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSSetShader(
  ID3D11HullShader * pHullShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSSetShader\n";
  auto unwrapped_pHullShader = unwrap(pHullShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->HSSetShader(
    unwrapped_pHullShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->HSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->HSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::DSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->DSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::DSSetShader(
  ID3D11DomainShader * pDomainShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSSetShader\n";
  auto unwrapped_pDomainShader = unwrap(pDomainShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->DSSetShader(
    unwrapped_pDomainShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::DSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->DSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::DSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->DSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView *const * ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSSetShaderResources\n";
  ID3D11ShaderResourceView  *tmp_ppShaderResourceViews[32];
  for (uint32_t i = 0; i < NumViews; i++) tmp_ppShaderResourceViews[i] = unwrap(ppShaderResourceViews[i]);
  m_pID3D11DeviceContext->CSSetShaderResources(
    StartSlot, 
    NumViews, 
    tmp_ppShaderResourceViews
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetUnorderedAccessViews(
  UINT StartSlot, 
  UINT NumUAVs, 
  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews, 
  const UINT * pUAVInitialCounts
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSSetUnorderedAccessViews\n";
  ID3D11UnorderedAccessView  *tmp_ppUnorderedAccessViews[32];
  for (uint32_t i = 0; i < NumUAVs; i++) tmp_ppUnorderedAccessViews[i] = unwrap(ppUnorderedAccessViews[i]);
  m_pID3D11DeviceContext->CSSetUnorderedAccessViews(
    StartSlot, 
    NumUAVs, 
    tmp_ppUnorderedAccessViews, 
    pUAVInitialCounts
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetShader(
  ID3D11ComputeShader * pComputeShader, 
  ID3D11ClassInstance *const * ppClassInstances, 
  UINT NumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSSetShader\n";
  auto unwrapped_pComputeShader = unwrap(pComputeShader);
  ID3D11ClassInstance  *tmp_ppClassInstances[32];
  for (uint32_t i = 0; i < NumClassInstances; i++) tmp_ppClassInstances[i] = unwrap(ppClassInstances[i]);
  m_pID3D11DeviceContext->CSSetShader(
    unwrapped_pComputeShader, 
    tmp_ppClassInstances, 
    NumClassInstances
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState *const * ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSSetSamplers\n";
  ID3D11SamplerState  *tmp_ppSamplers[32];
  for (uint32_t i = 0; i < NumSamplers; i++) tmp_ppSamplers[i] = unwrap(ppSamplers[i]);
  m_pID3D11DeviceContext->CSSetSamplers(
    StartSlot, 
    NumSamplers, 
    tmp_ppSamplers
  );
}
void __stdcall WrappedID3D11DeviceContext4::CSSetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer *const * ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSSetConstantBuffers\n";
  ID3D11Buffer  *tmp_ppConstantBuffers[32];
  for (uint32_t i = 0; i < NumBuffers; i++) tmp_ppConstantBuffers[i] = unwrap(ppConstantBuffers[i]);
  m_pID3D11DeviceContext->CSSetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    tmp_ppConstantBuffers
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSGetConstantBuffers\n";
  m_pID3D11DeviceContext->VSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::PSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSGetShaderResources\n";
  m_pID3D11DeviceContext->PSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::PSGetShader(
  ID3D11PixelShader ** ppPixelShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->PSGetShader(
    ppPixelShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppPixelShader && *ppPixelShader ) *ppPixelShader = getWrapper<ID3D11PixelShader, WrappedID3D11PixelShader>(*ppPixelShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::PSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSGetSamplers\n";
  m_pID3D11DeviceContext->PSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::VSGetShader(
  ID3D11VertexShader ** ppVertexShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->VSGetShader(
    ppVertexShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppVertexShader && *ppVertexShader ) *ppVertexShader = getWrapper<ID3D11VertexShader, WrappedID3D11VertexShader>(*ppVertexShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::PSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::PSGetConstantBuffers\n";
  m_pID3D11DeviceContext->PSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::IAGetInputLayout(
  ID3D11InputLayout ** ppInputLayout
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IAGetInputLayout\n";
  m_pID3D11DeviceContext->IAGetInputLayout(
    ppInputLayout
  );
    if (true && ppInputLayout && *ppInputLayout ) *ppInputLayout = getWrapper<ID3D11InputLayout, WrappedID3D11InputLayout>(*ppInputLayout);
}
void __stdcall WrappedID3D11DeviceContext4::IAGetVertexBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppVertexBuffers, 
  UINT * pStrides, 
  UINT * pOffsets
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IAGetVertexBuffers\n";
  m_pID3D11DeviceContext->IAGetVertexBuffers(
    StartSlot, 
    NumBuffers, 
    ppVertexBuffers, 
    pStrides, 
    pOffsets
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppVertexBuffers && ppVertexBuffers[i]) ppVertexBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppVertexBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::IAGetIndexBuffer(
  ID3D11Buffer ** pIndexBuffer, 
  DXGI_FORMAT * Format, 
  UINT * Offset
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IAGetIndexBuffer\n";
  m_pID3D11DeviceContext->IAGetIndexBuffer(
    pIndexBuffer, 
    Format, 
    Offset
  );
    if (true && pIndexBuffer && *pIndexBuffer ) *pIndexBuffer = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(*pIndexBuffer);
}
void __stdcall WrappedID3D11DeviceContext4::GSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSGetConstantBuffers\n";
  m_pID3D11DeviceContext->GSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::GSGetShader(
  ID3D11GeometryShader ** ppGeometryShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->GSGetShader(
    ppGeometryShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppGeometryShader && *ppGeometryShader ) *ppGeometryShader = getWrapper<ID3D11GeometryShader, WrappedID3D11GeometryShader>(*ppGeometryShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::IAGetPrimitiveTopology(
  D3D11_PRIMITIVE_TOPOLOGY * pTopology
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::IAGetPrimitiveTopology\n";
  m_pID3D11DeviceContext->IAGetPrimitiveTopology(
    pTopology
  );
}
void __stdcall WrappedID3D11DeviceContext4::VSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSGetShaderResources\n";
  m_pID3D11DeviceContext->VSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::VSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::VSGetSamplers\n";
  m_pID3D11DeviceContext->VSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::GetPredication(
  ID3D11Predicate ** ppPredicate, 
  BOOL * pPredicateValue
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GetPredication\n";
  m_pID3D11DeviceContext->GetPredication(
    ppPredicate, 
    pPredicateValue
  );
    if (true && ppPredicate && *ppPredicate ) *ppPredicate = getWrapper<ID3D11Predicate, WrappedID3D11Predicate>(*ppPredicate);
}
void __stdcall WrappedID3D11DeviceContext4::GSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSGetShaderResources\n";
  m_pID3D11DeviceContext->GSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::GSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GSGetSamplers\n";
  m_pID3D11DeviceContext->GSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::OMGetRenderTargets(
  UINT NumViews, 
  ID3D11RenderTargetView ** ppRenderTargetViews, 
  ID3D11DepthStencilView ** ppDepthStencilView
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMGetRenderTargets\n";
  m_pID3D11DeviceContext->OMGetRenderTargets(
    NumViews, 
    ppRenderTargetViews, 
    ppDepthStencilView
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppRenderTargetViews && ppRenderTargetViews[i]) ppRenderTargetViews[i] = getWrapper<ID3D11RenderTargetView, WrappedID3D11RenderTargetView1>(ppRenderTargetViews[i]);
    if (true && ppDepthStencilView && *ppDepthStencilView ) *ppDepthStencilView = getWrapper<ID3D11DepthStencilView, WrappedID3D11DepthStencilView>(*ppDepthStencilView);
}
void __stdcall WrappedID3D11DeviceContext4::OMGetRenderTargetsAndUnorderedAccessViews(
  UINT NumRTVs, 
  ID3D11RenderTargetView ** ppRenderTargetViews, 
  ID3D11DepthStencilView ** ppDepthStencilView, 
  UINT UAVStartSlot, 
  UINT NumUAVs, 
  ID3D11UnorderedAccessView ** ppUnorderedAccessViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMGetRenderTargetsAndUnorderedAccessViews\n";
  m_pID3D11DeviceContext->OMGetRenderTargetsAndUnorderedAccessViews(
    NumRTVs, 
    ppRenderTargetViews, 
    ppDepthStencilView, 
    UAVStartSlot, 
    NumUAVs, 
    ppUnorderedAccessViews
  );
  for (uint32_t i = 0; i < NumRTVs; i++) if (true && ppRenderTargetViews && ppRenderTargetViews[i]) ppRenderTargetViews[i] = getWrapper<ID3D11RenderTargetView, WrappedID3D11RenderTargetView1>(ppRenderTargetViews[i]);
    if (true && ppDepthStencilView && *ppDepthStencilView ) *ppDepthStencilView = getWrapper<ID3D11DepthStencilView, WrappedID3D11DepthStencilView>(*ppDepthStencilView);
  for (uint32_t i = 0; i < NumUAVs; i++) if (true && ppUnorderedAccessViews && ppUnorderedAccessViews[i]) ppUnorderedAccessViews[i] = getWrapper<ID3D11UnorderedAccessView, WrappedID3D11UnorderedAccessView1>(ppUnorderedAccessViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::OMGetBlendState(
  ID3D11BlendState ** ppBlendState, 
  FLOAT * BlendFactor, 
  UINT * pSampleMask
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMGetBlendState\n";
  m_pID3D11DeviceContext->OMGetBlendState(
    ppBlendState, 
    BlendFactor, 
    pSampleMask
  );
    if (true && ppBlendState && *ppBlendState ) *ppBlendState = getWrapper<ID3D11BlendState, WrappedID3D11BlendState1>(*ppBlendState);
}
void __stdcall WrappedID3D11DeviceContext4::OMGetDepthStencilState(
  ID3D11DepthStencilState ** ppDepthStencilState, 
  UINT * pStencilRef
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::OMGetDepthStencilState\n";
  m_pID3D11DeviceContext->OMGetDepthStencilState(
    ppDepthStencilState, 
    pStencilRef
  );
    if (true && ppDepthStencilState && *ppDepthStencilState ) *ppDepthStencilState = getWrapper<ID3D11DepthStencilState, WrappedID3D11DepthStencilState>(*ppDepthStencilState);
}
void __stdcall WrappedID3D11DeviceContext4::SOGetTargets(
  UINT NumBuffers, 
  ID3D11Buffer ** ppSOTargets
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::SOGetTargets\n";
  m_pID3D11DeviceContext->SOGetTargets(
    NumBuffers, 
    ppSOTargets
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppSOTargets && ppSOTargets[i]) ppSOTargets[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppSOTargets[i]);
}
void __stdcall WrappedID3D11DeviceContext4::RSGetState(
  ID3D11RasterizerState ** ppRasterizerState
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSGetState\n";
  m_pID3D11DeviceContext->RSGetState(
    ppRasterizerState
  );
    if (true && ppRasterizerState && *ppRasterizerState ) *ppRasterizerState = getWrapper<ID3D11RasterizerState, WrappedID3D11RasterizerState2>(*ppRasterizerState);
}
void __stdcall WrappedID3D11DeviceContext4::RSGetViewports(
  UINT * pNumViewports, 
  D3D11_VIEWPORT * pViewports
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSGetViewports\n";
  auto unwrapped_pNumViewports = unwrap(pNumViewports);
  m_pID3D11DeviceContext->RSGetViewports(
    unwrapped_pNumViewports, 
    pViewports
  );
}
void __stdcall WrappedID3D11DeviceContext4::RSGetScissorRects(
  UINT * pNumRects, 
  D3D11_RECT * pRects
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::RSGetScissorRects\n";
  auto unwrapped_pNumRects = unwrap(pNumRects);
  m_pID3D11DeviceContext->RSGetScissorRects(
    unwrapped_pNumRects, 
    pRects
  );
}
void __stdcall WrappedID3D11DeviceContext4::HSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSGetShaderResources\n";
  m_pID3D11DeviceContext->HSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::HSGetShader(
  ID3D11HullShader ** ppHullShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->HSGetShader(
    ppHullShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppHullShader && *ppHullShader ) *ppHullShader = getWrapper<ID3D11HullShader, WrappedID3D11HullShader>(*ppHullShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::HSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSGetSamplers\n";
  m_pID3D11DeviceContext->HSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::HSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::HSGetConstantBuffers\n";
  m_pID3D11DeviceContext->HSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::DSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSGetShaderResources\n";
  m_pID3D11DeviceContext->DSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::DSGetShader(
  ID3D11DomainShader ** ppDomainShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->DSGetShader(
    ppDomainShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppDomainShader && *ppDomainShader ) *ppDomainShader = getWrapper<ID3D11DomainShader, WrappedID3D11DomainShader>(*ppDomainShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::DSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSGetSamplers\n";
  m_pID3D11DeviceContext->DSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::DSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::DSGetConstantBuffers\n";
  m_pID3D11DeviceContext->DSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetShaderResources(
  UINT StartSlot, 
  UINT NumViews, 
  ID3D11ShaderResourceView ** ppShaderResourceViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSGetShaderResources\n";
  m_pID3D11DeviceContext->CSGetShaderResources(
    StartSlot, 
    NumViews, 
    ppShaderResourceViews
  );
  for (uint32_t i = 0; i < NumViews; i++) if (true && ppShaderResourceViews && ppShaderResourceViews[i]) ppShaderResourceViews[i] = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(ppShaderResourceViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetUnorderedAccessViews(
  UINT StartSlot, 
  UINT NumUAVs, 
  ID3D11UnorderedAccessView ** ppUnorderedAccessViews
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSGetUnorderedAccessViews\n";
  m_pID3D11DeviceContext->CSGetUnorderedAccessViews(
    StartSlot, 
    NumUAVs, 
    ppUnorderedAccessViews
  );
  for (uint32_t i = 0; i < NumUAVs; i++) if (true && ppUnorderedAccessViews && ppUnorderedAccessViews[i]) ppUnorderedAccessViews[i] = getWrapper<ID3D11UnorderedAccessView, WrappedID3D11UnorderedAccessView1>(ppUnorderedAccessViews[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetShader(
  ID3D11ComputeShader ** ppComputeShader, 
  ID3D11ClassInstance ** ppClassInstances, 
  UINT * pNumClassInstances
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSGetShader\n";
  auto unwrapped_pNumClassInstances = unwrap(pNumClassInstances);
  m_pID3D11DeviceContext->CSGetShader(
    ppComputeShader, 
    ppClassInstances, 
    unwrapped_pNumClassInstances
  );
    if (true && ppComputeShader && *ppComputeShader ) *ppComputeShader = getWrapper<ID3D11ComputeShader, WrappedID3D11ComputeShader>(*ppComputeShader);
  for (uint32_t i = 0; i < *pNumClassInstances; i++) if (true && ppClassInstances && ppClassInstances[i]) ppClassInstances[i] = getWrapper<ID3D11ClassInstance, WrappedID3D11ClassInstance>(ppClassInstances[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetSamplers(
  UINT StartSlot, 
  UINT NumSamplers, 
  ID3D11SamplerState ** ppSamplers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSGetSamplers\n";
  m_pID3D11DeviceContext->CSGetSamplers(
    StartSlot, 
    NumSamplers, 
    ppSamplers
  );
  for (uint32_t i = 0; i < NumSamplers; i++) if (true && ppSamplers && ppSamplers[i]) ppSamplers[i] = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(ppSamplers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::CSGetConstantBuffers(
  UINT StartSlot, 
  UINT NumBuffers, 
  ID3D11Buffer ** ppConstantBuffers
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::CSGetConstantBuffers\n";
  m_pID3D11DeviceContext->CSGetConstantBuffers(
    StartSlot, 
    NumBuffers, 
    ppConstantBuffers
  );
  for (uint32_t i = 0; i < NumBuffers; i++) if (true && ppConstantBuffers && ppConstantBuffers[i]) ppConstantBuffers[i] = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(ppConstantBuffers[i]);
}
void __stdcall WrappedID3D11DeviceContext4::ClearState() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::ClearState\n";
  return m_pID3D11DeviceContext->ClearState();
}
void __stdcall WrappedID3D11DeviceContext4::Flush() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::Flush\n";
  return m_pID3D11DeviceContext->Flush();
}
D3D11_DEVICE_CONTEXT_TYPE __stdcall WrappedID3D11DeviceContext4::GetType() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GetType\n";
  auto ret = m_pID3D11DeviceContext->GetType();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11DeviceContext4::GetContextFlags() {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::GetContextFlags\n";
  auto ret = m_pID3D11DeviceContext->GetContextFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::FinishCommandList(
  BOOL RestoreDeferredContextState, 
  ID3D11CommandList ** ppCommandList
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceContext << ")::FinishCommandList\n";
  auto ret = m_pID3D11DeviceContext->FinishCommandList(
    RestoreDeferredContextState, 
    ppCommandList
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppCommandList && *ppCommandList ) *ppCommandList = getWrapper<ID3D11CommandList, WrappedID3D11CommandList>(*ppCommandList);
  return ret;
}
void __stdcall WrappedID3D11DeviceContext4::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11DeviceContext4::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11DeviceContext4(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11DeviceContext4::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11DeviceContext4(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11DeviceContext4)) {
      *ppvObject = (ID3D11DeviceContext4*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceContext3)) {
      *ppvObject = (ID3D11DeviceContext3*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceContext2)) {
      *ppvObject = (ID3D11DeviceContext2*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceContext1)) {
      *ppvObject = (ID3D11DeviceContext1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceContext)) {
      *ppvObject = (ID3D11DeviceContext*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11DeviceContext4::AddRef() {
  out() << "ID3D11DeviceContext4(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11DeviceContext4::Release() {
  out() << "ID3D11DeviceContext4(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::RegisterHardwareContentProtectionTeardownStatusEvent(
  HANDLE hEvent, 
  DWORD * pdwCookie
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::RegisterHardwareContentProtectionTeardownStatusEvent\n";
  auto ret = m_pIDXGIAdapter3->RegisterHardwareContentProtectionTeardownStatusEvent(
    hEvent, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIAdapter3::UnregisterHardwareContentProtectionTeardownStatus(
  DWORD dwCookie
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::UnregisterHardwareContentProtectionTeardownStatus\n";
  m_pIDXGIAdapter3->UnregisterHardwareContentProtectionTeardownStatus(
    dwCookie
  );
}
HRESULT __stdcall WrappedIDXGIAdapter3::QueryVideoMemoryInfo(
  UINT NodeIndex, 
  DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, 
  DXGI_QUERY_VIDEO_MEMORY_INFO * pVideoMemoryInfo
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::QueryVideoMemoryInfo\n";
  auto ret = m_pIDXGIAdapter3->QueryVideoMemoryInfo(
    NodeIndex, 
    MemorySegmentGroup, 
    pVideoMemoryInfo
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::SetVideoMemoryReservation(
  UINT NodeIndex, 
  DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, 
  UINT64 Reservation
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::SetVideoMemoryReservation\n";
  auto ret = m_pIDXGIAdapter3->SetVideoMemoryReservation(
    NodeIndex, 
    MemorySegmentGroup, 
    Reservation
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::RegisterVideoMemoryBudgetChangeNotificationEvent(
  HANDLE hEvent, 
  DWORD * pdwCookie
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::RegisterVideoMemoryBudgetChangeNotificationEvent\n";
  auto ret = m_pIDXGIAdapter3->RegisterVideoMemoryBudgetChangeNotificationEvent(
    hEvent, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIAdapter3::UnregisterVideoMemoryBudgetChangeNotification(
  DWORD dwCookie
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter3 << ")::UnregisterVideoMemoryBudgetChangeNotification\n";
  m_pIDXGIAdapter3->UnregisterVideoMemoryBudgetChangeNotification(
    dwCookie
  );
}
HRESULT __stdcall WrappedIDXGIAdapter3::GetDesc2(
  DXGI_ADAPTER_DESC2 * pDesc
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter2 << ")::GetDesc2\n";
  auto ret = m_pIDXGIAdapter2->GetDesc2(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::GetDesc1(
  DXGI_ADAPTER_DESC1 * pDesc
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter1 << ")::GetDesc1\n";
  auto ret = m_pIDXGIAdapter1->GetDesc1(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::EnumOutputs(
  UINT Output, 
  IDXGIOutput ** ppOutput
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter << ")::EnumOutputs\n";
  auto ret = m_pIDXGIAdapter->EnumOutputs(
    Output, 
    ppOutput
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppOutput && *ppOutput ) *ppOutput = getWrapper<IDXGIOutput, WrappedIDXGIOutput5>(*ppOutput);
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::GetDesc(
  DXGI_ADAPTER_DESC * pDesc
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter << ")::GetDesc\n";
  auto ret = m_pIDXGIAdapter->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::CheckInterfaceSupport(
  const GUID & InterfaceName, 
  LARGE_INTEGER * pUMDVersion
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIAdapter << ")::CheckInterfaceSupport\n";
  auto ret = m_pIDXGIAdapter->CheckInterfaceSupport(
    InterfaceName, 
    pUMDVersion
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIAdapter3(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIAdapter3::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIAdapter3(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIAdapter3)) {
      *ppvObject = (IDXGIAdapter3*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIAdapter2)) {
      *ppvObject = (IDXGIAdapter2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIAdapter1)) {
      *ppvObject = (IDXGIAdapter1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIAdapter)) {
      *ppvObject = (IDXGIAdapter*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIAdapter3::AddRef() {
  out() << "IDXGIAdapter3(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIAdapter3::Release() {
  out() << "IDXGIAdapter3(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::DuplicateOutput1(
  struct IUnknown * pDevice, 
  UINT Flags, 
  UINT SupportedFormatsCount, 
  const DXGI_FORMAT * pSupportedFormats, 
  IDXGIOutputDuplication ** ppOutputDuplication
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput5 << ")::DuplicateOutput1\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto ret = m_pIDXGIOutput5->DuplicateOutput1(
    unwrapped_pDevice, 
    Flags, 
    SupportedFormatsCount, 
    pSupportedFormats, 
    ppOutputDuplication
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppOutputDuplication && *ppOutputDuplication ) *ppOutputDuplication = getWrapper<IDXGIOutputDuplication, WrappedIDXGIOutputDuplication>(*ppOutputDuplication);
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::CheckOverlayColorSpaceSupport(
  DXGI_FORMAT Format, 
  DXGI_COLOR_SPACE_TYPE ColorSpace, 
  struct IUnknown * pConcernedDevice, 
  UINT * pFlags
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput4 << ")::CheckOverlayColorSpaceSupport\n";
  auto unwrapped_pConcernedDevice = unwrap(pConcernedDevice);
  auto ret = m_pIDXGIOutput4->CheckOverlayColorSpaceSupport(
    Format, 
    ColorSpace, 
    unwrapped_pConcernedDevice, 
    pFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::CheckOverlaySupport(
  DXGI_FORMAT EnumFormat, 
  struct IUnknown * pConcernedDevice, 
  UINT * pFlags
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput3 << ")::CheckOverlaySupport\n";
  auto unwrapped_pConcernedDevice = unwrap(pConcernedDevice);
  auto ret = m_pIDXGIOutput3->CheckOverlaySupport(
    EnumFormat, 
    unwrapped_pConcernedDevice, 
    pFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedIDXGIOutput5::SupportsOverlays() {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput2 << ")::SupportsOverlays\n";
  auto ret = m_pIDXGIOutput2->SupportsOverlays();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetDisplayModeList1(
  DXGI_FORMAT EnumFormat, 
  UINT Flags, 
  UINT * pNumModes, 
  DXGI_MODE_DESC1 * pDesc
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput1 << ")::GetDisplayModeList1\n";
  auto unwrapped_pNumModes = unwrap(pNumModes);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pIDXGIOutput1->GetDisplayModeList1(
    EnumFormat, 
    Flags, 
    unwrapped_pNumModes, 
    unwrapped_pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::FindClosestMatchingMode1(
  const DXGI_MODE_DESC1 * pModeToMatch, 
  DXGI_MODE_DESC1 * pClosestMatch, 
  struct IUnknown * pConcernedDevice
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput1 << ")::FindClosestMatchingMode1\n";
  auto unwrapped_pModeToMatch = unwrap(pModeToMatch);
  auto unwrapped_pConcernedDevice = unwrap(pConcernedDevice);
  auto ret = m_pIDXGIOutput1->FindClosestMatchingMode1(
    unwrapped_pModeToMatch, 
    pClosestMatch, 
    unwrapped_pConcernedDevice
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetDisplaySurfaceData1(
  IDXGIResource * pDestination
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput1 << ")::GetDisplaySurfaceData1\n";
  auto unwrapped_pDestination = unwrap(pDestination);
  auto ret = m_pIDXGIOutput1->GetDisplaySurfaceData1(
    unwrapped_pDestination
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::DuplicateOutput(
  struct IUnknown * pDevice, 
  IDXGIOutputDuplication ** ppOutputDuplication
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput1 << ")::DuplicateOutput\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto ret = m_pIDXGIOutput1->DuplicateOutput(
    unwrapped_pDevice, 
    ppOutputDuplication
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppOutputDuplication && *ppOutputDuplication ) *ppOutputDuplication = getWrapper<IDXGIOutputDuplication, WrappedIDXGIOutputDuplication>(*ppOutputDuplication);
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetDesc(
  DXGI_OUTPUT_DESC * pDesc
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetDesc\n";
  auto ret = m_pIDXGIOutput->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetDisplayModeList(
  DXGI_FORMAT EnumFormat, 
  UINT Flags, 
  UINT * pNumModes, 
  DXGI_MODE_DESC * pDesc
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetDisplayModeList\n";
  auto unwrapped_pNumModes = unwrap(pNumModes);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pIDXGIOutput->GetDisplayModeList(
    EnumFormat, 
    Flags, 
    unwrapped_pNumModes, 
    unwrapped_pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::FindClosestMatchingMode(
  const DXGI_MODE_DESC * pModeToMatch, 
  DXGI_MODE_DESC * pClosestMatch, 
  struct IUnknown * pConcernedDevice
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::FindClosestMatchingMode\n";
  auto unwrapped_pModeToMatch = unwrap(pModeToMatch);
  auto unwrapped_pConcernedDevice = unwrap(pConcernedDevice);
  auto ret = m_pIDXGIOutput->FindClosestMatchingMode(
    unwrapped_pModeToMatch, 
    pClosestMatch, 
    unwrapped_pConcernedDevice
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::WaitForVBlank() {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::WaitForVBlank\n";
  auto ret = m_pIDXGIOutput->WaitForVBlank();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::TakeOwnership(
  struct IUnknown * pDevice, 
  BOOL Exclusive
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::TakeOwnership\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto ret = m_pIDXGIOutput->TakeOwnership(
    unwrapped_pDevice, 
    Exclusive
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIOutput5::ReleaseOwnership() {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::ReleaseOwnership\n";
  return m_pIDXGIOutput->ReleaseOwnership();
}
HRESULT __stdcall WrappedIDXGIOutput5::GetGammaControlCapabilities(
  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetGammaControlCapabilities\n";
  auto ret = m_pIDXGIOutput->GetGammaControlCapabilities(
    pGammaCaps
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::SetGammaControl(
  const DXGI_GAMMA_CONTROL * pArray
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::SetGammaControl\n";
  auto unwrapped_pArray = unwrap(pArray);
  auto ret = m_pIDXGIOutput->SetGammaControl(
    unwrapped_pArray
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetGammaControl(
  DXGI_GAMMA_CONTROL * pArray
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetGammaControl\n";
  auto ret = m_pIDXGIOutput->GetGammaControl(
    pArray
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::SetDisplaySurface(
  IDXGISurface * pScanoutSurface
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::SetDisplaySurface\n";
  auto unwrapped_pScanoutSurface = unwrap(pScanoutSurface);
  auto ret = m_pIDXGIOutput->SetDisplaySurface(
    unwrapped_pScanoutSurface
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetDisplaySurfaceData(
  IDXGISurface * pDestination
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetDisplaySurfaceData\n";
  auto unwrapped_pDestination = unwrap(pDestination);
  auto ret = m_pIDXGIOutput->GetDisplaySurfaceData(
    unwrapped_pDestination
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetFrameStatistics(
  DXGI_FRAME_STATISTICS * pStats
) {
  out() << "IDXGIOutput5(" << m_pIDXGIOutput << ")::GetFrameStatistics\n";
  auto ret = m_pIDXGIOutput->GetFrameStatistics(
    pStats
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIOutput5(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIOutput5(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIOutput5(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIOutput5(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIOutput5::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIOutput5(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIOutput5)) {
      *ppvObject = (IDXGIOutput5*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIOutput4)) {
      *ppvObject = (IDXGIOutput4*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIOutput3)) {
      *ppvObject = (IDXGIOutput3*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIOutput2)) {
      *ppvObject = (IDXGIOutput2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIOutput1)) {
      *ppvObject = (IDXGIOutput1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIOutput)) {
      *ppvObject = (IDXGIOutput*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIOutput5::AddRef() {
  out() << "IDXGIOutput5(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIOutput5::Release() {
  out() << "IDXGIOutput5(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetHDRMetaData(
  DXGI_HDR_METADATA_TYPE Type, 
  UINT Size, 
  void * pMetaData
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain4 << ")::SetHDRMetaData\n";
  auto ret = m_pIDXGISwapChain4->SetHDRMetaData(
    Type, 
    Size, 
    pMetaData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedIDXGISwapChain4::GetCurrentBackBufferIndex() {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain3 << ")::GetCurrentBackBufferIndex\n";
  auto ret = m_pIDXGISwapChain3->GetCurrentBackBufferIndex();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::CheckColorSpaceSupport(
  DXGI_COLOR_SPACE_TYPE ColorSpace, 
  UINT * pColorSpaceSupport
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain3 << ")::CheckColorSpaceSupport\n";
  auto ret = m_pIDXGISwapChain3->CheckColorSpaceSupport(
    ColorSpace, 
    pColorSpaceSupport
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetColorSpace1(
  DXGI_COLOR_SPACE_TYPE ColorSpace
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain3 << ")::SetColorSpace1\n";
  auto ret = m_pIDXGISwapChain3->SetColorSpace1(
    ColorSpace
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::ResizeBuffers1(
  UINT BufferCount, 
  UINT Width, 
  UINT Height, 
  DXGI_FORMAT Format, 
  UINT SwapChainFlags, 
  const UINT * pCreationNodeMask, 
  struct IUnknown *const * ppPresentQueue
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain3 << ")::ResizeBuffers1\n";
  struct IUnknown  *tmp_ppPresentQueue[32];
  for (uint32_t i = 0; i < BufferCount; i++) tmp_ppPresentQueue[i] = unwrap(ppPresentQueue[i]);
  auto ret = m_pIDXGISwapChain3->ResizeBuffers1(
    BufferCount, 
    Width, 
    Height, 
    Format, 
    SwapChainFlags, 
    pCreationNodeMask, 
    tmp_ppPresentQueue
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetSourceSize(
  UINT Width, 
  UINT Height
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::SetSourceSize\n";
  auto ret = m_pIDXGISwapChain2->SetSourceSize(
    Width, 
    Height
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetSourceSize(
  UINT * pWidth, 
  UINT * pHeight
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::GetSourceSize\n";
  auto ret = m_pIDXGISwapChain2->GetSourceSize(
    pWidth, 
    pHeight
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetMaximumFrameLatency(
  UINT MaxLatency
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::SetMaximumFrameLatency\n";
  auto ret = m_pIDXGISwapChain2->SetMaximumFrameLatency(
    MaxLatency
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetMaximumFrameLatency(
  UINT * pMaxLatency
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::GetMaximumFrameLatency\n";
  auto ret = m_pIDXGISwapChain2->GetMaximumFrameLatency(
    pMaxLatency
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HANDLE __stdcall WrappedIDXGISwapChain4::GetFrameLatencyWaitableObject() {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::GetFrameLatencyWaitableObject\n";
  auto ret = m_pIDXGISwapChain2->GetFrameLatencyWaitableObject();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetMatrixTransform(
  const DXGI_MATRIX_3X2_F * pMatrix
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::SetMatrixTransform\n";
  auto unwrapped_pMatrix = unwrap(pMatrix);
  auto ret = m_pIDXGISwapChain2->SetMatrixTransform(
    unwrapped_pMatrix
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetMatrixTransform(
  DXGI_MATRIX_3X2_F * pMatrix
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain2 << ")::GetMatrixTransform\n";
  auto ret = m_pIDXGISwapChain2->GetMatrixTransform(
    pMatrix
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetDesc1(
  DXGI_SWAP_CHAIN_DESC1 * pDesc
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetDesc1\n";
  auto ret = m_pIDXGISwapChain1->GetDesc1(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetFullscreenDesc(
  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetFullscreenDesc\n";
  auto ret = m_pIDXGISwapChain1->GetFullscreenDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetHwnd(
  HWND * pHwnd
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetHwnd\n";
  auto ret = m_pIDXGISwapChain1->GetHwnd(
    pHwnd
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetCoreWindow(
  const IID & refiid, 
  void ** ppUnk
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetCoreWindow\n";
  auto ret = m_pIDXGISwapChain1->GetCoreWindow(
    refiid, 
    ppUnk
  );
  out() << "\treturned " << ret << "\n";
  if(*ppUnk)
    HandleWrap(refiid, (void**)ppUnk);
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::Present1(
  UINT SyncInterval, 
  UINT PresentFlags, 
  const DXGI_PRESENT_PARAMETERS * pPresentParameters
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::Present1\n";
  auto unwrapped_pPresentParameters = unwrap(pPresentParameters);
  auto ret = m_pIDXGISwapChain1->Present1(
    SyncInterval, 
    PresentFlags, 
    unwrapped_pPresentParameters
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedIDXGISwapChain4::IsTemporaryMonoSupported() {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::IsTemporaryMonoSupported\n";
  auto ret = m_pIDXGISwapChain1->IsTemporaryMonoSupported();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetRestrictToOutput(
  IDXGIOutput ** ppRestrictToOutput
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetRestrictToOutput\n";
  auto ret = m_pIDXGISwapChain1->GetRestrictToOutput(
    ppRestrictToOutput
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRestrictToOutput && *ppRestrictToOutput ) *ppRestrictToOutput = getWrapper<IDXGIOutput, WrappedIDXGIOutput5>(*ppRestrictToOutput);
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetBackgroundColor(
  const DXGI_RGBA * pColor
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::SetBackgroundColor\n";
  auto unwrapped_pColor = unwrap(pColor);
  auto ret = m_pIDXGISwapChain1->SetBackgroundColor(
    unwrapped_pColor
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetBackgroundColor(
  DXGI_RGBA * pColor
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetBackgroundColor\n";
  auto ret = m_pIDXGISwapChain1->GetBackgroundColor(
    pColor
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetRotation(
  DXGI_MODE_ROTATION Rotation
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::SetRotation\n";
  auto ret = m_pIDXGISwapChain1->SetRotation(
    Rotation
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetRotation(
  DXGI_MODE_ROTATION * pRotation
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain1 << ")::GetRotation\n";
  auto ret = m_pIDXGISwapChain1->GetRotation(
    pRotation
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::Present(
  UINT SyncInterval, 
  UINT Flags
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::Present\n";
  auto ret = m_pIDXGISwapChain->Present(
    SyncInterval, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetBuffer(
  UINT Buffer, 
  const IID & riid, 
  void ** ppSurface
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetBuffer\n";
  auto ret = m_pIDXGISwapChain->GetBuffer(
    Buffer, 
    riid, 
    ppSurface
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppSurface);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetFullscreenState(
  BOOL Fullscreen, 
  IDXGIOutput * pTarget
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::SetFullscreenState\n";
  auto unwrapped_pTarget = unwrap(pTarget);
  auto ret = m_pIDXGISwapChain->SetFullscreenState(
    Fullscreen, 
    unwrapped_pTarget
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetFullscreenState(
  BOOL * pFullscreen, 
  IDXGIOutput ** ppTarget
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetFullscreenState\n";
  auto ret = m_pIDXGISwapChain->GetFullscreenState(
    pFullscreen, 
    ppTarget
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTarget && *ppTarget ) *ppTarget = getWrapper<IDXGIOutput, WrappedIDXGIOutput5>(*ppTarget);
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetDesc(
  DXGI_SWAP_CHAIN_DESC * pDesc
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetDesc\n";
  auto ret = m_pIDXGISwapChain->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::ResizeBuffers(
  UINT BufferCount, 
  UINT Width, 
  UINT Height, 
  DXGI_FORMAT NewFormat, 
  UINT SwapChainFlags
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::ResizeBuffers\n";
  auto ret = m_pIDXGISwapChain->ResizeBuffers(
    BufferCount, 
    Width, 
    Height, 
    NewFormat, 
    SwapChainFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::ResizeTarget(
  const DXGI_MODE_DESC * pNewTargetParameters
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::ResizeTarget\n";
  auto unwrapped_pNewTargetParameters = unwrap(pNewTargetParameters);
  auto ret = m_pIDXGISwapChain->ResizeTarget(
    unwrapped_pNewTargetParameters
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetContainingOutput(
  IDXGIOutput ** ppOutput
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetContainingOutput\n";
  auto ret = m_pIDXGISwapChain->GetContainingOutput(
    ppOutput
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppOutput && *ppOutput ) *ppOutput = getWrapper<IDXGIOutput, WrappedIDXGIOutput5>(*ppOutput);
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetFrameStatistics(
  DXGI_FRAME_STATISTICS * pStats
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetFrameStatistics\n";
  auto ret = m_pIDXGISwapChain->GetFrameStatistics(
    pStats
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetLastPresentCount(
  UINT * pLastPresentCount
) {
  out() << "IDXGISwapChain4(" << m_pIDXGISwapChain << ")::GetLastPresentCount\n";
  auto ret = m_pIDXGISwapChain->GetLastPresentCount(
    pLastPresentCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetDevice(
  const IID & riid, 
  void ** ppDevice
) {
  out() << "IDXGISwapChain4(" << m_pIDXGIDeviceSubObject << ")::GetDevice\n";
  auto ret = m_pIDXGIDeviceSubObject->GetDevice(
    riid, 
    ppDevice
  );
  out() << "\treturned " << ret << "\n";
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGISwapChain4(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGISwapChain4(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGISwapChain4(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGISwapChain4(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGISwapChain4::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGISwapChain4(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGISwapChain4)) {
      *ppvObject = (IDXGISwapChain4*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISwapChain3)) {
      *ppvObject = (IDXGISwapChain3*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISwapChain2)) {
      *ppvObject = (IDXGISwapChain2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISwapChain1)) {
      *ppvObject = (IDXGISwapChain1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGISwapChain)) {
      *ppvObject = (IDXGISwapChain*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDeviceSubObject)) {
      *ppvObject = (IDXGIDeviceSubObject*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGISwapChain4::AddRef() {
  out() << "IDXGISwapChain4(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGISwapChain4::Release() {
  out() << "IDXGISwapChain4(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::OfferResources1(
  UINT NumResources, 
  IDXGIResource *const * ppResources, 
  DXGI_OFFER_RESOURCE_PRIORITY Priority, 
  UINT Flags
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice4 << ")::OfferResources1\n";
  IDXGIResource  *tmp_ppResources[32];
  for (uint32_t i = 0; i < NumResources; i++) tmp_ppResources[i] = unwrap(ppResources[i]);
  auto ret = m_pIDXGIDevice4->OfferResources1(
    NumResources, 
    tmp_ppResources, 
    Priority, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::ReclaimResources1(
  UINT NumResources, 
  IDXGIResource *const * ppResources, 
  DXGI_RECLAIM_RESOURCE_RESULTS * pResults
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice4 << ")::ReclaimResources1\n";
  IDXGIResource  *tmp_ppResources[32];
  for (uint32_t i = 0; i < NumResources; i++) tmp_ppResources[i] = unwrap(ppResources[i]);
  auto ret = m_pIDXGIDevice4->ReclaimResources1(
    NumResources, 
    tmp_ppResources, 
    pResults
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIDevice4::Trim() {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice3 << ")::Trim\n";
  return m_pIDXGIDevice3->Trim();
}
HRESULT __stdcall WrappedIDXGIDevice4::OfferResources(
  UINT NumResources, 
  IDXGIResource *const * ppResources, 
  DXGI_OFFER_RESOURCE_PRIORITY Priority
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice2 << ")::OfferResources\n";
  IDXGIResource  *tmp_ppResources[32];
  for (uint32_t i = 0; i < NumResources; i++) tmp_ppResources[i] = unwrap(ppResources[i]);
  auto ret = m_pIDXGIDevice2->OfferResources(
    NumResources, 
    tmp_ppResources, 
    Priority
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::ReclaimResources(
  UINT NumResources, 
  IDXGIResource *const * ppResources, 
  BOOL * pDiscarded
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice2 << ")::ReclaimResources\n";
  IDXGIResource  *tmp_ppResources[32];
  for (uint32_t i = 0; i < NumResources; i++) tmp_ppResources[i] = unwrap(ppResources[i]);
  auto ret = m_pIDXGIDevice2->ReclaimResources(
    NumResources, 
    tmp_ppResources, 
    pDiscarded
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::EnqueueSetEvent(
  HANDLE hEvent
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice2 << ")::EnqueueSetEvent\n";
  auto ret = m_pIDXGIDevice2->EnqueueSetEvent(
    hEvent
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::SetMaximumFrameLatency(
  UINT MaxLatency
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice1 << ")::SetMaximumFrameLatency\n";
  auto ret = m_pIDXGIDevice1->SetMaximumFrameLatency(
    MaxLatency
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::GetMaximumFrameLatency(
  UINT * pMaxLatency
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice1 << ")::GetMaximumFrameLatency\n";
  auto ret = m_pIDXGIDevice1->GetMaximumFrameLatency(
    pMaxLatency
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::GetAdapter(
  IDXGIAdapter ** pAdapter
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice << ")::GetAdapter\n";
  auto ret = m_pIDXGIDevice->GetAdapter(
    pAdapter
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && pAdapter && *pAdapter ) *pAdapter = getWrapper<IDXGIAdapter, WrappedIDXGIAdapter3>(*pAdapter);
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::CreateSurface(
  const DXGI_SURFACE_DESC * pDesc, 
  UINT NumSurfaces, 
  DXGI_USAGE Usage, 
  const DXGI_SHARED_RESOURCE * pSharedResource, 
  IDXGISurface ** ppSurface
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice << ")::CreateSurface\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pSharedResource = unwrap(pSharedResource);
  auto ret = m_pIDXGIDevice->CreateSurface(
    unwrapped_pDesc, 
    NumSurfaces, 
    Usage, 
    unwrapped_pSharedResource, 
    ppSurface
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSurface && *ppSurface ) *ppSurface = getWrapper<IDXGISurface, WrappedIDXGISurface2>(*ppSurface);
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::QueryResourceResidency(
  struct IUnknown *const * ppResources, 
  DXGI_RESIDENCY * pResidencyStatus, 
  UINT NumResources
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice << ")::QueryResourceResidency\n";
  struct IUnknown  *tmp_ppResources[32];
  for (uint32_t i = 0; i < NumResources; i++) tmp_ppResources[i] = unwrap(ppResources[i]);
  auto ret = m_pIDXGIDevice->QueryResourceResidency(
    tmp_ppResources, 
    pResidencyStatus, 
    NumResources
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::SetGPUThreadPriority(
  INT Priority
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice << ")::SetGPUThreadPriority\n";
  auto ret = m_pIDXGIDevice->SetGPUThreadPriority(
    Priority
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::GetGPUThreadPriority(
  INT * pPriority
) {
  out() << "IDXGIDevice4(" << m_pIDXGIDevice << ")::GetGPUThreadPriority\n";
  auto ret = m_pIDXGIDevice->GetGPUThreadPriority(
    pPriority
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIDevice4(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIDevice4(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIDevice4(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIDevice4(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIDevice4::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIDevice4(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIDevice4)) {
      *ppvObject = (IDXGIDevice4*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDevice3)) {
      *ppvObject = (IDXGIDevice3*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDevice2)) {
      *ppvObject = (IDXGIDevice2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDevice1)) {
      *ppvObject = (IDXGIDevice1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIDevice)) {
      *ppvObject = (IDXGIDevice*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIDevice4::AddRef() {
  out() << "IDXGIDevice4(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIDevice4::Release() {
  out() << "IDXGIDevice4(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::CheckFeatureSupport(
  DXGI_FEATURE Feature, 
  void * pFeatureSupportData, 
  UINT FeatureSupportDataSize
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory5 << ")::CheckFeatureSupport\n";
  auto unwrapped_pFeatureSupportData = unwrap(pFeatureSupportData);
  auto ret = m_pIDXGIFactory5->CheckFeatureSupport(
    Feature, 
    unwrapped_pFeatureSupportData, 
    FeatureSupportDataSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::EnumAdapterByLuid(
  LUID AdapterLuid, 
  const IID & riid, 
  void ** ppvAdapter
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory4 << ")::EnumAdapterByLuid\n";
  auto ret = m_pIDXGIFactory4->EnumAdapterByLuid(
    AdapterLuid, 
    riid, 
    ppvAdapter
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppvAdapter);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::EnumWarpAdapter(
  const IID & riid, 
  void ** ppvAdapter
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory4 << ")::EnumWarpAdapter\n";
  auto ret = m_pIDXGIFactory4->EnumWarpAdapter(
    riid, 
    ppvAdapter
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppvAdapter);
    }
  }
  return ret;
}
UINT __stdcall WrappedIDXGIFactory5::GetCreationFlags() {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory3 << ")::GetCreationFlags\n";
  auto ret = m_pIDXGIFactory3->GetCreationFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedIDXGIFactory5::IsWindowedStereoEnabled() {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::IsWindowedStereoEnabled\n";
  auto ret = m_pIDXGIFactory2->IsWindowedStereoEnabled();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::CreateSwapChainForHwnd(
  struct IUnknown * pDevice, 
  HWND hWnd, 
  const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc, 
  IDXGIOutput * pRestrictToOutput, 
  IDXGISwapChain1 ** ppSwapChain
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::CreateSwapChainForHwnd\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pFullscreenDesc = unwrap(pFullscreenDesc);
  auto unwrapped_pRestrictToOutput = unwrap(pRestrictToOutput);
  auto ret = m_pIDXGIFactory2->CreateSwapChainForHwnd(
    unwrapped_pDevice, 
    hWnd, 
    unwrapped_pDesc, 
    unwrapped_pFullscreenDesc, 
    unwrapped_pRestrictToOutput, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain1, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::CreateSwapChainForCoreWindow(
  struct IUnknown * pDevice, 
  struct IUnknown * pWindow, 
  const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
  IDXGIOutput * pRestrictToOutput, 
  IDXGISwapChain1 ** ppSwapChain
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::CreateSwapChainForCoreWindow\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pWindow = unwrap(pWindow);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pRestrictToOutput = unwrap(pRestrictToOutput);
  auto ret = m_pIDXGIFactory2->CreateSwapChainForCoreWindow(
    unwrapped_pDevice, 
    unwrapped_pWindow, 
    unwrapped_pDesc, 
    unwrapped_pRestrictToOutput, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain1, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::GetSharedResourceAdapterLuid(
  HANDLE hResource, 
  LUID * pLuid
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::GetSharedResourceAdapterLuid\n";
  auto ret = m_pIDXGIFactory2->GetSharedResourceAdapterLuid(
    hResource, 
    pLuid
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::RegisterStereoStatusWindow(
  HWND WindowHandle, 
  UINT wMsg, 
  DWORD * pdwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::RegisterStereoStatusWindow\n";
  auto ret = m_pIDXGIFactory2->RegisterStereoStatusWindow(
    WindowHandle, 
    wMsg, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::RegisterStereoStatusEvent(
  HANDLE hEvent, 
  DWORD * pdwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::RegisterStereoStatusEvent\n";
  auto ret = m_pIDXGIFactory2->RegisterStereoStatusEvent(
    hEvent, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIFactory5::UnregisterStereoStatus(
  DWORD dwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::UnregisterStereoStatus\n";
  m_pIDXGIFactory2->UnregisterStereoStatus(
    dwCookie
  );
}
HRESULT __stdcall WrappedIDXGIFactory5::RegisterOcclusionStatusWindow(
  HWND WindowHandle, 
  UINT wMsg, 
  DWORD * pdwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::RegisterOcclusionStatusWindow\n";
  auto ret = m_pIDXGIFactory2->RegisterOcclusionStatusWindow(
    WindowHandle, 
    wMsg, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::RegisterOcclusionStatusEvent(
  HANDLE hEvent, 
  DWORD * pdwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::RegisterOcclusionStatusEvent\n";
  auto ret = m_pIDXGIFactory2->RegisterOcclusionStatusEvent(
    hEvent, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedIDXGIFactory5::UnregisterOcclusionStatus(
  DWORD dwCookie
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::UnregisterOcclusionStatus\n";
  m_pIDXGIFactory2->UnregisterOcclusionStatus(
    dwCookie
  );
}
HRESULT __stdcall WrappedIDXGIFactory5::CreateSwapChainForComposition(
  struct IUnknown * pDevice, 
  const DXGI_SWAP_CHAIN_DESC1 * pDesc, 
  IDXGIOutput * pRestrictToOutput, 
  IDXGISwapChain1 ** ppSwapChain
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory2 << ")::CreateSwapChainForComposition\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pRestrictToOutput = unwrap(pRestrictToOutput);
  auto ret = m_pIDXGIFactory2->CreateSwapChainForComposition(
    unwrapped_pDevice, 
    unwrapped_pDesc, 
    unwrapped_pRestrictToOutput, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain1, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::EnumAdapters1(
  UINT Adapter, 
  IDXGIAdapter1 ** ppAdapter
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory1 << ")::EnumAdapters1\n";
  auto ret = m_pIDXGIFactory1->EnumAdapters1(
    Adapter, 
    ppAdapter
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppAdapter && *ppAdapter ) *ppAdapter = getWrapper<IDXGIAdapter1, WrappedIDXGIAdapter3>(*ppAdapter);
  return ret;
}
BOOL __stdcall WrappedIDXGIFactory5::IsCurrent() {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory1 << ")::IsCurrent\n";
  auto ret = m_pIDXGIFactory1->IsCurrent();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::EnumAdapters(
  UINT Adapter, 
  IDXGIAdapter ** ppAdapter
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory << ")::EnumAdapters\n";
  auto ret = m_pIDXGIFactory->EnumAdapters(
    Adapter, 
    ppAdapter
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppAdapter && *ppAdapter ) *ppAdapter = getWrapper<IDXGIAdapter, WrappedIDXGIAdapter3>(*ppAdapter);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::MakeWindowAssociation(
  HWND WindowHandle, 
  UINT Flags
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory << ")::MakeWindowAssociation\n";
  auto ret = m_pIDXGIFactory->MakeWindowAssociation(
    WindowHandle, 
    Flags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::GetWindowAssociation(
  HWND * pWindowHandle
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory << ")::GetWindowAssociation\n";
  auto ret = m_pIDXGIFactory->GetWindowAssociation(
    pWindowHandle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::CreateSwapChain(
  struct IUnknown * pDevice, 
  DXGI_SWAP_CHAIN_DESC * pDesc, 
  IDXGISwapChain ** ppSwapChain
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory << ")::CreateSwapChain\n";
  auto unwrapped_pDevice = unwrap(pDevice);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pIDXGIFactory->CreateSwapChain(
    unwrapped_pDevice, 
    unwrapped_pDesc, 
    ppSwapChain
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSwapChain && *ppSwapChain ) *ppSwapChain = getWrapper<IDXGISwapChain, WrappedIDXGISwapChain4>(*ppSwapChain);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::CreateSoftwareAdapter(
  HMODULE Module, 
  IDXGIAdapter ** ppAdapter
) {
  out() << "IDXGIFactory5(" << m_pIDXGIFactory << ")::CreateSoftwareAdapter\n";
  auto ret = m_pIDXGIFactory->CreateSoftwareAdapter(
    Module, 
    ppAdapter
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppAdapter && *ppAdapter ) *ppAdapter = getWrapper<IDXGIAdapter, WrappedIDXGIAdapter3>(*ppAdapter);
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::SetPrivateData(
  const GUID & Name, 
  UINT DataSize, 
  const void * pData
) {
  out() << "IDXGIFactory5(" << m_pIDXGIObject << ")::SetPrivateData\n";
  auto ret = m_pIDXGIObject->SetPrivateData(
    Name, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::SetPrivateDataInterface(
  const GUID & Name, 
  const struct IUnknown * pUnknown
) {
  out() << "IDXGIFactory5(" << m_pIDXGIObject << ")::SetPrivateDataInterface\n";
  auto unwrapped_pUnknown = unwrap(pUnknown);
  auto ret = m_pIDXGIObject->SetPrivateDataInterface(
    Name, 
    unwrapped_pUnknown
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::GetPrivateData(
  const GUID & Name, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "IDXGIFactory5(" << m_pIDXGIObject << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pIDXGIObject->GetPrivateData(
    Name, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::GetParent(
  const IID & riid, 
  void ** ppParent
) {
  out() << "IDXGIFactory5(" << m_pIDXGIObject << ")::GetParent\n";
  auto ret = m_pIDXGIObject->GetParent(
    riid, 
    ppParent
  );
  out() << "\treturned " << ret << "\n";
  {
    if(!ret) {
      HandleWrap(riid, ppParent);
    }
  }
  return ret;
}
HRESULT __stdcall WrappedIDXGIFactory5::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "IDXGIFactory5(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(IDXGIFactory5)) {
      *ppvObject = (IDXGIFactory5*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIFactory4)) {
      *ppvObject = (IDXGIFactory4*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIFactory3)) {
      *ppvObject = (IDXGIFactory3*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIFactory2)) {
      *ppvObject = (IDXGIFactory2*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIFactory1)) {
      *ppvObject = (IDXGIFactory1*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIFactory)) {
      *ppvObject = (IDXGIFactory*)this;
      return ret;
    }
    if(riid == __uuidof(IDXGIObject)) {
      *ppvObject = (IDXGIObject*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedIDXGIFactory5::AddRef() {
  out() << "IDXGIFactory5(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedIDXGIFactory5::Release() {
  out() << "IDXGIFactory5(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::OpenSharedFence(
  HANDLE hFence, 
  const IID & ReturnedInterface, 
  void ** ppFence
) {
  out() << "ID3D11Device5(" << m_pID3D11Device5 << ")::OpenSharedFence\n";
  auto ret = m_pID3D11Device5->OpenSharedFence(
    hFence, 
    ReturnedInterface, 
    ppFence
  );
  out() << "\treturned " << ret << "\n";
  if(*ppFence)
    HandleWrap(ReturnedInterface, (void**)ppFence);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateFence(
  UINT64 InitialValue, 
  D3D11_FENCE_FLAG Flags, 
  const IID & ReturnedInterface, 
  void ** ppFence
) {
  out() << "ID3D11Device5(" << m_pID3D11Device5 << ")::CreateFence\n";
  auto ret = m_pID3D11Device5->CreateFence(
    InitialValue, 
    Flags, 
    ReturnedInterface, 
    ppFence
  );
  out() << "\treturned " << ret << "\n";
  if(*ppFence)
    HandleWrap(ReturnedInterface, (void**)ppFence);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::RegisterDeviceRemovedEvent(
  HANDLE hEvent, 
  DWORD * pdwCookie
) {
  out() << "ID3D11Device5(" << m_pID3D11Device4 << ")::RegisterDeviceRemovedEvent\n";
  auto ret = m_pID3D11Device4->RegisterDeviceRemovedEvent(
    hEvent, 
    pdwCookie
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Device5::UnregisterDeviceRemoved(
  DWORD dwCookie
) {
  out() << "ID3D11Device5(" << m_pID3D11Device4 << ")::UnregisterDeviceRemoved\n";
  m_pID3D11Device4->UnregisterDeviceRemoved(
    dwCookie
  );
}
HRESULT __stdcall WrappedID3D11Device5::CreateTexture2D1(
  const D3D11_TEXTURE2D_DESC1 * pDesc1, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Texture2D1 ** ppTexture2D
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateTexture2D1\n";
  auto unwrapped_pDesc1 = unwrap(pDesc1);
  auto ret = m_pID3D11Device3->CreateTexture2D1(
    unwrapped_pDesc1, 
    pInitialData, 
    ppTexture2D
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTexture2D && *ppTexture2D ) *ppTexture2D = getWrapper<ID3D11Texture2D1, WrappedID3D11Texture2D1>(*ppTexture2D);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateTexture3D1(
  const D3D11_TEXTURE3D_DESC1 * pDesc1, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Texture3D1 ** ppTexture3D
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateTexture3D1\n";
  auto unwrapped_pDesc1 = unwrap(pDesc1);
  auto ret = m_pID3D11Device3->CreateTexture3D1(
    unwrapped_pDesc1, 
    pInitialData, 
    ppTexture3D
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTexture3D && *ppTexture3D ) *ppTexture3D = getWrapper<ID3D11Texture3D1, WrappedID3D11Texture3D1>(*ppTexture3D);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateRasterizerState2(
  const D3D11_RASTERIZER_DESC2 * pRasterizerDesc, 
  ID3D11RasterizerState2 ** ppRasterizerState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateRasterizerState2\n";
  auto unwrapped_pRasterizerDesc = unwrap(pRasterizerDesc);
  auto ret = m_pID3D11Device3->CreateRasterizerState2(
    unwrapped_pRasterizerDesc, 
    ppRasterizerState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRasterizerState && *ppRasterizerState ) *ppRasterizerState = getWrapper<ID3D11RasterizerState2, WrappedID3D11RasterizerState2>(*ppRasterizerState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateShaderResourceView1(
  ID3D11Resource * pResource, 
  const D3D11_SHADER_RESOURCE_VIEW_DESC1 * pDesc1, 
  ID3D11ShaderResourceView1 ** ppSRView1
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateShaderResourceView1\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc1 = unwrap(pDesc1);
  auto ret = m_pID3D11Device3->CreateShaderResourceView1(
    unwrapped_pResource, 
    unwrapped_pDesc1, 
    ppSRView1
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSRView1 && *ppSRView1 ) *ppSRView1 = getWrapper<ID3D11ShaderResourceView1, WrappedID3D11ShaderResourceView1>(*ppSRView1);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateUnorderedAccessView1(
  ID3D11Resource * pResource, 
  const D3D11_UNORDERED_ACCESS_VIEW_DESC1 * pDesc1, 
  ID3D11UnorderedAccessView1 ** ppUAView1
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateUnorderedAccessView1\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc1 = unwrap(pDesc1);
  auto ret = m_pID3D11Device3->CreateUnorderedAccessView1(
    unwrapped_pResource, 
    unwrapped_pDesc1, 
    ppUAView1
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppUAView1 && *ppUAView1 ) *ppUAView1 = getWrapper<ID3D11UnorderedAccessView1, WrappedID3D11UnorderedAccessView1>(*ppUAView1);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateRenderTargetView1(
  ID3D11Resource * pResource, 
  const D3D11_RENDER_TARGET_VIEW_DESC1 * pDesc1, 
  ID3D11RenderTargetView1 ** ppRTView1
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateRenderTargetView1\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc1 = unwrap(pDesc1);
  auto ret = m_pID3D11Device3->CreateRenderTargetView1(
    unwrapped_pResource, 
    unwrapped_pDesc1, 
    ppRTView1
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRTView1 && *ppRTView1 ) *ppRTView1 = getWrapper<ID3D11RenderTargetView1, WrappedID3D11RenderTargetView1>(*ppRTView1);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateQuery1(
  const D3D11_QUERY_DESC1 * pQueryDesc1, 
  ID3D11Query1 ** ppQuery1
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateQuery1\n";
  auto unwrapped_pQueryDesc1 = unwrap(pQueryDesc1);
  auto ret = m_pID3D11Device3->CreateQuery1(
    unwrapped_pQueryDesc1, 
    ppQuery1
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppQuery1 && *ppQuery1 ) *ppQuery1 = getWrapper<ID3D11Query1, WrappedID3D11Query1>(*ppQuery1);
  return ret;
}
void __stdcall WrappedID3D11Device5::GetImmediateContext3(
  ID3D11DeviceContext3 ** ppImmediateContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::GetImmediateContext3\n";
  m_pID3D11Device3->GetImmediateContext3(
    ppImmediateContext
  );
    if (true && ppImmediateContext && *ppImmediateContext ) *ppImmediateContext = getWrapper<ID3D11DeviceContext3, WrappedID3D11DeviceContext4>(*ppImmediateContext);
}
HRESULT __stdcall WrappedID3D11Device5::CreateDeferredContext3(
  UINT ContextFlags, 
  ID3D11DeviceContext3 ** ppDeferredContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::CreateDeferredContext3\n";
  auto ret = m_pID3D11Device3->CreateDeferredContext3(
    ContextFlags, 
    ppDeferredContext
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDeferredContext && *ppDeferredContext ) *ppDeferredContext = getWrapper<ID3D11DeviceContext3, WrappedID3D11DeviceContext4>(*ppDeferredContext);
  return ret;
}
void __stdcall WrappedID3D11Device5::WriteToSubresource(
  ID3D11Resource * pDstResource, 
  UINT DstSubresource, 
  const D3D11_BOX * pDstBox, 
  const void * pSrcData, 
  UINT SrcRowPitch, 
  UINT SrcDepthPitch
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::WriteToSubresource\n";
  auto unwrapped_pDstResource = unwrap(pDstResource);
  auto unwrapped_pDstBox = unwrap(pDstBox);
  auto unwrapped_pSrcData = unwrap(pSrcData);
  m_pID3D11Device3->WriteToSubresource(
    unwrapped_pDstResource, 
    DstSubresource, 
    unwrapped_pDstBox, 
    unwrapped_pSrcData, 
    SrcRowPitch, 
    SrcDepthPitch
  );
}
void __stdcall WrappedID3D11Device5::ReadFromSubresource(
  void * pDstData, 
  UINT DstRowPitch, 
  UINT DstDepthPitch, 
  ID3D11Resource * pSrcResource, 
  UINT SrcSubresource, 
  const D3D11_BOX * pSrcBox
) {
  out() << "ID3D11Device5(" << m_pID3D11Device3 << ")::ReadFromSubresource\n";
  auto unwrapped_pSrcResource = unwrap(pSrcResource);
  auto unwrapped_pSrcBox = unwrap(pSrcBox);
  m_pID3D11Device3->ReadFromSubresource(
    pDstData, 
    DstRowPitch, 
    DstDepthPitch, 
    unwrapped_pSrcResource, 
    SrcSubresource, 
    unwrapped_pSrcBox
  );
}
void __stdcall WrappedID3D11Device5::GetImmediateContext2(
  ID3D11DeviceContext2 ** ppImmediateContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device2 << ")::GetImmediateContext2\n";
  m_pID3D11Device2->GetImmediateContext2(
    ppImmediateContext
  );
    if (true && ppImmediateContext && *ppImmediateContext ) *ppImmediateContext = getWrapper<ID3D11DeviceContext2, WrappedID3D11DeviceContext4>(*ppImmediateContext);
}
HRESULT __stdcall WrappedID3D11Device5::CreateDeferredContext2(
  UINT ContextFlags, 
  ID3D11DeviceContext2 ** ppDeferredContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device2 << ")::CreateDeferredContext2\n";
  auto ret = m_pID3D11Device2->CreateDeferredContext2(
    ContextFlags, 
    ppDeferredContext
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDeferredContext && *ppDeferredContext ) *ppDeferredContext = getWrapper<ID3D11DeviceContext2, WrappedID3D11DeviceContext4>(*ppDeferredContext);
  return ret;
}
void __stdcall WrappedID3D11Device5::GetResourceTiling(
  ID3D11Resource * pTiledResource, 
  UINT * pNumTilesForEntireResource, 
  D3D11_PACKED_MIP_DESC * pPackedMipDesc, 
  D3D11_TILE_SHAPE * pStandardTileShapeForNonPackedMips, 
  UINT * pNumSubresourceTilings, 
  UINT FirstSubresourceTilingToGet, 
  D3D11_SUBRESOURCE_TILING * pSubresourceTilingsForNonPackedMips
) {
  out() << "ID3D11Device5(" << m_pID3D11Device2 << ")::GetResourceTiling\n";
  auto unwrapped_pTiledResource = unwrap(pTiledResource);
  auto unwrapped_pNumSubresourceTilings = unwrap(pNumSubresourceTilings);
  m_pID3D11Device2->GetResourceTiling(
    unwrapped_pTiledResource, 
    pNumTilesForEntireResource, 
    pPackedMipDesc, 
    pStandardTileShapeForNonPackedMips, 
    unwrapped_pNumSubresourceTilings, 
    FirstSubresourceTilingToGet, 
    pSubresourceTilingsForNonPackedMips
  );
}
HRESULT __stdcall WrappedID3D11Device5::CheckMultisampleQualityLevels1(
  DXGI_FORMAT Format, 
  UINT SampleCount, 
  UINT Flags, 
  UINT * pNumQualityLevels
) {
  out() << "ID3D11Device5(" << m_pID3D11Device2 << ")::CheckMultisampleQualityLevels1\n";
  auto ret = m_pID3D11Device2->CheckMultisampleQualityLevels1(
    Format, 
    SampleCount, 
    Flags, 
    pNumQualityLevels
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Device5::GetImmediateContext1(
  ID3D11DeviceContext1 ** ppImmediateContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::GetImmediateContext1\n";
  m_pID3D11Device1->GetImmediateContext1(
    ppImmediateContext
  );
    if (true && ppImmediateContext && *ppImmediateContext ) *ppImmediateContext = getWrapper<ID3D11DeviceContext1, WrappedID3D11DeviceContext4>(*ppImmediateContext);
}
HRESULT __stdcall WrappedID3D11Device5::CreateDeferredContext1(
  UINT ContextFlags, 
  ID3D11DeviceContext1 ** ppDeferredContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::CreateDeferredContext1\n";
  auto ret = m_pID3D11Device1->CreateDeferredContext1(
    ContextFlags, 
    ppDeferredContext
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDeferredContext && *ppDeferredContext ) *ppDeferredContext = getWrapper<ID3D11DeviceContext1, WrappedID3D11DeviceContext4>(*ppDeferredContext);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateBlendState1(
  const D3D11_BLEND_DESC1 * pBlendStateDesc, 
  ID3D11BlendState1 ** ppBlendState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::CreateBlendState1\n";
  auto unwrapped_pBlendStateDesc = unwrap(pBlendStateDesc);
  auto ret = m_pID3D11Device1->CreateBlendState1(
    unwrapped_pBlendStateDesc, 
    ppBlendState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppBlendState && *ppBlendState ) *ppBlendState = getWrapper<ID3D11BlendState1, WrappedID3D11BlendState1>(*ppBlendState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateRasterizerState1(
  const D3D11_RASTERIZER_DESC1 * pRasterizerDesc, 
  ID3D11RasterizerState1 ** ppRasterizerState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::CreateRasterizerState1\n";
  auto unwrapped_pRasterizerDesc = unwrap(pRasterizerDesc);
  auto ret = m_pID3D11Device1->CreateRasterizerState1(
    unwrapped_pRasterizerDesc, 
    ppRasterizerState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRasterizerState && *ppRasterizerState ) *ppRasterizerState = getWrapper<ID3D11RasterizerState1, WrappedID3D11RasterizerState2>(*ppRasterizerState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateDeviceContextState(
  UINT Flags, 
  const D3D_FEATURE_LEVEL * pFeatureLevels, 
  UINT FeatureLevels, 
  UINT SDKVersion, 
  const IID & EmulatedInterface, 
  D3D_FEATURE_LEVEL * pChosenFeatureLevel, 
  ID3DDeviceContextState ** ppContextState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::CreateDeviceContextState\n";
  auto ret = m_pID3D11Device1->CreateDeviceContextState(
    Flags, 
    pFeatureLevels, 
    FeatureLevels, 
    SDKVersion, 
    EmulatedInterface, 
    pChosenFeatureLevel, 
    ppContextState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppContextState && *ppContextState ) *ppContextState = getWrapper<ID3DDeviceContextState, WrappedID3DDeviceContextState>(*ppContextState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::OpenSharedResource1(
  HANDLE hResource, 
  const IID & returnedInterface, 
  void ** ppResource
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::OpenSharedResource1\n";
  auto ret = m_pID3D11Device1->OpenSharedResource1(
    hResource, 
    returnedInterface, 
    ppResource
  );
  out() << "\treturned " << ret << "\n";
  if(*ppResource)
    HandleWrap(returnedInterface, (void**)ppResource);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::OpenSharedResourceByName(
  LPCWSTR lpName, 
  DWORD dwDesiredAccess, 
  const IID & returnedInterface, 
  void ** ppResource
) {
  out() << "ID3D11Device5(" << m_pID3D11Device1 << ")::OpenSharedResourceByName\n";
  auto ret = m_pID3D11Device1->OpenSharedResourceByName(
    lpName, 
    dwDesiredAccess, 
    returnedInterface, 
    ppResource
  );
  out() << "\treturned " << ret << "\n";
  if(*ppResource)
    HandleWrap(returnedInterface, (void**)ppResource);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateBuffer(
  const D3D11_BUFFER_DESC * pDesc, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Buffer ** ppBuffer
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateBuffer\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pInitialData = unwrap(pInitialData);
  auto ret = m_pID3D11Device->CreateBuffer(
    unwrapped_pDesc, 
    unwrapped_pInitialData, 
    ppBuffer
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppBuffer && *ppBuffer ) *ppBuffer = getWrapper<ID3D11Buffer, WrappedID3D11Buffer>(*ppBuffer);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateTexture1D(
  const D3D11_TEXTURE1D_DESC * pDesc, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Texture1D ** ppTexture1D
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateTexture1D\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateTexture1D(
    unwrapped_pDesc, 
    pInitialData, 
    ppTexture1D
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTexture1D && *ppTexture1D ) *ppTexture1D = getWrapper<ID3D11Texture1D, WrappedID3D11Texture1D>(*ppTexture1D);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateTexture2D(
  const D3D11_TEXTURE2D_DESC * pDesc, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Texture2D ** ppTexture2D
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateTexture2D\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateTexture2D(
    unwrapped_pDesc, 
    pInitialData, 
    ppTexture2D
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTexture2D && *ppTexture2D ) *ppTexture2D = getWrapper<ID3D11Texture2D, WrappedID3D11Texture2D1>(*ppTexture2D);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateTexture3D(
  const D3D11_TEXTURE3D_DESC * pDesc, 
  const D3D11_SUBRESOURCE_DATA * pInitialData, 
  ID3D11Texture3D ** ppTexture3D
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateTexture3D\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateTexture3D(
    unwrapped_pDesc, 
    pInitialData, 
    ppTexture3D
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppTexture3D && *ppTexture3D ) *ppTexture3D = getWrapper<ID3D11Texture3D, WrappedID3D11Texture3D1>(*ppTexture3D);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateShaderResourceView(
  ID3D11Resource * pResource, 
  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc, 
  ID3D11ShaderResourceView ** ppSRView
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateShaderResourceView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateShaderResourceView(
    unwrapped_pResource, 
    unwrapped_pDesc, 
    ppSRView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSRView && *ppSRView ) *ppSRView = getWrapper<ID3D11ShaderResourceView, WrappedID3D11ShaderResourceView1>(*ppSRView);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateUnorderedAccessView(
  ID3D11Resource * pResource, 
  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc, 
  ID3D11UnorderedAccessView ** ppUAView
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateUnorderedAccessView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateUnorderedAccessView(
    unwrapped_pResource, 
    unwrapped_pDesc, 
    ppUAView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppUAView && *ppUAView ) *ppUAView = getWrapper<ID3D11UnorderedAccessView, WrappedID3D11UnorderedAccessView1>(*ppUAView);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateRenderTargetView(
  ID3D11Resource * pResource, 
  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc, 
  ID3D11RenderTargetView ** ppRTView
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateRenderTargetView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateRenderTargetView(
    unwrapped_pResource, 
    unwrapped_pDesc, 
    ppRTView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRTView && *ppRTView ) *ppRTView = getWrapper<ID3D11RenderTargetView, WrappedID3D11RenderTargetView1>(*ppRTView);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateDepthStencilView(
  ID3D11Resource * pResource, 
  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc, 
  ID3D11DepthStencilView ** ppDepthStencilView
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateDepthStencilView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11Device->CreateDepthStencilView(
    unwrapped_pResource, 
    unwrapped_pDesc, 
    ppDepthStencilView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDepthStencilView && *ppDepthStencilView ) *ppDepthStencilView = getWrapper<ID3D11DepthStencilView, WrappedID3D11DepthStencilView>(*ppDepthStencilView);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateInputLayout(
  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs, 
  UINT NumElements, 
  const void * pShaderBytecodeWithInputSignature, 
  SIZE_T BytecodeLength, 
  ID3D11InputLayout ** ppInputLayout
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateInputLayout\n";
  auto ret = m_pID3D11Device->CreateInputLayout(
    pInputElementDescs, 
    NumElements, 
    pShaderBytecodeWithInputSignature, 
    BytecodeLength, 
    ppInputLayout
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppInputLayout && *ppInputLayout ) *ppInputLayout = getWrapper<ID3D11InputLayout, WrappedID3D11InputLayout>(*ppInputLayout);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateVertexShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11VertexShader ** ppVertexShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateVertexShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateVertexShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppVertexShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppVertexShader && *ppVertexShader ) *ppVertexShader = getWrapper<ID3D11VertexShader, WrappedID3D11VertexShader>(*ppVertexShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateGeometryShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11GeometryShader ** ppGeometryShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateGeometryShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateGeometryShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppGeometryShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppGeometryShader && *ppGeometryShader ) *ppGeometryShader = getWrapper<ID3D11GeometryShader, WrappedID3D11GeometryShader>(*ppGeometryShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateGeometryShaderWithStreamOutput(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  const D3D11_SO_DECLARATION_ENTRY * pSODeclaration, 
  UINT NumEntries, 
  const UINT * pBufferStrides, 
  UINT NumStrides, 
  UINT RasterizedStream, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11GeometryShader ** ppGeometryShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateGeometryShaderWithStreamOutput\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateGeometryShaderWithStreamOutput(
    pShaderBytecode, 
    BytecodeLength, 
    pSODeclaration, 
    NumEntries, 
    pBufferStrides, 
    NumStrides, 
    RasterizedStream, 
    unwrapped_pClassLinkage, 
    ppGeometryShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppGeometryShader && *ppGeometryShader ) *ppGeometryShader = getWrapper<ID3D11GeometryShader, WrappedID3D11GeometryShader>(*ppGeometryShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreatePixelShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11PixelShader ** ppPixelShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreatePixelShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreatePixelShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppPixelShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppPixelShader && *ppPixelShader ) *ppPixelShader = getWrapper<ID3D11PixelShader, WrappedID3D11PixelShader>(*ppPixelShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateHullShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11HullShader ** ppHullShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateHullShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateHullShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppHullShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppHullShader && *ppHullShader ) *ppHullShader = getWrapper<ID3D11HullShader, WrappedID3D11HullShader>(*ppHullShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateDomainShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11DomainShader ** ppDomainShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateDomainShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateDomainShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppDomainShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDomainShader && *ppDomainShader ) *ppDomainShader = getWrapper<ID3D11DomainShader, WrappedID3D11DomainShader>(*ppDomainShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateComputeShader(
  const void * pShaderBytecode, 
  SIZE_T BytecodeLength, 
  ID3D11ClassLinkage * pClassLinkage, 
  ID3D11ComputeShader ** ppComputeShader
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateComputeShader\n";
  auto unwrapped_pClassLinkage = unwrap(pClassLinkage);
  auto ret = m_pID3D11Device->CreateComputeShader(
    pShaderBytecode, 
    BytecodeLength, 
    unwrapped_pClassLinkage, 
    ppComputeShader
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppComputeShader && *ppComputeShader ) *ppComputeShader = getWrapper<ID3D11ComputeShader, WrappedID3D11ComputeShader>(*ppComputeShader);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateClassLinkage(
  ID3D11ClassLinkage ** ppLinkage
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateClassLinkage\n";
  auto ret = m_pID3D11Device->CreateClassLinkage(
    ppLinkage
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppLinkage && *ppLinkage ) *ppLinkage = getWrapper<ID3D11ClassLinkage, WrappedID3D11ClassLinkage>(*ppLinkage);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateBlendState(
  const D3D11_BLEND_DESC * pBlendStateDesc, 
  ID3D11BlendState ** ppBlendState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateBlendState\n";
  auto unwrapped_pBlendStateDesc = unwrap(pBlendStateDesc);
  auto ret = m_pID3D11Device->CreateBlendState(
    unwrapped_pBlendStateDesc, 
    ppBlendState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppBlendState && *ppBlendState ) *ppBlendState = getWrapper<ID3D11BlendState, WrappedID3D11BlendState1>(*ppBlendState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateDepthStencilState(
  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc, 
  ID3D11DepthStencilState ** ppDepthStencilState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateDepthStencilState\n";
  auto unwrapped_pDepthStencilDesc = unwrap(pDepthStencilDesc);
  auto ret = m_pID3D11Device->CreateDepthStencilState(
    unwrapped_pDepthStencilDesc, 
    ppDepthStencilState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDepthStencilState && *ppDepthStencilState ) *ppDepthStencilState = getWrapper<ID3D11DepthStencilState, WrappedID3D11DepthStencilState>(*ppDepthStencilState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateRasterizerState(
  const D3D11_RASTERIZER_DESC * pRasterizerDesc, 
  ID3D11RasterizerState ** ppRasterizerState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateRasterizerState\n";
  auto unwrapped_pRasterizerDesc = unwrap(pRasterizerDesc);
  auto ret = m_pID3D11Device->CreateRasterizerState(
    unwrapped_pRasterizerDesc, 
    ppRasterizerState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppRasterizerState && *ppRasterizerState ) *ppRasterizerState = getWrapper<ID3D11RasterizerState, WrappedID3D11RasterizerState2>(*ppRasterizerState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateSamplerState(
  const D3D11_SAMPLER_DESC * pSamplerDesc, 
  ID3D11SamplerState ** ppSamplerState
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateSamplerState\n";
  auto unwrapped_pSamplerDesc = unwrap(pSamplerDesc);
  auto ret = m_pID3D11Device->CreateSamplerState(
    unwrapped_pSamplerDesc, 
    ppSamplerState
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppSamplerState && *ppSamplerState ) *ppSamplerState = getWrapper<ID3D11SamplerState, WrappedID3D11SamplerState>(*ppSamplerState);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateQuery(
  const D3D11_QUERY_DESC * pQueryDesc, 
  ID3D11Query ** ppQuery
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateQuery\n";
  auto unwrapped_pQueryDesc = unwrap(pQueryDesc);
  auto ret = m_pID3D11Device->CreateQuery(
    unwrapped_pQueryDesc, 
    ppQuery
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppQuery && *ppQuery ) *ppQuery = getWrapper<ID3D11Query, WrappedID3D11Query1>(*ppQuery);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreatePredicate(
  const D3D11_QUERY_DESC * pPredicateDesc, 
  ID3D11Predicate ** ppPredicate
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreatePredicate\n";
  auto unwrapped_pPredicateDesc = unwrap(pPredicateDesc);
  auto ret = m_pID3D11Device->CreatePredicate(
    unwrapped_pPredicateDesc, 
    ppPredicate
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppPredicate && *ppPredicate ) *ppPredicate = getWrapper<ID3D11Predicate, WrappedID3D11Predicate>(*ppPredicate);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateCounter(
  const D3D11_COUNTER_DESC * pCounterDesc, 
  ID3D11Counter ** ppCounter
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateCounter\n";
  auto unwrapped_pCounterDesc = unwrap(pCounterDesc);
  auto ret = m_pID3D11Device->CreateCounter(
    unwrapped_pCounterDesc, 
    ppCounter
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppCounter && *ppCounter ) *ppCounter = getWrapper<ID3D11Counter, WrappedID3D11Counter>(*ppCounter);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CreateDeferredContext(
  UINT ContextFlags, 
  ID3D11DeviceContext ** ppDeferredContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CreateDeferredContext\n";
  auto ret = m_pID3D11Device->CreateDeferredContext(
    ContextFlags, 
    ppDeferredContext
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDeferredContext && *ppDeferredContext ) *ppDeferredContext = getWrapper<ID3D11DeviceContext, WrappedID3D11DeviceContext4>(*ppDeferredContext);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::OpenSharedResource(
  HANDLE hResource, 
  const IID & ReturnedInterface, 
  void ** ppResource
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::OpenSharedResource\n";
  auto ret = m_pID3D11Device->OpenSharedResource(
    hResource, 
    ReturnedInterface, 
    ppResource
  );
  out() << "\treturned " << ret << "\n";
  if(*ppResource)
    HandleWrap(ReturnedInterface, (void**)ppResource);
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CheckFormatSupport(
  DXGI_FORMAT Format, 
  UINT * pFormatSupport
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CheckFormatSupport\n";
  auto ret = m_pID3D11Device->CheckFormatSupport(
    Format, 
    pFormatSupport
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CheckMultisampleQualityLevels(
  DXGI_FORMAT Format, 
  UINT SampleCount, 
  UINT * pNumQualityLevels
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CheckMultisampleQualityLevels\n";
  auto ret = m_pID3D11Device->CheckMultisampleQualityLevels(
    Format, 
    SampleCount, 
    pNumQualityLevels
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Device5::CheckCounterInfo(
  D3D11_COUNTER_INFO * pCounterInfo
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CheckCounterInfo\n";
  m_pID3D11Device->CheckCounterInfo(
    pCounterInfo
  );
}
HRESULT __stdcall WrappedID3D11Device5::CheckCounter(
  const D3D11_COUNTER_DESC * pDesc, 
  D3D11_COUNTER_TYPE * pType, 
  UINT * pActiveCounters, 
  LPSTR szName, 
  UINT * pNameLength, 
  LPSTR szUnits, 
  UINT * pUnitsLength, 
  LPSTR szDescription, 
  UINT * pDescriptionLength
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CheckCounter\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto unwrapped_pNameLength = unwrap(pNameLength);
  auto unwrapped_pUnitsLength = unwrap(pUnitsLength);
  auto unwrapped_pDescriptionLength = unwrap(pDescriptionLength);
  auto ret = m_pID3D11Device->CheckCounter(
    unwrapped_pDesc, 
    pType, 
    pActiveCounters, 
    szName, 
    unwrapped_pNameLength, 
    szUnits, 
    unwrapped_pUnitsLength, 
    szDescription, 
    unwrapped_pDescriptionLength
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::CheckFeatureSupport(
  D3D11_FEATURE Feature, 
  void * pFeatureSupportData, 
  UINT FeatureSupportDataSize
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::CheckFeatureSupport\n";
  auto ret = m_pID3D11Device->CheckFeatureSupport(
    Feature, 
    pFeatureSupportData, 
    FeatureSupportDataSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11Device->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::SetPrivateData\n";
  auto ret = m_pID3D11Device->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11Device->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
D3D_FEATURE_LEVEL __stdcall WrappedID3D11Device5::GetFeatureLevel() {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetFeatureLevel\n";
  auto ret = m_pID3D11Device->GetFeatureLevel();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11Device5::GetCreationFlags() {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetCreationFlags\n";
  auto ret = m_pID3D11Device->GetCreationFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::GetDeviceRemovedReason() {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetDeviceRemovedReason\n";
  auto ret = m_pID3D11Device->GetDeviceRemovedReason();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Device5::GetImmediateContext(
  ID3D11DeviceContext ** ppImmediateContext
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetImmediateContext\n";
  m_pID3D11Device->GetImmediateContext(
    ppImmediateContext
  );
    if (true && ppImmediateContext && *ppImmediateContext ) *ppImmediateContext = getWrapper<ID3D11DeviceContext, WrappedID3D11DeviceContext4>(*ppImmediateContext);
}
HRESULT __stdcall WrappedID3D11Device5::SetExceptionMode(
  UINT RaiseFlags
) {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::SetExceptionMode\n";
  auto ret = m_pID3D11Device->SetExceptionMode(
    RaiseFlags
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11Device5::GetExceptionMode() {
  out() << "ID3D11Device5(" << m_pID3D11Device << ")::GetExceptionMode\n";
  auto ret = m_pID3D11Device->GetExceptionMode();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Device5::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Device5(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Device5)) {
      *ppvObject = (ID3D11Device5*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Device4)) {
      *ppvObject = (ID3D11Device4*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Device3)) {
      *ppvObject = (ID3D11Device3*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Device2)) {
      *ppvObject = (ID3D11Device2*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Device1)) {
      *ppvObject = (ID3D11Device1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11Device)) {
      *ppvObject = (ID3D11Device*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Device5::AddRef() {
  out() << "ID3D11Device5(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Device5::Release() {
  out() << "ID3D11Device5(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11Multithread::Enter() {
  out() << "ID3D11Multithread(" << m_pID3D11Multithread << ")::Enter\n";
  return m_pID3D11Multithread->Enter();
}
void __stdcall WrappedID3D11Multithread::Leave() {
  out() << "ID3D11Multithread(" << m_pID3D11Multithread << ")::Leave\n";
  return m_pID3D11Multithread->Leave();
}
BOOL __stdcall WrappedID3D11Multithread::SetMultithreadProtected(
  BOOL bMTProtect
) {
  out() << "ID3D11Multithread(" << m_pID3D11Multithread << ")::SetMultithreadProtected\n";
  auto ret = m_pID3D11Multithread->SetMultithreadProtected(
    bMTProtect
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11Multithread::GetMultithreadProtected() {
  out() << "ID3D11Multithread(" << m_pID3D11Multithread << ")::GetMultithreadProtected\n";
  auto ret = m_pID3D11Multithread->GetMultithreadProtected();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Multithread::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Multithread(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Multithread)) {
      *ppvObject = (ID3D11Multithread*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Multithread::AddRef() {
  out() << "ID3D11Multithread(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Multithread::Release() {
  out() << "ID3D11Multithread(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CheckFeatureSupport(
  D3D11_FEATURE_VIDEO Feature, 
  void * pFeatureSupportData, 
  UINT FeatureSupportDataSize
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice2 << ")::CheckFeatureSupport\n";
  auto ret = m_pID3D11VideoDevice2->CheckFeatureSupport(
    Feature, 
    pFeatureSupportData, 
    FeatureSupportDataSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::NegotiateCryptoSessionKeyExchangeMT(
  ID3D11CryptoSession * pCryptoSession, 
  D3D11_CRYPTO_SESSION_KEY_EXCHANGE_FLAGS flags, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice2 << ")::NegotiateCryptoSessionKeyExchangeMT\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoDevice2->NegotiateCryptoSessionKeyExchangeMT(
    unwrapped_pCryptoSession, 
    flags, 
    DataSize, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetCryptoSessionPrivateDataSize(
  const GUID * pCryptoType, 
  const GUID * pDecoderProfile, 
  const GUID * pKeyExchangeType, 
  UINT * pPrivateInputSize, 
  UINT * pPrivateOutputSize
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice1 << ")::GetCryptoSessionPrivateDataSize\n";
  auto unwrapped_pCryptoType = unwrap(pCryptoType);
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto unwrapped_pKeyExchangeType = unwrap(pKeyExchangeType);
  auto ret = m_pID3D11VideoDevice1->GetCryptoSessionPrivateDataSize(
    unwrapped_pCryptoType, 
    unwrapped_pDecoderProfile, 
    unwrapped_pKeyExchangeType, 
    pPrivateInputSize, 
    pPrivateOutputSize
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetVideoDecoderCaps(
  const GUID * pDecoderProfile, 
  UINT SampleWidth, 
  UINT SampleHeight, 
  const DXGI_RATIONAL * pFrameRate, 
  UINT BitRate, 
  const GUID * pCryptoType, 
  UINT * pDecoderCaps
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice1 << ")::GetVideoDecoderCaps\n";
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto unwrapped_pFrameRate = unwrap(pFrameRate);
  auto unwrapped_pCryptoType = unwrap(pCryptoType);
  auto ret = m_pID3D11VideoDevice1->GetVideoDecoderCaps(
    unwrapped_pDecoderProfile, 
    SampleWidth, 
    SampleHeight, 
    unwrapped_pFrameRate, 
    BitRate, 
    unwrapped_pCryptoType, 
    pDecoderCaps
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CheckVideoDecoderDownsampling(
  const D3D11_VIDEO_DECODER_DESC * pInputDesc, 
  DXGI_COLOR_SPACE_TYPE InputColorSpace, 
  const D3D11_VIDEO_DECODER_CONFIG * pInputConfig, 
  const DXGI_RATIONAL * pFrameRate, 
  const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc, 
  BOOL * pSupported, 
  BOOL * pRealTimeHint
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice1 << ")::CheckVideoDecoderDownsampling\n";
  auto unwrapped_pInputDesc = unwrap(pInputDesc);
  auto unwrapped_pInputConfig = unwrap(pInputConfig);
  auto unwrapped_pFrameRate = unwrap(pFrameRate);
  auto unwrapped_pOutputDesc = unwrap(pOutputDesc);
  auto ret = m_pID3D11VideoDevice1->CheckVideoDecoderDownsampling(
    unwrapped_pInputDesc, 
    InputColorSpace, 
    unwrapped_pInputConfig, 
    unwrapped_pFrameRate, 
    unwrapped_pOutputDesc, 
    pSupported, 
    pRealTimeHint
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::RecommendVideoDecoderDownsampleParameters(
  const D3D11_VIDEO_DECODER_DESC * pInputDesc, 
  DXGI_COLOR_SPACE_TYPE InputColorSpace, 
  const D3D11_VIDEO_DECODER_CONFIG * pInputConfig, 
  const DXGI_RATIONAL * pFrameRate, 
  D3D11_VIDEO_SAMPLE_DESC * pRecommendedOutputDesc
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice1 << ")::RecommendVideoDecoderDownsampleParameters\n";
  auto unwrapped_pInputDesc = unwrap(pInputDesc);
  auto unwrapped_pInputConfig = unwrap(pInputConfig);
  auto unwrapped_pFrameRate = unwrap(pFrameRate);
  auto ret = m_pID3D11VideoDevice1->RecommendVideoDecoderDownsampleParameters(
    unwrapped_pInputDesc, 
    InputColorSpace, 
    unwrapped_pInputConfig, 
    unwrapped_pFrameRate, 
    pRecommendedOutputDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoDecoder(
  const D3D11_VIDEO_DECODER_DESC * pVideoDesc, 
  const D3D11_VIDEO_DECODER_CONFIG * pConfig, 
  ID3D11VideoDecoder ** ppDecoder
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoDecoder\n";
  auto unwrapped_pVideoDesc = unwrap(pVideoDesc);
  auto unwrapped_pConfig = unwrap(pConfig);
  auto ret = m_pID3D11VideoDevice->CreateVideoDecoder(
    unwrapped_pVideoDesc, 
    unwrapped_pConfig, 
    ppDecoder
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppDecoder && *ppDecoder ) *ppDecoder = getWrapper<ID3D11VideoDecoder, WrappedID3D11VideoDecoder>(*ppDecoder);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoProcessor(
  ID3D11VideoProcessorEnumerator * pEnum, 
  UINT RateConversionIndex, 
  ID3D11VideoProcessor ** ppVideoProcessor
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoProcessor\n";
  auto unwrapped_pEnum = unwrap(pEnum);
  auto ret = m_pID3D11VideoDevice->CreateVideoProcessor(
    unwrapped_pEnum, 
    RateConversionIndex, 
    ppVideoProcessor
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppVideoProcessor && *ppVideoProcessor ) *ppVideoProcessor = getWrapper<ID3D11VideoProcessor, WrappedID3D11VideoProcessor>(*ppVideoProcessor);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateAuthenticatedChannel(
  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType, 
  ID3D11AuthenticatedChannel ** ppAuthenticatedChannel
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateAuthenticatedChannel\n";
  auto ret = m_pID3D11VideoDevice->CreateAuthenticatedChannel(
    ChannelType, 
    ppAuthenticatedChannel
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppAuthenticatedChannel && *ppAuthenticatedChannel ) *ppAuthenticatedChannel = getWrapper<ID3D11AuthenticatedChannel, WrappedID3D11AuthenticatedChannel>(*ppAuthenticatedChannel);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateCryptoSession(
  const GUID * pCryptoType, 
  const GUID * pDecoderProfile, 
  const GUID * pKeyExchangeType, 
  ID3D11CryptoSession ** ppCryptoSession
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateCryptoSession\n";
  auto unwrapped_pCryptoType = unwrap(pCryptoType);
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto unwrapped_pKeyExchangeType = unwrap(pKeyExchangeType);
  auto ret = m_pID3D11VideoDevice->CreateCryptoSession(
    unwrapped_pCryptoType, 
    unwrapped_pDecoderProfile, 
    unwrapped_pKeyExchangeType, 
    ppCryptoSession
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppCryptoSession && *ppCryptoSession ) *ppCryptoSession = getWrapper<ID3D11CryptoSession, WrappedID3D11CryptoSession>(*ppCryptoSession);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoDecoderOutputView(
  ID3D11Resource * pResource, 
  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc, 
  ID3D11VideoDecoderOutputView ** ppVDOVView
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoDecoderOutputView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->CreateVideoDecoderOutputView(
    unwrapped_pResource, 
    unwrapped_pDesc, 
    ppVDOVView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppVDOVView && *ppVDOVView ) *ppVDOVView = getWrapper<ID3D11VideoDecoderOutputView, WrappedID3D11VideoDecoderOutputView>(*ppVDOVView);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoProcessorInputView(
  ID3D11Resource * pResource, 
  ID3D11VideoProcessorEnumerator * pEnum, 
  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc, 
  ID3D11VideoProcessorInputView ** ppVPIView
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoProcessorInputView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pEnum = unwrap(pEnum);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->CreateVideoProcessorInputView(
    unwrapped_pResource, 
    unwrapped_pEnum, 
    unwrapped_pDesc, 
    ppVPIView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppVPIView && *ppVPIView ) *ppVPIView = getWrapper<ID3D11VideoProcessorInputView, WrappedID3D11VideoProcessorInputView>(*ppVPIView);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoProcessorOutputView(
  ID3D11Resource * pResource, 
  ID3D11VideoProcessorEnumerator * pEnum, 
  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc, 
  ID3D11VideoProcessorOutputView ** ppVPOView
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoProcessorOutputView\n";
  auto unwrapped_pResource = unwrap(pResource);
  auto unwrapped_pEnum = unwrap(pEnum);
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->CreateVideoProcessorOutputView(
    unwrapped_pResource, 
    unwrapped_pEnum, 
    unwrapped_pDesc, 
    ppVPOView
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppVPOView && *ppVPOView ) *ppVPOView = getWrapper<ID3D11VideoProcessorOutputView, WrappedID3D11VideoProcessorOutputView>(*ppVPOView);
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CreateVideoProcessorEnumerator(
  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc, 
  ID3D11VideoProcessorEnumerator ** ppEnum
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CreateVideoProcessorEnumerator\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->CreateVideoProcessorEnumerator(
    unwrapped_pDesc, 
    ppEnum
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppEnum && *ppEnum ) *ppEnum = getWrapper<ID3D11VideoProcessorEnumerator, WrappedID3D11VideoProcessorEnumerator1>(*ppEnum);
  return ret;
}
UINT __stdcall WrappedID3D11VideoDevice2::GetVideoDecoderProfileCount() {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::GetVideoDecoderProfileCount\n";
  auto ret = m_pID3D11VideoDevice->GetVideoDecoderProfileCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetVideoDecoderProfile(
  UINT Index, 
  GUID * pDecoderProfile
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::GetVideoDecoderProfile\n";
  auto ret = m_pID3D11VideoDevice->GetVideoDecoderProfile(
    Index, 
    pDecoderProfile
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CheckVideoDecoderFormat(
  const GUID * pDecoderProfile, 
  DXGI_FORMAT Format, 
  BOOL * pSupported
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CheckVideoDecoderFormat\n";
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto ret = m_pID3D11VideoDevice->CheckVideoDecoderFormat(
    unwrapped_pDecoderProfile, 
    Format, 
    pSupported
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetVideoDecoderConfigCount(
  const D3D11_VIDEO_DECODER_DESC * pDesc, 
  UINT * pCount
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::GetVideoDecoderConfigCount\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->GetVideoDecoderConfigCount(
    unwrapped_pDesc, 
    pCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetVideoDecoderConfig(
  const D3D11_VIDEO_DECODER_DESC * pDesc, 
  UINT Index, 
  D3D11_VIDEO_DECODER_CONFIG * pConfig
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::GetVideoDecoderConfig\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11VideoDevice->GetVideoDecoderConfig(
    unwrapped_pDesc, 
    Index, 
    pConfig
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::GetContentProtectionCaps(
  const GUID * pCryptoType, 
  const GUID * pDecoderProfile, 
  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::GetContentProtectionCaps\n";
  auto unwrapped_pCryptoType = unwrap(pCryptoType);
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto ret = m_pID3D11VideoDevice->GetContentProtectionCaps(
    unwrapped_pCryptoType, 
    unwrapped_pDecoderProfile, 
    pCaps
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::CheckCryptoKeyExchange(
  const GUID * pCryptoType, 
  const GUID * pDecoderProfile, 
  UINT Index, 
  GUID * pKeyExchangeType
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::CheckCryptoKeyExchange\n";
  auto unwrapped_pCryptoType = unwrap(pCryptoType);
  auto unwrapped_pDecoderProfile = unwrap(pDecoderProfile);
  auto ret = m_pID3D11VideoDevice->CheckCryptoKeyExchange(
    unwrapped_pCryptoType, 
    unwrapped_pDecoderProfile, 
    Index, 
    pKeyExchangeType
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::SetPrivateData\n";
  auto ret = m_pID3D11VideoDevice->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoDevice2(" << m_pID3D11VideoDevice << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoDevice->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoDevice2::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoDevice2(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoDevice2)) {
      *ppvObject = (ID3D11VideoDevice2*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoDevice1)) {
      *ppvObject = (ID3D11VideoDevice1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoDevice)) {
      *ppvObject = (ID3D11VideoDevice*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDevice2::AddRef() {
  out() << "ID3D11VideoDevice2(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoDevice2::Release() {
  out() << "ID3D11VideoDevice2(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::DecoderBeginFrame1(
  ID3D11VideoDecoder * pDecoder, 
  ID3D11VideoDecoderOutputView * pView, 
  UINT ContentKeySize, 
  const void * pContentKey, 
  UINT NumComponentHistograms, 
  const UINT * pHistogramOffsets, 
  ID3D11Buffer *const * ppHistogramBuffers
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext3 << ")::DecoderBeginFrame1\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto unwrapped_pView = unwrap(pView);
  ID3D11Buffer  *tmp_ppHistogramBuffers[32];
  for (uint32_t i = 0; i < NumComponentHistograms; i++) tmp_ppHistogramBuffers[i] = unwrap(ppHistogramBuffers[i]);
  auto ret = m_pID3D11VideoContext3->DecoderBeginFrame1(
    unwrapped_pDecoder, 
    unwrapped_pView, 
    ContentKeySize, 
    pContentKey, 
    NumComponentHistograms, 
    pHistogramOffsets, 
    tmp_ppHistogramBuffers
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::SubmitDecoderBuffers2(
  ID3D11VideoDecoder * pDecoder, 
  UINT NumBuffers, 
  const D3D11_VIDEO_DECODER_BUFFER_DESC2 * pBufferDesc
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext3 << ")::SubmitDecoderBuffers2\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext3->SubmitDecoderBuffers2(
    unwrapped_pDecoder, 
    NumBuffers, 
    pBufferDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputHDRMetaData(
  ID3D11VideoProcessor * pVideoProcessor, 
  DXGI_HDR_METADATA_TYPE Type, 
  UINT Size, 
  const void * pHDRMetaData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext2 << ")::VideoProcessorSetOutputHDRMetaData\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext2->VideoProcessorSetOutputHDRMetaData(
    unwrapped_pVideoProcessor, 
    Type, 
    Size, 
    pHDRMetaData
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputHDRMetaData(
  ID3D11VideoProcessor * pVideoProcessor, 
  DXGI_HDR_METADATA_TYPE * pType, 
  UINT Size, 
  void * pMetaData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext2 << ")::VideoProcessorGetOutputHDRMetaData\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext2->VideoProcessorGetOutputHDRMetaData(
    unwrapped_pVideoProcessor, 
    pType, 
    Size, 
    pMetaData
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamHDRMetaData(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  DXGI_HDR_METADATA_TYPE Type, 
  UINT Size, 
  const void * pHDRMetaData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext2 << ")::VideoProcessorSetStreamHDRMetaData\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext2->VideoProcessorSetStreamHDRMetaData(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Type, 
    Size, 
    pHDRMetaData
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamHDRMetaData(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  DXGI_HDR_METADATA_TYPE * pType, 
  UINT Size, 
  void * pMetaData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext2 << ")::VideoProcessorGetStreamHDRMetaData\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext2->VideoProcessorGetStreamHDRMetaData(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pType, 
    Size, 
    pMetaData
  );
}
HRESULT __stdcall WrappedID3D11VideoContext3::SubmitDecoderBuffers1(
  ID3D11VideoDecoder * pDecoder, 
  UINT NumBuffers, 
  const D3D11_VIDEO_DECODER_BUFFER_DESC1 * pBufferDesc
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::SubmitDecoderBuffers1\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext1->SubmitDecoderBuffers1(
    unwrapped_pDecoder, 
    NumBuffers, 
    pBufferDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::GetDataForNewHardwareKey(
  ID3D11CryptoSession * pCryptoSession, 
  UINT PrivateInputSize, 
  const void * pPrivatInputData, 
  UINT64 * pPrivateOutputData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::GetDataForNewHardwareKey\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto ret = m_pID3D11VideoContext1->GetDataForNewHardwareKey(
    unwrapped_pCryptoSession, 
    PrivateInputSize, 
    pPrivatInputData, 
    pPrivateOutputData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::CheckCryptoSessionStatus(
  ID3D11CryptoSession * pCryptoSession, 
  D3D11_CRYPTO_SESSION_STATUS * pStatus
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::CheckCryptoSessionStatus\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto ret = m_pID3D11VideoContext1->CheckCryptoSessionStatus(
    unwrapped_pCryptoSession, 
    pStatus
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::DecoderEnableDownsampling(
  ID3D11VideoDecoder * pDecoder, 
  DXGI_COLOR_SPACE_TYPE InputColorSpace, 
  const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc, 
  UINT ReferenceFrameCount
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::DecoderEnableDownsampling\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto unwrapped_pOutputDesc = unwrap(pOutputDesc);
  auto ret = m_pID3D11VideoContext1->DecoderEnableDownsampling(
    unwrapped_pDecoder, 
    InputColorSpace, 
    unwrapped_pOutputDesc, 
    ReferenceFrameCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::DecoderUpdateDownsampling(
  ID3D11VideoDecoder * pDecoder, 
  const D3D11_VIDEO_SAMPLE_DESC * pOutputDesc
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::DecoderUpdateDownsampling\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto unwrapped_pOutputDesc = unwrap(pOutputDesc);
  auto ret = m_pID3D11VideoContext1->DecoderUpdateDownsampling(
    unwrapped_pDecoder, 
    unwrapped_pOutputDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputColorSpace1(
  ID3D11VideoProcessor * pVideoProcessor, 
  DXGI_COLOR_SPACE_TYPE ColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorSetOutputColorSpace1\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorSetOutputColorSpace1(
    unwrapped_pVideoProcessor, 
    ColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputShaderUsage(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL ShaderUsage
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorSetOutputShaderUsage\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorSetOutputShaderUsage(
    unwrapped_pVideoProcessor, 
    ShaderUsage
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputColorSpace1(
  ID3D11VideoProcessor * pVideoProcessor, 
  DXGI_COLOR_SPACE_TYPE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorGetOutputColorSpace1\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorGetOutputColorSpace1(
    unwrapped_pVideoProcessor, 
    pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputShaderUsage(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL * pShaderUsage
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorGetOutputShaderUsage\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorGetOutputShaderUsage(
    unwrapped_pVideoProcessor, 
    pShaderUsage
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamColorSpace1(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  DXGI_COLOR_SPACE_TYPE ColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorSetStreamColorSpace1\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorSetStreamColorSpace1(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    ColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamMirror(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  BOOL FlipHorizontal, 
  BOOL FlipVertical
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorSetStreamMirror\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorSetStreamMirror(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    FlipHorizontal, 
    FlipVertical
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamColorSpace1(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  DXGI_COLOR_SPACE_TYPE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorGetStreamColorSpace1\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorGetStreamColorSpace1(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamMirror(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnable, 
  BOOL * pFlipHorizontal, 
  BOOL * pFlipVertical
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorGetStreamMirror\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext1->VideoProcessorGetStreamMirror(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnable, 
    pFlipHorizontal, 
    pFlipVertical
  );
}
HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorGetBehaviorHints(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT OutputWidth, 
  UINT OutputHeight, 
  DXGI_FORMAT OutputFormat, 
  UINT StreamCount, 
  const D3D11_VIDEO_PROCESSOR_STREAM_BEHAVIOR_HINT * pStreams, 
  UINT * pBehaviorHints
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext1 << ")::VideoProcessorGetBehaviorHints\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto ret = m_pID3D11VideoContext1->VideoProcessorGetBehaviorHints(
    unwrapped_pVideoProcessor, 
    OutputWidth, 
    OutputHeight, 
    OutputFormat, 
    StreamCount, 
    pStreams, 
    pBehaviorHints
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::GetDecoderBuffer(
  ID3D11VideoDecoder * pDecoder, 
  D3D11_VIDEO_DECODER_BUFFER_TYPE Type, 
  UINT * pBufferSize, 
  void ** ppBuffer
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::GetDecoderBuffer\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext->GetDecoderBuffer(
    unwrapped_pDecoder, 
    Type, 
    pBufferSize, 
    ppBuffer
  );
  out() << "\treturned " << ret << "\n";
  assert(false && "Wrap not implemented; Emit Seg Fault");
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::ReleaseDecoderBuffer(
  ID3D11VideoDecoder * pDecoder, 
  D3D11_VIDEO_DECODER_BUFFER_TYPE Type
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::ReleaseDecoderBuffer\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext->ReleaseDecoderBuffer(
    unwrapped_pDecoder, 
    Type
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::DecoderBeginFrame(
  ID3D11VideoDecoder * pDecoder, 
  ID3D11VideoDecoderOutputView * pView, 
  UINT ContentKeySize, 
  const void * pContentKey
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::DecoderBeginFrame\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto unwrapped_pView = unwrap(pView);
  auto ret = m_pID3D11VideoContext->DecoderBeginFrame(
    unwrapped_pDecoder, 
    unwrapped_pView, 
    ContentKeySize, 
    pContentKey
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::DecoderEndFrame(
  ID3D11VideoDecoder * pDecoder
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::DecoderEndFrame\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext->DecoderEndFrame(
    unwrapped_pDecoder
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::SubmitDecoderBuffers(
  ID3D11VideoDecoder * pDecoder, 
  UINT NumBuffers, 
  const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::SubmitDecoderBuffers\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto ret = m_pID3D11VideoContext->SubmitDecoderBuffers(
    unwrapped_pDecoder, 
    NumBuffers, 
    pBufferDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
APP_DEPRECATED_HRESULT __stdcall WrappedID3D11VideoContext3::DecoderExtension(
  ID3D11VideoDecoder * pDecoder, 
  const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::DecoderExtension\n";
  auto unwrapped_pDecoder = unwrap(pDecoder);
  auto unwrapped_pExtensionData = unwrap(pExtensionData);
  auto ret = m_pID3D11VideoContext->DecoderExtension(
    unwrapped_pDecoder, 
    unwrapped_pExtensionData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputTargetRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL Enable, 
  const RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputTargetRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pRect = unwrap(pRect);
  m_pID3D11VideoContext->VideoProcessorSetOutputTargetRect(
    unwrapped_pVideoProcessor, 
    Enable, 
    unwrapped_pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputBackgroundColor(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL YCbCr, 
  const D3D11_VIDEO_COLOR * pColor
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputBackgroundColor\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pColor = unwrap(pColor);
  m_pID3D11VideoContext->VideoProcessorSetOutputBackgroundColor(
    unwrapped_pVideoProcessor, 
    YCbCr, 
    unwrapped_pColor
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputColorSpace(
  ID3D11VideoProcessor * pVideoProcessor, 
  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputColorSpace\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pColorSpace = unwrap(pColorSpace);
  m_pID3D11VideoContext->VideoProcessorSetOutputColorSpace(
    unwrapped_pVideoProcessor, 
    unwrapped_pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputAlphaFillMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode, 
  UINT StreamIndex
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputAlphaFillMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetOutputAlphaFillMode(
    unwrapped_pVideoProcessor, 
    AlphaFillMode, 
    StreamIndex
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputConstriction(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL Enable, 
  SIZE Size
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputConstriction\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetOutputConstriction(
    unwrapped_pVideoProcessor, 
    Enable, 
    Size
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputStereoMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL Enable
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputStereoMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetOutputStereoMode(
    unwrapped_pVideoProcessor, 
    Enable
  );
}
APP_DEPRECATED_HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorSetOutputExtension(
  ID3D11VideoProcessor * pVideoProcessor, 
  const GUID * pExtensionGuid, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetOutputExtension\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pExtensionGuid = unwrap(pExtensionGuid);
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoContext->VideoProcessorSetOutputExtension(
    unwrapped_pVideoProcessor, 
    unwrapped_pExtensionGuid, 
    DataSize, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputTargetRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL * Enabled, 
  RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputTargetRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputTargetRect(
    unwrapped_pVideoProcessor, 
    Enabled, 
    pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputBackgroundColor(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL * pYCbCr, 
  D3D11_VIDEO_COLOR * pColor
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputBackgroundColor\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputBackgroundColor(
    unwrapped_pVideoProcessor, 
    pYCbCr, 
    pColor
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputColorSpace(
  ID3D11VideoProcessor * pVideoProcessor, 
  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputColorSpace\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputColorSpace(
    unwrapped_pVideoProcessor, 
    pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputAlphaFillMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode, 
  UINT * pStreamIndex
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputAlphaFillMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputAlphaFillMode(
    unwrapped_pVideoProcessor, 
    pAlphaFillMode, 
    pStreamIndex
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputConstriction(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL * pEnabled, 
  SIZE * pSize
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputConstriction\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputConstriction(
    unwrapped_pVideoProcessor, 
    pEnabled, 
    pSize
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputStereoMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  BOOL * pEnabled
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputStereoMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetOutputStereoMode(
    unwrapped_pVideoProcessor, 
    pEnabled
  );
}
APP_DEPRECATED_HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorGetOutputExtension(
  ID3D11VideoProcessor * pVideoProcessor, 
  const GUID * pExtensionGuid, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetOutputExtension\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pExtensionGuid = unwrap(pExtensionGuid);
  auto ret = m_pID3D11VideoContext->VideoProcessorGetOutputExtension(
    unwrapped_pVideoProcessor, 
    unwrapped_pExtensionGuid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamFrameFormat(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_FRAME_FORMAT FrameFormat
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamFrameFormat\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamFrameFormat(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    FrameFormat
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamColorSpace(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamColorSpace\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pColorSpace = unwrap(pColorSpace);
  m_pID3D11VideoContext->VideoProcessorSetStreamColorSpace(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    unwrapped_pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamOutputRate(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate, 
  BOOL RepeatFrame, 
  const DXGI_RATIONAL * pCustomRate
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamOutputRate\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pCustomRate = unwrap(pCustomRate);
  m_pID3D11VideoContext->VideoProcessorSetStreamOutputRate(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    OutputRate, 
    RepeatFrame, 
    unwrapped_pCustomRate
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamSourceRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  const RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamSourceRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pRect = unwrap(pRect);
  m_pID3D11VideoContext->VideoProcessorSetStreamSourceRect(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    unwrapped_pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamDestRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  const RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamDestRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pRect = unwrap(pRect);
  m_pID3D11VideoContext->VideoProcessorSetStreamDestRect(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    unwrapped_pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamAlpha(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  FLOAT Alpha
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamAlpha\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamAlpha(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    Alpha
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamPalette(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  UINT Count, 
  const UINT * pEntries
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamPalette\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamPalette(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Count, 
    pEntries
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamPixelAspectRatio(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  const DXGI_RATIONAL * pSourceAspectRatio, 
  const DXGI_RATIONAL * pDestinationAspectRatio
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamPixelAspectRatio\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pSourceAspectRatio = unwrap(pSourceAspectRatio);
  auto unwrapped_pDestinationAspectRatio = unwrap(pDestinationAspectRatio);
  m_pID3D11VideoContext->VideoProcessorSetStreamPixelAspectRatio(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    unwrapped_pSourceAspectRatio, 
    unwrapped_pDestinationAspectRatio
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamLumaKey(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  FLOAT Lower, 
  FLOAT Upper
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamLumaKey\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamLumaKey(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    Lower, 
    Upper
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamStereoFormat(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format, 
  BOOL LeftViewFrame0, 
  BOOL BaseViewFrame0, 
  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode, 
  int MonoOffset
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamStereoFormat\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamStereoFormat(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    Format, 
    LeftViewFrame0, 
    BaseViewFrame0, 
    FlipMode, 
    MonoOffset
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamAutoProcessingMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamAutoProcessingMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamAutoProcessingMode(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamFilter(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_PROCESSOR_FILTER Filter, 
  BOOL Enable, 
  int Level
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamFilter\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamFilter(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Filter, 
    Enable, 
    Level
  );
}
APP_DEPRECATED_HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamExtension(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  const GUID * pExtensionGuid, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamExtension\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pExtensionGuid = unwrap(pExtensionGuid);
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoContext->VideoProcessorSetStreamExtension(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    unwrapped_pExtensionGuid, 
    DataSize, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamFrameFormat(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_FRAME_FORMAT * pFrameFormat
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamFrameFormat\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamFrameFormat(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pFrameFormat
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamColorSpace(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamColorSpace\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamColorSpace(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pColorSpace
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamOutputRate(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate, 
  BOOL * pRepeatFrame, 
  DXGI_RATIONAL * pCustomRate
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamOutputRate\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamOutputRate(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pOutputRate, 
    pRepeatFrame, 
    pCustomRate
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamSourceRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled, 
  RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamSourceRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamSourceRect(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled, 
    pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamDestRect(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled, 
  RECT * pRect
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamDestRect\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamDestRect(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled, 
    pRect
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamAlpha(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled, 
  FLOAT * pAlpha
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamAlpha\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamAlpha(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled, 
    pAlpha
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamPalette(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  UINT Count, 
  UINT * pEntries
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamPalette\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamPalette(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Count, 
    pEntries
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamPixelAspectRatio(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled, 
  DXGI_RATIONAL * pSourceAspectRatio, 
  DXGI_RATIONAL * pDestinationAspectRatio
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamPixelAspectRatio\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamPixelAspectRatio(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled, 
    pSourceAspectRatio, 
    pDestinationAspectRatio
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamLumaKey(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled, 
  FLOAT * pLower, 
  FLOAT * pUpper
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamLumaKey\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamLumaKey(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled, 
    pLower, 
    pUpper
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamStereoFormat(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnable, 
  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat, 
  BOOL * pLeftViewFrame0, 
  BOOL * pBaseViewFrame0, 
  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode, 
  int * MonoOffset
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamStereoFormat\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamStereoFormat(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnable, 
    pFormat, 
    pLeftViewFrame0, 
    pBaseViewFrame0, 
    pFlipMode, 
    MonoOffset
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamAutoProcessingMode(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnabled
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamAutoProcessingMode\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamAutoProcessingMode(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnabled
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamFilter(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  D3D11_VIDEO_PROCESSOR_FILTER Filter, 
  BOOL * pEnabled, 
  int * pLevel
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamFilter\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamFilter(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Filter, 
    pEnabled, 
    pLevel
  );
}
APP_DEPRECATED_HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamExtension(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  const GUID * pExtensionGuid, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamExtension\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pExtensionGuid = unwrap(pExtensionGuid);
  auto ret = m_pID3D11VideoContext->VideoProcessorGetStreamExtension(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    unwrapped_pExtensionGuid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::VideoProcessorBlt(
  ID3D11VideoProcessor * pVideoProcessor, 
  ID3D11VideoProcessorOutputView * pView, 
  UINT OutputFrame, 
  UINT StreamCount, 
  const D3D11_VIDEO_PROCESSOR_STREAM * pStreams
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorBlt\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  auto unwrapped_pView = unwrap(pView);
  auto ret = m_pID3D11VideoContext->VideoProcessorBlt(
    unwrapped_pVideoProcessor, 
    unwrapped_pView, 
    OutputFrame, 
    StreamCount, 
    pStreams
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::NegotiateCryptoSessionKeyExchange(
  ID3D11CryptoSession * pCryptoSession, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::NegotiateCryptoSessionKeyExchange\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoContext->NegotiateCryptoSessionKeyExchange(
    unwrapped_pCryptoSession, 
    DataSize, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::EncryptionBlt(
  ID3D11CryptoSession * pCryptoSession, 
  ID3D11Texture2D * pSrcSurface, 
  ID3D11Texture2D * pDstSurface, 
  UINT IVSize, 
  void * pIV
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::EncryptionBlt\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto unwrapped_pSrcSurface = unwrap(pSrcSurface);
  auto unwrapped_pDstSurface = unwrap(pDstSurface);
  auto unwrapped_pIV = unwrap(pIV);
  m_pID3D11VideoContext->EncryptionBlt(
    unwrapped_pCryptoSession, 
    unwrapped_pSrcSurface, 
    unwrapped_pDstSurface, 
    IVSize, 
    unwrapped_pIV
  );
}
void __stdcall WrappedID3D11VideoContext3::DecryptionBlt(
  ID3D11CryptoSession * pCryptoSession, 
  ID3D11Texture2D * pSrcSurface, 
  ID3D11Texture2D * pDstSurface, 
  D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, 
  UINT ContentKeySize, 
  const void * pContentKey, 
  UINT IVSize, 
  void * pIV
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::DecryptionBlt\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto unwrapped_pSrcSurface = unwrap(pSrcSurface);
  auto unwrapped_pDstSurface = unwrap(pDstSurface);
  auto unwrapped_pEncryptedBlockInfo = unwrap(pEncryptedBlockInfo);
  auto unwrapped_pIV = unwrap(pIV);
  m_pID3D11VideoContext->DecryptionBlt(
    unwrapped_pCryptoSession, 
    unwrapped_pSrcSurface, 
    unwrapped_pDstSurface, 
    unwrapped_pEncryptedBlockInfo, 
    ContentKeySize, 
    pContentKey, 
    IVSize, 
    unwrapped_pIV
  );
}
void __stdcall WrappedID3D11VideoContext3::StartSessionKeyRefresh(
  ID3D11CryptoSession * pCryptoSession, 
  UINT RandomNumberSize, 
  void * pRandomNumber
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::StartSessionKeyRefresh\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  m_pID3D11VideoContext->StartSessionKeyRefresh(
    unwrapped_pCryptoSession, 
    RandomNumberSize, 
    pRandomNumber
  );
}
void __stdcall WrappedID3D11VideoContext3::FinishSessionKeyRefresh(
  ID3D11CryptoSession * pCryptoSession
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::FinishSessionKeyRefresh\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  m_pID3D11VideoContext->FinishSessionKeyRefresh(
    unwrapped_pCryptoSession
  );
}
HRESULT __stdcall WrappedID3D11VideoContext3::GetEncryptionBltKey(
  ID3D11CryptoSession * pCryptoSession, 
  UINT KeySize, 
  void * pReadbackKey
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::GetEncryptionBltKey\n";
  auto unwrapped_pCryptoSession = unwrap(pCryptoSession);
  auto ret = m_pID3D11VideoContext->GetEncryptionBltKey(
    unwrapped_pCryptoSession, 
    KeySize, 
    pReadbackKey
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::NegotiateAuthenticatedChannelKeyExchange(
  ID3D11AuthenticatedChannel * pChannel, 
  UINT DataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::NegotiateAuthenticatedChannelKeyExchange\n";
  auto unwrapped_pChannel = unwrap(pChannel);
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11VideoContext->NegotiateAuthenticatedChannelKeyExchange(
    unwrapped_pChannel, 
    DataSize, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::QueryAuthenticatedChannel(
  ID3D11AuthenticatedChannel * pChannel, 
  UINT InputSize, 
  const void * pInput, 
  UINT OutputSize, 
  void * pOutput
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::QueryAuthenticatedChannel\n";
  auto unwrapped_pChannel = unwrap(pChannel);
  auto ret = m_pID3D11VideoContext->QueryAuthenticatedChannel(
    unwrapped_pChannel, 
    InputSize, 
    pInput, 
    OutputSize, 
    pOutput
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::ConfigureAuthenticatedChannel(
  ID3D11AuthenticatedChannel * pChannel, 
  UINT InputSize, 
  const void * pInput, 
  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::ConfigureAuthenticatedChannel\n";
  auto unwrapped_pChannel = unwrap(pChannel);
  auto ret = m_pID3D11VideoContext->ConfigureAuthenticatedChannel(
    unwrapped_pChannel, 
    InputSize, 
    pInput, 
    pOutput
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorSetStreamRotation(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL Enable, 
  D3D11_VIDEO_PROCESSOR_ROTATION Rotation
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorSetStreamRotation\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorSetStreamRotation(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    Enable, 
    Rotation
  );
}
void __stdcall WrappedID3D11VideoContext3::VideoProcessorGetStreamRotation(
  ID3D11VideoProcessor * pVideoProcessor, 
  UINT StreamIndex, 
  BOOL * pEnable, 
  D3D11_VIDEO_PROCESSOR_ROTATION * pRotation
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11VideoContext << ")::VideoProcessorGetStreamRotation\n";
  auto unwrapped_pVideoProcessor = unwrap(pVideoProcessor);
  m_pID3D11VideoContext->VideoProcessorGetStreamRotation(
    unwrapped_pVideoProcessor, 
    StreamIndex, 
    pEnable, 
    pRotation
  );
}
void __stdcall WrappedID3D11VideoContext3::GetDevice(
  ID3D11Device ** ppDevice
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11DeviceChild << ")::GetDevice\n";
  m_pID3D11DeviceChild->GetDevice(
    ppDevice
  );
  if(*ppDevice)
    HandleWrap(__uuidof(ID3D11Device), (void**)ppDevice);
}
HRESULT __stdcall WrappedID3D11VideoContext3::GetPrivateData(
  const GUID & guid, 
  UINT * pDataSize, 
  void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11DeviceChild << ")::GetPrivateData\n";
  auto unwrapped_pDataSize = unwrap(pDataSize);
  auto ret = m_pID3D11DeviceChild->GetPrivateData(
    guid, 
    unwrapped_pDataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::SetPrivateData(
  const GUID & guid, 
  UINT DataSize, 
  const void * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11DeviceChild << ")::SetPrivateData\n";
  auto ret = m_pID3D11DeviceChild->SetPrivateData(
    guid, 
    DataSize, 
    pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::SetPrivateDataInterface(
  const GUID & guid, 
  const struct IUnknown * pData
) {
  out() << "ID3D11VideoContext3(" << m_pID3D11DeviceChild << ")::SetPrivateDataInterface\n";
  auto unwrapped_pData = unwrap(pData);
  auto ret = m_pID3D11DeviceChild->SetPrivateDataInterface(
    guid, 
    unwrapped_pData
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11VideoContext3::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11VideoContext3(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11VideoContext3)) {
      *ppvObject = (ID3D11VideoContext3*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoContext2)) {
      *ppvObject = (ID3D11VideoContext2*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoContext1)) {
      *ppvObject = (ID3D11VideoContext1*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11VideoContext)) {
      *ppvObject = (ID3D11VideoContext*)this;
      return ret;
    }
    if(riid == __uuidof(ID3D11DeviceChild)) {
      *ppvObject = (ID3D11DeviceChild*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11VideoContext3::AddRef() {
  out() << "ID3D11VideoContext3(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11VideoContext3::Release() {
  out() << "ID3D11VideoContext3(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionType::GetDesc(
  D3D11_SHADER_TYPE_DESC * pDesc
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetDesc\n";
  auto ret = m_pID3D11ShaderReflectionType->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionType::GetMemberTypeByIndex(
  UINT Index
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetMemberTypeByIndex\n";
  auto ret = m_pID3D11ShaderReflectionType->GetMemberTypeByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionType::GetMemberTypeByName(
  LPCSTR Name
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetMemberTypeByName\n";
  auto ret = m_pID3D11ShaderReflectionType->GetMemberTypeByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
LPCSTR __stdcall WrappedID3D11ShaderReflectionType::GetMemberTypeName(
  UINT Index
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetMemberTypeName\n";
  auto ret = m_pID3D11ShaderReflectionType->GetMemberTypeName(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionType::IsEqual(
  struct ID3D11ShaderReflectionType * pType
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::IsEqual\n";
  auto unwrapped_pType = unwrap(pType);
  auto ret = m_pID3D11ShaderReflectionType->IsEqual(
    unwrapped_pType
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionType::GetSubType() {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetSubType\n";
  auto ret = m_pID3D11ShaderReflectionType->GetSubType();
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionType::GetBaseClass() {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetBaseClass\n";
  auto ret = m_pID3D11ShaderReflectionType->GetBaseClass();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflectionType::GetNumInterfaces() {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetNumInterfaces\n";
  auto ret = m_pID3D11ShaderReflectionType->GetNumInterfaces();
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionType::GetInterfaceByIndex(
  UINT uIndex
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::GetInterfaceByIndex\n";
  auto ret = m_pID3D11ShaderReflectionType->GetInterfaceByIndex(
    uIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionType::IsOfType(
  struct ID3D11ShaderReflectionType * pType
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::IsOfType\n";
  auto unwrapped_pType = unwrap(pType);
  auto ret = m_pID3D11ShaderReflectionType->IsOfType(
    unwrapped_pType
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionType::ImplementsInterface(
  struct ID3D11ShaderReflectionType * pBase
) {
  out() << "ID3D11ShaderReflectionType(" << m_pID3D11ShaderReflectionType << ")::ImplementsInterface\n";
  auto unwrapped_pBase = unwrap(pBase);
  auto ret = m_pID3D11ShaderReflectionType->ImplementsInterface(
    unwrapped_pBase
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionVariable::GetDesc(
  D3D11_SHADER_VARIABLE_DESC * pDesc
) {
  out() << "ID3D11ShaderReflectionVariable(" << m_pID3D11ShaderReflectionVariable << ")::GetDesc\n";
  auto ret = m_pID3D11ShaderReflectionVariable->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionType * __stdcall WrappedID3D11ShaderReflectionVariable::GetType() {
  out() << "ID3D11ShaderReflectionVariable(" << m_pID3D11ShaderReflectionVariable << ")::GetType\n";
  auto ret = m_pID3D11ShaderReflectionVariable->GetType();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionConstantBuffer * __stdcall WrappedID3D11ShaderReflectionVariable::GetBuffer() {
  out() << "ID3D11ShaderReflectionVariable(" << m_pID3D11ShaderReflectionVariable << ")::GetBuffer\n";
  auto ret = m_pID3D11ShaderReflectionVariable->GetBuffer();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflectionVariable::GetInterfaceSlot(
  UINT uArrayIndex
) {
  out() << "ID3D11ShaderReflectionVariable(" << m_pID3D11ShaderReflectionVariable << ")::GetInterfaceSlot\n";
  auto ret = m_pID3D11ShaderReflectionVariable->GetInterfaceSlot(
    uArrayIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflectionConstantBuffer::GetDesc(
  D3D11_SHADER_BUFFER_DESC * pDesc
) {
  out() << "ID3D11ShaderReflectionConstantBuffer(" << m_pID3D11ShaderReflectionConstantBuffer << ")::GetDesc\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D11ShaderReflectionConstantBuffer->GetDesc(
    unwrapped_pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionVariable * __stdcall WrappedID3D11ShaderReflectionConstantBuffer::GetVariableByIndex(
  UINT Index
) {
  out() << "ID3D11ShaderReflectionConstantBuffer(" << m_pID3D11ShaderReflectionConstantBuffer << ")::GetVariableByIndex\n";
  auto ret = m_pID3D11ShaderReflectionConstantBuffer->GetVariableByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionVariable * __stdcall WrappedID3D11ShaderReflectionConstantBuffer::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D11ShaderReflectionConstantBuffer(" << m_pID3D11ShaderReflectionConstantBuffer << ")::GetVariableByName\n";
  auto ret = m_pID3D11ShaderReflectionConstantBuffer->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ShaderReflection(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ShaderReflection)) {
      *ppvObject = (ID3D11ShaderReflection*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ShaderReflection::AddRef() {
  out() << "ID3D11ShaderReflection(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ShaderReflection::Release() {
  out() << "ID3D11ShaderReflection(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetDesc(
  D3D11_SHADER_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetDesc\n";
  auto ret = m_pID3D11ShaderReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionConstantBuffer * __stdcall WrappedID3D11ShaderReflection::GetConstantBufferByIndex(
  UINT Index
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetConstantBufferByIndex\n";
  auto ret = m_pID3D11ShaderReflection->GetConstantBufferByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionConstantBuffer * __stdcall WrappedID3D11ShaderReflection::GetConstantBufferByName(
  LPCSTR Name
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetConstantBufferByName\n";
  auto ret = m_pID3D11ShaderReflection->GetConstantBufferByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetResourceBindingDesc(
  UINT ResourceIndex, 
  D3D11_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetResourceBindingDesc\n";
  auto ret = m_pID3D11ShaderReflection->GetResourceBindingDesc(
    ResourceIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetInputParameterDesc(
  UINT ParameterIndex, 
  D3D11_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetInputParameterDesc\n";
  auto ret = m_pID3D11ShaderReflection->GetInputParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetOutputParameterDesc(
  UINT ParameterIndex, 
  D3D11_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetOutputParameterDesc\n";
  auto ret = m_pID3D11ShaderReflection->GetOutputParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetPatchConstantParameterDesc(
  UINT ParameterIndex, 
  D3D11_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetPatchConstantParameterDesc\n";
  auto ret = m_pID3D11ShaderReflection->GetPatchConstantParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionVariable * __stdcall WrappedID3D11ShaderReflection::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetVariableByName\n";
  auto ret = m_pID3D11ShaderReflection->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetResourceBindingDescByName(
  LPCSTR Name, 
  D3D11_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetResourceBindingDescByName\n";
  auto ret = m_pID3D11ShaderReflection->GetResourceBindingDescByName(
    Name, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetMovInstructionCount() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetMovInstructionCount\n";
  auto ret = m_pID3D11ShaderReflection->GetMovInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetMovcInstructionCount() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetMovcInstructionCount\n";
  auto ret = m_pID3D11ShaderReflection->GetMovcInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetConversionInstructionCount() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetConversionInstructionCount\n";
  auto ret = m_pID3D11ShaderReflection->GetConversionInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetBitwiseInstructionCount() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetBitwiseInstructionCount\n";
  auto ret = m_pID3D11ShaderReflection->GetBitwiseInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
D3D_PRIMITIVE __stdcall WrappedID3D11ShaderReflection::GetGSInputPrimitive() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetGSInputPrimitive\n";
  auto ret = m_pID3D11ShaderReflection->GetGSInputPrimitive();
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D11ShaderReflection::IsSampleFrequencyShader() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::IsSampleFrequencyShader\n";
  auto ret = m_pID3D11ShaderReflection->IsSampleFrequencyShader();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetNumInterfaceSlots() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetNumInterfaceSlots\n";
  auto ret = m_pID3D11ShaderReflection->GetNumInterfaceSlots();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ShaderReflection::GetMinFeatureLevel(
  enum D3D_FEATURE_LEVEL * pLevel
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetMinFeatureLevel\n";
  auto ret = m_pID3D11ShaderReflection->GetMinFeatureLevel(
    pLevel
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D11ShaderReflection::GetThreadGroupSize(
  UINT * pSizeX, 
  UINT * pSizeY, 
  UINT * pSizeZ
) {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetThreadGroupSize\n";
  auto ret = m_pID3D11ShaderReflection->GetThreadGroupSize(
    pSizeX, 
    pSizeY, 
    pSizeZ
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D11ShaderReflection::GetRequiresFlags() {
  out() << "ID3D11ShaderReflection(" << m_pID3D11ShaderReflection << ")::GetRequiresFlags\n";
  auto ret = m_pID3D11ShaderReflection->GetRequiresFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11LibraryReflection::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11LibraryReflection(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11LibraryReflection)) {
      *ppvObject = (ID3D11LibraryReflection*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11LibraryReflection::AddRef() {
  out() << "ID3D11LibraryReflection(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11LibraryReflection::Release() {
  out() << "ID3D11LibraryReflection(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11LibraryReflection::GetDesc(
  D3D11_LIBRARY_DESC * pDesc
) {
  out() << "ID3D11LibraryReflection(" << m_pID3D11LibraryReflection << ")::GetDesc\n";
  auto ret = m_pID3D11LibraryReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11FunctionReflection * __stdcall WrappedID3D11LibraryReflection::GetFunctionByIndex(
  INT FunctionIndex
) {
  out() << "ID3D11LibraryReflection(" << m_pID3D11LibraryReflection << ")::GetFunctionByIndex\n";
  auto ret = m_pID3D11LibraryReflection->GetFunctionByIndex(
    FunctionIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionReflection::GetDesc(
  D3D11_FUNCTION_DESC * pDesc
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetDesc\n";
  auto ret = m_pID3D11FunctionReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionConstantBuffer * __stdcall WrappedID3D11FunctionReflection::GetConstantBufferByIndex(
  UINT BufferIndex
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetConstantBufferByIndex\n";
  auto ret = m_pID3D11FunctionReflection->GetConstantBufferByIndex(
    BufferIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionConstantBuffer * __stdcall WrappedID3D11FunctionReflection::GetConstantBufferByName(
  LPCSTR Name
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetConstantBufferByName\n";
  auto ret = m_pID3D11FunctionReflection->GetConstantBufferByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionReflection::GetResourceBindingDesc(
  UINT ResourceIndex, 
  D3D11_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetResourceBindingDesc\n";
  auto ret = m_pID3D11FunctionReflection->GetResourceBindingDesc(
    ResourceIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11ShaderReflectionVariable * __stdcall WrappedID3D11FunctionReflection::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetVariableByName\n";
  auto ret = m_pID3D11FunctionReflection->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionReflection::GetResourceBindingDescByName(
  LPCSTR Name, 
  D3D11_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetResourceBindingDescByName\n";
  auto ret = m_pID3D11FunctionReflection->GetResourceBindingDescByName(
    Name, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D11FunctionParameterReflection * __stdcall WrappedID3D11FunctionReflection::GetFunctionParameter(
  INT ParameterIndex
) {
  out() << "ID3D11FunctionReflection(" << m_pID3D11FunctionReflection << ")::GetFunctionParameter\n";
  auto ret = m_pID3D11FunctionReflection->GetFunctionParameter(
    ParameterIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionParameterReflection::GetDesc(
  D3D11_PARAMETER_DESC * pDesc
) {
  out() << "ID3D11FunctionParameterReflection(" << m_pID3D11FunctionParameterReflection << ")::GetDesc\n";
  auto ret = m_pID3D11FunctionParameterReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Module::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Module(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Module)) {
      *ppvObject = (ID3D11Module*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Module::AddRef() {
  out() << "ID3D11Module(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Module::Release() {
  out() << "ID3D11Module(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Module::CreateInstance(
  LPCSTR pNamespace, 
  struct ID3D11ModuleInstance ** ppModuleInstance
) {
  out() << "ID3D11Module(" << m_pID3D11Module << ")::CreateInstance\n";
  auto ret = m_pID3D11Module->CreateInstance(
    pNamespace, 
    ppModuleInstance
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppModuleInstance && *ppModuleInstance ) *ppModuleInstance = getWrapper<ID3D11ModuleInstance, WrappedID3D11ModuleInstance>(*ppModuleInstance);
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11ModuleInstance(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11ModuleInstance)) {
      *ppvObject = (ID3D11ModuleInstance*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11ModuleInstance::AddRef() {
  out() << "ID3D11ModuleInstance(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11ModuleInstance::Release() {
  out() << "ID3D11ModuleInstance(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindConstantBuffer(
  UINT uSrcSlot, 
  UINT uDstSlot, 
  UINT cbDstOffset
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindConstantBuffer\n";
  auto ret = m_pID3D11ModuleInstance->BindConstantBuffer(
    uSrcSlot, 
    uDstSlot, 
    cbDstOffset
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindConstantBufferByName(
  LPCSTR pName, 
  UINT uDstSlot, 
  UINT cbDstOffset
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindConstantBufferByName\n";
  auto ret = m_pID3D11ModuleInstance->BindConstantBufferByName(
    pName, 
    uDstSlot, 
    cbDstOffset
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindResource(
  UINT uSrcSlot, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindResource\n";
  auto ret = m_pID3D11ModuleInstance->BindResource(
    uSrcSlot, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindResourceByName(
  LPCSTR pName, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindResourceByName\n";
  auto ret = m_pID3D11ModuleInstance->BindResourceByName(
    pName, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindSampler(
  UINT uSrcSlot, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindSampler\n";
  auto ret = m_pID3D11ModuleInstance->BindSampler(
    uSrcSlot, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindSamplerByName(
  LPCSTR pName, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindSamplerByName\n";
  auto ret = m_pID3D11ModuleInstance->BindSamplerByName(
    pName, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindUnorderedAccessView(
  UINT uSrcSlot, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindUnorderedAccessView\n";
  auto ret = m_pID3D11ModuleInstance->BindUnorderedAccessView(
    uSrcSlot, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindUnorderedAccessViewByName(
  LPCSTR pName, 
  UINT uDstSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindUnorderedAccessViewByName\n";
  auto ret = m_pID3D11ModuleInstance->BindUnorderedAccessViewByName(
    pName, 
    uDstSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindResourceAsUnorderedAccessView(
  UINT uSrcSrvSlot, 
  UINT uDstUavSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindResourceAsUnorderedAccessView\n";
  auto ret = m_pID3D11ModuleInstance->BindResourceAsUnorderedAccessView(
    uSrcSrvSlot, 
    uDstUavSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11ModuleInstance::BindResourceAsUnorderedAccessViewByName(
  LPCSTR pSrvName, 
  UINT uDstUavSlot, 
  UINT uCount
) {
  out() << "ID3D11ModuleInstance(" << m_pID3D11ModuleInstance << ")::BindResourceAsUnorderedAccessViewByName\n";
  auto ret = m_pID3D11ModuleInstance->BindResourceAsUnorderedAccessViewByName(
    pSrvName, 
    uDstUavSlot, 
    uCount
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Linker::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11Linker(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11Linker)) {
      *ppvObject = (ID3D11Linker*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11Linker::AddRef() {
  out() << "ID3D11Linker(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11Linker::Release() {
  out() << "ID3D11Linker(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Linker::Link(
  struct ID3D11ModuleInstance * pEntry, 
  LPCSTR pEntryName, 
  LPCSTR pTargetName, 
  UINT uFlags, 
  ID3DBlob ** ppShaderBlob, 
  ID3DBlob ** ppErrorBuffer
) {
  out() << "ID3D11Linker(" << m_pID3D11Linker << ")::Link\n";
  auto unwrapped_pEntry = unwrap(pEntry);
  auto ret = m_pID3D11Linker->Link(
    unwrapped_pEntry, 
    pEntryName, 
    pTargetName, 
    uFlags, 
    ppShaderBlob, 
    ppErrorBuffer
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Linker::UseLibrary(
  struct ID3D11ModuleInstance * pLibraryMI
) {
  out() << "ID3D11Linker(" << m_pID3D11Linker << ")::UseLibrary\n";
  auto unwrapped_pLibraryMI = unwrap(pLibraryMI);
  auto ret = m_pID3D11Linker->UseLibrary(
    unwrapped_pLibraryMI
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11Linker::AddClipPlaneFromCBuffer(
  UINT uCBufferSlot, 
  UINT uCBufferEntry
) {
  out() << "ID3D11Linker(" << m_pID3D11Linker << ")::AddClipPlaneFromCBuffer\n";
  auto ret = m_pID3D11Linker->AddClipPlaneFromCBuffer(
    uCBufferSlot, 
    uCBufferEntry
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11LinkingNode::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11LinkingNode(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11LinkingNode)) {
      *ppvObject = (ID3D11LinkingNode*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11LinkingNode::AddRef() {
  out() << "ID3D11LinkingNode(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11LinkingNode::Release() {
  out() << "ID3D11LinkingNode(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D11FunctionLinkingGraph)) {
      *ppvObject = (ID3D11FunctionLinkingGraph*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D11FunctionLinkingGraph::AddRef() {
  out() << "ID3D11FunctionLinkingGraph(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D11FunctionLinkingGraph::Release() {
  out() << "ID3D11FunctionLinkingGraph(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::CreateModuleInstance(
  struct ID3D11ModuleInstance ** ppModuleInstance, 
  ID3DBlob ** ppErrorBuffer
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::CreateModuleInstance\n";
  auto ret = m_pID3D11FunctionLinkingGraph->CreateModuleInstance(
    ppModuleInstance, 
    ppErrorBuffer
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppModuleInstance && *ppModuleInstance ) *ppModuleInstance = getWrapper<ID3D11ModuleInstance, WrappedID3D11ModuleInstance>(*ppModuleInstance);
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::SetInputSignature(
  const D3D11_PARAMETER_DESC * pInputParameters, 
  UINT cInputParameters, 
  struct ID3D11LinkingNode ** ppInputNode
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::SetInputSignature\n";
  auto unwrapped_pInputParameters = unwrap(pInputParameters);
  auto ret = m_pID3D11FunctionLinkingGraph->SetInputSignature(
    unwrapped_pInputParameters, 
    cInputParameters, 
    ppInputNode
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppInputNode && *ppInputNode ) *ppInputNode = getWrapper<ID3D11LinkingNode, WrappedID3D11LinkingNode>(*ppInputNode);
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::SetOutputSignature(
  const D3D11_PARAMETER_DESC * pOutputParameters, 
  UINT cOutputParameters, 
  struct ID3D11LinkingNode ** ppOutputNode
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::SetOutputSignature\n";
  auto unwrapped_pOutputParameters = unwrap(pOutputParameters);
  auto ret = m_pID3D11FunctionLinkingGraph->SetOutputSignature(
    unwrapped_pOutputParameters, 
    cOutputParameters, 
    ppOutputNode
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppOutputNode && *ppOutputNode ) *ppOutputNode = getWrapper<ID3D11LinkingNode, WrappedID3D11LinkingNode>(*ppOutputNode);
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::CallFunction(
  LPCSTR pModuleInstanceNamespace, 
  struct ID3D11Module * pModuleWithFunctionPrototype, 
  LPCSTR pFunctionName, 
  struct ID3D11LinkingNode ** ppCallNode
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::CallFunction\n";
  auto unwrapped_pModuleWithFunctionPrototype = unwrap(pModuleWithFunctionPrototype);
  auto ret = m_pID3D11FunctionLinkingGraph->CallFunction(
    pModuleInstanceNamespace, 
    unwrapped_pModuleWithFunctionPrototype, 
    pFunctionName, 
    ppCallNode
  );
  out() << "\treturned " << ret << "\n";
    if (!ret && ppCallNode && *ppCallNode ) *ppCallNode = getWrapper<ID3D11LinkingNode, WrappedID3D11LinkingNode>(*ppCallNode);
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::PassValue(
  struct ID3D11LinkingNode * pSrcNode, 
  INT SrcParameterIndex, 
  struct ID3D11LinkingNode * pDstNode, 
  INT DstParameterIndex
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::PassValue\n";
  auto unwrapped_pSrcNode = unwrap(pSrcNode);
  auto unwrapped_pDstNode = unwrap(pDstNode);
  auto ret = m_pID3D11FunctionLinkingGraph->PassValue(
    unwrapped_pSrcNode, 
    SrcParameterIndex, 
    unwrapped_pDstNode, 
    DstParameterIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::PassValueWithSwizzle(
  struct ID3D11LinkingNode * pSrcNode, 
  INT SrcParameterIndex, 
  LPCSTR pSrcSwizzle, 
  struct ID3D11LinkingNode * pDstNode, 
  INT DstParameterIndex, 
  LPCSTR pDstSwizzle
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::PassValueWithSwizzle\n";
  auto unwrapped_pSrcNode = unwrap(pSrcNode);
  auto unwrapped_pDstNode = unwrap(pDstNode);
  auto ret = m_pID3D11FunctionLinkingGraph->PassValueWithSwizzle(
    unwrapped_pSrcNode, 
    SrcParameterIndex, 
    pSrcSwizzle, 
    unwrapped_pDstNode, 
    DstParameterIndex, 
    pDstSwizzle
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::GetLastError(
  ID3DBlob ** ppErrorBuffer
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::GetLastError\n";
  auto ret = m_pID3D11FunctionLinkingGraph->GetLastError(
    ppErrorBuffer
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D11FunctionLinkingGraph::GenerateHlsl(
  UINT uFlags, 
  ID3DBlob ** ppBuffer
) {
  out() << "ID3D11FunctionLinkingGraph(" << m_pID3D11FunctionLinkingGraph << ")::GenerateHlsl\n";
  auto ret = m_pID3D11FunctionLinkingGraph->GenerateHlsl(
    uFlags, 
    ppBuffer
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionType::GetDesc(
  D3D12_SHADER_TYPE_DESC * pDesc
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetDesc\n";
  auto ret = m_pID3D12ShaderReflectionType->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionType::GetMemberTypeByIndex(
  UINT Index
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetMemberTypeByIndex\n";
  auto ret = m_pID3D12ShaderReflectionType->GetMemberTypeByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionType::GetMemberTypeByName(
  LPCSTR Name
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetMemberTypeByName\n";
  auto ret = m_pID3D12ShaderReflectionType->GetMemberTypeByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
LPCSTR __stdcall WrappedID3D12ShaderReflectionType::GetMemberTypeName(
  UINT Index
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetMemberTypeName\n";
  auto ret = m_pID3D12ShaderReflectionType->GetMemberTypeName(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionType::IsEqual(
  struct ID3D12ShaderReflectionType * pType
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::IsEqual\n";
  auto unwrapped_pType = unwrap(pType);
  auto ret = m_pID3D12ShaderReflectionType->IsEqual(
    unwrapped_pType
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionType::GetSubType() {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetSubType\n";
  auto ret = m_pID3D12ShaderReflectionType->GetSubType();
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionType::GetBaseClass() {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetBaseClass\n";
  auto ret = m_pID3D12ShaderReflectionType->GetBaseClass();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflectionType::GetNumInterfaces() {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetNumInterfaces\n";
  auto ret = m_pID3D12ShaderReflectionType->GetNumInterfaces();
  out() << "\treturned " << ret << "\n";
  return ret;
}
struct ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionType::GetInterfaceByIndex(
  UINT uIndex
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::GetInterfaceByIndex\n";
  auto ret = m_pID3D12ShaderReflectionType->GetInterfaceByIndex(
    uIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionType::IsOfType(
  struct ID3D12ShaderReflectionType * pType
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::IsOfType\n";
  auto unwrapped_pType = unwrap(pType);
  auto ret = m_pID3D12ShaderReflectionType->IsOfType(
    unwrapped_pType
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionType::ImplementsInterface(
  struct ID3D12ShaderReflectionType * pBase
) {
  out() << "ID3D12ShaderReflectionType(" << m_pID3D12ShaderReflectionType << ")::ImplementsInterface\n";
  auto unwrapped_pBase = unwrap(pBase);
  auto ret = m_pID3D12ShaderReflectionType->ImplementsInterface(
    unwrapped_pBase
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionVariable::GetDesc(
  D3D12_SHADER_VARIABLE_DESC * pDesc
) {
  out() << "ID3D12ShaderReflectionVariable(" << m_pID3D12ShaderReflectionVariable << ")::GetDesc\n";
  auto ret = m_pID3D12ShaderReflectionVariable->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionType * __stdcall WrappedID3D12ShaderReflectionVariable::GetType() {
  out() << "ID3D12ShaderReflectionVariable(" << m_pID3D12ShaderReflectionVariable << ")::GetType\n";
  auto ret = m_pID3D12ShaderReflectionVariable->GetType();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionConstantBuffer * __stdcall WrappedID3D12ShaderReflectionVariable::GetBuffer() {
  out() << "ID3D12ShaderReflectionVariable(" << m_pID3D12ShaderReflectionVariable << ")::GetBuffer\n";
  auto ret = m_pID3D12ShaderReflectionVariable->GetBuffer();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflectionVariable::GetInterfaceSlot(
  UINT uArrayIndex
) {
  out() << "ID3D12ShaderReflectionVariable(" << m_pID3D12ShaderReflectionVariable << ")::GetInterfaceSlot\n";
  auto ret = m_pID3D12ShaderReflectionVariable->GetInterfaceSlot(
    uArrayIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflectionConstantBuffer::GetDesc(
  D3D12_SHADER_BUFFER_DESC * pDesc
) {
  out() << "ID3D12ShaderReflectionConstantBuffer(" << m_pID3D12ShaderReflectionConstantBuffer << ")::GetDesc\n";
  auto unwrapped_pDesc = unwrap(pDesc);
  auto ret = m_pID3D12ShaderReflectionConstantBuffer->GetDesc(
    unwrapped_pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionVariable * __stdcall WrappedID3D12ShaderReflectionConstantBuffer::GetVariableByIndex(
  UINT Index
) {
  out() << "ID3D12ShaderReflectionConstantBuffer(" << m_pID3D12ShaderReflectionConstantBuffer << ")::GetVariableByIndex\n";
  auto ret = m_pID3D12ShaderReflectionConstantBuffer->GetVariableByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionVariable * __stdcall WrappedID3D12ShaderReflectionConstantBuffer::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D12ShaderReflectionConstantBuffer(" << m_pID3D12ShaderReflectionConstantBuffer << ")::GetVariableByName\n";
  auto ret = m_pID3D12ShaderReflectionConstantBuffer->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D12ShaderReflection(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D12ShaderReflection)) {
      *ppvObject = (ID3D12ShaderReflection*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D12ShaderReflection::AddRef() {
  out() << "ID3D12ShaderReflection(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D12ShaderReflection::Release() {
  out() << "ID3D12ShaderReflection(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetDesc(
  D3D12_SHADER_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetDesc\n";
  auto ret = m_pID3D12ShaderReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionConstantBuffer * __stdcall WrappedID3D12ShaderReflection::GetConstantBufferByIndex(
  UINT Index
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetConstantBufferByIndex\n";
  auto ret = m_pID3D12ShaderReflection->GetConstantBufferByIndex(
    Index
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionConstantBuffer * __stdcall WrappedID3D12ShaderReflection::GetConstantBufferByName(
  LPCSTR Name
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetConstantBufferByName\n";
  auto ret = m_pID3D12ShaderReflection->GetConstantBufferByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetResourceBindingDesc(
  UINT ResourceIndex, 
  D3D12_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetResourceBindingDesc\n";
  auto ret = m_pID3D12ShaderReflection->GetResourceBindingDesc(
    ResourceIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetInputParameterDesc(
  UINT ParameterIndex, 
  D3D12_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetInputParameterDesc\n";
  auto ret = m_pID3D12ShaderReflection->GetInputParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetOutputParameterDesc(
  UINT ParameterIndex, 
  D3D12_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetOutputParameterDesc\n";
  auto ret = m_pID3D12ShaderReflection->GetOutputParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetPatchConstantParameterDesc(
  UINT ParameterIndex, 
  D3D12_SIGNATURE_PARAMETER_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetPatchConstantParameterDesc\n";
  auto ret = m_pID3D12ShaderReflection->GetPatchConstantParameterDesc(
    ParameterIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionVariable * __stdcall WrappedID3D12ShaderReflection::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetVariableByName\n";
  auto ret = m_pID3D12ShaderReflection->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetResourceBindingDescByName(
  LPCSTR Name, 
  D3D12_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetResourceBindingDescByName\n";
  auto ret = m_pID3D12ShaderReflection->GetResourceBindingDescByName(
    Name, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetMovInstructionCount() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetMovInstructionCount\n";
  auto ret = m_pID3D12ShaderReflection->GetMovInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetMovcInstructionCount() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetMovcInstructionCount\n";
  auto ret = m_pID3D12ShaderReflection->GetMovcInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetConversionInstructionCount() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetConversionInstructionCount\n";
  auto ret = m_pID3D12ShaderReflection->GetConversionInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetBitwiseInstructionCount() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetBitwiseInstructionCount\n";
  auto ret = m_pID3D12ShaderReflection->GetBitwiseInstructionCount();
  out() << "\treturned " << ret << "\n";
  return ret;
}
D3D_PRIMITIVE __stdcall WrappedID3D12ShaderReflection::GetGSInputPrimitive() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetGSInputPrimitive\n";
  auto ret = m_pID3D12ShaderReflection->GetGSInputPrimitive();
  out() << "\treturned " << ret << "\n";
  return ret;
}
BOOL __stdcall WrappedID3D12ShaderReflection::IsSampleFrequencyShader() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::IsSampleFrequencyShader\n";
  auto ret = m_pID3D12ShaderReflection->IsSampleFrequencyShader();
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetNumInterfaceSlots() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetNumInterfaceSlots\n";
  auto ret = m_pID3D12ShaderReflection->GetNumInterfaceSlots();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12ShaderReflection::GetMinFeatureLevel(
  enum D3D_FEATURE_LEVEL * pLevel
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetMinFeatureLevel\n";
  auto ret = m_pID3D12ShaderReflection->GetMinFeatureLevel(
    pLevel
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT __stdcall WrappedID3D12ShaderReflection::GetThreadGroupSize(
  UINT * pSizeX, 
  UINT * pSizeY, 
  UINT * pSizeZ
) {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetThreadGroupSize\n";
  auto ret = m_pID3D12ShaderReflection->GetThreadGroupSize(
    pSizeX, 
    pSizeY, 
    pSizeZ
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
UINT64 __stdcall WrappedID3D12ShaderReflection::GetRequiresFlags() {
  out() << "ID3D12ShaderReflection(" << m_pID3D12ShaderReflection << ")::GetRequiresFlags\n";
  auto ret = m_pID3D12ShaderReflection->GetRequiresFlags();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12LibraryReflection::QueryInterface(
  const IID & riid, 
  void ** ppvObject
) {
  out() << "ID3D12LibraryReflection(" << m_pIUnknown << ")::QueryInterface\n";
  auto ret = m_pIUnknown->QueryInterface(
    riid, 
    ppvObject
  );
  out() << "\treturned " << ret << "\n";
  if(!ret) {
    if(riid == __uuidof(ID3D12LibraryReflection)) {
      *ppvObject = (ID3D12LibraryReflection*)this;
      return ret;
    }
    if(riid == __uuidof(IUnknown)) {
      *ppvObject = (IUnknown*)this;
      return ret;
    }
  }
  {
    if(!ret) {
      HandleWrap(riid, ppvObject);
    }
  }
  return ret;
}
ULONG __stdcall WrappedID3D12LibraryReflection::AddRef() {
  out() << "ID3D12LibraryReflection(" << m_pIUnknown << ")::AddRef\n";
  auto ret = m_pIUnknown->AddRef();
  out() << "\treturned " << ret << "\n";
  return ret;
}
ULONG __stdcall WrappedID3D12LibraryReflection::Release() {
  out() << "ID3D12LibraryReflection(" << m_pIUnknown << ")::Release\n";
  auto ret = m_pIUnknown->Release();
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12LibraryReflection::GetDesc(
  D3D12_LIBRARY_DESC * pDesc
) {
  out() << "ID3D12LibraryReflection(" << m_pID3D12LibraryReflection << ")::GetDesc\n";
  auto ret = m_pID3D12LibraryReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12FunctionReflection * __stdcall WrappedID3D12LibraryReflection::GetFunctionByIndex(
  INT FunctionIndex
) {
  out() << "ID3D12LibraryReflection(" << m_pID3D12LibraryReflection << ")::GetFunctionByIndex\n";
  auto ret = m_pID3D12LibraryReflection->GetFunctionByIndex(
    FunctionIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12FunctionReflection::GetDesc(
  D3D12_FUNCTION_DESC * pDesc
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetDesc\n";
  auto ret = m_pID3D12FunctionReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionConstantBuffer * __stdcall WrappedID3D12FunctionReflection::GetConstantBufferByIndex(
  UINT BufferIndex
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetConstantBufferByIndex\n";
  auto ret = m_pID3D12FunctionReflection->GetConstantBufferByIndex(
    BufferIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionConstantBuffer * __stdcall WrappedID3D12FunctionReflection::GetConstantBufferByName(
  LPCSTR Name
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetConstantBufferByName\n";
  auto ret = m_pID3D12FunctionReflection->GetConstantBufferByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12FunctionReflection::GetResourceBindingDesc(
  UINT ResourceIndex, 
  D3D12_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetResourceBindingDesc\n";
  auto ret = m_pID3D12FunctionReflection->GetResourceBindingDesc(
    ResourceIndex, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12ShaderReflectionVariable * __stdcall WrappedID3D12FunctionReflection::GetVariableByName(
  LPCSTR Name
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetVariableByName\n";
  auto ret = m_pID3D12FunctionReflection->GetVariableByName(
    Name
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12FunctionReflection::GetResourceBindingDescByName(
  LPCSTR Name, 
  D3D12_SHADER_INPUT_BIND_DESC * pDesc
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetResourceBindingDescByName\n";
  auto ret = m_pID3D12FunctionReflection->GetResourceBindingDescByName(
    Name, 
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
ID3D12FunctionParameterReflection * __stdcall WrappedID3D12FunctionReflection::GetFunctionParameter(
  INT ParameterIndex
) {
  out() << "ID3D12FunctionReflection(" << m_pID3D12FunctionReflection << ")::GetFunctionParameter\n";
  auto ret = m_pID3D12FunctionReflection->GetFunctionParameter(
    ParameterIndex
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT __stdcall WrappedID3D12FunctionParameterReflection::GetDesc(
  D3D12_PARAMETER_DESC * pDesc
) {
  out() << "ID3D12FunctionParameterReflection(" << m_pID3D12FunctionParameterReflection << ")::GetDesc\n";
  auto ret = m_pID3D12FunctionParameterReflection->GetDesc(
    pDesc
  );
  out() << "\treturned " << ret << "\n";
  return ret;
}
HRESULT WrappedCreateDXGIFactory(
  const IID & riid, 
  void ** ppFactory
) {
  out() << "CreateDXGIFactory\n";
  auto ret = CreateDXGIFactory(
    riid, 
    ppFactory
  );
  if(!ret)
    HandleWrap(riid, (void**)ppFactory);
  return ret;
}
HRESULT WrappedCreateDXGIFactory1(
  const IID & riid, 
  void ** ppFactory
) {
  out() << "CreateDXGIFactory1\n";
  auto ret = CreateDXGIFactory1(
    riid, 
    ppFactory
  );
  if(!ret)
    HandleWrap(riid, (void**)ppFactory);
  return ret;
}
UINT WrappedD3D11CalcSubresource(
  UINT MipSlice, 
  UINT ArraySlice, 
  UINT MipLevels
) {
  out() << "D3D11CalcSubresource\n";
  auto ret = D3D11CalcSubresource(
    MipSlice, 
    ArraySlice, 
    MipLevels
  );
  return ret;
}
HRESULT WrappedD3D11CreateDevice(
  IDXGIAdapter * pAdapter, 
  D3D_DRIVER_TYPE DriverType, 
  HMODULE Software, 
  UINT Flags, 
  const D3D_FEATURE_LEVEL * pFeatureLevels, 
  UINT FeatureLevels, 
  UINT SDKVersion, 
  ID3D11Device ** ppDevice, 
  D3D_FEATURE_LEVEL * pFeatureLevel, 
  ID3D11DeviceContext ** ppImmediateContext
) {
  out() << "D3D11CreateDevice\n";
  auto ret = D3D11CreateDevice(
    unwrap(pAdapter), 
    DriverType, 
    Software, 
    Flags, 
    pFeatureLevels, 
    FeatureLevels, 
    SDKVersion, 
    ppDevice, 
    pFeatureLevel, 
    ppImmediateContext
  );
  if (!ret && ppDevice && *ppDevice)
    *ppDevice = getWrapper<ID3D11Device, WrappedID3D11Device5>(*ppDevice);
  if (!ret && ppImmediateContext && *ppImmediateContext)
    *ppImmediateContext = getWrapper<ID3D11DeviceContext, WrappedID3D11DeviceContext4>(*ppImmediateContext);
  return ret;
}
HRESULT WrappedD3D11CreateDeviceAndSwapChain(
  IDXGIAdapter * pAdapter, 
  D3D_DRIVER_TYPE DriverType, 
  HMODULE Software, 
  UINT Flags, 
  const D3D_FEATURE_LEVEL * pFeatureLevels, 
  UINT FeatureLevels, 
  UINT SDKVersion, 
  const DXGI_SWAP_CHAIN_DESC * pSwapChainDesc, 
  IDXGISwapChain ** ppSwapChain, 
  ID3D11Device ** ppDevice, 
  D3D_FEATURE_LEVEL * pFeatureLevel, 
  ID3D11DeviceContext ** ppImmediateContext
) {
  out() << "D3D11CreateDeviceAndSwapChain\n";
  auto ret = D3D11CreateDeviceAndSwapChain(
    unwrap(pAdapter), 
    DriverType, 
    Software, 
    Flags, 
    pFeatureLevels, 
    FeatureLevels, 
    SDKVersion, 
    unwrap(pSwapChainDesc), 
    ppSwapChain, 
    ppDevice, 
    pFeatureLevel, 
    ppImmediateContext
  );
  if (!ret && ppSwapChain && *ppSwapChain)
    *ppSwapChain = getWrapper<IDXGISwapChain, WrappedIDXGISwapChain4>(*ppSwapChain);
  if (!ret && ppDevice && *ppDevice)
    *ppDevice = getWrapper<ID3D11Device, WrappedID3D11Device5>(*ppDevice);
  if (!ret && ppImmediateContext && *ppImmediateContext)
    *ppImmediateContext = getWrapper<ID3D11DeviceContext, WrappedID3D11DeviceContext4>(*ppImmediateContext);
  return ret;
}
HRESULT WrappedCreateDXGIFactory2(
  UINT Flags, 
  const IID & riid, 
  void ** ppFactory
) {
  out() << "CreateDXGIFactory2\n";
  auto ret = CreateDXGIFactory2(
    Flags, 
    riid, 
    ppFactory
  );
  if(!ret)
    HandleWrap(riid, (void**)ppFactory);
  return ret;
}
HRESULT WrappedDXGIGetDebugInterface1(
  UINT Flags, 
  const IID & riid, 
  void ** pDebug
) {
  out() << "DXGIGetDebugInterface1\n";
  auto ret = DXGIGetDebugInterface1(
    Flags, 
    riid, 
    pDebug
  );
  if(!ret)
    HandleWrap(riid, (void**)pDebug);
  return ret;
}
HRESULT WrappedD3DReadFileToBlob(
  LPCWSTR pFileName, 
  ID3DBlob ** ppContents
) {
  out() << "D3DReadFileToBlob\n";
  auto ret = D3DReadFileToBlob(
    pFileName, 
    ppContents
  );
  if (!ret && ppContents && *ppContents)
    *ppContents = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppContents);
  return ret;
}
HRESULT WrappedD3DWriteBlobToFile(
  ID3DBlob * pBlob, 
  LPCWSTR pFileName, 
  BOOL bOverwrite
) {
  out() << "D3DWriteBlobToFile\n";
  auto ret = D3DWriteBlobToFile(
    unwrap(pBlob), 
    pFileName, 
    bOverwrite
  );
  return ret;
}
HRESULT WrappedD3DCompile(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  LPCSTR pSourceName, 
  const D3D_SHADER_MACRO * pDefines, 
  ID3DInclude * pInclude, 
  LPCSTR pEntrypoint, 
  LPCSTR pTarget, 
  UINT Flags1, 
  UINT Flags2, 
  ID3DBlob ** ppCode, 
  ID3DBlob ** ppErrorMsgs
) {
  out() << "D3DCompile\n";
  auto ret = D3DCompile(
    pSrcData, 
    SrcDataSize, 
    pSourceName, 
    pDefines, 
    unwrap(pInclude), 
    pEntrypoint, 
    pTarget, 
    Flags1, 
    Flags2, 
    ppCode, 
    ppErrorMsgs
  );
  if (!ret && ppCode && *ppCode)
    *ppCode = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppCode);
  if (!ret && ppErrorMsgs && *ppErrorMsgs)
    *ppErrorMsgs = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppErrorMsgs);
  return ret;
}
HRESULT WrappedD3DCompile2(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  LPCSTR pSourceName, 
  const D3D_SHADER_MACRO * pDefines, 
  ID3DInclude * pInclude, 
  LPCSTR pEntrypoint, 
  LPCSTR pTarget, 
  UINT Flags1, 
  UINT Flags2, 
  UINT SecondaryDataFlags, 
  LPCVOID pSecondaryData, 
  SIZE_T SecondaryDataSize, 
  ID3DBlob ** ppCode, 
  ID3DBlob ** ppErrorMsgs
) {
  out() << "D3DCompile2\n";
  auto ret = D3DCompile2(
    pSrcData, 
    SrcDataSize, 
    pSourceName, 
    pDefines, 
    unwrap(pInclude), 
    pEntrypoint, 
    pTarget, 
    Flags1, 
    Flags2, 
    SecondaryDataFlags, 
    pSecondaryData, 
    SecondaryDataSize, 
    ppCode, 
    ppErrorMsgs
  );
  if (!ret && ppCode && *ppCode)
    *ppCode = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppCode);
  if (!ret && ppErrorMsgs && *ppErrorMsgs)
    *ppErrorMsgs = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppErrorMsgs);
  return ret;
}
HRESULT WrappedD3DCompileFromFile(
  LPCWSTR pFileName, 
  const D3D_SHADER_MACRO * pDefines, 
  ID3DInclude * pInclude, 
  LPCSTR pEntrypoint, 
  LPCSTR pTarget, 
  UINT Flags1, 
  UINT Flags2, 
  ID3DBlob ** ppCode, 
  ID3DBlob ** ppErrorMsgs
) {
  out() << "D3DCompileFromFile\n";
  auto ret = D3DCompileFromFile(
    pFileName, 
    pDefines, 
    unwrap(pInclude), 
    pEntrypoint, 
    pTarget, 
    Flags1, 
    Flags2, 
    ppCode, 
    ppErrorMsgs
  );
  if (!ret && ppCode && *ppCode)
    *ppCode = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppCode);
  if (!ret && ppErrorMsgs && *ppErrorMsgs)
    *ppErrorMsgs = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppErrorMsgs);
  return ret;
}
HRESULT WrappedD3DPreprocess(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  LPCSTR pSourceName, 
  const D3D_SHADER_MACRO * pDefines, 
  ID3DInclude * pInclude, 
  ID3DBlob ** ppCodeText, 
  ID3DBlob ** ppErrorMsgs
) {
  out() << "D3DPreprocess\n";
  auto ret = D3DPreprocess(
    pSrcData, 
    SrcDataSize, 
    pSourceName, 
    unwrap(pDefines), 
    unwrap(pInclude), 
    ppCodeText, 
    ppErrorMsgs
  );
  if (!ret && ppCodeText && *ppCodeText)
    *ppCodeText = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppCodeText);
  if (!ret && ppErrorMsgs && *ppErrorMsgs)
    *ppErrorMsgs = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppErrorMsgs);
  return ret;
}
HRESULT WrappedD3DGetDebugInfo(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  ID3DBlob ** ppDebugInfo
) {
  out() << "D3DGetDebugInfo\n";
  auto ret = D3DGetDebugInfo(
    pSrcData, 
    SrcDataSize, 
    ppDebugInfo
  );
  if (!ret && ppDebugInfo && *ppDebugInfo)
    *ppDebugInfo = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppDebugInfo);
  return ret;
}
HRESULT WrappedD3DReflect(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  const IID & pInterface, 
  void ** ppReflector
) {
  out() << "D3DReflect\n";
  auto ret = D3DReflect(
    pSrcData, 
    SrcDataSize, 
    pInterface, 
    ppReflector
  );
  assert(false && "Wrap not implemented; Emit Seg Fault");
  return ret;
}
HRESULT WrappedD3DReflectLibrary(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  const IID & riid, 
  LPVOID * ppReflector
) {
  out() << "D3DReflectLibrary\n";
  auto ret = D3DReflectLibrary(
    pSrcData, 
    SrcDataSize, 
    riid, 
    unwrap(ppReflector)
  );
  return ret;
}
HRESULT WrappedD3DDisassemble(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  UINT Flags, 
  LPCSTR szComments, 
  ID3DBlob ** ppDisassembly
) {
  out() << "D3DDisassemble\n";
  auto ret = D3DDisassemble(
    pSrcData, 
    SrcDataSize, 
    Flags, 
    szComments, 
    ppDisassembly
  );
  if (!ret && ppDisassembly && *ppDisassembly)
    *ppDisassembly = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppDisassembly);
  return ret;
}
HRESULT WrappedD3DDisassembleRegion(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  UINT Flags, 
  LPCSTR szComments, 
  SIZE_T StartByteOffset, 
  SIZE_T NumInsts, 
  SIZE_T * pFinishByteOffset, 
  ID3DBlob ** ppDisassembly
) {
  out() << "D3DDisassembleRegion\n";
  auto ret = D3DDisassembleRegion(
    pSrcData, 
    SrcDataSize, 
    Flags, 
    szComments, 
    StartByteOffset, 
    NumInsts, 
    pFinishByteOffset, 
    ppDisassembly
  );
  if (!ret && ppDisassembly && *ppDisassembly)
    *ppDisassembly = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppDisassembly);
  return ret;
}
HRESULT WrappedD3DCreateLinker(
  struct ID3D11Linker ** ppLinker
) {
  out() << "D3DCreateLinker\n";
  auto ret = D3DCreateLinker(
    ppLinker
  );
  if (!ret && ppLinker && *ppLinker)
    *ppLinker = getWrapper<ID3D11Linker, WrappedID3D11Linker>(*ppLinker);
  return ret;
}
HRESULT WrappedD3DLoadModule(
  LPCVOID pSrcData, 
  SIZE_T cbSrcDataSize, 
  struct ID3D11Module ** ppModule
) {
  out() << "D3DLoadModule\n";
  auto ret = D3DLoadModule(
    pSrcData, 
    cbSrcDataSize, 
    ppModule
  );
  if (!ret && ppModule && *ppModule)
    *ppModule = getWrapper<ID3D11Module, WrappedID3D11Module>(*ppModule);
  return ret;
}
HRESULT WrappedD3DCreateFunctionLinkingGraph(
  UINT uFlags, 
  struct ID3D11FunctionLinkingGraph ** ppFunctionLinkingGraph
) {
  out() << "D3DCreateFunctionLinkingGraph\n";
  auto ret = D3DCreateFunctionLinkingGraph(
    uFlags, 
    ppFunctionLinkingGraph
  );
  if (!ret && ppFunctionLinkingGraph && *ppFunctionLinkingGraph)
    *ppFunctionLinkingGraph = getWrapper<ID3D11FunctionLinkingGraph, WrappedID3D11FunctionLinkingGraph>(*ppFunctionLinkingGraph);
  return ret;
}
HRESULT WrappedD3DGetTraceInstructionOffsets(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  UINT Flags, 
  SIZE_T StartInstIndex, 
  SIZE_T NumInsts, 
  SIZE_T * pOffsets, 
  SIZE_T * pTotalInsts
) {
  out() << "D3DGetTraceInstructionOffsets\n";
  auto ret = D3DGetTraceInstructionOffsets(
    pSrcData, 
    SrcDataSize, 
    Flags, 
    StartInstIndex, 
    NumInsts, 
    unwrap(pOffsets), 
    pTotalInsts
  );
  return ret;
}
HRESULT WrappedD3DGetInputSignatureBlob(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  ID3DBlob ** ppSignatureBlob
) {
  out() << "D3DGetInputSignatureBlob\n";
  auto ret = D3DGetInputSignatureBlob(
    pSrcData, 
    SrcDataSize, 
    ppSignatureBlob
  );
  if (!ret && ppSignatureBlob && *ppSignatureBlob)
    *ppSignatureBlob = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppSignatureBlob);
  return ret;
}
HRESULT WrappedD3DGetOutputSignatureBlob(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  ID3DBlob ** ppSignatureBlob
) {
  out() << "D3DGetOutputSignatureBlob\n";
  auto ret = D3DGetOutputSignatureBlob(
    pSrcData, 
    SrcDataSize, 
    ppSignatureBlob
  );
  if (!ret && ppSignatureBlob && *ppSignatureBlob)
    *ppSignatureBlob = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppSignatureBlob);
  return ret;
}
HRESULT WrappedD3DGetInputAndOutputSignatureBlob(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  ID3DBlob ** ppSignatureBlob
) {
  out() << "D3DGetInputAndOutputSignatureBlob\n";
  auto ret = D3DGetInputAndOutputSignatureBlob(
    pSrcData, 
    SrcDataSize, 
    ppSignatureBlob
  );
  if (!ret && ppSignatureBlob && *ppSignatureBlob)
    *ppSignatureBlob = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppSignatureBlob);
  return ret;
}
HRESULT WrappedD3DStripShader(
  LPCVOID pShaderBytecode, 
  SIZE_T BytecodeLength, 
  UINT uStripFlags, 
  ID3DBlob ** ppStrippedBlob
) {
  out() << "D3DStripShader\n";
  auto ret = D3DStripShader(
    pShaderBytecode, 
    BytecodeLength, 
    uStripFlags, 
    ppStrippedBlob
  );
  if (!ret && ppStrippedBlob && *ppStrippedBlob)
    *ppStrippedBlob = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppStrippedBlob);
  return ret;
}
HRESULT WrappedD3DGetBlobPart(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  D3D_BLOB_PART Part, 
  UINT Flags, 
  ID3DBlob ** ppPart
) {
  out() << "D3DGetBlobPart\n";
  auto ret = D3DGetBlobPart(
    pSrcData, 
    SrcDataSize, 
    Part, 
    Flags, 
    ppPart
  );
  if (!ret && ppPart && *ppPart)
    *ppPart = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppPart);
  return ret;
}
HRESULT WrappedD3DSetBlobPart(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  D3D_BLOB_PART Part, 
  UINT Flags, 
  LPCVOID pPart, 
  SIZE_T PartSize, 
  ID3DBlob ** ppNewShader
) {
  out() << "D3DSetBlobPart\n";
  auto ret = D3DSetBlobPart(
    pSrcData, 
    SrcDataSize, 
    Part, 
    Flags, 
    pPart, 
    PartSize, 
    ppNewShader
  );
  if (!ret && ppNewShader && *ppNewShader)
    *ppNewShader = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppNewShader);
  return ret;
}
HRESULT WrappedD3DCreateBlob(
  SIZE_T Size, 
  ID3DBlob ** ppBlob
) {
  out() << "D3DCreateBlob\n";
  auto ret = D3DCreateBlob(
    Size, 
    ppBlob
  );
  if (!ret && ppBlob && *ppBlob)
    *ppBlob = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppBlob);
  return ret;
}
HRESULT WrappedD3DCompressShaders(
  UINT uNumShaders, 
  D3D_SHADER_DATA * pShaderData, 
  UINT uFlags, 
  ID3DBlob ** ppCompressedData
) {
  out() << "D3DCompressShaders\n";
  auto ret = D3DCompressShaders(
    uNumShaders, 
    pShaderData, 
    uFlags, 
    ppCompressedData
  );
  if (!ret && ppCompressedData && *ppCompressedData)
    *ppCompressedData = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppCompressedData);
  return ret;
}
HRESULT WrappedD3DDecompressShaders(
  LPCVOID pSrcData, 
  SIZE_T SrcDataSize, 
  UINT uNumShaders, 
  UINT uStartIndex, 
  UINT * pIndices, 
  UINT uFlags, 
  ID3DBlob ** ppShaders, 
  UINT * pTotalShaders
) {
  out() << "D3DDecompressShaders\n";
  auto ret = D3DDecompressShaders(
    pSrcData, 
    SrcDataSize, 
    uNumShaders, 
    uStartIndex, 
    pIndices, 
    uFlags, 
    ppShaders, 
    pTotalShaders
  );
  if (!ret && ppShaders && *ppShaders)
    *ppShaders = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppShaders);
  return ret;
}
HRESULT WrappedD3DDisassemble10Effect(
  struct ID3D10Effect * pEffect, 
  UINT Flags, 
  ID3DBlob ** ppDisassembly
) {
  out() << "D3DDisassemble10Effect\n";
  auto ret = D3DDisassemble10Effect(
    unwrap(pEffect), 
    Flags, 
    ppDisassembly
  );
  if (!ret && ppDisassembly && *ppDisassembly)
    *ppDisassembly = getWrapper<ID3DBlob, WrappedID3D10Blob>(*ppDisassembly);
  return ret;
}
