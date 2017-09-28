//
// Created by david on 28/09/17.
//

#ifndef DEADVINT_2_MAPPARSER_HPP
#define DEADVINT_2_MAPPARSER_HPP

#include "../rapidjson/document.h"
#include <string>

class MapParser {
    MapParser(const std::string& content);

    void getData();

private:
    void getAuthorData(rapidjson::Value& v);
    void getDimensionsData(rapidjson::Value& v);

private:
    std::string mContent;

    std::string mAuthor;
    int mWidth, mHeight;
};

#endif //DEADVINT_2_MAPPARSER_HPP
