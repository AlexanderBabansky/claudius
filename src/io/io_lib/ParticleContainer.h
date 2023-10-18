//
// Created by Jan on 20.04.2019.
//

#ifndef CLAUDIUS_IO_LIB_PARTICLECONTAINER_H_
#define CLAUDIUS_IO_LIB_PARTICLECONTAINER_H_

#include <vector>
#include <cstddef>

// todo do 3float iterator
class ParticleContainer {
 public:
  const unsigned int particleCount();
  void setParticle(size_t index, float x, float y, float z);
  void setColor(size_t index, int r, int g, int b);
  void setRemission(size_t index, float r);
  void resizePositions(unsigned int count);
  void resizeColors(unsigned int count);
  void resizeRemissions(unsigned int count);
  const bool isEmpty();
  const bool hasColorData();
  const bool hasRemissionData();
  const float* getParticleData();
  const float* getRemissionsData();
  const int* getColorData();
 private:
  std::vector<float> particles;
  std::vector<int> colors;
  std::vector<float> remissions;
};

#endif //CLAUDIUS_IO_LIB_PARTICLECONTAINER_H_
