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

static std::vector<unsigned char> &getBlob()
{
  static std::vector<unsigned char> blob(0x100);
  return blob;
}

static size_t getEventNumber()
{
	static size_t counter = 0u;
	return counter++;
}

template<typename T>
size_t serializePtr(T const *v)
{
	if (!v)
	{
		return 0u;
	}
  GLOBAL_LOCK;
  auto &blob = getBlob();
  size_t offset = blob.size();
  blob.resize(blob.size() + sizeof(*v));
  memcpy(&blob[offset], v, sizeof(*v));
  return offset;
}

template<typename T>
size_t serializeRef(T const &v)
{
	GLOBAL_LOCK;
	auto &blob = getBlob();
	size_t offset = blob.size();
	blob.resize(blob.size() + sizeof(v));
	memcpy(&blob[offset], &v, sizeof(v));
	return offset;
}

template<typename T>
T const *getInBlobPtr(size_t offset)
{
	if (!offset)
	{
		return nullptr;
	}
  GLOBAL_LOCK;
  auto &blob = getBlob();
  return (T const *)&blob[offset];
}

template<typename T>
T const &getInBlobRef(size_t offset)
{
	GLOBAL_LOCK;
	auto &blob = getBlob();
	return *(T const *)&blob[offset];
}

enum class ParamAnnot
{
	_IN_,
	_OUT_,
	_INOUT_,
	_IN_ARRAY_,
	_OUT_ARRAY_,
	_INOUT_ARRAY_
};

struct Param
{
	std::string type;
	std::string undertype;
	std::string name;
	ParamAnnot annot;
	int ptrs;
	bool isInterface;
	size_t size;
};

struct Method
{
	std::string retTy;
	std::string name;
	std::unordered_map<std::string, Param> params;
};

struct Interface
{
	std::string name;
	std::unordered_map<std::string, Method> methods;
};

static std::unordered_map<std::string, Interface> &getGlobalObjTable()
{
	static std::unordered_map<std::string, Interface> table;
	return table;
}

struct TableCTX
{
	Interface curInterface;
	Method curMethod;
};

static TableCTX &getGlobalTableCTX()
{
	static TableCTX ctx;
	return ctx;
}

static void CLASS_BEGIN(std::string name)
{
	auto &ctx = getGlobalTableCTX();
	ctx.curInterface = { name, {} };
}

static void CLASS_END(std::string name)
{
	auto &ctx = getGlobalTableCTX();
	assert(ctx.curInterface.name == name);
	getGlobalObjTable()[ctx.curInterface.name] = ctx.curInterface;
	ctx.curInterface = {};
}

static void METHOD_BEGIN(std::string retTy, std::string name)
{
	auto &ctx = getGlobalTableCTX();
	ctx.curMethod = { retTy, name,{} };
}

static void METHOD_END(std::string retTy, std::string name)
{
	auto &ctx = getGlobalTableCTX();
	assert(ctx.curMethod.name == name);
	ctx.curInterface.methods[name] = ctx.curMethod;
	ctx.curMethod = {};
}

static void FUNC_END(std::string name)
{
	auto &ctx = getGlobalTableCTX();
	assert(ctx.curMethod.name == name);
	ctx.curInterface.methods[name] = ctx.curMethod;
	ctx.curMethod = {};
}

static void PARAM(std::string type, std::string undertype, std::string name, ParamAnnot annot,
	int ptrs, bool inInterface, size_t size)
{
	auto &ctx = getGlobalTableCTX();
	ctx.curMethod.params[name] = {
		type, undertype, name, annot, ptrs, inInterface, size
	};
}

