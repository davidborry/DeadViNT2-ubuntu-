//
// Created by david on 28/09/17.
//


#include <fstream>
#include <iostream>
#include "../../headers/game/MapReader.hpp"

using namespace std;
using namespace rapidjson;

void MapReader::load(const string& filename) {
    mFilename = filename;

    ifstream file(filename);
    string content;

    file.seekg(0, ios::end);
    content.reserve(file.tellg());
    file.seekg(0, ios::beg);

    content.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    mContent = content;
}

string MapReader::getContent() const {
    return mContent;
}
