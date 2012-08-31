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
    long duration { 0 };
    vector<AnimationTrack *> tracks;
  };

  Animation::Animation(const string &name) : d(new AnimationPrivate()) {
    d->name = name;
  }

  Animation::~Animation() {
    delete d;
  }

  const string &Animation::name() const {
    return d->name;
  }

  const long Animation::duration() const {
    return d->duration;
  }

  void Animation::setDuration(long duration) {
    d->duration = duration;
  }

  const vector<AnimationTrack *> &Animation::tracks() const {
    return d->tracks;
  }

  AnimationTrack *Animation::createTrack(const string &name) {
    AnimationTrack *track = new AnimationTrack(name);
    // add to list
    d->tracks.push_back(track);
    // return mesh
    return track;
  }
}
