#include "Objects/CelestialObject.h"

largeFloat CelestialObject::gConstant = 6.67 * pow(10, -11);

CelestialObject::CelestialObject()
{
}

CelestialObject::CelestialObject(Model m, CelestialType objType, Vector cameraPos, Vector cameraDir, float scaledRadiusFactor, float scaledDistanceFactor) : name(""), objType(objType), netForces(Vector(0)), acceleration(Vector(0)), tilt(0), scaledRadiusFactor(scaledRadiusFactor), scaledDistanceFactor(scaledDistanceFactor)
{
    if (objType == STAR)
    {
        this->mass = pow(10, 30);
        this->radius = 500000.0f;
    }
    if (objType == PLANET)
    {
        this->mass = pow(10, 24);
        this->radius = 10000.0f;
    }
    if (objType == MOON)
    {
        this->mass = pow(10, 18);
        this->radius = 1000.0f;
    }

    this->radius *= 1000;
    this->position = cameraPos + (cameraDir * static_cast<largeFloat>(this->radius * scaledRadiusFactor * scaledDistanceFactor * 5));
    this->prevPosition = this->position;
    this->intensity = 2.0;

    this->velocity = Vector();
    this->model = glm::mat4(1.0f);
    this->objectModel = m;
    this->color = glm::vec3(1.0f);

    this->tilt = 0.0;
    this->rotationAngle = 0.0;
    this->angularVelocity = 0.0;

    threadRunning = false;
    forceVector = false;
    trajectoryVector = false;
    dirVector = false;
}

CelestialObject::CelestialObject(const CelestialObject *obj, Vector cameraPos, Vector cameraDir, float scaledRadiusFactor, float scaledDistanceFactor)
    : name(obj->getName()), objType(obj->getObjType()), mass(obj->getMass()), radius(obj->getRadius()), netForces(Vector(0)), velocity(Vector(0)), acceleration(Vector(0)), scaledRadiusFactor(scaledRadiusFactor), scaledDistanceFactor(scaledDistanceFactor), tilt(glm::radians(obj->getTilt()))
{
    this->position = cameraPos + (cameraDir * static_cast<largeFloat>(this->radius * scaledRadiusFactor * scaledDistanceFactor * 5));
    this->prevPosition = this->position;

    this->model = glm::mat4(1.0f);
    this->objectModel = obj->getModel();
    this->color = obj->getColor();
    this->intensity = obj->getIntensity();

    this->rotationAngle = 0;
    this->angularVelocity = glm::radians(obj->getAngularVelocity());

    threadRunning = false;
    forceVector = false;
    trajectoryVector = false;
    dirVector = false;
}

CelestialObject::CelestialObject(std::string modelString, const std::string &name, CelestialType objType, const Vector &position, largeFloat mass, largeFloat radius, float scaledRadiusFactor, float scaledDistanceFactor, Vector velocity, float tilt, float angularVelocity)
    : name(name), objType(objType), prevPosition(Vector(0)), position(position * 1000), mass(mass), radius(radius * 1000), netForces(Vector(0)), velocity(velocity * 1000), acceleration(Vector(0)), scaledRadiusFactor(scaledRadiusFactor), scaledDistanceFactor(scaledDistanceFactor), tilt(glm::radians(tilt))
{
    this->model = glm::mat4(1.0f);
    this->objectModel = Model(modelString);
    this->color = glm::vec3(1.0f);
    this->intensity = 2.0;

    this->rotationAngle = 0;
    this->angularVelocity = angularVelocity;

    threadRunning = false;
    forceVector = false;
    trajectoryVector = false;
    dirVector = false;

    this->objectModel.upload();
}

CelestialObject::~CelestialObject()
{
    objectModel.cleanup();
    trajectory.cleanup();
}

void CelestialObject::updatePosition(float dt)
{
    prevPosition = position;
    position += (velocity * dt) + (acceleration * 0.5 * dt * dt);
    netForces = largeFloat(0);

    rotationAngle += angularVelocity * dt;
    rotationAngle = fmod(rotationAngle, glm::two_pi<float>());
}

void CelestialObject::updateForce(const CelestialObject *other)
{
    largeFloat distance = (other->getPosition() - position).magnitude;
    Vector direction = (other->getPosition() - position).normalized();
    Vector force = direction * ((gConstant * mass * other->getMass()) / (distance * distance));
    netForces += force;
}

void CelestialObject::updateVelocity(float dt)
{
    setNetForceVector();

    Vector newAcc = (netForces / mass);
    velocity += (acceleration + newAcc) * dt * 0.5;
    acceleration = newAcc;
}

