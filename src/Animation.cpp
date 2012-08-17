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
    long time { 0 };
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

  void Animation::setName(const string &name) {
    d->name = name;
  }

  const long Animation::duration() const {
    return d->duration;
  }

  void Animation::setDuration(long duration) {
    d->duration = duration;
  }

  const long Animation::time() const {
    return d->time;
  }

  void Animation::addTime(long delta) {
    d->time += delta;
  }

  void Animation::setTime(long time) {
    d->time = time;
  }

  Matrix4f Animation::transform(const string &trackName) const {
    // get transform for track
    for (AnimationTrack *track: d->tracks)
      if (track->name() == trackName)
        return track->transform(d->time % d->duration);
    // return identity matrix
    return Matrix4f();
  }

  const vector<AnimationTrack *> &Animation::tracks() const {
    return d->tracks;
  }

  void Animation::addTrack(AnimationTrack *track) {
    d->tracks.push_back(track);
  }
}
