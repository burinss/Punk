#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "PModel.hpp"
#include "PComponent.hpp"
#include "PStaticMeshComponent.hpp"
#include "PGameObject.hpp"
#include "PShader.hpp"
#include "PMesh3D.hpp"
#include "PTexture.h"
#include <glad/glad.h>
#include "PVertexArrayBuffer.hpp"
#include "PIDSequence.hpp"
#include "PMaterial.hpp"
#include "PGlobals.hpp"

void PModel::loadModel(std::string const& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	_directory = path.substr(0, path.find_last_of('/'));

	std::cout << "Loaded file have:"<<std::endl
		<< scene->mNumMaterials << " materials" << std::endl
		<< scene->mNumAnimations <<" animations" <<std::endl
		<< scene->mNumCameras << " cameras"<<std::endl
		<< scene->mNumLights << " lights"<<std::endl
		<< scene->mNumMeshes << " meshes"<<std::endl
		<<scene->mNumTextures<< " textures"<<std::endl;

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void PModel::processNode(aiNode* node, const aiScene* scene)
{
	auto AssimpToGLMVec3 = [](const aiVector3D& vec) {
		return glm::vec3(vec.x, vec.y, vec.z); };

	auto AssimpToGLMQuat = [](const aiQuaternion& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	};

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;
	node->mTransformation.Decompose(scale,rotation,position);
	//PIDSequence sequenceGen("Mesh");
	std::cout << node->mName.C_Str() << std::endl;
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::shared_ptr<PMesh3D> loadedMesh;
		loadedMesh = processMesh(mesh, scene);
		//loadedMesh->id_ = sequenceGen.Next();

		_meshes.push_back(loadedMesh);
		std::shared_ptr<PGameObject> tmp = std::make_shared<PGameObject>(node->mName.C_Str());
		tmp->SetPosition(AssimpToGLMVec3(position));
		tmp->SetOrientation(AssimpToGLMQuat(rotation));
		tmp->SetScale(AssimpToGLMVec3(scale));
		std::shared_ptr<PStaticMeshComponent> meshComponent = std::static_pointer_cast<PStaticMeshComponent>(PComponent::CreateMeshComponent(tmp));
		meshComponent->SetMesh(std::static_pointer_cast<PMesh>(loadedMesh));
		objects.push_back(tmp);
		//_meshes.insert(std::pair<std::string,PMesh3D>( node->mName.C_Str(), processMesh(mesh, scene)));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

std::shared_ptr<PMesh3D> PModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	
	PVertex3DDataStruct vertexData;
	// data to fill
	std::vector<PVertex3D> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<PTexture>> textures;

	std::cout << mesh->mName.C_Str() << std::endl;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		PVertex3D vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
			vertex.uv = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;

		//vertices.push_back(vertex);

		vertexData.vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			vertexData.indices.push_back(face.mIndices[j]);
			//indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//auto numTextures = material->GetTextureCount(aiTextureType_DIFFUSE) + material->GetTextureCount(aiTextureType_SPECULAR) + material->GetTextureCount(aiTextureType_HEIGHT);
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	std::shared_ptr<PMaterial> meshMaterial;
	auto find = m_materials.find(material->GetName().C_Str());
	if (find == m_materials.end())
	{
		auto tmp = std::make_shared<PMaterial>();
		tmp->SetName(material->GetName().C_Str());

		aiString str;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		std::cout << this->_directory << str.C_Str() << std::endl;
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < _textures_loaded.size(); j++)
		{

			if (std::strcmp(_textures_loaded[j]->path.data(), str.C_Str()) == 0)
			{
				tmp->Set("diffuse", _textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			std::shared_ptr<PTexture> texture = std::make_shared<PTexture>();
			texture->type = "diffuse";
			texture->path = _directory + "/" + std::string(str.data);
			tmp->Set("diffuse", texture);
			_textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
		meshMaterial = tmp;
		PSystems::GetTaskRunner()->AttachProcess(tmp);
	}
	else
		meshMaterial = find->second;


	// 1. diffuse maps
	//std::vector<std::shared_ptr<PTexture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	//std::vector<std::shared_ptr<PTexture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	//std::vector<std::shared_ptr<PTexture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	//std::vector<std::shared_ptr<PTexture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	//return PMesh3D(vertices, indices, textures);
	return std::make_shared<PMesh3D>(vertexData, meshMaterial);
}

std::vector<std::shared_ptr<PTexture>> PModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<std::shared_ptr<PTexture>> textures;

	unsigned int textureCount = mat->GetTextureCount(type);

	if (textureCount == 0) //If there are no textures
	{
		aiColor4D aiColor(0.0f, 0.0f, 0.0f,0.f);
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			float opacity;
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			mat->Get(AI_MATKEY_OPACITY, opacity);
			textures.push_back(std::make_shared<PTexture>(glm::vec4(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255, opacity*255)));
			return textures;
		}
	}
	else
	{

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::cout << this->_directory << str.C_Str() << std::endl;
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < _textures_loaded.size(); j++)
			{

				if (std::strcmp(_textures_loaded[j]->path.data(), str.C_Str()) == 0)
				{
					textures.push_back(_textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				std::shared_ptr<PTexture> texture = std::make_shared<PTexture>();
				texture->Load(str.C_Str(), this->_directory);
				texture->type = typeName;
				texture->path = str.C_Str();
				textures.push_back(texture);
				_textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.

			}
		}
	}
	return textures;
}

PModel::PModel(std::string const& path, bool gamma)
{
	loadModel(path);
}

void PModel::Draw(PShader* shader)
{
	//for (unsigned int i = 0; i < _meshes.size(); i++)
	//	_meshes.at(i)->Render(shader);
	/*for (auto mesh : _meshes)
		mesh.second.Render(shader);*/
}

void PModel::Init()
{

}
