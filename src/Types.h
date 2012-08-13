#ifndef TYPES_H
#define TYPES_H

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace SimpleGL {
  typedef std::string String;

  typedef glm::detail::tvec2<int> Vector2i;
  typedef glm::detail::tvec3<int> Vector3i;
  typedef glm::detail::tvec4<int> Vector4i;

  typedef glm::detail::tmat2x2<int> Matrix2i;
  typedef glm::detail::tmat3x3<int> Matrix3i;
  typedef glm::detail::tmat4x4<int> Matrix4i;

  typedef glm::detail::tvec2<float> Vector2f;
  typedef glm::detail::tvec3<float> Vector3f;
  typedef glm::detail::tvec4<float> Vector4f;

  typedef glm::detail::tmat2x2<float> Matrix2f;
  typedef glm::detail::tmat3x3<float> Matrix3f;
  typedef glm::detail::tmat4x4<float> Matrix4f;

  typedef glm::quat Quaternion;

  enum AttributeType {
    AT_POSITION = 0x00001,
    AT_NORMAL = 0x0002,
    AT_COLOR = 0x0004,
    AT_TEXCOORD0 = 0x0008,
    AT_TEXCOORD1 = 0x0010,
    AT_TEXCOORD2 = 0x0020,
    AT_TEXCOORD3 = 0x0040,
    AT_BONEINFO0 = 0x0080,
    AT_BONEINFO1 = 0x0100,
    AT_BONEINFO2 = 0x0200,
    AT_BONEINFO3 = 0x0400
  };

  enum AttributePosition {
    AP_POSITION = 0,
    AP_NORMAL = 1,
    AP_COLOR = 2,
    AP_TEXCOORD0 = 3,
    AP_TEXCOORD1 = 4,
    AP_TEXCOORD2 = 5,
    AP_TEXCOORD3 = 6,
    AP_BONEID0 = 7,
    AP_BONEWEIGHT0 = 8,
    AP_BONEID1 = 9,
    AP_BONEWEIGHT1 = 10,
    AP_BONEID2 = 11,
    AP_BONEWEIGHT2 = 12,
    AP_BONEID3 = 13,
    AP_BONEWEIGHT3 = 14
  };

  enum LightType {
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT
  };

  enum ShaderType {
    ST_VERTEX,
    ST_GEOMETRY,
    ST_FRAGMENT
  };

  enum TextureFormat {
    TF_RGBA8,
    TF_RGBA16F
  };

  enum TransformSpace {
    TS_LOCAL,
    TS_WORLD
  };
}

#endif
