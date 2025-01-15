#include "CameraTour.hpp"
#include <fstream>

namespace gps
{
    void CameraTour::sampleCameraPosition(const glm::vec3 &position, const glm::vec3 &front)
    {
        positions.emplace_back(position, front);
    }

    void CameraTour::saveCameraPositionsToFile(const std::string &filename)
    {
        if (std::ofstream file(filename); file.is_open())
        {
            for (const auto &pos : positions)
            {
                file << pos.first.x << " " << pos.first.y << " " << pos.first.z << " "
                     << pos.second.x << " " << pos.second.y << " " << pos.second.z << "\n";
            }
            file.close();
        }
    }

    void CameraTour::loadCameraPositionsFromFile(const std::string &filename)
    {
        positions.clear();
        if (std::ifstream file(filename); file.is_open())
        {
            glm::vec3 pos, front;
            while (file >> pos.x >> pos.y >> pos.z >> front.x >> front.y >> front.z)
            {
                positions.emplace_back(pos, front);
            }
            file.close();
        }
    }

    void CameraTour::startTour()
    {
        tourActive = true;
        elapsedTime = 0.0f;
        currentIndex = 0;
    }

    void CameraTour::stopTour()
    {
        tourActive = false;
    }

    void CameraTour::updateTour(float deltaTime, gps::Camera &camera, float duration)
    {
        if (!tourActive || currentIndex >= positions.size() - 1)
        {
            stopTour();
            return;
        }

        elapsedTime += deltaTime;
        float t = elapsedTime / duration;
        if (t >= 1.0f)
        {
            t = 0.0f;
            elapsedTime = 0.0f;
            currentIndex++;
        }

        if (currentIndex < positions.size() - 1)
        {
            glm::vec3 interpolatedPos = interpolate(positions[currentIndex].first, positions[currentIndex + 1].first, t);
            glm::vec3 interpolatedFront = interpolate(positions[currentIndex].second, positions[currentIndex + 1].second, t);
            camera.setPosition(interpolatedPos);
            camera.setFront(interpolatedFront);
        }
    }

    glm::vec3 CameraTour::interpolate(const glm::vec3 &start, const glm::vec3 &end, float t)
    {
        return start + t * (end - start);
    }
}