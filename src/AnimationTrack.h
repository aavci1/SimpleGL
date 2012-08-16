#ifndef ANIMATIONTRACK_H
#define ANIMATIONTRACK_H

#include "Types.h"

namespace SimpleGL {
  class AnimationTrackPrivate;

  class AnimationTrack {
  public:
    AnimationTrack();
    ~AnimationTrack();

    void setNodeName(const string &name);
    void addPositionKey(float time, Vector3f position);
    void addOrientationKey(float time, Quaternion orientation);
    void addScaleKey(float time, Vector3f scale);

    Matrix4f calculateTransform(float time);
  private:
    AnimationTrackPrivate *d;
  };
}

#endif // ANIMATIONTRACK_H
