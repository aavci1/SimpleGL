#ifndef ANIMATIONTRACK_H
#define ANIMATIONTRACK_H

#include "Types.h"

namespace SimpleGL {
  class AnimationTrackPrivate;

  class AnimationTrack {
  public:
    AnimationTrack(const string &name);
    ~AnimationTrack();

    const string &name() const;
    void setName(const string &name);

    void addPositionKey(long time, Vector3f position);
    void addOrientationKey(long time, Quaternion orientation);
    void addScaleKey(long time, Vector3f scale);

    Matrix4f transform(long time);

  private:
    AnimationTrackPrivate *d;
  };
}

#endif // ANIMATIONTRACK_H
