#pragma once
#include "PunkDefines.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "PMesh3D.hpp"
//
#include <map>
class PunkBase;
class PShader;
class PGameObject;
class PIDSequence;
class PMaterial;

class PModel {
private:
	//PIDSequence m_sequenseGen;
	friend class PunkBase;
	/*  Model Data */
	std::vector<std::shared_ptr<PTexture>> _textures_loaded;
	std::map<std::string,std::shared_ptr<PMaterial>> m_materials;
	std::vector<std::shared_ptr<PMesh3D>> _meshes;
	std::vector<std::shared_ptr<PGameObject>> objects;

	std::string _directory;
	bool gammaCorrection;
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<PMesh3D> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<PTexture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	//std::map<std::string, PMesh3D> _meshes;
	PModel(std::string const& path, bool gamma = false);
	~PModel() {};
	void Draw(PShader* shader);
	void Init();
};