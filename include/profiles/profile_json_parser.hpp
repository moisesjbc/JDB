/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

    This file is part of sandwiches-game.

    sandwiches-game is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sandwiches-game is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sandwiches-game.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef PROFILE_JSON_PARSER_HPP
#define PROFILE_JSON_PARSER_HPP

#include <profiles/profile.hpp>
#include <utilities/json.hpp>
using json = nlohmann::json;

namespace jdb {

class ProfileJSONParser
{
    public:
        /***
         * Writting to JSON
         ***/
        json writeToJSON(Profile& profile);
        void writeToJSON(Profile& profile, std::string filePath);


        /***
         * Reading from JSON
         ***/
        Profile readFromJSON(json profileJSON);
        Profile readFromJSON(std::string filePath);
};

} // namespace jdb

#endif // PROFILE_JSON_PARSER_HPP
