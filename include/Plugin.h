#pragma once

#include <unordered_map>
#include <functional>

#ifdef _WIN32
#include <Windows.h>
#else
typedef void* HMOUDLE
#endif

typedef void* ThreadContex;
typedef void* PluginContex;
typedef std::unordered_map<std::string, std::string> PluginConfig;

typedef PluginContex(*PluginInit)(PluginConfig&);
typedef void (*PluginDestroy)(PluginContex);
typedef bool (*PluginHandler)(std::unordered_map<std::string, std::string>&, std::unordered_map<std::string, std::string>&, ThreadContex, PluginContex);

class Plugin
{
public:
	Plugin(const std::string& cmd, const std::string& path, const PluginConfig& config);
	~Plugin();

	static Plugin* GetPlugin(std::string cmd);
	static void DestroyAllPlugin();

	bool Handle(std::unordered_map<std::string, std::string>&, std::unordered_map<std::string, std::string>&, ThreadContex);

private:
	std::string		m_strPluginCmd;
	std::string		m_strPluginPath;
	PluginContex	m_ptrPluginContex;
	PluginConfig	m_mapConfig;
	HMODULE			m_hPlugin;
	PluginInit		m_pfnInit;
	PluginDestroy	m_pfnDestroy;
	PluginHandler	m_pfnHandler;

	static std::unordered_map<std::string, Plugin*> s_Plugins;
};
