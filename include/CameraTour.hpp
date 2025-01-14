#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <Camera.hpp>

namespace gps
{
    class CameraTour
    {
    public:
        void sampleCameraPosition(const glm::vec3 &position, const glm::vec3 &front);
        void saveCameraPositionsToFile(const std::string &filename);
        void loadCameraPositionsFromFile(const std::string &filename);
        void startTour();
        void stopTour();
        void updateTour(float deltaTime, gps::Camera &camera, float duration);
        [[nodiscard]] bool isTourActive() const { return tourActive; }
        [[nodiscard]] bool isEmpty() const { return positions.empty(); }

    private:
        std::vector<std::pair<glm::vec3, glm::vec3>> positions;
        bool tourActive = false;
        float elapsedTime = 0.0f;
        size_t currentIndex = 0;

        glm::vec3 interpolate(const glm::vec3 &start, const glm::vec3 &end, float t);
    };
}