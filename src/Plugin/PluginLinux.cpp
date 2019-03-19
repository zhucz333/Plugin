#include <string.h>
#include <dlfcn.h>

#include "Plugin.h"

Plugin::Plugin(const std::string& cmd, const std::string& path, const PluginConfig& config)
{
	m_strPluginCmd = cmd;
	m_strPluginPath = path + ".so";
	m_mapConfig = config;

	s_Plugins[cmd] = this;

	m_hPlugin = dlopen(m_strPluginPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	if (m_hPlugin) {
		if (m_pfnInit = dlsym(m_hPlugin, "PluginInit")) {
			m_ptrPluginContex = m_pfnInit(m_mapConfig);
		}
		m_pfnHandler = dlsym(m_hPlugin, "HandleRequest");
	}
}

Plugin::~Plugin()
{
	if (m_hPlugin) {
		if (m_pfnDestroy = dlsym(m_hPlugin, "PluginFree")) {
			m_pfnDestroy(m_ptrPluginContex);
		}

		dlclose(m_hPlugin);
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