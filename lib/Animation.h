#ifndef ANIMATION_H
#define ANIMATION_H

#include "Types.h"

namespace SimpleGL {
  class AnimationPrivate;

  class Animation {
  public:
    Animation(const string &name);
    ~Animation();

    const string &name() const;

    const long duration() const;
    void setDuration(const long duration);

    const vector<AnimationTrackPtr> &tracks() const;
    AnimationTrackPtr createTrack(const string &name);

  private:
    AnimationPrivate *d;
  };
}

#endif // ANIMATION_H
