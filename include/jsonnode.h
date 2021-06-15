/*
 * jsonnode.h
 *
 *  Created on: Apr 8, 2020
 *      Author: duyi
 */

#ifndef INCLUDE_JSONNODE_H_
#define INCLUDE_JSONNODE_H_
#include "picojson.h"
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

class JsonNode
{

    public:
        JsonNode(picojson::value paraObject)
        {
            from(paraObject);
        }

        JsonNode()
        {
        }

        ~JsonNode()
        {
        }

        picojson::value to()
        {
            //objejct to value
            return picojson::value(m_value);
        }

        void from(const picojson::value &obj)
        {
            if (obj.is<picojson::object>()) {
                m_value = obj.get<picojson::object>();

                //filter the pairs whose key is neither "enable" nor "value" from stream
                for (picojson::object::iterator it = m_value.begin();
                     it != m_value.end(); ++it) {
                    if ((it->first != KeyNameEnable()) && (it->first != KeyNameValue())) {
                        m_value.erase(it);
                    }
                }

                //			the extreme cases
                //		    "techID": {
                //		      "": false,   the default value should be lost in this case, so we should filter it first.
                //		      "value": ""
                //		    }

                //set the default value for json node
                if ((!obj.contains(
                         KeyNameEnable()))//in case there is not a pair whose key is "enable" in the stream.
                    || (obj.contains(KeyNameEnable())
                        && !(m_value[KeyNameEnable()].is<bool>()))) {
                    m_value[KeyNameEnable()] = picojson::value(
                                                   true);//the enable status indicates that if we should get advanced data from testset
                }

                if ((!obj.contains(
                         KeyNameValue()))//in case there is not a pair whose key is "value" in the stream.
                    || (obj.contains(KeyNameValue())
                        && !(m_value[KeyNameValue()].is<std::string>()))) {
                    m_value[KeyNameValue()] = picojson::value(std::string(""));
                }
            } else {
                cout << "JSON node is not an object" << endl;
            }
        }

        static const std::string KeyNameEnable()
        {
            return std::string("enable");
        }

        static const std::string KeyNameValue()
        {
            return std::string("value");
        }

        picojson::value &operator[](const std::string
                                    keyOfItem) // to use this class like a map
        {
            if (!picojson::value(m_value).contains(keyOfItem)) {
                picojson::value v;
                m_value[keyOfItem] = v;
            }

            return m_value[keyOfItem];
        }



    private:
        picojson::object m_value;

};


#endif /* INCLUDE_JSONNODE_H_ */
