//
// Created by Jan on 20.04.2019.
//

#include "ParticleContainer.h"
const unsigned int ParticleContainer::particleCount() {
  return particles.size() / 3;
}
void ParticleContainer::setParticle(size_t index, float x, float y, float z) {
  particles[index * 3] = x;
  particles[index * 3 + 1] = y;
  particles[index * 3 + 2] = z;
}
const bool ParticleContainer::isEmpty() {
  return particleCount() == 0;
}
const float *ParticleContainer::getParticleData() {
  return particles.data();
}
const bool ParticleContainer::hasColorData() {
    return !colors.empty();
}
const int *ParticleContainer::getColorData() {
    return colors.data();
}
void ParticleContainer::setColor(size_t index, int r, int g, int b)
{
    colors[index * 3] = r;
    colors[index * 3 + 1] = g;
    colors[index * 3 + 2] = b;
}
const bool ParticleContainer::hasRemissionData() {
    return !remissions.empty();
}
const float *ParticleContainer::getRemissionsData() {
    return remissions.data();
}
void ParticleContainer::setRemission(size_t index, float r)
{
    remissions[index] = r;
}
void ParticleContainer::resizePositions(unsigned int count) {
    particles.resize(count * 3);
}
void ParticleContainer::resizeColors(unsigned int count) {
    colors.resize(count * 3);
}
void ParticleContainer::resizeRemissions(unsigned int count) {
    remissions.resize(count);
}
