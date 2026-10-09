#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

// Represents a single campus resource (study room, laptop, calculator,
// lab equipment, tutoring appointment, etc.)
class Resource {
private:
    std::string resourceID;
    std::string resourceName;
    std::string resourceType;
    bool available;

public:
    Resource();
    Resource(const std::string& id, const std::string& name,
              const std::string& type, bool isAvailable);

    // Getters
    std::string getID() const;
    std::string getName() const;
    std::string getType() const;
    bool isAvailable() const;

    // Setters
    void setAvailable(bool isAvailable);

    // Prints a single formatted line describing this resource.
    void display() const;
};

#endif // RESOURCE_H