void CelestialObject::setNetForceVector()
{
    auto addVertex = [&](Vector vertex)
    {
        Vertex pos;
        pos.Position = glm::vec3(vertex.x, vertex.y, vertex.z);
        this->netForceVertices.push_back(pos);
    };

    netForceVertices.clear();
    Vector vertex1 = (position / scaledDistanceFactor) + (netForces.normalized() * radius * scaledRadiusFactor);
    Vector vertex2 = vertex1 + (netForces.normalized() * 100);
    addVertex(vertex1);
    addVertex(vertex2);

    Vector ref;
    Vector d = netForces.normalized();
    ref = (abs(d.x) < 0.999f) ? Vector(0, 1, 0) : Vector(1, 0, 0);

    Vector r = (d.cross(ref)).normalized();
    Vector u = (d.cross(r)).normalized();
    Vector p0 = (vertex2) + (d * 3);
    Vector p1 = (vertex2) + (d * -6 + r * 3);
    Vector p2 = (vertex2) + (d * -6 + r * -3);

    addVertex(p0);
    addVertex(p1);
    addVertex(p0);
    addVertex(p2);

    netForceVector.setVertices(netForceVertices);
}

void CelestialObject::setDirectionVector()
{
    auto addVertex = [&](Vector vertex)
    {
        Vertex pos;
        pos.Position = glm::vec3(vertex.x, vertex.y, vertex.z);
        this->directionVertices.push_back(pos);
    };

    directionVertices.clear();
    Vector vertex1 = (position / scaledDistanceFactor) + (velocity.normalized() * radius * scaledRadiusFactor);
    Vector vertex2 = vertex1 + (velocity.normalized() * 500);
    addVertex(vertex1);
    addVertex(vertex2);

    Vector ref;
    Vector d = velocity.normalized();
    ref = (abs(d.x) < 0.999f) ? Vector(0, 1, 0) : Vector(1, 0, 0);

    Vector r = (d.cross(ref)).normalized();
    Vector u = (d.cross(r)).normalized();
    Vector p0 = (vertex2) + (d * 3);
    Vector p1 = (vertex2) + (d * -6 + r * 3);
    Vector p2 = (vertex2) + (d * -6 + r * -3);

    addVertex(p0);
    addVertex(p1);
    addVertex(p0);
    addVertex(p2);

    directionVector.setVertices(directionVertices);
}

void CelestialObject::startTrajectoryThread(std::vector<CelestialObject *> &objects, float alpha, float timeSpeed)
{
    if (!threadRunning && trajectoryVector)
    {
        if (trajectoryVertices.size() > 0)
            trajectory.setVertices(trajectoryVertices);
        std::thread trajectoryThread(&CelestialObject::getTrajectory, this, objects, alpha, timeSpeed);
        trajectoryThread.detach();
    }
    else
    {
        trajectoryVertices.clear();
        trajectory.setVertices(trajectoryVertices);
    }
}

void CelestialObject::getTrajectory(std::vector<CelestialObject *> &objects, float alpha, float timeSpeed)
{
    std::vector<Vertex> vertices;
    Vector forces, pos, prevPos, vel, acc;
    bool stop = false;

    forces = largeFloat(0);
    float totalLength = 0;
    prevPos = prevPosition;
    pos = position;
    vel = velocity;
    acc = acceleration;

    auto addVertex = [&vertices](Vector vertex)
    {
        Vertex pos;
        pos.Position = glm::vec3(vertex.x, vertex.y, vertex.z);
        vertices.push_back(pos);
    };

    float dt = 20000;
    for (int i = 0; i < 200; i++)
    {
        prevPos = pos;
        pos += (vel * dt) + (acc * 0.5 * dt * dt);

        for (const CelestialObject *obj : objects)
        {
            if (this != obj)
            {
                if (pos.distance(obj->getPosition()) / scaledDistanceFactor < obj->getRadius() * scaledRadiusFactor)
                {
                    stop = true;
                    break;
                }

                largeFloat distance = (obj->getPosition() - pos).magnitude;
                Vector direction = (obj->getPosition() - pos).normalized();
                Vector force = direction * ((gConstant * getMass() * obj->getMass()) / (distance * distance));
                forces += force;
            }
        }

        Vector newAcc = (forces / mass);
        vel += (acc + newAcc) * dt * 0.5;
        acc = newAcc;
        forces = largeFloat(0);

        totalLength += static_cast<float>(pos.distance(prevPos) / scaledDistanceFactor);

        if (stop || totalLength >= 1000)
        {
            if (vertices.size() > 1 && vertices.size() % 2 != 0)
            {
                vertices.pop_back();
                vertices.pop_back();
                vertices.pop_back();
            }
            break;
        }

        if (getPosition().distance(pos) >= getRadius() && i > 9)
            addVertex(pos / scaledDistanceFactor);
    }

    if (!stop)
    {
        Vector ref;
        Vector d = (pos - prevPos).normalized();
        ref = (abs(d.x) < 0.999f) ? Vector(0, 1, 0) : Vector(1, 0, 0);

        Vector r = (d.cross(ref)).normalized();
        Vector u = (d.cross(r)).normalized();
        Vector p0 = (pos / scaledDistanceFactor) + (d * 3);
        Vector p1 = (pos / scaledDistanceFactor) + (d * -6 + r * 3);
        Vector p2 = (pos / scaledDistanceFactor) + (d * -6 + r * -3);

        addVertex(p0);
        addVertex(p1);
        addVertex(p0);
        addVertex(p2);
    }

    trajectoryVertices = vertices;

    threadRunning = false;
}

