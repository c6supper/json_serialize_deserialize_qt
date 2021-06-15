/*
 * advprofileserializer.h
 *
 *  Created on: Apr 2, 2020
 *      Author: duyi
 */

#ifndef INCLUDE_ADVPROFILESERIALIZER_H_
#define INCLUDE_ADVPROFILESERIALIZER_H_
#include "advprofileparser.h"

class AdvProfileSerializer: public AdvProfileParser
{
    public:

        AdvProfileSerializer()
        {
        }
        virtual ~AdvProfileSerializer()
        {
        }

        static AdvProfileSerializer *Instance()
        {
            static AdvProfileSerializer instance;
            return &instance;
        }

        void tofile()
        {
            std::ofstream advProfile(
                AdvProfileParser::profileLocation().c_str()); //created or opened with ios::out which corresponds to the stdio equivalent "w"

            if (advProfile.is_open()) {
                try {
                    toJson().serialize(std::ostream_iterator<char>(advProfile));
                } catch (...) {
                    advProfile.close();
                    return;
                }
            }

            advProfile.close();
        }

        std::string toString()
        {
            std::string json;

            try {
                json = toJson().serialize();
            } catch (...) {
                return json;
            }

            return json;
        }
};
#endif /* INCLUDE_ADVPROFILESERIALIZER_H_ */
