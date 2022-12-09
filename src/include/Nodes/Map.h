#pragma once


#include "Node.h"
#include <map>

class Map : public Node {
private:
    glm::vec2 size;

public:
    Map(const std::string &path, const std::map<char, class Node *> &Nodes);

    const glm::vec2 &GetSize() const;
};
