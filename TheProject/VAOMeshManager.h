#ifndef MeshManager_HG
#define MeshManager_HG
#include <map>
#include "sDrawInfo.h"

class VAOMeshManager
{
public:
	VAOMeshManager();
	~VAOMeshManager();

	bool LoadModelIntoVAO(sDrawInfo &drawInfo, unsigned int shaderProgramID);
	bool FindDrawInfoByModelName(sDrawInfo &drawInfo);
private:
	bool m_LoadModelFromFile(sDrawInfo&);
	bool m_LoadDrawInfo_Into_VAO(sDrawInfo&, int);
	std::map< std::string /*model name*/, sDrawInfo > m_mapModel_to_VAOID;
};


#endif
