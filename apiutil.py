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

class APIContext:
	def __init__(self):
		self.apiTypes = {}
		self.apiFuncs = {}
		self.curType = None
		self.curFunc = None

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

def PARAM(ctx, type, name):
	assert(ctx.curFunc != None)
	ctx.curFunc.params.append((type, name))

def FUNC_END(ctx, name):
	assert(ctx.curFunc != None)
	if ctx.curType != None:
		ctx.curType.methods.append(ctx.curFunc)
	else:
		ctx.apiFuncs[ctx.curFunc.name] = ctx.curFunc
	ctx.curFunc = None