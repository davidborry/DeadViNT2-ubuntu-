//
// Created by david on 28/09/17.
//

#ifndef DEADVINT_2_MAPPARSER_HPP
#define DEADVINT_2_MAPPARSER_HPP

#include "../rapidjson/document.h"
#include "../util/Utility.hpp"
#include <string>

class MapParser {
public:
    MapParser(const std::string& content);

    void getData();

    std::string getAuthor() const;
    int getWidth() const;
    int getHeight() const;
    Point getSpawnPoint() const;

    std::vector<Point> getObstacles() const;
    std::vector<Point> getFences() const;

private:
    void getAuthorData(rapidjson::Value& v);
    void getDimensionsData(rapidjson::Value& v);
    void getSpawnPointData(rapidjson::Value& v);
    void getObjectsData(rapidjson::Value& v);

    void getObstaclesData(rapidjson::Value& v);
    void getFencesData(rapidjson::Value& v);

private:
    std::string mContent;

    std::string mAuthor;
    int mWidth, mHeight;
    Point mSpawnPoint;
    std::vector<Point> mObstacles;
    std::vector<Point> mFences;
};

#endif //DEADVINT_2_MAPPARSER_HPP
