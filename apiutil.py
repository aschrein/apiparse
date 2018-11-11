class APIType:
	def __init__(self, name):
		self.name = name
		self.bases = []
		self.methods = []
	def getBases(self, ctx, out):
		for baseName in self.bases:
			#assert(baseName in ctx.apiTypes.keys())
			out.append(ctx.apiTypes[baseName])
			ctx.apiTypes[baseName].getBases(ctx, out)
		return out
	def hasBase(self, ctx, base):
		for baseName in self.bases:
			if baseName == base:
				return True;
			if ctx.apiTypes[baseName].hasBase(ctx, base):
				return True
		return False
	def dump(self, ctx):
		print("begin class " + self.name)
		print("inherits:")
		for base in self.getBases(ctx, []):
			print("  " + base.name)
		print("methods:")
		for meth in self.methods:
			print("  " + meth.name)
		print("end class")

class APIFunc:
	def __init__(self, name):
		self.name = name
		self.retTy = None
		self.params = []

import re

class APIParam:
	def countPtrs(txt):
		result = 0
		for char in txt:
			if char == "*":
				result += 1
		return result
	def __init__(self, annot, type, name):
		self.name = name
		self.annot = "IN"
		self.number = "NOT_SET"
		self.type = type
		self.undertype = type.replace("const", "").replace("*", "").replace(" ", "")
		self.ptrsNum = countPtrs(self.type)

		if "_Inout_" in annot:
			self.annot = "INOUT"
		elif "_Out_writes_all_opt_" in annot:
			m = re.search(r'_Out_writes_all_opt_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Out_writes_bytes_opt_" in annot:
			m = re.search(r'_Out_writes_bytes_opt_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Out_writes_bytes_" in annot:
			m = re.search(r'_Out_writes_bytes_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Out_writes_opt_" in annot:
			m = re.search(r'_Out_writes_opt_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Out_writes_all_" in annot:
			m = re.search(r'_Out_writes_all_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Out_writes_" in annot:
			m = re.search(r'_Out_writes_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_Outptr_result_bytebuffer_" in annot:
			m = re.search(r'_Outptr_result_bytebuffer_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "OUT_ARRAY"
		elif "_COM_Outptr_" in annot:
			self.annot = "OUT"
		elif "[iid_is][out]" in annot:
			self.annot = "OUT"
		elif "_COM_Outptr_opt_" in annot:
			self.annot = "OUT"
		elif "_Outptr_" in annot:
			self.annot = "OUT"
		elif "_In_reads_bytes_opt_" in annot:
			m = re.search(r'_In_reads_bytes_opt_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "IN_ARRAY"
		elif "_In_reads_bytes_" in annot:
			m = re.search(r'_In_reads_bytes_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "IN_ARRAY"
		elif "_In_reads_opt_" in annot:
			m = re.search(r'_In_reads_opt_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "IN_ARRAY"
		elif "_In_reads_" in annot:
			m = re.search(r'_In_reads_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "IN_ARRAY"
		elif "_Inout_opt_bytecount_" in annot:
			m = re.search(r'_Inout_opt_bytecount_\((.*)\)', annot)
			self.number = m.group(1)
			self.annot = "INOUT_ARRAY"
		elif "_Outptr_opt_result_maybenull_" in annot:
			self.annot = "OUT"
		elif "_Out_" in annot:
			self.annot = "OUT"

class APIStruct:
	def __init__(self, name):
		self.name = name
		self.fields = []

class APIField:
	def __init__(self, type, name):
		self.type = type
		self.name = name

class APIEnum:
	def __init__(self, name):
		self.name = name
		self.fields = []

class APIEnumField:
	def __init__(self, name, val):
		self.val = val
		self.name = name

class APIContext:
	def __init__(self):
		self.apiTypes = {}
		self.apiStructs = {}
		self.apiEnums = {}
		self.apiFuncs = {}
		self.curType = None
		self.curStruct = None
		self.curEnum = None
		self.curFunc = None
		self.dumpSet = []
		self.wrapTable = {}

def STRUCT_BEGIN(ctx, name):
	assert(ctx.curStruct == None)
	ctx.curStruct = APIStruct(name)

def ADD_FIELD(ctx, type, name):
	assert(ctx.curStruct != None)
	ctx.curStruct.fields.append(APIField(type, name))

def STRUCT_END(ctx, name):
	assert(ctx.curStruct != None)
	ctx.apiStructs[ctx.curStruct.name] = ctx.curStruct
	ctx.curStruct = None

def ENUM_BEGIN(ctx, name):
	assert(ctx.curEnum == None)
	ctx.curEnum = APIEnum(name)

def ADD_ENUM_FIELD(ctx, name, val):
	assert(ctx.curEnum != None)
	ctx.curEnum.fields.append(APIEnumField(name, val))

def ENUM_END(ctx, name):
	assert(ctx.curEnum != None)
	ctx.apiEnums[ctx.curEnum.name] = ctx.curEnum
	ctx.curEnum = None

def CLASS_BEGIN(ctx, name):
	assert(ctx.curType == None)
	ctx.curType = APIType( name)

def CLASS_END(ctx, name):
	assert(ctx.curType != None)
	ctx.apiTypes[ctx.curType.name] = ctx.curType
	ctx.curType = None

def INHERITS(ctx, name):
	assert(ctx.curType != None)
	ctx.curType.bases.append(name)

def FUNC_BEGIN(ctx, name):
	assert(ctx.curFunc == None)
	ctx.curFunc = APIFunc(name)

def RETURN_TYPE(ctx, name):
	assert(ctx.curFunc != None)
	ctx.curFunc.retTy = name

def PARAM(ctx, annot, type, name):
	assert(ctx.curFunc != None)
	ctx.curFunc.params.append(APIParam(annot, type, name))

def FUNC_END(ctx, name):
	assert(ctx.curFunc != None)
	if ctx.curType != None:
		ctx.curType.methods.append(ctx.curFunc)
	else:
		ctx.apiFuncs[ctx.curFunc.name] = ctx.curFunc
	ctx.curFunc = None