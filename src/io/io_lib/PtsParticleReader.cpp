#include <fstream>
#include <iostream>
#include "PtsParticleReader.h"
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include <fmt/format.h>
#include <ProgressReporter.h>
#include <thread>

// remission: float
// RGB: rgb
// quality: float

void split(std::string const &original, std::vector<std::string> &results) {
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find(start, end, ' ');
    while (next != end) {
        auto x = std::string(start, next);
        if (!x.empty()) {
            results.push_back(x);
        }
        start = next + 1;
        next = std::find(start, end, ' ');
    }
    if (!std::string(start, next).empty()) {
        results.emplace_back(start, next);
    }
}

const char *split2(const char *ptr, const char *end, float dataOut[8])
{
    const char *valuePtr = ptr;
    const char *valuePtrEnd = ptr;
    size_t dataIndex = 0;
    while (ptr != end) {
        char c = *ptr;
        if (c == '\r') {
            ++ptr;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\n') {
            if (valuePtr < valuePtrEnd && dataIndex < 8) {
                dataOut[dataIndex] = std::stof(std::string(valuePtr, valuePtrEnd - valuePtr));
                ++dataIndex;
            }
            ++ptr;
            if (c == '\n') {
                return ptr;
            }
            valuePtr = ptr;
            continue;
        }
        ++ptr;
        valuePtrEnd = ptr;
    }
    return ptr;
}

// we need to flip Y and Z, because they are the other way around in .pts files
const unsigned int PTS_X_INDEX = 0;
const unsigned int PTS_Y_INDEX = 2;
const unsigned int PTS_Z_INDEX = 1;

