def Ind(N):
	return " " * N
def countPtrs(txt):
	result = 0
	for char in txt:
		if char == "*":
			result += 1
	return result

"""
def writeCPP(ctx, Ty, base, Meth):
	print(Ind(2) + "out() << \"static void " + Meth.name + "_\" << getEventNumber() << \"() {\\n\";")
	UnwrapTable = {}
	def addTemp(type, name):
		if countPtrs(param.type) == 2 and param.annot in ["IN_ARRAY", "INOUT_ARRAY"]:
			print(Ind(2) +  "out() << \"  \" << \"" + param.undertype + " *tmp_" + param.name + "[0x80];\\n\"")
			print(Ind(2) + "for (uint32_t i = 0; i < " + param.number + "; i++) tmp_" + param.name + "[i] = getInBlobPtr<" +  + "(" + param.name + "[i]);")
			UnwrapTable[param.name] = "tmp_" + param.name + ""
		elif countPtrs(param.type) == 1 and param.annot in ["IN", "INOUT"]:
			print(Ind(2) + "auto unwrapped_" + param.name + " = unwrap("+ param.name + ");")
			UnwrapTable[param.name] = "unwrapped_" + param.name + ""
			print(Ind(2) + "type *tmp_" + name + ";\\n")
		else:
			print("" + str(countPtrs(param.type)) + " " + param.annot)
			assert(False) #unsupported

	def wrapParam(param):
		if "INOUT" in param.annot:
			
		elif "IN" in param.annot:
			if param.ptrsNum == 0:
				print(Ind(2) + "out() << \"  \" << \"getInBlobRef<" + param.undertype + ">(\" << serializeRef(" + param.name + ")")
			elif param.ptrsNum == 1:
				print(Ind(2) + "out() << \"  \" << \"getInBlobPtr<" + param.undertype + ">(\" << serializePtr(" + param.name + ")")
			else:
				assert(False) #unsupported
		else:
			print(Ind(2) + "out() << \"  \" << \"&tmp_" + param.name + "\" << \"\\n\";")
	


	print(Ind(2) + "out() << \"}\\n\";")
"""

