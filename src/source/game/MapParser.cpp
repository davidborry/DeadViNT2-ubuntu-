//
// Created by david on 28/09/17.
//

#include <iostream>
#include "../../headers/game/MapParser.hpp"

using namespace std;
using namespace rapidjson;

MapParser::MapParser(const std::string &content) :
mContent(content),
mSpawnPoint(0,0),
mObstacles()
{

}

void MapParser::getData() {
    Document d;
    d.Parse(mContent.c_str());

    cout << "CONTENT: " << mContent << endl;

    getAuthorData(d);
    getDimensionsData(d);
    getSpawnPointData(d);
    getObjectsData(d);

    /* for (SizeType i = 0; i < array.Size(); i++) {// rapidjson uses SizeType instead of size_t.
         auto point = array[i].GetArray();
         for (SizeType j = 0; j < point.Size(); j++){
             std::cout <<point[j].GetInt() <<" ";
         }
     }*/
}

void MapParser::getAuthorData(Value &v) {
    Value& author = v["author"];

    mAuthor = author.GetString();
    cout << mAuthor << endl;

}

void MapParser::getDimensionsData(Value& v) {
    Value& dimensions = v["dimensions"];
    mWidth = dimensions["width"].GetInt();
    mHeight = dimensions["height"].GetInt();

    cout << "DIMS: " << mWidth << " x " << mHeight << endl;
}

void MapParser::getSpawnPointData(Value &v) {
    Value& spawn = v["spawn"];

    mSpawnPoint = Point(spawn[0].GetInt(), spawn[1].GetInt());

    cout << "SPAWN: " << mSpawnPoint.x << ";" << mSpawnPoint.y << endl;
}

void MapParser::getObjectsData(Value &v) {
    Value& objects = v["objects"];

    getObstaclesData(objects);
    getFencesData(objects);
}

void MapParser::getObstaclesData(Value &v) {
    Value& obstacles = v["obstacles"];

    for(SizeType i = 0; i < obstacles.Size(); i++){
        Point a(obstacles[i][0].GetInt(), obstacles[i][1].GetInt());
        mObstacles.push_back(a);
        cout << "OBSTACLE: " <<a.x << ", " << a.y << endl;
    }
}


void MapParser::getFencesData(Value &v) {
    Value& fences = v["fences"];

    for(SizeType i = 0; i < fences.Size(); i++){
        Point a(fences[i][0].GetInt(), fences[i][1].GetInt());
        mFences.push_back(a);
        cout << "FENCES: " <<a.x << ", " << a.y << endl;
    }
}

string MapParser::getAuthor() const {
    return mAuthor;
}

int MapParser::getWidth() const {
    return mWidth;
}

int MapParser::getHeight() const {
    return mHeight;
}

Point MapParser::getSpawnPoint() const {
    return mSpawnPoint;
}

vector<Point> MapParser::getObstacles() const {
    return mObstacles;
}

vector<Point> MapParser::getFences() const {
    return mFences;
}