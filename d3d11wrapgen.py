def Ind(N):
	return " " * N

def dumpMethod(obj, Meth, declOnly):
	PN = len(Meth.params)
	if PN == 0:
		if declOnly:
			print(Ind(2) + Meth.retTy + " " + Meth.name + "() override;")
		else:
			print(Ind(0) + Meth.retTy + " Wrapped" + obj + "::" + Meth.name + "() {")
			print(Ind(2) + "std::cout << \"" + obj + "::" + Meth.name + "\\n\";")
			print(Ind(2) + "return m_pWrapped->" + Meth.name + "();")
			print(Ind(0) + "}")
	else:
		if declOnly:
			print(Ind(2) + Meth.retTy + " " + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + Meth.params[Pi][0] + " " + Meth.params[Pi][1] + com)
			print(Ind(2) + ") override;")
		else:
			print(Ind(0) + Meth.retTy + " Wrapped" + obj + "::" + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(2) + Meth.params[Pi][0] + " " + Meth.params[Pi][1] + com)
			print(Ind(0) + ") {")
			print(Ind(2) + "std::cout << \"" + obj + "::" + Meth.name + "\\n\";")
			if Meth.retTy == "void":
				print(Ind(2) + "m_pWrapped->" + Meth.name + "(")
			else:
				print(Ind(2) + "auto ret = m_pWrapped->" + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + Meth.params[Pi][1] + com)
			print(Ind(2) + ");")
			if Meth.name in ["QueryInterface", "GetParent", "GetDevice",
			"GetBuffer", "GetDecoderBuffer", "OpenSharedResource", "GetResource",
			"GetCoreWindow", "OpenSharedResource1", "OpenSharedResourceByName",
			"EnumAdapterByLuid", "EnumWarpAdapter", "OpenSharedFence", "CreateFence", "Map"]:
				print(Ind(2) + "(void)*(int*)nullptr; // Wrap not implemented; Emit Seg Fault")
			else:
				for Pi in range(0, PN):
					if "**" in Meth.params[Pi][0]:
						RawType = Meth.params[Pi][0].split(" ")[0]
						if RawType == "struct":
							RawType = Meth.params[Pi][0].split(" ")[1]
						print(Ind(2) + "if (*" + Meth.params[Pi][1] + ")")
						#print(Ind(4) + "*" + Meth.params[Pi][1] + " = new Wrapped" + RawType + "(*" + Meth.params[Pi][1] + ");")
						print(Ind(4) + "*" + Meth.params[Pi][1] + " = getWrapper<" + RawType +
						", Wrapped" + RawType + ">(*" + Meth.params[Pi][1] + ");")
			if Meth.retTy != "void":
				print(Ind(2) + "return ret;")
			print(Ind(0) + "}")
	

def genWrappers(ctx):
	print("""#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11_3.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <unordered_map>
#include <iostream>
static std::unordered_map<void*, void*> &getWrapTable()
{
  static std::unordered_map<void*, void*> table;
  return table;
}
template<typename T, typename WT>
WT *getWrapper(T *t)
{
  auto &wt = getWrapTable();
  auto fd = wt.find(t);
  if (fd == wt.end()) {
    auto *wrap = new WT(t);
    wt.emplace(t, wrap);
    return static_cast<WT*>(wrap);
  }
  else
  {
    return static_cast<WT*>((*fd).second);
  }
}""")
	for TyName, Ty in ctx.apiTypes.items():
		print("#if 0")
		Ty.dump(ctx)
		print("#endif")
		print("class Wrapped" + TyName + " : public " + TyName + " {")
		print("private:")
		print(Ind(2) + TyName + " *m_pWrapped;")
		print("public:")
		print(Ind(2) + "Wrapped" + TyName + "(" + TyName + " *pWrapped) : m_pWrapped(pWrapped) {}")

		MethSet = {}
		for Meth in Ty.methods:
			MethSet[Meth.name] = Meth
			#dumpMethod(TyName, Meth, True)
		for Base in Ty.getBases(ctx, []):
			for Meth in Base.methods:
				MethSet[Meth.name] = Meth
				#print("//inherited from " + Base.name)
				#dumpMethod(TyName, Meth, True)
		for Name, Meth in MethSet.items():
			dumpMethod(TyName, Meth, True)
		print("};")
	print("typedef WrappedID3D10Blob WrappedID3DBlob;")
	for TyName, Ty in ctx.apiTypes.items():

		MethSet = {}
		for Meth in Ty.methods:
			MethSet[Meth.name] = Meth
		for Base in Ty.getBases(ctx, []):
			for Meth in Base.methods:
				MethSet[Meth.name] = Meth
		for Name, Meth in MethSet.items():
			dumpMethod(TyName, Meth, False)	
		
		
	for FName, FTy in ctx.apiFuncs.items():
		PN = len(FTy.params)
		if PN == 0:
			print(Ind(0) + FTy.retTy + " Wrapped" + FTy.name + "() {")
			print(Ind(2) + "return " + FTy.name + "();")
			print(Ind(0) + "}")
		else:
			print(Ind(0) + FTy.retTy + " Wrapped" + FTy.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(2) + FTy.params[Pi][0] + " " + FTy.params[Pi][1] + com)
			print(Ind(0) + ") {")
			if FTy.retTy == "void":
				print(Ind(2) + "" + FTy.name + "(")
			else:
				print(Ind(2) + "auto ret = " + FTy.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + FTy.params[Pi][1] + com)
			print(Ind(2) + ");")
			if FTy.name in ["D3DReflect"]:
				print(Ind(2) + "(void)*(int*)nullptr; // Wrap not implemented; Emit Seg Fault")
			else:
				for Pi in range(0, PN):
					if "**" in FTy.params[Pi][0]:
						RawType = FTy.params[Pi][0].split(" ")[0]
						if RawType == "struct":
							RawType = FTy.params[Pi][0].split(" ")[1]
						print(Ind(2) + "if (*" + FTy.params[Pi][1] + ")")
						print(Ind(4) + "*" + FTy.params[Pi][1] + " = getWrapper<" + RawType +
						", Wrapped" + RawType + ">(*" + FTy.params[Pi][1] + ");")
			print(Ind(2) + "std::cout << \"" + FTy.name + "\\n\";")
			if FTy.retTy != "void":
				print(Ind(2) + "return ret;")
			print(Ind(0) + "}")