def dumpMethod(ctx, Ty, base, Meth, declOnly):
	def dumpCPP():
		print(Ind(2) + "dumpMethodEvent((void*)this, \"" + base.name + "\", \"" + Meth.name + "\", {")
		for param in Meth.params:
			print(Ind(4) + "{\"" + param.name + "\", (void*)&" + param.name + "},")
		print(Ind(2) + "});")

	PN = len(Meth.params)
	wrapName = "m_p" + base.name
	if PN == 0:
		if declOnly:
			print(Ind(2) + Meth.retTy + " __stdcall " + Meth.name + "() override;")
		else:
			print(Ind(0) + Meth.retTy + " __stdcall Wrapped" + Ty.name + "::" + Meth.name + "() {")
			#print(Ind(2) + "out() << \"" + Ty.name + "(\" << " + wrapName + " << \")::" + Meth.name + "\\n\";")
			print(Ind(2) + "assert(" + wrapName + ");")
			######## RECURSION CHECK
			print(Ind(2) + "bool recursionFlag = false;")
			print(Ind(2) + "getRecursionFlag().compare_exchange_strong(recursionFlag, true);")
			########
			if Meth.retTy != "void":
				
				#print(Ind(2) + "out() << \"\\treturned \" << ret << \"\\n\";")

				##### special case for Release()
				if Meth.name == "AddRef":
					print(Ind(2) + "assert(" + "m_pMain);")
					print(Ind(2) + "auto ret = " + "m_pMain->" + Meth.name + "();")
					
					print(Ind(2) + "if (recursionFlag) {")
					print(Ind(4) + "return ret;")
					print(Ind(2) + "}")
					print(Ind(2) + "getRecursionFlag() = false;")
					print(Ind(2) + "out() << \"// returned \" << ret << \"\\n\";")
					dumpCPP()
					print(Ind(2) + "return ret;")
				elif Meth.name == "Release":
					print(Ind(2) + "assert(" + "m_pMain);")
					print(Ind(2) + "auto ret = " + "m_pMain->" + Meth.name + "();")
					print(Ind(2) + "if (recursionFlag) {")
					print(Ind(4) + "if (!ret) delete this;")
					print(Ind(4) + "return ret;")
					print(Ind(2) + "}")
					print(Ind(2) + "getRecursionFlag() = false;")
					print(Ind(2) + "out() << \"// returned \" << ret << \"\\n\";")
					dumpCPP()
					print(Ind(2) + "if (!ret) delete this;")
					print(Ind(2) + "return ret;")
				else:
					print(Ind(2) + "auto ret = " + wrapName + "->" + Meth.name + "();")
					######## RECURSION CHECK
					print(Ind(2) + "if (recursionFlag)")
					if Meth.retTy != "void":
						print(Ind(4) + "return ret;")
					else:
						print(Ind(4) + "return;")
					print(Ind(2) + "getRecursionFlag() = false;")
					########
					dumpCPP()
					print(Ind(2) + "return ret;")
			else:
				print(Ind(2) + "" + wrapName + "->" + Meth.name + "();")
				######## RECURSION CHECK
				print(Ind(2) + "if (recursionFlag)")
				if Meth.retTy != "void":
					print(Ind(4) + "return ret;")
				else:
					print(Ind(4) + "return;")
				print(Ind(2) + "getRecursionFlag() = false;")
				dumpCPP()
				########
			
			print(Ind(0) + "}")
	else:
		if declOnly:
			print(Ind(2) + Meth.retTy + " __stdcall " + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + Meth.params[Pi].type + " " + Meth.params[Pi].name + com)
			print(Ind(2) + ") override;")
		else:
			print(Ind(0) + Meth.retTy + " __stdcall Wrapped" + Ty.name + "::" + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(2) + Meth.params[Pi].type + " " + Meth.params[Pi].name + com)
			print(Ind(0) + ") {")
			print(Ind(2) + "assert(" + wrapName + ");")
			#print(Ind(2) + "out() << \"" + Ty.name + "(\" << " + wrapName + " << \")::" + Meth.name + "\\n\";")
			######## RECURSION CHECK
			print(Ind(2) + "bool recursionFlag = false;")
			print(Ind(2) + "getRecursionFlag().compare_exchange_strong(recursionFlag, true);")
			########
			####### PRE-PROCESS
			NumWrapped = ""
			#if Meth.name in ["QueryInterface"]:
				#NumWrapped = 
			# for scalar, ptr and array types there is different unwrap code
			UnwrapTable = {}
			for param in Meth.params:

				if countPtrs(param.type) == 2 and param.annot in ["IN_ARRAY", "INOUT_ARRAY"]:
					print(Ind(2) + param.type.split("*")[0].replace("const", "") + " *tmp_" + param.name + "[0x80];")
					print(Ind(2) + "for (uint32_t i = 0; i < " + param.number + "; i++) tmp_" + param.name + "[i] = unwrap(" + param.name + "[i]);")
					UnwrapTable[param.name] = "tmp_" + param.name + ""
				elif countPtrs(param.type) == 1 and param.annot in ["IN", "INOUT"]:
					print(Ind(2) + "auto unwrapped_" + param.name + " = unwrap("+ param.name + ");")
					UnwrapTable[param.name] = "unwrapped_" + param.name + ""
				else:
					UnwrapTable[param.name] = param.name

			####### CALL
			if Meth.retTy == "void":
				print(Ind(2) + "" + wrapName + "->" + Meth.name + "(")
			else:
				print(Ind(2) + "auto ret = " + wrapName + "->" + Meth.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + UnwrapTable[Meth.params[Pi].name] + com)
			print(Ind(2) + ");")
			######## RECURSION CHECK
			print(Ind(2) + "if (recursionFlag)")
			if Meth.retTy != "void":
				print(Ind(4) + "return ret;")
			else:
				print(Ind(4) + "return;")
			print(Ind(2) + "getRecursionFlag() = false;")
			########
			#if Meth.retTy != "void":
				#print(Ind(2) + "out() << \"\\treturned \" << ret << \"\\n\";")
			####### POST-PROCESS
			if Meth.name in ["GetDevice"]:
				print(Ind(2) + "if(*" + Meth.params[-1].name + ")")
				print(Ind(4) + "HandleWrap(__uuidof(ID3D11Device), (void**)" + Meth.params[-1].name + ");")
			elif Meth.name in ["GetResource"]:
				print(Ind(2) + "if(*" + Meth.params[-1].name + ")")
				print(Ind(4) + "HandleWrap(__uuidof(ID3D11Resource), (void**)" + Meth.params[-1].name + ");")
			elif Meth.name in ["OpenSharedResource", "OpenSharedFence", "CreateFence"]:
				print(Ind(2) + "if(*" + Meth.params[-1].name + ")")
				print(Ind(4) + "HandleWrap(ReturnedInterface, (void**)" + Meth.params[-1].name + ");")
			elif Meth.name in ["GetCoreWindow"]:
				print(Ind(2) + "if(*" + Meth.params[-1].name + ")")
				print(Ind(4) + "HandleWrap(refiid, (void**)" + Meth.params[-1].name + ");")
			elif Meth.name in ["OpenSharedResourceByName", "OpenSharedResource1"]:
				print(Ind(2) + "if(*" + Meth.params[-1].name + ")")
				print(Ind(4) + "HandleWrap(returnedInterface, (void**)" + Meth.params[-1].name + ");")
			elif Meth.name in ["GetParent",
			"GetBuffer",
			"EnumAdapterByLuid", "EnumWarpAdapter"]:
				print(Ind(2) + "{")
				print(Ind(4) + "if(!ret) {")
				print(Ind(6) + "HandleWrap(riid, " + Meth.params[-1].name + ");")
				print(Ind(4) + "}")
				print(Ind(2) + "}")
			elif Meth.name in ["QueryInterface"]:
				wrappedTypes = [Ty] + Ty.getBases(ctx, [])
				print(Ind(2) + "if(!ret) {")
				for base in wrappedTypes:
					print(Ind(4) + "if(riid == __uuidof(" + base.name + ")) {")
					print(Ind(6) + "*" + Meth.params[-1].name + " = (" + base.name + "*)this;")
					print(Ind(6) + "return ret;")
					print(Ind(4) + "}")
				print(Ind(2) + "}")
				print(Ind(2) + "{")
				print(Ind(4) + "if(!ret) {")
				print(Ind(6) + "HandleWrap(riid, " + Meth.params[-1].name + ");")
				print(Ind(4) + "}")
				print(Ind(2) + "}")
			elif Meth.name in ["GetDecoderBuffer"]:
				print(Ind(2) + "assert(false && \"Wrap not implemented; Emit Seg Fault\");")
			else:
				rescheck = "true"
				if Meth.retTy == "HRESULT":
					rescheck = "!ret"


				for param in Meth.params:
					if not "OUT" in param.annot:
						continue
					preloopcheck = ""
					paramNumberItem = param.number
					if param.number != "NOT_SET":
						if param.number in Meth.paramTable:
							numParam = Meth.paramTable[param.number]
							paramNumberItem = numParam.name
							if numParam.ptrsNum == 1:
								preloopcheck = "if (" + paramNumberItem + ")"
								paramNumberItem = "*" + paramNumberItem
						else:
							print(Ind(2) + "// Suspicious place")
					if countPtrs(param.type) == 2 and param.annot in ["INOUT_ARRAY"]:
						RawType = param.type.split(" ")[0]
						if RawType == "struct":
							RawType = param.type.split(" ")[1]
						if RawType in ctx.apiTypes.keys():
							print(Ind(2) +preloopcheck + "for (uint32_t i = 0; i < " + paramNumberItem + "; i++) if ("+rescheck+" && tmp_" + param.name + "[i]) " + param.name + "[i] = getWrapper<" + RawType +
							", Wrapped" + ctx.wrapTable[RawType].name + ">(tmp_" + param.name + "[i]);")
					if countPtrs(param.type) == 2 and param.annot in ["OUT_ARRAY"]:
						RawType = param.type.split(" ")[0]
						if RawType == "struct":
							RawType = param.type.split(" ")[1]
						if RawType in ctx.apiTypes.keys():
							print(Ind(2) + preloopcheck + "for (uint32_t i = 0; i < " + paramNumberItem + "; i++) if ("+rescheck+" && " + param.name + " && " + param.name + "[i]) " + param.name + "[i] = getWrapper<" + RawType +
							", Wrapped" + ctx.wrapTable[RawType].name + ">(" + param.name + "[i]);")
					elif countPtrs(param.type) == 2 and param.annot in ["OUT", "INOUT"]:
						RawType = param.type.split(" ")[0]
						if RawType == "struct":
							RawType = param.type.split(" ")[1]
						if RawType in ctx.apiTypes.keys():
							print(Ind(2) + "if ("+rescheck+" && " + param.name + " && *" + param.name  + " ) *" + param.name + " = getWrapper<" + RawType + ", Wrapped" + ctx.wrapTable[RawType].name + ">(*" + param.name + ");")
					else:
						pass
			""""
				for Pi in range(0, PN):
					if "**" in Meth.params[Pi].type:
						RawType = Meth.params[Pi].type.split(" ")[0]
						if RawType == "struct":
							RawType = Meth.params[Pi].type.split(" ")[1]
						print(Ind(2) + "if (*" + Meth.params[Pi].name + ")")
						#print(Ind(4) + "*" + Meth.params[Pi].name + " = new Wrapped" + RawType + "(*" + Meth.params[Pi].name + ");")
						print(Ind(4) + "*" + Meth.params[Pi].name + " = getWrapper<" + RawType +
						", Wrapped" + RawType + ">(*" + Meth.params[Pi].name + ");")
						"""

			dumpCPP()

			if Meth.retTy != "void":
				print(Ind(2) + "return ret;")
			print(Ind(0) + "}")
	
