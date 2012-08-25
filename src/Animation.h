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
    void setName(const string &name);

    const long duration() const;
    void setDuration(long duration);

    const long time() const;
    void setTime(long time);
    void addTime(long delta);

    Matrix4f transform(const string &trackName) const;

    const vector<AnimationTrack *> &tracks() const;
    AnimationTrack *createTrack(const string &name);

  private:
    AnimationPrivate *d;
  };
}

#endif // ANIMATION_H
