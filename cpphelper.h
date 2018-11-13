#pragma once
#include <vector>
#include <string>
#include <sstream>
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
#include <map>
#include <fstream>
#include <assert.h>
#include <mutex>
#include <atomic>
#include <thread>

char const *blobIn();

template<typename T>
T const *getInBlobPtr(size_t offset)
{
	if (!offset)
	{
		return nullptr;
	}
	auto blob = blobIn();
	return (T *)&blob[offset];
}

template<typename T>
T const &getInBlobRef(size_t offset)
{
	auto blob = blobIn();
	return *(T const *)&blob[offset];
}

std::unordered_map<size_t, size_t> &getInterfaceTable();

struct MapDesc
{
	char *pData;
	size_t size;
};

// pResource -> subresourceID -> MapDesc
std::map<std::pair<ID3D11Resource *, UINT>, MapDesc> &getMapTable();

template<typename T> T *getInterface(size_t index)
{
	if (!index)
	{
		return nullptr;
	}
	assert(getInterfaceTable().find(index) != getInterfaceTable().end());
	return (T*)getInterfaceTable().find(index)->second;
}

static void setInterface(size_t index, void *ptr)
{
	getInterfaceTable()[index] = (size_t)ptr;
}