def genQueryImpl(ctx):
	print("""template<typename T> void HandleWrap(
  const IID & riid, 
  T ** ppvObject) {""")
	#print(Ind(2) + "HRESULT hr = S_OK;")
	for Ty in ctx.apiTypes.values():
		if Ty.name == "ID3DInclude":
			continue
		print(Ind(2) + "if(riid == __uuidof(" + Ty.name + ")) {")
		print(Ind(4) + "*ppvObject = (T*)getWrapper<" + Ty.name +
						", Wrapped" + ctx.wrapTable[Ty.name].name + ">((" + Ty.name + "*)*ppvObject);")
		print(Ind(4) + "return;")
		print(Ind(2) + "}")
	print(Ind(2) + "{")
	#print(Ind(4) + "out() << \"[WARNING] Unknown(\" << *ppvObject << \") riid:\" << riid << \"\\n\";")
	#print(Ind(4) + "return pObj->" + name + "(riid, ppvObject);")
	#print(Ind(4) + "assert(false && \"Wrap not implemented; Emit Seg Fault\");")
	print(Ind(2) + "}")
	#print(Ind(2) + "return S_OK;")
	print(Ind(0) + "}")

def genInterfaceTableInit(ctx):
	print("class TableInit {")
	print("public:")
	print(Ind(2) + "TableInit() {")
	for TyName, Ty in ctx.apiTypes.items():
		print(Ind(4) + "CLASS_BEGIN(\"" + Ty.name + "\");")
		for Meth in Ty.methods:
			print(Ind(4) + "METHOD_BEGIN(\"" + Meth.retTy + "\", \"" + Meth.name + "\");")
			for param in Meth.params:
				isInterface = "true"
				if not param.undertype in ctx.apiTypes:
					isInterface = "false"
				undersize = "sizeof(" + param.undertype + ")"
				if param.undertype == "void":
					undersize = "0"
				print(Ind(4) + "PARAM(\"" + param.type + "\", \"" + param.undertype +
				"\", \"" + param.name +
				"\", ParamAnnot::_" + param.annot + "_" +
				", " + str(param.ptrsNum) +
				", " + isInterface +
				", sizeof(" + param.type + ")" +
				", " + undersize + "" +
				", \"" + param.number + "\"" +
				");")
			print(Ind(4) + "METHOD_END(\"" + Meth.retTy + "\", \"" + Meth.name + "\");")
		print(Ind(4) + "CLASS_END(\"" + Ty.name + "\");")

	print(Ind(4) + "CLASS_BEGIN(\"GLOBAL\");")
	for FName, FTy in ctx.apiFuncs.items():
		print(Ind(4) + "METHOD_BEGIN(\"" + FTy.retTy + "\", \"" + FTy.name + "\");")
		for param in FTy.params:
			isInterface = "true"
			if not param.undertype in ctx.apiTypes:
				isInterface = "false"
			undersize = "sizeof(" + param.undertype + ")"
			if param.undertype == "void":
				undersize = "0"
			print(Ind(4) + "PARAM(\"" + param.type + "\", \"" + param.undertype +
			"\", \"" + param.name +
			"\", ParamAnnot::_" + param.annot + "_" +
			", " + str(param.ptrsNum) +
			", " + isInterface +
			", sizeof(" + param.type + ")" +
			", " + undersize + "" +
			", \"" + param.number + "\"" +
			");")
		print(Ind(4) + "METHOD_END(\"" + FTy.retTy + "\", \"" + FTy.name + "\");")
	print(Ind(4) + "CLASS_END(\"GLOBAL\");")
	print(Ind(2) + "}")
	print("} g_tableInit;")