Vector CelestialObject::getNetForces() const
{
    return netForces;
}

void CelestialObject::addForce(Vector force)
{
    netForces += force;
}

void CelestialObject::addVelocity(Vector acceleration)
{
    velocity += acceleration;
}

void CelestialObject::addPosition(Vector pos)
{
    prevPosition = position;
    position += pos;
}

void CelestialObject::setPosition(Vector pos)
{
    prevPosition = position;
    position = pos;
}

void CelestialObject::setAcceleration(Vector acc)
{
    acceleration = acc;
}

void CelestialObject::setOrbitalValues(CelestialObject *centralBody)
{
    if (centralBody == nullptr)
    {
        orbitalPeriod = NAN;
        i = NAN;
        a = NAN;
        rp = NAN;
        ra = NAN;
        return;
    }

    Vector r = (position - centralBody->getPosition());
    Vector v = (velocity - centralBody->getVelocity());

    largeFloat mu = gConstant * (centralBody->getMass());

    // Angular momentum
    h = r.cross(v);

    // Eccentricity
    Vector eVec = (v.cross(h) / mu) - r.normalized();
    e = eVec.magnitude;

    largeFloat energy =
        (v.dot(v) * 0.5) - (mu / r.magnitude);

    // Semi-major axis
    a = -mu / (2.0 * energy);

    // Inclination
    largeFloat cos_i = h.y / h.magnitude;
    if (cos_i > 1.0)
        cos_i = 1.0;
    if (cos_i < -1.0)
        cos_i = -1.0;
    i = glm::degrees(acos(static_cast<float>(cos_i)));

    if (!isOrbiting())
    {
        i = NAN;
        a = NAN;
        rp = NAN;
        ra = NAN;
        orbitalPeriod = NAN;
    }
    else
    {
        rp = a * (1 - e);
        ra = a * (1 + e);
        orbitalPeriod = 2 * glm::pi<float>() * sqrt(pow(a, 3) / mu);
    }

    escapeVel = sqrt((2 * gConstant * centralBody->getMass()) / r.magnitude);
    circularVel = sqrt((gConstant * centralBody->getMass()) / r.magnitude);
}

void CelestialObject::setEnergyValues(std::vector<CelestialObject *> objects)
{
    kineticEnergy = mass * (velocity * velocity).magnitude / 2.0;
    potentialEnergy = 0;

    for (CelestialObject *obj : objects)
    {
        if (this != obj)
        {
            largeFloat distance = (obj->getPosition() - position).magnitude;
            potentialEnergy -= (gConstant * mass * obj->mass) / distance;
        }
    }

    totalEnergy = kineticEnergy + potentialEnergy;
}

std::string CelestialObject::getName() const
{
    return name;
}

CelestialType CelestialObject::getObjType() const
{
    return objType;
}

std::string CelestialObject::getObjTypeStr() const
{
    if (objType == STAR)
        return "Star";
    if (objType == PLANET)
        return "Planet";
    if (objType == MOON)
        return "Moon";

    return "";
}

Model CelestialObject::getModel() const
{
    return objectModel;
}

Vector CelestialObject::getPosition() const
{
    return position;
}

glm::mat4 CelestialObject::getModelMatrix(float alpha)
{
    glm::vec3 renderPos = glm::vec3(((prevPosition * (1 - alpha) + position * alpha) / scaledDistanceFactor).getGLM());
    model = glm::translate(glm::mat4(1.0f), renderPos);

    float scale = (float)(radius * scaledRadiusFactor);
    model = glm::scale(model, glm::vec3(scale));

    return model;
}

Vector CelestialObject::getRenderPos(float alpha) const
{
    return (prevPosition * (1 - alpha) + position * alpha) / scaledDistanceFactor;
}

float CelestialObject::getIntensity() const
{
    return intensity;
}

