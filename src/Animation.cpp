#include "Animation.h"

#include "AnimationTrack.h"

namespace SimpleGL {
  class AnimationPrivate {
  public:
    AnimationPrivate() {
    }

    ~AnimationPrivate() {
      for (uint i = 0; i < tracks.size(); ++i)
        delete tracks[i];
    }

    string name { "" };
    float duration { 0.0f };
    float ticksPerSecond { 0.0f };
    vector<AnimationTrack *> tracks;
  };

  Animation::Animation() : d(new AnimationPrivate()) {
  }

  Animation::~Animation() {
    delete d;
  }

  void Animation::setName(const string &name) {
    d->name = name;
  }

  void Animation::setDuration(float duration) {
    d->duration = duration;
  }

  void Animation::setTicksPerSecond(float ticksPerSecond) {
    d->ticksPerSecond = ticksPerSecond;
  }

  vector<AnimationTrack *> &Animation::tracks() const {
    return d->tracks;
  }
}