def genWrappers(ctx):
	print("""#include <cppdumputils.h>""")

	derivFlags = set()
	for TyName, Ty in ctx.apiTypes.items():
		for base in Ty.getBases(ctx, []):
			derivFlags.add(base.name)

	for TyName, Ty in ctx.apiTypes.items():
		if not TyName in derivFlags:
			for base in Ty.getBases(ctx, []):
				ctx.wrapTable[base.name] = Ty
			ctx.dumpSet.append(Ty)
			if Ty.name == "ID3D10Blob":
				ctx.wrapTable["ID3DBlob"] = Ty
			ctx.wrapTable[Ty.name] = Ty

	genInterfaceTableInit(ctx)
	genQueryImpl(ctx)

	for Ty in ctx.dumpSet:
		TyName = Ty.name
		print("#if 0")
		Ty.dump(ctx)
		print("#endif")
		print("class Wrapped" + TyName + " : public " + TyName + " {")
		print("private:")
		wrappedTypes = [Ty] + Ty.getBases(ctx, [])
		for base in wrappedTypes:
			print(Ind(2) + base.name + " *m_p" + base.name + ";")
		if Ty.hasBase(ctx, "IUnknown"):
			print(Ind(2) + "IUnknown *m_pMain;")
		print("public:")
		#### CONSTRUCTOR
		print(Ind(2) + "template<typename T>")
		print(Ind(2) + "Wrapped" + TyName + "(T *pWrapped) {")
		print(Ind(4) + "out() << \"// [CREATE] " + TyName + "(0x\" << this << \", 0x\" << pWrapped << \" )\\n\";")
		print(Ind(4) + "assert(pWrapped);")
		#print(Ind(4) + "GLOBAL_LOCK;")
		print(Ind(4) + "auto &uwt = getUnwrapTable();")
		print(Ind(4) + "auto &wt = getWrapTable();")
		print(Ind(4) + "HRESULT res = S_OK;")

		for base in reversed(wrappedTypes):
			print(Ind(4) + "m_p" + base.name + " = nullptr;");
			if Ty.hasBase(ctx, "IUnknown"):
				print(Ind(4) + "res = pWrapped->QueryInterface(__uuidof(" + base.name + "), (void **)&m_p" + base.name + ");")
				print(Ind(4) + "if (!res) {")
				print(Ind(6) + "m_p" + base.name + "->Release();")
				print(Ind(6) + "uwt[reinterpret_cast<size_t>((void*)this)] = reinterpret_cast<size_t>((void*)m_p" + base.name + ");")
				print(Ind(6) + "wt[reinterpret_cast<size_t>((void*)m_p" + base.name + ")] = reinterpret_cast<size_t>((void*)this);")
				print(Ind(6) + "m_pMain = (IUnknown *)m_p" + base.name + ";")
				#print(Ind(6) + "out() << \"[MAP] \" << m_p" + base.name + " << \" -> \" << this << \"\\n\";")
				print(Ind(4) + "}")

			else:
				print(Ind(4) + "m_p" + base.name + " = (" + base.name + "*)pWrapped;")
		print(Ind(2) + "}")
		#### DESTRUCTOR
		print(Ind(2) + "~Wrapped" + TyName + "() {")
		print(Ind(4) + "out() << \"// [DESTROY] " + TyName + "(0x\" << this << \" )\\n\";")
		print(Ind(4) + "GLOBAL_LOCK;")
		print(Ind(4) + "auto &uwt = getUnwrapTable();")
		print(Ind(4) + "auto &wt = getWrapTable();")
		print(Ind(4) + "uwt.erase(reinterpret_cast<size_t>((void*)this));")
		for base in reversed(wrappedTypes):
			if Ty.hasBase(ctx, "IUnknown"):
				print(Ind(4) + "wt.erase(reinterpret_cast<size_t>((void*)m_p" + base.name + "));")
			print(Ind(4) + "m_p" + base.name + " = nullptr;");
		#if Ty.hasBase(ctx, "IUnknown"):
		#	print(Ind(4) + "for (int i = 0; i < 10; i++) " + wrapName + "->AddRef();")
		print(Ind(2) + "}")

		MethSet = {}
		for Meth in Ty.methods:
			MethSet[Meth.name] = (Ty, Meth)
		for Base in Ty.getBases(ctx, []):
			for Meth in Base.methods:
				MethSet[Meth.name] = (Base, Meth)
		for Name, Meth in MethSet.items():
			dumpMethod(ctx, Ty, Meth[0], Meth[1], True)
		print("};")
	print("typedef WrappedID3D10Blob WrappedID3DBlob;")
	for Ty in ctx.dumpSet:

		MethSet = {}
		for Meth in Ty.methods:
			MethSet[Meth.name] = (Ty, Meth)
		for Base in Ty.getBases(ctx, []):
			for Meth in Base.methods:
				MethSet[Meth.name] = (Base, Meth)
		for Name, Meth in MethSet.items():
			dumpMethod(ctx, Ty, Meth[0], Meth[1], False)	
		
		
	for FName, FTy in ctx.apiFuncs.items():
		def dumpCPP():
			print(Ind(2) + "dumpFunctionEvent(\"" + FTy.name + "\", {")
			for param in FTy.params:
				print(Ind(4) + "{\"" + param.name + "\", (void*)&" + param.name + "},")
			print(Ind(2) + "});")
		PN = len(FTy.params)
		if PN == 0:
			print(Ind(0) + FTy.retTy + " Wrapped" + FTy.name + "() {")
			#print(Ind(2) + "out() << \"" + FTy.name + "\\n\";")
			print(Ind(2) + "return " + FTy.name + "();")
			print(Ind(0) + "}")
		else:
			print(Ind(0) + FTy.retTy + " Wrapped" + FTy.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(2) + FTy.params[Pi].type + " " + FTy.params[Pi].name + com)
			print(Ind(0) + ") {")
			#print(Ind(2) + "out() << \"" + FTy.name + "\\n\";")
			######
			######## RECURSION CHECK
			print(Ind(2) + "bool recursionFlag = false;")
			print(Ind(2) + "getRecursionFlag().compare_exchange_strong(recursionFlag, true);")
			########
			NumWrapped = ""
			#if Meth.name in ["QueryInterface"]:
				#NumWrapped = 
			# for scalar, ptr and array types there is different unwrap code
			UnwrapTable = {}
			for param in FTy.params:
				if countPtrs(param.type) == 2 and param.annot in ["IN_ARRAY", "INOUT_ARRAY"]:
					print(Ind(2) + param.type.split("*")[0].replace("const", "") + " *tmp_" + param.name + "[32];")
					print(Ind(2) + "for (uint32_t i = 0; i < " + param.number + "; i++) tmp_" + param.name + "[i] = unwrap(" + param.name + "[i]);")
					UnwrapTable[param.name] = "tmp_" + param.name + ""
				elif countPtrs(param.type) == 1 and param.annot in ["IN", "INOUT"]:
					UnwrapTable[param.name] = "unwrap(" + param.name + ")"
				else:
					UnwrapTable[param.name] = param.name
			
			if FTy.retTy == "void":
				print(Ind(2) + "" + FTy.name + "(")
			else:
				print(Ind(2) + "auto ret = " + FTy.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + UnwrapTable[FTy.params[Pi].name] + com)
			print(Ind(2) + ");")
			######## RECURSION CHECK
			print(Ind(2) + "if (recursionFlag)")
			if FTy.retTy != "void":
				print(Ind(4) + "return ret;")
			else:
				print(Ind(4) + "return;")
			print(Ind(2) + "getRecursionFlag() = false;")
			########

			if FTy.name in ["D3DReflect"]:
				print(Ind(2) + "assert(false && \"Wrap not implemented; Emit Seg Fault\");")
			elif FTy.name in ["DXGIGetDebugInterface", "DXGIGetDebugInterface1",
			"CreateDXGIFactory2", "CreateDXGIFactory", "CreateDXGIFactory1"]:
				print(Ind(2) + "if(!ret)")
				print(Ind(4) + "HandleWrap(riid, (void**)" + FTy.params[-1].name + ");")
			else:
				for Pi in range(0, PN):
					if "**" in FTy.params[Pi].type:
						RawType = FTy.params[Pi].type.split(" ")[0]
						if RawType == "struct":
							RawType = FTy.params[Pi].type.split(" ")[1]
						print(Ind(2) + "if (!ret && " + FTy.params[Pi].name + " && *" + FTy.params[Pi].name + ")")
						print(Ind(4) + "*" + FTy.params[Pi].name + " = getWrapper<" + RawType +
						", Wrapped" + ctx.wrapTable[RawType].name + ">(*" + FTy.params[Pi].name + ");")
			
			dumpCPP()
			
			if FTy.retTy != "void":
				print(Ind(2) + "return ret;")
			print(Ind(0) + "}")