void printParamInit(std::stringstream &ss, Param const &param, void *pData)
{
	if (param.annot == ParamAnnot::_IN_)
	{
		if (param.ptrs == 0)
		{
			if (param.undertype == "UINT")
			{
				ss << "  " << param.type << " tmp_" << param.name << " = " << *(UINT*)pData << ";\n";
				return;
			}
			else if (param.undertype == "HWND")
			{
				ss << "  " << param.type << " tmp_" << param.name << " = " << *(HWND*)pData << ";\n";
				return;
			}
			assert(false && "unsopported");
		}
		else if (param.ptrs == 1)
		{

		}
		else if (param.ptrs == 2)
		{
			assert(false && "unsopported");
		}
	}
	else if (param.annot == ParamAnnot::_INOUT_)
	{
		assert(false && "unsopported");
		/*
		if (param.ptrs == 0)
		{
			assert(false && "unsopported");
		}
		else if (param.ptrs == 1)
		{
			if (param.undertype == "UINT")
			{
				if (pData && *(void**)pData)
				{
					auto ptr = (UINT*)pData;
					ss << "  " << param.undertype << " v_tmp_" << param.name << " = " <<  << ";\n";
					ss << "  " << param.undertype << " *tmp_" << param.name << " = " << " v_tmp_" << param.name << ";\n";
				}
				else
				{

				}
				return;
			}
			
		}
		else if (param.ptrs == 2)
		{
			assert(false && "unsopported");
		}
		*/
	}
	else if (param.annot == ParamAnnot::_IN_ARRAY_)
	{
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_OUT_)
	{
	}
	else if (param.annot == ParamAnnot::_OUT_ARRAY_)
	{
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_INOUT_ARRAY_)
	{
		assert(false && "unsopported");
	}
	/*
	if (param.annot == ParamAnnot::_IN_)
		{
			assert(param.ptrs == 0 || param.ptrs == 1);
			if (param.ptrs == 1)
			{
				ss << "  " << param.type << " tmp_" << param.name << " = getInBlobRef<" << param.type << ">(" << item.second << ");\n";
			}
			else
			{
				ss << "  " << param.type << " tmp_" << param.name << " = getInBlobRef<" << param.type << ">(" << item.second << ");\n";
			}
		}
		else if(param.annot == ParamAnnot::_INOUT_)
		{
			if (item.second)
			{
				if (param.ptrs == 2)
				{
					ss << "  " << param.type << " tmp_" << param.name << " = getInterface<" << param.undertype << ">(" << item.second << ");\n";
				}
				else if (param.ptrs == 1)
				{

				}
				else
				{
					assert(false && "Unsupported");
				}
			}
			else
				ss << "  " << param.type << " tmp_" << param.name << ";\n";
		}
	*/
}

void printParam(std::stringstream &ss, Param const &param, void *pData)
{
		ss << "  " << " tmp_" << param.name;
}

void printParamFinale(std::stringstream &ss, Param const &param, void *pData)
{
	if (param.annot == ParamAnnot::_INOUT_)
	{
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_OUT_)
	{
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_OUT_ARRAY_)
	{
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_INOUT_ARRAY_)
	{
		assert(false && "unsopported");
	}
}

void dumpMethodEvent(
	void *pThis,
	std::string objName,
	std::string methName,
	std::unordered_map<std::string, void *> const &paramValues
) {
	GLOBAL_LOCK;
	auto &g_classTable = getGlobalObjTable();
	auto iter = g_classTable.find(objName);
	assert(iter != g_classTable.end());
	Interface const &obj = iter->second;
	auto miter = obj.methods.find(methName);
	assert(miter != obj.methods.end());
	Method const &method = miter->second;

	std::stringstream ss;
	ss << "static void " << method.name << "_" << getEventNumber() << "() {\n";
	assert(method.params.size() == paramValues.size());
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamInit(ss, iter->second, item.second);
	}
	ss << "  " << "getInterface<" << obj.name <<">(" << pThis << ")->" << method.name << "(\n";
	int i = 0;
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParam(ss, iter->second, item.second);
		if (i == method.params.size() - 1)
		{
			ss << "\n";
		}
		else
		{
			ss << ", \n";
		}
	}
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamFinale(ss, iter->second, item.second);
	}
	/*out() << "  " << "getInBlobPtr<DXGI_SURFACE_DESC>(" << serializePtr(pDesc) << "), \n";
	out() << "  " << "getInBlobRef<UINT>(" << serializeRef(NumSurfaces) << ", \n";
	out() << "  " << "getInBlobRef<DXGI_USAGE>(" << serializeRef(Usage) << ", \n";
	out() << "  " << "getInBlobPtr<DXGI_SHARED_RESOURCE>(" << serializePtr(pSharedResource) << ", \n";
	out() << "  " << "&tmp_ppSurface" << "\n";*/
	ss << "  " << ");\n";

	//out() << "  " << "setInterface(" << *ppSurface << ", &tmp_ppSurface" << ");\n";
	ss << "}\n";
	out() << ss.str();
}

void dumpFunctionEvent(
	Method const &method,
	std::unordered_map<std::string, void *> const &paramValues
) {
	std::stringstream ss;
	ss << "static void " << method.name << "_" << getEventNumber() << "() {\n";
	assert(method.params.size() == paramValues.size());
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamInit(ss, iter->second, item.second);
	}
	ss << "  " << method.name << "(\n";
	int i = 0;
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParam(ss, iter->second, item.second);
		if (i == method.params.size() - 1)
		{
			ss << "\n";
		}
		else
		{
			ss << ", \n";
		}
	}
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamFinale(ss, iter->second, item.second);
	}
	ss << "  " << ");\n";
	ss << "}\n";
	out() << ss.str();
}