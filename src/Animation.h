#ifndef ANIMATION_H
#define ANIMATION_H

#include "Types.h"

namespace SimpleGL {
  class AnimationTrack;

  class AnimationPrivate;

  class Animation {
  public:
    Animation();
    ~Animation();

    void setName(const string &name);
    void setDuration(float duration);
    void setTicksPerSecond(float ticksPerSecond);

    vector<AnimationTrack *> &tracks() const;

  private:
    AnimationPrivate *d;
  };
}

#endif // ANIMATION_H
