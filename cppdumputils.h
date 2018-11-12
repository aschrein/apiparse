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
#include <atomic>
#include <thread>

static std::ofstream &out()
{
	static bool init = false;
  static std::ofstream file("dump.cpp");
	if (!init)
	{
		init = true;
		file << "#include <cppdumputils.h>\n";
		file << "extern HWND g_window;\n";
		file << "#include <decls.inc>\n";
		file << "typedef void(*Event)(void);\n";
		file << "std::vector<std::pair<std::string, Event>> g_events = {\n";
		file << "#include <table.inc>\n";
		file << "};\n";
	}
  file.setf(std::ios::unitbuf);
  return file;
}

static std::ofstream &declOut()
{
	static std::ofstream file("decls.inc", std::ios_base::out | std::ios_base::binary);
	file.setf(std::ios::unitbuf);
	return file;
}

static std::ofstream &tableOut()
{
	static std::ofstream file("table.inc", std::ios_base::out | std::ios_base::binary);
	file.setf(std::ios::unitbuf);
	return file;
}

static std::ofstream &blobOut()
{
	static std::ofstream file("blob", std::ios_base::out | std::ios_base::binary);
	file.setf(std::ios::unitbuf);
	return file;
}

static size_t &blobCounter()
{
	static size_t counter = 0;
	return counter;
}

static std::vector<char> const &blobIn()
{
	static bool init = false;
	static std::vector<char> blob;
	if (!init)
	{
		std::ifstream file("blob", std::ios_base::in | std::ios_base::binary | std::ios::ate);
		assert(file.is_open());
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		blob = std::vector<char>(size);
		assert(file.read(blob.data(), size));
	}
	return blob;
}

std::mutex &getGlobalLock()
{
  static std::mutex m;
  return m;
}

#define GLOBAL_LOCK std::lock_guard<std::mutex> GLOBAL_LOCK_VAR(getGlobalLock())

