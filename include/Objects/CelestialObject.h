#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Rendering/Model.h"
#include "Meshes/TrajectoryMesh.h"
#include "Rendering/Shader.h"
#include "Utils/Vector.h"

enum CelestialType
{
    STAR,
    PLANET,
    MOON
};

class CelestialObject
{
private:
    std::string name;
    CelestialType objType;
    Model objectModel;
    TrajectoryMesh trajectory, netForceVector, directionVector;
    glm::mat4 model;
    glm::vec3 color;
    Vector prevPosition, position, velocity, acceleration, netForces;
    largeFloat mass, radius;
    Vector h;
    largeFloat kineticEnergy, potentialEnergy, totalEnergy;
    largeFloat e, a, i, rp, ra, circularVel, escapeVel, orbitalPeriod;

    std::vector<Vertex> trajectoryVertices, netForceVertices, directionVertices;
    std::atomic<bool> threadRunning;

    float intensity, angularVelocity, rotationAngle, tilt;
    float scaledRadiusFactor, scaledDistanceFactor;

    bool forceVector, trajectoryVector, dirVector;

public:
    static largeFloat gConstant;

    CelestialObject();
    ~CelestialObject();
    CelestialObject(Model m, CelestialType objType, Vector cameraPos, Vector cameraDir, float scaledRadiusFactor, float scaledDistanceFactor);
    CelestialObject(const CelestialObject *obj, Vector cameraPos, Vector cameraDir, float scaledRadiusFactor, float scaledDistanceFactor);
    CelestialObject(std::string modelString, const std::string &name, CelestialType objType, const Vector &position, largeFloat mass, largeFloat radius, float scaledRadiusFactor, float scaledDistanceFactor, Vector velocity = Vector(0), float tilt = 0.0, float angularVelocity = 0.0);
    void CelestialObject::startTrajectoryThread(std::vector<CelestialObject *> &objects, float alpha, float timeSpeed);
    void getTrajectory(std::vector<CelestialObject *> &objects, float alpha, float timeSpeed);
    Vector getNetForces() const;
    void setNetForceVector();
    void setDirectionVector();
    void addForce(Vector force);
    void addVelocity(Vector acceleration);
    void addPosition(Vector pos);
    void setPosition(Vector pos);
    void setAcceleration(Vector acc);
    void setOrbitalValues(CelestialObject *centralBody);
    void setEnergyValues(std::vector<CelestialObject *> objects);
    std::string getName() const;
    CelestialType getObjType() const;
    std::string getObjTypeStr() const;
    Model getModel() const;
    float getIntensity() const;
    Vector getVelocity() const;
    Vector getPosition() const;
    glm::mat4 getModelMatrix(float alpha);
    Vector getRenderPos(float alpha) const;
    Vector getAcceleration() const;
    largeFloat getSurfaceGravity() const;
    largeFloat getSurfaceEscapeVelocity() const;
    largeFloat getEscapeVelocity() const;
    largeFloat getCircularVelocity() const;
    float getAngularVelocity() const;
    float getTilt() const;
    largeFloat getMass() const;
    largeFloat getRadius() const;
    Vector getAngularMomentum() const;
    largeFloat getEccentricity() const;
    largeFloat getSemiMajorAxis() const;
    largeFloat getInclination() const;
    largeFloat getApoapsis() const;
    largeFloat getPeriapsis() const;
    largeFloat getOrbitalPeriod() const;
    largeFloat getRotationalPeriod() const;
    largeFloat getPotentialEnergy() const;
    largeFloat getKineticEnergy() const;
    largeFloat getTotalEnergy() const;
    void setDirection(Vector direction);
    void updateObject(std::string name, largeFloat radius, largeFloat mass, largeFloat velocity, float tilt, float rotPeriod, float intensity);
    bool isOrbiting() const;
    glm::vec3 getColor() const;
    bool isNetForceVector() const;
    bool isTrajectoryVector() const;
    void toggleNetForceVector();
    void toggleTrajectoryVector();
    void toggleDirectionVector(bool dirVec);

    void updatePosition(float dt);
    void updateForce(const CelestialObject *object);
    void updateVelocity(float dt);

    void drawTrajectory(Shader &shader) const;
    void drawNetForceVector(Shader &shader) const;
    void drawDirectionVector(Shader &shader) const;

    void drawModel(Shader &shader, float alpha, bool depthDraw = false, bool highlight = false, float thickness = 0.0);
};
