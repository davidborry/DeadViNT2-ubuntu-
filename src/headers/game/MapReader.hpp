//
// Created by david on 28/09/17.
//

#ifndef DEADVINT_2_MAPREADER_HPP
#define DEADVINT_2_MAPREADER_HPP

#include <string>
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"


class MapReader {
public:

    void load(const std::string& filename);

    std::string getContent() const;

private:
    std::string mFilename;
    std::string mContent;

};

#endif //DEADVINT_2_MAPREADER_HPP
