#include "AnimationTrack.h"

namespace SimpleGL {

  class AnimationTrackPrivate {
  public:
    AnimationTrackPrivate() {
    }

    ~AnimationTrackPrivate() {
    }

    string name { "" };
    vector<pair<float, Vector3f>> positionKeys;
    vector<pair<float, Quaternion>> orientationKeys;
    vector<pair<float, Vector3f>> scaleKeys;
  };

  AnimationTrack::AnimationTrack() : d(new AnimationTrackPrivate()) {
  }

  AnimationTrack::~AnimationTrack() {
    delete d;
  }

  void AnimationTrack::setNodeName(const string &name) {
    d->name = name;
  }

  void AnimationTrack::addPositionKey(float time, Vector3f position) {
    d->positionKeys.push_back(make_pair(time, position));
  }

  void AnimationTrack::addOrientationKey(float time, Quaternion orientation) {
    d->orientationKeys.push_back(make_pair(time, orientation));
  }

  void AnimationTrack::addScaleKey(float time, Vector3f scale) {
    d->scaleKeys.push_back(make_pair(time, scale));
  }

  template <class T>
  T calculateValue(const vector<pair<float, T>> &values, float time) {
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
    float time1 = values[index].first, time2 = values[index + 1].first;
    T value1 = values[index].second, value2 = values[index + 1].second;

    return (time - time1) / (time2 - time1) * value1 + (time2 - time) / (time2 - time1) * value2;
  }

  Matrix4f AnimationTrack::calculateTransform(float time) {
    Vector3f position = calculateValue<Vector3f>(d->positionKeys, time);
    Quaternion orientation = calculateValue<Quaternion>(d->orientationKeys, time);
    Vector3f scale = calculateValue<Vector3f>(d->scaleKeys, time);
    // calculate transform
    return glm::translate(position) * glm::mat4_cast(orientation) * glm::scale(scale);
  }
}
