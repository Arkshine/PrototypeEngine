#ifndef COMMON_IMETATOOL_H
#define COMMON_IMETATOOL_H

#include <cstddef>

#include "interface.h"

class IMetaLoader;

/**
*	@addtogroup MetaLoader
*
*	@{
*/

/**
*	Interface to a tool that can be loaded out of a library.
*/
class IMetaTool : public IBaseInterface
{
public:
	/**
	*	Starts up the tool.
	*	@param loader Meta loader.
	*/
	virtual bool Startup( IMetaLoader& loader, CreateInterfaceFn* pFactories, const size_t uiNumFactories ) = 0;

	/**
	*	Runs the tool.
	*/
	virtual bool Run() = 0;

	/**
	*	Shuts down the tool.
	*/
	virtual void Shutdown() = 0;
};

/**
*	Default interface name.
*/
#define DEFAULT_IMETATOOL_NAME "IMetaToolV001"

/** @} */

#endif //COMMON_IMETATOOL_H
