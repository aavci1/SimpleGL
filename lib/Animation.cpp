#include "Animation.h"

#include "AnimationTrack.h"

namespace SimpleGL {
  class AnimationPrivate {
  public:
    AnimationPrivate() {
    }

    ~AnimationPrivate() {
    }

    string name { "" };
    long duration { 0 };
    vector<AnimationTrackPtr> tracks;
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

  void Animation::setDuration(const long duration) {
    d->duration = duration;
  }

  const vector<AnimationTrackPtr> &Animation::tracks() const {
    return d->tracks;
  }

  AnimationTrackPtr Animation::createTrack(const string &name) {
    AnimationTrackPtr track { new AnimationTrack(name) };
    // add to list
    d->tracks.push_back(track);
    // return mesh
    return track;
  }
}