std::atomic<bool> &getRecursionFlag(std::thread::id tid = std::this_thread::get_id())
{
	static std::unordered_map<std::thread::id, std::atomic<bool>> m;
	// could this be a race condtion? neeh
	// when shit hits the fan this gotta be fixed
	return m[tid];
}

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
	bool expectedRecursionFlag = false;
	if (getRecursionFlag())
	{
		out() << "// getWrapper() recursion escape\n";
		return t;
	}
  GLOBAL_LOCK;
  auto &wt = getWrapTable();
  auto fd = wt.find(reinterpret_cast<size_t>((void*)t));
  if (fd == wt.end()) {
		out() << "// getWrapper() table miss\n";
    auto *wrap = new WT(t);
    return reinterpret_cast<T*>(wrap);
  }
  else
  {
		out() << "// getWrapper() table hit\n";
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

static std::unordered_map<size_t, size_t> &getInterfaceTable()
{
	static std::unordered_map<size_t, size_t> table;
	return table;
}

template<typename T> T *getInterface(size_t index)
{
	assert(getInterfaceTable().find(index) != getInterfaceTable().end());
	return (T*)getInterfaceTable().find(index)->second;
}

static void setInterface(size_t index, void *ptr)
{
	getInterfaceTable()[index] = (size_t)ptr;
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
	size_t offset = blobCounter();
	blobCounter() += sizeof(*v);
	blobOut().write((char*)v, sizeof(*v));
	return offset;
}

static size_t serializePtr(void const *v, size_t size)
{
	if (!v)
	{
		return 0u;
	}
	size_t offset = blobCounter();
	blobCounter() += size;
	blobOut().write((char*)v, size);
	return offset;
}

template<typename T>
size_t serializeRef(T const &v)
{
	size_t offset = blobCounter();
	blobCounter() += sizeof(v);
	blobOut().write((char*)&v, sizeof(v));
	return offset;
}

template<typename T>
T *getInBlobPtr(size_t offset)
{
	if (!offset)
	{
		return nullptr;
	}
  auto &blob = blobIn();
  return (T *)&blob[offset];
}

template<typename T>
T const &getInBlobRef(size_t offset)
{
	auto &blob = blobIn();
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
	size_t undersize;
	std::string numparam;
};

struct Method
{
	std::string retTy;
	std::string name;
	std::unordered_map<std::string, Param> params;
	std::vector<std::string> paramsOrdered;
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

static void PARAM(std::string type, std::string undertype, std::string name, ParamAnnot annot,
	int ptrs, bool inInterface, size_t size, size_t undersize, std::string numparam)
{
	auto &ctx = getGlobalTableCTX();
	ctx.curMethod.params[name] = {
		type, undertype, name, annot, ptrs, inInterface, size, undersize, numparam
	};
	ctx.curMethod.paramsOrdered.push_back(name);
}

#define __INDENT__ "/*" << __LINE__ << "*/"

void printParamInit(std::stringstream &ss, Method const &method,
	std::unordered_map<std::string, void *> const &paramValues, std::string paramName)
{
	auto const &param = method.params.find(paramName)->second;
#if 0
	if (param.annot != ParamAnnot::_IN_)
		return;
#endif
	auto pData = paramValues.find(paramName)->second;
	if (param.annot == ParamAnnot::_IN_)
	{
#if 1
#if 1
		if (param.undertype == "D3D11_SUBRESOURCE_DATA")
		{
			if (method.name == "CreateBuffer")
			{
				D3D11_SUBRESOURCE_DATA *pSubres = *(D3D11_SUBRESOURCE_DATA**)pData;
				D3D11_BUFFER_DESC *desc = *(D3D11_BUFFER_DESC**)paramValues.find("pDesc")->second;
				if (desc && pSubres)
				{
					int num = desc->ByteWidth;
					size_t hndl = serializePtr(pSubres, sizeof(*pSubres));
					ss << __INDENT__ << param.undertype << " *tmp_" << param.name
						<< " = getInBlobPtr<" << param.undertype << ">(" << hndl << ");\n";
					size_t memhndl = serializePtr(pSubres->pSysMem, num);
					ss << __INDENT__ << "tmp_" << param.name
						<< "->pSysMem = getInBlobPtr<void>(" << memhndl << ");\n";
				}
				else
				{
					ss << __INDENT__ << param.type << " tmp_" << param.name
						<< " = nullptr;\n";
				}
				return;
			}
			else
			{
				assert(false && "unsopported");
			}
		}
#endif
		if (param.ptrs == 0)
		{
#if 1
			if (param.undertype == "UINT")
			{
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = " << *(UINT*)pData << ";\n";
				return;
			}
			else if (param.undertype == "HWND")
			{
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = g_window;//(HWND)0x" << *(HWND*)pData << ";\n";
				return;
			}
			else if (param.undertype == "GUID" || param.undertype == "IID")
			{
				auto guid = *(GUID*)pData;
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = {"
					<< guid.Data1
					<< ", " << guid.Data2
					<< ", " << guid.Data3
					<< ", {" << (int)guid.Data4[0]
					<< ", " << (int)guid.Data4[1]
					<< ", " << (int)guid.Data4[2]
					<< ", " << (int)guid.Data4[3]
					<< ", " << (int)guid.Data4[4]
					<< ", " << (int)guid.Data4[5]
					<< ", " << (int)guid.Data4[6]
					<< ", " << (int)guid.Data4[7]
					<< "}};\n";
				return;
			}
			size_t hndl = serializePtr(pData, param.size);
			ss << __INDENT__ << param.type << " tmp_" << param.name << " = getInBlobRef<" << param.undertype << ">(" << hndl << ");\n";
			//assert(false && "unsopported");
#endif
		}
		else
#endif
#if 1
			if (param.ptrs == 1)
		{
			if (*(void**)pData)
			{
				if (param.isInterface)
				{
#if 1
					ss << __INDENT__ << param.type << " tmp_" << param.name
						<< " = getInterface<" << param.undertype << ">(0x" << *(void**)pData << ");\n";
#endif
				}
				else
				{
#if 1
					size_t hndl = serializePtr(*(void**)pData, param.size);
					ss << __INDENT__ << param.type << " tmp_" << param.name
						<< " = getInBlobPtr<" << param.undertype << ">(" << hndl << ");\n";
#endif
				}
			}
			else
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = nullptr;\n";
		}
		else
#endif
			if (param.ptrs == 2)
		{
			assert(false && "unsopported");
		}
	}
	else if (param.annot == ParamAnnot::_INOUT_)
	{
		assert(param.ptrs == 1);
		// we don't care if it wasn't provided
		if (!*(void**)pData)
		{
			ss << __INDENT__ << param.type << " tmp_" << param.name
				<< " = nullptr;\n";
			return;
		}
		// we don't care about non interfaces either
		if (!param.isInterface)
		{
			size_t hndl = serializePtr(*(void**)pData, param.undersize);
			ss << __INDENT__ << param.type << " tmp_" << param.name
				<< " = getInBlobPtr<" << param.undertype << ">(" << hndl << ");\n";
			return;
		}
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
					ss << __INDENT__ << param.undertype << " v_tmp_" << param.name << " = " <<  << ";\n";
					ss << __INDENT__ << param.undertype << " *tmp_" << param.name << " = " << " v_tmp_" << param.name << ";\n";
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
		if (!*(void**)pData)
		{
			ss << __INDENT__ << param.type << " tmp_" << param.name
				<< " = nullptr;\n";
			return;
		}
		/* special case for
		D3D11_SUBRESOURCE_DATA
    D3D11_MAPPED_SUBRESOURCE
		*/
		if (param.undertype == "D3D11_SUBRESOURCE_DATA")
		{
			D3D11_SUBRESOURCE_DATA *pSubres = *(D3D11_SUBRESOURCE_DATA**)pData;
			if (method.name == "CreateTexture1D")
			{
				assert(false && "unsopported");
			}
			else if (method.name == "CreateTexture2D")
			{
				D3D11_TEXTURE2D_DESC *desc = *(D3D11_TEXTURE2D_DESC**)paramValues.find("pDesc")->second;
				int num = desc->ArraySize * desc->MipLevels;
				size_t hndl = serializePtr(pSubres, num * sizeof(*pSubres));
				ss << __INDENT__ << param.undertype << " *tmp_" << param.name
					<< " = getInBlobPtr<" << param.undertype << ">(" << hndl << ");\n";
				assert(desc->ArraySize * desc->MipLevels);
				//ss << __INDENT__ << "for (int i = 0; i < tmp_pDesc->ArraySize * tmp_pDesc->MipLevels; i++)\n";
				for (int i = 0; i < desc->ArraySize; i++)
				{
					for (int j = 0; j < desc->MipLevels; j++)
					{
						int subresId = i * desc->MipLevels + j;
						size_t memhndl = serializePtr(pSubres[subresId].pSysMem, pSubres[subresId].SysMemSlicePitch / (1u << (2u*j)));
						ss << __INDENT__ << "tmp_" << param.name
							<< "[" << subresId << "].pSysMem = getInBlobPtr<void>(" << memhndl << ");\n";
					}
				}
			}
			else if (method.name == "CreateTexture3D")
			{
				assert(false && "unsopported");
			}
			else
			{
				assert(false);
			}
			
			return;

		}
		auto numparam = method.params.find(param.numparam)->second;
		assert(numparam.ptrs == 0 && (
			numparam.undertype == "UINT"

			|| numparam.undertype == "SIZE_T"
			));
		auto num = *(UINT*)paramValues.find(param.numparam)->second;
		if (param.isInterface)
		{
			assert(param.ptrs == 2);
			if (num)
			{
				ss << __INDENT__ << param.undertype << " *tmp_" << param.name
					<< "[] = {\n";
				void **pArr = *(void***)pData;
				for (int i = 0; i < num; i++)
				{
					ss << __INDENT__ << "getInterface<" << param.undertype << ">(0x" << pArr[i] << "), \n";
				}
				ss << __INDENT__ << "};\n";
			}
			else
			{
				ss << __INDENT__ << param.type << " tmp_" << param.name
					<< " = nullptr;\n";
			}
			return;
		}
		assert(param.ptrs == 1);
		
		char *pBase = *(char**)pData;
		if (param.undersize)
		{
			if (num)
			{
				ss << __INDENT__ << param.undertype << " tmp_" << param.name << "[" << num << "] = {\n";
				for (int i = 0; i < num; i++)
				{
					size_t hndl = serializePtr(pBase + i * param.undersize, param.size);
					ss << __INDENT__ << "getInBlobRef<" << param.undertype << ">(" << hndl << "),\n";
				}
				ss << "};\n";
			}
			else
			{
				ss << __INDENT__ << param.type << " tmp_" << param.name
					<< " = nullptr;\n";
			}
		}
		else
		{
			assert(param.undertype == "void");
			size_t hndl = serializePtr(pBase, param.size);
			ss << __INDENT__ << param.undertype << " *tmp_" << param.name << " = getInBlobPtr<void>(" << hndl << ");\n";
		}
		//assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_OUT_)
	{
		if (param.undertype == "D3D11_MAPPED_SUBRESOURCE")
		{
			assert(false && "unsopported");
		}
		if (!*(void**)pData)
		{
			ss << __INDENT__ << param.type << " tmp_" << param.name
				<< " = nullptr;\n";
			return;
		}
		if (param.ptrs == 0)
		{
			assert(false && "unsopported");
		}
		else if (param.ptrs == 1)
		{
			//assert(false && "unsopported");
			ss << __INDENT__ << param.undertype << " tmp_" << param.name << ";\n";
		}
		else if (param.ptrs == 2)
		{
			ss << __INDENT__ << param.undertype << " *tmp_" << param.name << " = nullptr;\n";
		}
	}
	else if (param.annot == ParamAnnot::_OUT_ARRAY_)
	{
		ss << __INDENT__ << param.undertype << " *tmp_" << param.name
			<< "[0x80] = {};\n";
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
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = getInBlobRef<" << param.type << ">(" << item.second << ");\n";
			}
			else
			{
				ss << __INDENT__ << param.type << " tmp_" << param.name << " = getInBlobRef<" << param.type << ">(" << item.second << ");\n";
			}
		}
		else if(param.annot == ParamAnnot::_INOUT_)
		{
			if (item.second)
			{
				if (param.ptrs == 2)
				{
					ss << __INDENT__ << param.type << " tmp_" << param.name << " = getInterface<" << param.undertype << ">(" << item.second << ");\n";
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
				ss << __INDENT__ << param.type << " tmp_" << param.name << ";\n";
		}
	*/
}

void printParam(std::stringstream &ss, Param const &param, void *pData)
{
	if (param.ptrs == 2)
	{
		if (param.annot == ParamAnnot::_INOUT_ARRAY_
			|| param.annot == ParamAnnot::_IN_ARRAY_
			)
			ss << __INDENT__ << " tmp_" << param.name;
		else
			ss << __INDENT__ << " &tmp_" << param.name;
	}
	else if (param.ptrs == 1)
	{
		if (param.annot == ParamAnnot::_OUT_)
			ss << __INDENT__ << " &tmp_" << param.name;
		else
			ss << __INDENT__ << " tmp_" << param.name;
	}
	 else
	{
		ss << __INDENT__ << " tmp_" << param.name;
	}
	
}

void printParamFinale(std::stringstream &ss, Method const &method,
	std::unordered_map<std::string, void *> const &paramValues, std::string paramName)
{
	auto const &param = method.params.find(paramName)->second;
	auto pData = paramValues.find(paramName)->second;
	if (param.annot == ParamAnnot::_INOUT_)
	{
		// we don't care about non interfaces being returned
		if (!param.isInterface)
		{
			return;
		}
		assert(false && "unsopported");
	}
	else if (param.annot == ParamAnnot::_OUT_)
	{
		if (param.undertype == "void" && param.ptrs ==2)
		{
			if (!*(void***)pData)
			{
				ss << __INDENT__ << "// " << param.name << " was nullptr\n";
				return;
			}
			ss << __INDENT__ << "setInterface(0x" << **(void***)pData << ", tmp_" << param.name << ");\n";
			return;
		}
		// we don't care about non interfaces being returned
		if (!param.isInterface)
		{
			return;
		}
		assert(param.ptrs == 2);
		assert(param.isInterface);
		ss << __INDENT__ << "setInterface(0x" << **(void***)pData << ", tmp_" << param.name << ");\n";
	}
	else if (param.annot == ParamAnnot::_OUT_ARRAY_)
	{
		// we don't care about non interfaces being returned
		if (!param.isInterface)
		{
			return;
		}
		assert(param.ptrs == 2);
		//assert(false && "unsopported");
		// sometimes Get* methods return arrays of crap, ok let's handle this
		if (!*(void**)pData)
		{
			return;
		}
		auto numparam = method.params.find(param.numparam)->second;
		// It could've been size_t but we assume it's little endian and num < 2^31
		int num = 0;
		assert(
			numparam.undertype == "UINT"
			|| numparam.undertype == "SIZE_T"
		);
		void *pNumData = paramValues.find(param.numparam)->second;
		if (numparam.ptrs == 0)
		{
			num = *(UINT*)pNumData;
		}
		else
		{
			if (!**(UINT**)pNumData)
			{
				return;
			}
			// If it's an out array of interfaces oops
			assert(false);
		}
		
		
		//ss << __INDENT__ <<  "for (int i = 0; i < tmp_" << numparam.name << "; i++)\n";
		void **pArr = *(void***)pData;
		for (int i = 0; i < num; i++)
		{
			ss << __INDENT__ << "setInterface(0x" << pArr[0] << ", tmp_" << param.name << "[" << i << "]);\n";
		}
		
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
	std::string funcName = method.name + "_" + std::to_string(getEventNumber());
	
	ss << "static void " << funcName << "() {\n";
	assert(method.params.size() == paramValues.size());
#if 1
	for (auto const &item : paramValues)
	{
		assert(item.second && "pointer to the argument mustn't be null!");
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamInit(ss, method, paramValues, item.first);
	}
#endif
	ss << __INDENT__ << "getInterface<" << obj.name <<">(0x" << pThis << ")->" << method.name << "(\n";
	int i = 0;
	for (auto const &paramName : method.paramsOrdered)
	{
		auto iter = method.params.find(paramName);
		printParam(ss, iter->second, paramValues.find(paramName)->second);
		if (i == method.params.size() - 1)
		{
			ss << "\n";
		}
		else
		{
			ss << ", \n";
		}
		i++;
	}
	ss << __INDENT__ << ");\n";
#if 1
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamFinale(ss, method, paramValues, item.first);
	}
#endif
	/*out() << "  " << "getInBlobPtr<DXGI_SURFACE_DESC>(" << serializePtr(pDesc) << "), \n";
	out() << "  " << "getInBlobRef<UINT>(" << serializeRef(NumSurfaces) << ", \n";
	out() << "  " << "getInBlobRef<DXGI_USAGE>(" << serializeRef(Usage) << ", \n";
	out() << "  " << "getInBlobPtr<DXGI_SHARED_RESOURCE>(" << serializePtr(pSharedResource) << ", \n";
	out() << "  " << "&tmp_ppSurface" << "\n";*/
	

	//out() << "  " << "setInterface(" << *ppSurface << ", &tmp_ppSurface" << ");\n";
	ss << "}\n";
	tableOut() << "{\"" << funcName << "\", " << funcName << "},\n";
	declOut() << "static void " << funcName << "();\n";
	out() << ss.str();
}

void dumpFunctionEvent(
	std::string methName,
	std::unordered_map<std::string, void *> const &paramValues
) {
	GLOBAL_LOCK;
	auto &g_classTable = getGlobalObjTable();
	auto iter = g_classTable.find("GLOBAL");
	assert(iter != g_classTable.end());
	Interface const &obj = iter->second;
	auto miter = obj.methods.find(methName);
	assert(miter != obj.methods.end());
	Method const &method = miter->second;

	std::stringstream ss;
	std::string funcName = method.name + "_" + std::to_string(getEventNumber());
	
	ss << "static void " << funcName << "() {\n";
	assert(method.params.size() == paramValues.size());
	for (auto const &item : paramValues)
	{
		assert(item.second && "pointer to the argument mustn't be null!");
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamInit(ss, method, paramValues, item.first);
	}
	ss << __INDENT__ << method.name << "(\n";
	int i = 0;
	for (auto const &paramName : method.paramsOrdered)
	{
		auto iter = method.params.find(paramName);
		printParam(ss, iter->second, paramValues.find(paramName)->second);
		if (i == method.params.size() - 1)
		{
			ss << "\n";
		}
		else
		{
			ss << ", \n";
		}
		i++;
	}
	ss << __INDENT__ << ");\n";
	for (auto const &item : paramValues)
	{
		auto iter = method.params.find(item.first);
		assert(iter != method.params.end());
		printParamFinale(ss, method, paramValues, item.first);
	}
	ss << "}\n";
	tableOut() << "{\"" << funcName << "\", " << funcName << "},\n";
	declOut() << "static void " << funcName << "();\n";
	out() << ss.str();
}