Vector CelestialObject::getVelocity() const
{
    return velocity;
}

Vector CelestialObject::getAcceleration() const
{
    return acceleration;
}

largeFloat CelestialObject::getSurfaceGravity() const
{
    return ((gConstant * mass) / ((radius) * (radius)));
}

largeFloat CelestialObject::getSurfaceEscapeVelocity() const
{
    return sqrt((2 * gConstant * mass) / (radius));
}

largeFloat CelestialObject::getEscapeVelocity() const
{
    return escapeVel;
}

largeFloat CelestialObject::getCircularVelocity() const
{
    return circularVel;
}

float CelestialObject::getAngularVelocity() const
{
    return glm::degrees(angularVelocity);
}

float CelestialObject::getTilt() const
{
    return glm::degrees(tilt);
}

largeFloat CelestialObject::getMass() const
{
    return mass;
}

Vector CelestialObject::getAngularMomentum() const
{
    return h;
}

largeFloat CelestialObject::getEccentricity() const
{
    return e;
}

largeFloat CelestialObject::getSemiMajorAxis() const
{
    return a;
}

largeFloat CelestialObject::getInclination() const
{
    return i;
}

largeFloat CelestialObject::getApoapsis() const
{
    return ra;
}

largeFloat CelestialObject::getPeriapsis() const
{
    return rp;
}

largeFloat CelestialObject::getPotentialEnergy() const
{
    return potentialEnergy;
}

largeFloat CelestialObject::getKineticEnergy() const
{
    return kineticEnergy;
}

largeFloat CelestialObject::getTotalEnergy() const
{
    return totalEnergy;
}

largeFloat CelestialObject::getOrbitalPeriod() const
{
    return orbitalPeriod;
}

largeFloat CelestialObject::getRotationalPeriod() const
{
    if (angularVelocity == 0)
        return 0;

    return abs((glm::two_pi<float>()) / angularVelocity);
}

void CelestialObject::setDirection(Vector direction)
{
    this->velocity = direction.normalized() * velocity.magnitude;
}

void CelestialObject::updateObject(std::string name, largeFloat radius, largeFloat mass, largeFloat velocity, float tilt, float rotPeriod, float intensity)
{
    this->name = name;
    this->radius = radius * 1000;
    this->mass = mass;
    this->velocity = (this->velocity.normalized().magnitude != 0) ? this->velocity.normalized() * static_cast<largeFloat>(velocity) : Vector(0, 0, 1) * static_cast<largeFloat>(velocity);
    this->tilt = glm::radians(tilt);
    if (rotPeriod != 0)
        this->angularVelocity = glm::two_pi<float>() / rotPeriod;
    else
        this->angularVelocity = 0;

    intensity = (intensity > 50) ? 50 : intensity;
    intensity = (intensity < 1) ? 1 : intensity;
    this->intensity = intensity;
}

bool CelestialObject::isOrbiting() const
{
    return !(e > 1.0 || a < 0 || std::isnan(a));
}

glm::vec3 CelestialObject::getColor() const
{
    return color;
}

bool CelestialObject::isNetForceVector() const
{
    return forceVector;
}

bool CelestialObject::isTrajectoryVector() const
{
    return trajectoryVector;
}

void CelestialObject::toggleNetForceVector()
{
    forceVector = !forceVector;
}

void CelestialObject::toggleTrajectoryVector()
{
    trajectoryVector = !trajectoryVector;
}

void CelestialObject::toggleDirectionVector(bool dirVec)
{
    dirVector = dirVec;
}

largeFloat CelestialObject::getRadius() const
{
    return radius;
}

void CelestialObject::drawTrajectory(Shader &shader) const
{
    trajectory.draw(shader);
}

void CelestialObject::drawNetForceVector(Shader &shader) const
{
    if (forceVector)
        netForceVector.draw(shader);
}

void CelestialObject::drawDirectionVector(Shader &shader) const
{
    if (dirVector)
        directionVector.draw(shader);
}

void CelestialObject::drawModel(Shader &shader, float alpha, bool depthDraw, bool highlight, float thickness)
{
    glm::vec3 renderPos = glm::vec3(((prevPosition * (1 - alpha) + position * alpha) / scaledDistanceFactor).getGLM());
    model = glm::translate(glm::mat4(1.0f), renderPos);

    model = glm::rotate(model, tilt, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotationAngle, glm::vec3(0, 1, 0));

    float scale = (float)(radius * scaledRadiusFactor);
    if (highlight)
        scale *= (thickness);

    model = glm::scale(model, glm::vec3(scale / objectModel.getModelRadius()));

    shader.use();
    shader.setMat4("model", model);

    objectModel.draw(shader, depthDraw);
}
