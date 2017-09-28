//
// Created by david on 28/09/17.
//

#include <iostream>
#include "../../headers/game/MapParser.hpp"

using namespace std;
using namespace rapidjson;

MapParser::MapParser(const std::string &content) :
mContent(content)
{

}

void MapParser::getData() {
    Document d;
    d.Parse(mContent.c_str());


    getAuthorData(d);
    getDimensionsData(d);

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