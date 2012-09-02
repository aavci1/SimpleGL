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

    const vector<pair<long, Vector3f>> &positionKeys() const;
    void createPositionKey(const long time, Vector3f position);

    const vector<pair<long, Quaternion>> &orientationKeys() const;
    void createOrientationKey(const long time, Quaternion orientation);

    const vector<pair<long, Vector3f>> &scaleKeys() const;
    void createScaleKey(const long time, Vector3f scale);

    const Matrix4f transform(const long time) const;

  private:
    AnimationTrackPrivate *d;
  };
}

#endif // ANIMATIONTRACK_H
