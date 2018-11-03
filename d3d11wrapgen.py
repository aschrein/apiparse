def Ind(N):
	return " " * N

def dumpMethod(Meth):
	PN = len(Meth.params)
	if PN == 0:
		print(Ind(2) + Meth.retTy + " " + Meth.name + "() override {")
		print(Ind(4) + "return m_pWrapped->" + Meth.name + "();")
		print(Ind(2) + "}")
	else:
		print(Ind(2) + Meth.retTy + " " + Meth.name + "(")
		for Pi in range(0, PN):
			com = ", "
			if Pi == PN - 1:
				com = ""
			print(Ind(4) + Meth.params[Pi][0] + " " + Meth.params[Pi][1] + com)
		print(Ind(2) + ") override {")
		if Meth.retTy == "void":
			print(Ind(4) + "m_pWrapped->" + Meth.name + "(")
		else:
			print(Ind(4) + "auto ret = m_pWrapped->" + Meth.name + "(")
		for Pi in range(0, PN):
			com = ", "
			if Pi == PN - 1:
				com = ""
			print(Ind(6) + Meth.params[Pi][1] + com)
		print(Ind(4) + ");")
		if Meth.name in ["QueryInterface", "GetParent", "GetDevice",
		"GetBuffer", "GetDecoderBuffer", "OpenSharedResource"]:
			print(Ind(4) + "(void)*(int*)nullptr; // Wrap not implemented; Emit Seg Fault")
		else:
			for Pi in range(0, PN):
				if "**" in Meth.params[Pi][0]:
					RawType = Meth.params[Pi][0].split(" ")[0]
					print(Ind(4) + "if (*" + Meth.params[Pi][1] + ")")
					print(Ind(6) + "*" + Meth.params[Pi][1] + " = new Wrapped" + RawType + "(*" + Meth.params[Pi][1] + ");")
		if Meth.retTy != "void":
			print(Ind(4) + "return ret;")
		print(Ind(2) + "}")
	

def genWrappers(ctx):
	print("""#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11_3.h>
#include <d3d11_4.h>""")
	for TyName, Ty in ctx.apiTypes.items():
		print("#if 0")
		Ty.dump(ctx)
		print("#endif")
		print("class Wrapped" + TyName + " : public " + TyName + " {")
		print("private:")
		print(Ind(2) + TyName + " *m_pWrapped;")
		print("public:")
		print(Ind(2) + "Wrapped" + TyName + "(" + TyName + " *pWrapped) : m_pWrapped(pWrapped) {}")
		MethFull = {}
		for Meth in Ty.methods:
			#MethFull[Meth.name] = Meth
			dumpMethod(Meth)
		for Base in Ty.getBases(ctx, []):
			for Meth in Base.methods:
				print("//inherited from " + Base.name)
				#MethFull[Meth.name] = "/*inherited from " + Base.name + "*/" + Meth
				dumpMethod(Meth)
		#for Meth in MethFull.values():
			
		
		print("};")
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
			print(Ind(2) + "return " + FTy.name + "(")
			for Pi in range(0, PN):
				com = ", "
				if Pi == PN - 1:
					com = ""
				print(Ind(4) + FTy.params[Pi][1] + com)
			print(Ind(2) + ");")
			print(Ind(0) + "}")