void processChunk(const char *ptr, const char *end,
                  ParticleContainer &particleContainer, size_t startIndex,
                  bool positionOnly, bool positionAndRgb,
                  bool positionRemissionAndRgb,
                  bool positionRemissionQualityAndRgb)
{
    size_t index = startIndex;
    while (ptr < end) {
        const char *wasPtr = ptr;
        float data[8];
        ptr = split2(ptr, end, data);
        if (ptr - wasPtr <= 3) {
            continue;
        }
        if (positionOnly) {
            particleContainer.setParticle(index, data[PTS_X_INDEX], data[PTS_Y_INDEX],
                                          data[PTS_Z_INDEX]);
        } else if (positionAndRgb) {
            particleContainer.setParticle(index, data[PTS_X_INDEX], data[PTS_Y_INDEX],
                                          data[PTS_Z_INDEX]);
            particleContainer.setColor(index, data[3], data[4], data[5]);
        } else if (positionRemissionAndRgb) {
            particleContainer.setParticle(index, data[PTS_X_INDEX], data[PTS_Y_INDEX],
                                          data[PTS_Z_INDEX]);
            particleContainer.setRemission(index, data[3]);
            particleContainer.setColor(index, data[4], data[5], data[6]);
        } else if (positionRemissionQualityAndRgb) {
            particleContainer.setParticle(index, data[PTS_X_INDEX], data[PTS_Y_INDEX],
                                          data[PTS_Z_INDEX]);
            particleContainer.setRemission(index, data[3]);
            particleContainer.setColor(index, data[5], data[6], data[7]);
        }
        ++index;
    }
}

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {

    if (file.fail()) {
        std::cerr << fmt::format("File does not exist") << std::endl;;
        return;
    }

    // todo we can do that nicer
    std::string line;
    std::getline(file, line);
    const int particleCount = std::stoi(line);
    std::cout << fmt::format("Estimated particle count: {}", particleCount) << std::endl;;
    std::getline(file, line);

    std::vector<std::string> vec;
    split(line, vec);

    const bool positionOnly = vec.size() == 3;
    const bool positionAndRgb = vec.size() == 6;
    const bool positionRemissionAndRgb = vec.size() == 7;
    const bool positionRemissionQualityAndRgb = vec.size() == 8;

    std::cout << fmt::format("positionOnly: {}", positionOnly ) << std::endl;;
    std::cout << fmt::format("positionAndRgb: {}", positionAndRgb ) << std::endl;;
    std::cout << fmt::format("positionRemissionAndRgb: {}", positionRemissionAndRgb ) << std::endl;;
    std::cout << fmt::format("positionRemissionQualityAndRgb: {}", positionRemissionQualityAndRgb ) << std::endl;;

    if (positionOnly) {
        std::cout << fmt::format("positionOnly") << std::endl;;
        std::cout << fmt::format("resize positions") << std::endl;;
        particleContainer.resizePositions(particleCount);
    } else if (positionAndRgb) {
        std::cout << fmt::format("positionAndRgb") << std::endl;;
        std::cout << fmt::format("resize positions") << std::endl;;
        particleContainer.resizePositions(particleCount);
        std::cout << fmt::format("resize colors") << std::endl;;
        particleContainer.resizeColors(particleCount);
    } else if (positionRemissionAndRgb) {
        std::cout << fmt::format("positionRemissionAndRgb") << std::endl;;
        std::cout << fmt::format("resize positions") << std::endl;;
        particleContainer.resizePositions(particleCount);
        std::cout << fmt::format("resize colors") << std::endl;;
        particleContainer.resizeColors(particleCount);
        std::cout << fmt::format("resize remissions") << std::endl;;
        particleContainer.resizeRemissions(particleCount);
    } else if (positionRemissionQualityAndRgb) {
        std::cout << fmt::format("positionRemissionQualityAndRgb") << std::endl;;
        std::cout << fmt::format("resize positions") << std::endl;;
        particleContainer.resizePositions(particleCount);
        std::cout << fmt::format("resize colors") << std::endl;;
        particleContainer.resizeColors(particleCount);
        std::cout << fmt::format("resize remissions") << std::endl;;
        particleContainer.resizeRemissions(particleCount);
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
    file.read(buffer.get(), size);
    if (file.fail()) {
        std::cerr << fmt::format("File does not exist") << std::endl;;
        return;
    }
    if (file.gcount() != size) {
        std::cerr << fmt::format("Error reading file") << std::endl;;
        return;
    }

    const char *ptr = buffer.get();
    const char *end = ptr + size;

    // skip first empty line
    float data[8];
    ptr = split2(ptr, end, data);

    int chunks = std::thread::hardware_concurrency();
    if (chunks == 0) {
        chunks = 2;
    }
    std::cout << fmt::format("Reading particles..") << std::endl;;
    size_t chunkLines = particleCount / chunks;
    if (chunkLines == 0) {
        chunks = 1;
        chunkLines = particleCount;
    }
    ProgressReporter progressReporter = ProgressReporter::createLoggingProgressReporter(chunks);
    std::vector<std::thread> threads;
    for (int i = 0; i < chunks; ++i) {
        const char *chunkBegin = ptr;
        size_t lines = 0;
        const char *lineBeginPtr = ptr;
        while (ptr < end) {
            if (*ptr == '\n') {
                if (ptr - lineBeginPtr > 3) {
                    ++lines;
                    if (lines >= chunkLines && i != chunks - 1) {
                        break;
                    }
                }
                ++ptr;
                lineBeginPtr = ptr;
                continue;
            }
            ++ptr;
        }
        size_t startIndex = i * chunkLines;
        threads.push_back(std::thread([chunkBegin, ptr, &particleContainer,
                                       startIndex, positionOnly, positionAndRgb,
                                       positionRemissionAndRgb,
                                       positionRemissionQualityAndRgb]() {
          processChunk(chunkBegin, ptr, particleContainer, startIndex,
                       positionOnly, positionAndRgb, positionRemissionAndRgb,
                       positionRemissionQualityAndRgb);
        }));
    }
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
        progressReporter.iterationDone();
    }

    std::cout << fmt::format("Reading done") << std::endl;;
    std::cout << fmt::format("Particle count: {}", particleContainer.particleCount()) << std::endl;
}

std::string PtsParticleReader::getReaderName() {
    return "pts";
}
