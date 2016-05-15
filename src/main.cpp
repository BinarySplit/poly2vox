#include <cstddef>
#include <vector>
#include <array>
#include <unordered_map>
#include <stdio.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//typedef std::array<aiVector3D, 3> tri;

template<typename T> T min(T a, T b) { return a < b ? a : b; }
template<typename T> T max(T a, T b) { return a < b ? b : a; }

struct tri {
  std::array<aiVector3D, 3> vert;
  std::array<aiVector2D, 3> texCoord;
  aiTexture tex;
};

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("argc == %i\n", argc);
    exit(1);
  }
  
  printf("opening %s\n", argv[1]);
  auto scene = aiImportFile(argv[1], 
				aiProcess_JoinIdenticalVertices|aiProcess_Triangulate|aiProcess_PreTransformVertices|
				aiProcess_SortByPType|aiProcess_FindDegenerates|aiProcess_FindInvalidData|aiProcess_OptimizeMeshes|
				aiProcess_OptimizeGraph|aiProcess_Debone);
  
  if(scene == nullptr) {
    printf("scene == nullptr\n");
    exit(1);
  }
  
  
  //Find bounding box
  aiVector3D bbMin(1e10f,1e10f,1e10f);
  aiVector3D bbMax(-1e10f,-1e10f,-1e10f);
  for(size_t i = 0; i < scene->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[i];
    if(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) {
      for(size_t j = 0; j < mesh->mNumVertices; j++) {
	auto vert = mesh->mVertices[j];
	bbMin.x = min(bbMin.x, vert.x);
	bbMin.y = min(bbMin.y, vert.y);
	bbMin.z = min(bbMin.z, vert.z);
	bbMax.x = max(bbMax.x, vert.x);
	bbMax.y = max(bbMax.y, vert.y);
	bbMax.z = max(bbMax.z, vert.z);
      }
    }
  }
  aiVector3D bbSpan = bbMax - bbMin;
  printf("min %f,%f,%f\n", bbMin.x, bbMin.y, bbMin.z);
  printf("max %f,%f,%f\n", bbMax.x, bbMax.y, bbMax.z);
  printf("span %f,%f,%f\n", bbSpan.x, bbSpan.y, bbSpan.z);
  
  
  std::vector<tri> tris;
  
  //Convert to tris
  for(size_t i = 0; i < scene->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[i];
    if(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) {
      for(size_t j = 0; j < mesh->mNumFaces; j++) {
	auto face = mesh->mFaces[j];
	if(face.mNumIndices == 3) {
// 	  tris.push_back(tri {
// 	    { mesh->mVertices[face.mIndices[0]],
// 	      mesh->mVertices[face.mIndices[1]],
// 	      mesh->mVertices[face.mIndices[2]] },
// 	    { mesh->mTextureCoords[face.mIndices[0]],
// 	      mesh->mTextureCoords[face.mIndices[1]],
// 	      mesh->mTextureCoords[face.mIndices[2]] },
// 	      scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, /*TODO*/)
// 	  });
	}
      }
    }
  }
  //Organize into bins
  //std::unordered_map<aiVector3t<int>, std::vector<tri*> > bins;
  //TODO
  //For each bin, render to voxel space then RLE compress
  //TODO
  
  
  aiReleaseImport(scene);
  
  return 0;
}
