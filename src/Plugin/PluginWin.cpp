#include <string.h>

#include "Plugin.h"

Plugin::Plugin(const std::string& cmd, const std::string& path, const PluginConfig& config)
{
	m_strPluginCmd = cmd;
	m_strPluginPath = path + ".so";
	m_mapConfig = config;

	s_Plugins[cmd] = this;

	m_hPlugin = ::LoadLibrary(m_strPluginPath.c_str());
	if (m_hPlugin) {
		if (m_pfnInit = (PluginInit)GetProcAddress(m_hPlugin, "PluginInit")) {
			m_ptrPluginContex = m_pfnInit(m_mapConfig);
		}
		m_pfnHandler = (PluginHandler)GetProcAddress(m_hPlugin, "HandleRequest");
	}
}

Plugin::~Plugin()
{
	if (m_hPlugin) {
		if (m_pfnDestroy = (PluginDestroy)GetProcAddress(m_hPlugin, "PluginFree")) {
			m_pfnDestroy(m_ptrPluginContex);
		}

		::FreeLibrary(m_hPlugin);

		m_hPlugin = NULL;
	}
}

bool Plugin::Handle(std::unordered_map<std::string, std::string>& req, std::unordered_map<std::string, std::string>& res, ThreadContex threadCtx)
{
	return m_pfnHandler(req, res, threadCtx, m_ptrPluginContex);
}

Plugin* Plugin::GetPlugin(std::string cmd)
{
	return s_Plugins[cmd];
}

void Plugin::DestroyAllPlugin()
{
	return s_Plugins.clear();
}