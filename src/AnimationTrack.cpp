#include "AnimationTrack.h"

namespace SimpleGL {

  class AnimationTrackPrivate {
  public:
    AnimationTrackPrivate() {
    }

    ~AnimationTrackPrivate() {
    }

    template <class T>
    T calculateValue(const vector<pair<long, T>> &values, long time) {
      if (values.size() == 0)
        return T();
      // find index
      uint index = 0;
      while (index < values.size() - 1) {
        if (values.at(index + 1).first > time)
          break;
        index++;
      }
      // time is smaller than the first value
      if (index == 0)
        return values[0].second;
      // time is bigger than the last value
      if (index == values.size() - 1)
        return values[values.size() - 1].second;
      // time is between two keys
      long time1 = values[index].first, time2 = values[index + 1].first;
      T value1 = values[index].second, value2 = values[index + 1].second;
      return float(time - time1) / float(time2 - time1) * value2 + float(time2 - time) / float(time2 - time1) * value1;
    }

    string name { "" };
    vector<pair<long, Vector3f>> positionKeys;
    vector<pair<long, Quaternion>> orientationKeys;
    vector<pair<long, Vector3f>> scaleKeys;
  };

  AnimationTrack::AnimationTrack(const string &name) : d(new AnimationTrackPrivate()) {
    d->name = name;
  }

  AnimationTrack::~AnimationTrack() {
    delete d;
  }

  const string &AnimationTrack::name() const {
    return d->name;
  }

  void AnimationTrack::addPositionKey(long time, Vector3f position) {
    d->positionKeys.push_back(make_pair(time, position));
  }

  void AnimationTrack::addOrientationKey(long time, Quaternion orientation) {
    d->orientationKeys.push_back(make_pair(time, orientation));
  }

  void AnimationTrack::addScaleKey(long time, Vector3f scale) {
    d->scaleKeys.push_back(make_pair(time, scale));
  }

  Matrix4f AnimationTrack::transform(long time) const {
    Vector3f position = d->calculateValue<Vector3f>(d->positionKeys, time);
    Quaternion orientation = d->calculateValue<Quaternion>(d->orientationKeys, time);
    Vector3f scale = d->calculateValue<Vector3f>(d->scaleKeys, time);
    // calculate transform
    return glm::translate(position) * glm::mat4_cast(orientation) * glm::scale(scale);
  }
}
