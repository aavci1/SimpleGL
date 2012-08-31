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
    void createPositionKey(long time, Vector3f position);

    const vector<pair<long, Quaternion>> &orientationKeys() const;
    void createOrientationKey(long time, Quaternion orientation);

    const vector<pair<long, Vector3f>> &scaleKeys() const;
    void createScaleKey(long time, Vector3f scale);

    Matrix4f transform(long time) const;

  private:
    AnimationTrackPrivate *d;
  };
}

#endif // ANIMATIONTRACK_H
