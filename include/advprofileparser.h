/*
 * advprofileparser.h
 *
 *  Created on: Apr 2, 2020
 *      Author: duyi
 */

#ifndef INCLUDE_ADVPROFILEPARSER_H_
#define INCLUDE_ADVPROFILEPARSER_H_

#include "jsonnode.h"
#include <algorithm>
#include <set>

class AdvProfileParser
{
    public:

        typedef std::map<std::string, JsonNode> AdvProfileObject;

        AdvProfileParser()
        {
        }

        virtual ~AdvProfileParser()
        {
        }

        static AdvProfileParser *Instance()
        {
            static AdvProfileParser instance;
            return &instance;
        }

        JsonNode &operator [](const std::string
                              keyOfJsonNode) //a reference must be returned here instead of a value to access the same object
        {
            if (m_profileParserObject.find(keyOfJsonNode) ==
                m_profileParserObject.end()) { //keyOfJsonNode not in the map
                JsonNode jsonNode;
                m_profileParserObject[keyOfJsonNode] =
                    jsonNode;//std::map::insert() does not insert your current object into the container, but a new element.
            }

            return m_profileParserObject[keyOfJsonNode];
        }

        int fromFile()
        {
            std::ifstream inFile(profileLocation().c_str());
            string inAll((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
            picojson::value v;
            inFile.close();
            std::string errStr;

            try {
                errStr = picojson::parse(v, inAll);
            } catch (...) {
                return PARSE_ERR;
            }

            if (!errStr.empty()) {
                return PARSE_ERR;
            } else {
                return fromJson(v);
            }
        }

        virtual int fromJson(picojson::value v)//load json to json node
        {
            picojson::value valueOfTheWhole;

            try {
                if (v.contains(KeyOfTheWhole())) { //in case KeyOfTheWhole doesn't exist.
                    valueOfTheWhole = v.get(KeyOfTheWhole());
                } else {
                    return USER_DEFINED_FORMAT_ERR;
                }

                if (!valueOfTheWhole.is<picojson::object>()) {
                    return USER_DEFINED_FORMAT_ERR;
                }

                picojson::object advElements = valueOfTheWhole.get<picojson::object>();

                for (picojson::object::const_iterator it = advElements.begin();
                     it != advElements.end(); ++it) {
                    if (it->second.is<picojson::object>()) {
                        JsonNode jsonNode(it->second);
                        m_profileParserObject[it->first] = jsonNode;
                    } else {
                        cerr << "One of elements is not an object." << endl;
                    }
                }
            } catch (...) {
                return PARSE_ERR;
            }

            return SUCCEED;
        }

        virtual picojson::value toJson()//compose json with JsonNode
        {
            picojson::object object;
            picojson::object theWhole;

            try {
                for (AdvProfileObject::iterator it = m_profileParserObject.begin();
                     it != m_profileParserObject.end(); ++it) {
                    object[it->first] = it->second.to();
                }

                theWhole[KeyOfTheWhole()] = picojson::value(object);
            } catch (...) {
                return picojson::value();
            }

            return picojson::value(theWhole);
        }

        bool fromString(const string advProfile, picojson::value &v)
        {
            std::string errStr;

            try {
                errStr = picojson::parse(v, advProfile);
            } catch (...) {
                return false;
            }

            if (!errStr.empty()) {
                return false;
            }

            return true;
        }

        virtual int merge(const string advProfile)
        {
            picojson::value v;

            if (!fromString(advProfile, v)) {
                return PARSE_ERR;
            }

            picojson::value valueOfTheWhole;

            try {
                if (v.contains(KeyOfTheWhole())) { //in case KeyOfTheWhole doesn't exist.
                    valueOfTheWhole = v.get(KeyOfTheWhole());
                } else {
                    return USER_DEFINED_FORMAT_ERR;
                }

                if (!valueOfTheWhole.is<picojson::object>()) {
                    return USER_DEFINED_FORMAT_ERR;
                }

                picojson::object advElements = valueOfTheWhole.get<picojson::object>();

                for (picojson::object::const_iterator it = advElements.begin();
                     it != advElements.end(); ++it) {
                    if (it->second.is<picojson::object>()) {
                        JsonNode jsonNode(it->second);
                        m_profileParserObject[it->first] = jsonNode;
                    } else {
                        return USER_DEFINED_FORMAT_ERR;
                    }
                }
            } catch (...) {
                return PARSE_ERR;
            }

            return SUCCEED;
        }

        static std::string profileLocation()
        {
            return std::string("/tmp/vconnect/advancedInfo.json");
        }

        static std::string KeyOfTheWhole()
        {
            return std::string("advancedInfo");
        }

    public:
        enum PARSE_RESULT {SUCCEED, PARSE_ERR, USER_DEFINED_FORMAT_ERR};

    protected:
        AdvProfileObject  m_profileParserObject;
};


#endif /* INCLUDE_ADVPROFILEPARSER_H_ */
