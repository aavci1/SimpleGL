#ifndef TYPES_H
#define TYPES_H

#include <sys/types.h>

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

  enum Key {
    KEY_UNKNOWN = -1,
    KEY_SPACE = 32,
    KEY_SPECIAL = 256,
    KEY_ESC = KEY_SPECIAL + 1,
    KEY_F1 = KEY_SPECIAL + 2,
    KEY_F2 = KEY_SPECIAL + 3,
    KEY_F3 = KEY_SPECIAL + 4,
    KEY_F4 = KEY_SPECIAL + 5,
    KEY_F5 = KEY_SPECIAL + 6,
    KEY_F6 = KEY_SPECIAL + 7,
    KEY_F7 = KEY_SPECIAL + 8,
    KEY_F8 = KEY_SPECIAL + 9,
    KEY_F9 = KEY_SPECIAL + 10,
    KEY_F10 = KEY_SPECIAL + 11,
    KEY_F11 = KEY_SPECIAL + 12,
    KEY_F12 = KEY_SPECIAL + 13,
    KEY_F13 = KEY_SPECIAL + 14,
    KEY_F14 = KEY_SPECIAL + 15,
    KEY_F15 = KEY_SPECIAL + 16,
    KEY_F16 = KEY_SPECIAL + 17,
    KEY_F17 = KEY_SPECIAL + 18,
    KEY_F18 = KEY_SPECIAL + 19,
    KEY_F19 = KEY_SPECIAL + 20,
    KEY_F20 = KEY_SPECIAL + 21,
    KEY_F21 = KEY_SPECIAL + 22,
    KEY_F22 = KEY_SPECIAL + 23,
    KEY_F23 = KEY_SPECIAL + 24,
    KEY_F24 = KEY_SPECIAL + 25,
    KEY_F25 = KEY_SPECIAL + 26,
    KEY_UP = KEY_SPECIAL + 27,
    KEY_DOWN = KEY_SPECIAL + 28,
    KEY_LEFT = KEY_SPECIAL + 29,
    KEY_RIGHT = KEY_SPECIAL + 30,
    KEY_LSHIFT = KEY_SPECIAL + 31,
    KEY_RSHIFT = KEY_SPECIAL + 32,
    KEY_LCTRL = KEY_SPECIAL + 33,
    KEY_RCTRL = KEY_SPECIAL + 34,
    KEY_LALT = KEY_SPECIAL + 35,
    KEY_RALT = KEY_SPECIAL + 36,
    KEY_TAB = KEY_SPECIAL + 37,
    KEY_ENTER = KEY_SPECIAL + 38,
    KEY_BACKSPACE = KEY_SPECIAL + 39,
    KEY_INSERT = KEY_SPECIAL + 40,
    KEY_DEL = KEY_SPECIAL + 41,
    KEY_PAGEUP = KEY_SPECIAL + 42,
    KEY_PAGEDOWN = KEY_SPECIAL + 43,
    KEY_HOME = KEY_SPECIAL + 44,
    KEY_END = KEY_SPECIAL + 45,
    KEY_KP_0 = KEY_SPECIAL + 46,
    KEY_KP_1 = KEY_SPECIAL + 47,
    KEY_KP_2 = KEY_SPECIAL + 48,
    KEY_KP_3 = KEY_SPECIAL + 49,
    KEY_KP_4 = KEY_SPECIAL + 50,
    KEY_KP_5 = KEY_SPECIAL + 51,
    KEY_KP_6 = KEY_SPECIAL + 52,
    KEY_KP_7 = KEY_SPECIAL + 53,
    KEY_KP_8 = KEY_SPECIAL + 54,
    KEY_KP_9 = KEY_SPECIAL + 55,
    KEY_KP_DIVIDE = KEY_SPECIAL + 56,
    KEY_KP_MULTIPLY = KEY_SPECIAL + 57,
    KEY_KP_SUBTRACT = KEY_SPECIAL + 58,
    KEY_KP_ADD = KEY_SPECIAL + 59,
    KEY_KP_DECIMAL = KEY_SPECIAL + 60,
    KEY_KP_EQUAL = KEY_SPECIAL + 61,
    KEY_KP_ENTER = KEY_SPECIAL + 62,
    KEY_KP_NUM_LOCK = KEY_SPECIAL + 63,
    KEY_CAPS_LOCK = KEY_SPECIAL + 64,
    KEY_SCROLL_LOCK = KEY_SPECIAL + 65,
    KEY_PAUSE = KEY_SPECIAL + 66,
    KEY_LSUPER = KEY_SPECIAL + 67,
    KEY_RSUPER = KEY_SPECIAL + 68,
    KEY_MENU = KEY_SPECIAL + 69,
    KEY_LAST = KEY_MENU
  };

}

#endif
