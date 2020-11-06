#pragma once


#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	SAFE_DELETE_VECLIST(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_DELETE((*iter));\
	}\
	p.clear();\
}

#define	SAFE_DELETE_MAP(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_DELETE(iter->second);\
	}\
	p.clear();\
}

#define	SAFE_DELETE_ARRAY_VECLIST(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_DELETE_ARRAY((*iter));\
	}\
	p.clear();\
}

#define	SAFE_DELETE_ARRAY_MAP(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_DELETE_ARRAY(iter->second);\
	}\
	p.clear();\
}

#define	SAFE_RELEASE_VECLIST(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_RELEASE((*iter));\
	}\
	p.clear();\
}

#define	SAFE_RELEASE_MAP(p)	\
{\
	auto		iter = p.begin();\
	auto		iterEnd = p.end();\
	for(; iter != iterEnd; ++iter)\
	{\
		SAFE_RELEASE(iter->second);\
	}\
	p.clear();\
}

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_pInst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst = nullptr;
#define	GET_SINGLE(Type)		Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

#define	DEVICE		GET_SINGLE(CDevice)->GetDevice()
#define	CONTEXT		GET_SINGLE(CDevice)->GetContext()
#define	SWAPCHAIN	GET_SINGLE(CDevice)->GetSwapChain()
#define	RESOLUTION	GET_SINGLE(CDevice)->GetResolution()
