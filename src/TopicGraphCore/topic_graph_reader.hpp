#pragma once

#include <vector>


class Edge;
class Topic;

class ITopicGraphReader {
public:
    virtual ~ITopicGraphReader() = default;

    virtual std::shared_ptr<const Topic> getTopic(uint32_t id) const = 0;
    virtual std::shared_ptr<const Topic> getTopic(const std::string &name) const = 0;

    virtual bool hasEdge(uint32_t from, uint32_t to) const = 0;
    virtual std::vector<Edge> getOutEdges(uint32_t from) const = 0;

    virtual std::shared_ptr<const Topic> operator[](size_t index) const = 0;


    virtual size_t numEdges() const = 0;
    virtual size_t numTopics() const = 0;
};