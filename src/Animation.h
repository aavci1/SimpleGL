#ifndef ANIMATION_H
#define ANIMATION_H

#include "Types.h"

namespace SimpleGL {
  class AnimationTrack;

  class AnimationPrivate;

  class Animation {
  public:
    Animation(const string &name);
    ~Animation();

    const string &name() const;

    const long duration() const;
    void setDuration(long duration);

    const vector<AnimationTrack *> &tracks() const;
    AnimationTrack *createTrack(const string &name);

  private:
    AnimationPrivate *d;
  };
}

#endif // ANIMATION_H
