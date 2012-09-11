#include "FbxImporter.h"

#include "Model.h"
#include "Root.h"

#define FBXSDK_NEW_API
#include <fbxsdk.h>

using namespace SimpleGL;

namespace FBXImporter {

  /* Tab character ("\t") counter */
  int numTabs = 0;

  /**
   * Print the required number of tabs.
   */
  void PrintTabs() {
      for(int i = 0; i < numTabs; i++)
          printf("\t");
  }

  /**
   * Return a string-based representation based on the attribute type.
   */
  FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
      switch(type) {
          case FbxNodeAttribute::eUnknown: return "unidentified";
          case FbxNodeAttribute::eNull: return "null";
          case FbxNodeAttribute::eMarker: return "marker";
          case FbxNodeAttribute::eSkeleton: return "skeleton";
          case FbxNodeAttribute::eMesh: return "mesh";
          case FbxNodeAttribute::eNurbs: return "nurb";
          case FbxNodeAttribute::ePatch: return "patch";
          case FbxNodeAttribute::eCamera: return "camera";
          case FbxNodeAttribute::eCameraStereo:    return "stereo";
          case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
          case FbxNodeAttribute::eLight: return "light";
          case FbxNodeAttribute::eOpticalReference: return "optical reference";
          case FbxNodeAttribute::eOpticalMarker: return "marker";
          case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
          case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
          case FbxNodeAttribute::eBoundary: return "boundary";
          case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
          case FbxNodeAttribute::eShape: return "shape";
          case FbxNodeAttribute::eLODGroup: return "lodgroup";
          case FbxNodeAttribute::eSubDiv: return "subdiv";
          default: return "unknown";
      }
  }

  /**
   * Print an attribute.
   */
  void PrintAttribute(FbxNodeAttribute* pAttribute) {
      if(!pAttribute) return;

      FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
      FbxString attrName = pAttribute->GetName();
      PrintTabs();
      // Note: to retrieve the chararcter array of a FbxString, use its Buffer() method.
      printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
  }

  /**
   * Print a node, its attributes, and all its children recursively.
   */
  void PrintNode(FbxNode* pNode) {
      PrintTabs();
      const char* nodeName = pNode->GetName();
      FbxDouble3 translation = pNode->LclTranslation.Get();
      FbxDouble3 rotation = pNode->LclRotation.Get();
      FbxDouble3 scaling = pNode->LclScaling.Get();

      // print the contents of the node.
      printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
          nodeName,
          translation[0], translation[1], translation[2],
          rotation[0], rotation[1], rotation[2],
          scaling[0], scaling[1], scaling[2]
          );
      numTabs++;

      // Print the node's attributes.
      for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
          PrintAttribute(pNode->GetNodeAttributeByIndex(i));

      // Recursively print the children nodes.
      for(int j = 0; j < pNode->GetChildCount(); j++)
          PrintNode(pNode->GetChild(j));

      numTabs--;
      PrintTabs();
      printf("</node>\n");
  }

  ModelPtr import(const string &name, const string &path) {
    ModelPtr model = Root::instance()->createModel(name);
    // create the fbx manager
    FbxManager *fbxManager = FbxManager::Create();
    // create io settings object
    FbxIOSettings *ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ioSettings);
    // create importer
    FbxImporter* fbxImporter = FbxImporter::Create(fbxManager,"");
    // try to initialize the importer
    if(!fbxImporter->Initialize(path.c_str(), -1, fbxManager->GetIOSettings()))
        return model;
    // create a dummy scene, to be populated from the scene imported from file
    FbxScene* fbxScene = FbxScene::Create(fbxManager,"myScene");
    // import the contents of the file into the scene.
    fbxImporter->Import(fbxScene);
    // file has been imported, destroy the improter
    fbxImporter->Destroy();

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node, because it should
    // not contain any attributes.
    FbxNode* rootNode = fbxScene->GetRootNode();
    if(rootNode) {
        for(int i = 0; i < rootNode->GetChildCount(); i++)
            PrintNode(rootNode->GetChild(i));
    }
    // Destroy the sdk manager and all other objects it was handling.
    fbxManager->Destroy();
    // return model
    return model;
  }
}