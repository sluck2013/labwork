/** Simple JSON class, which supports only simple JSON object.
 *  Note: INCOMPATIBLE with C++ 11+
 *  Usage: (a) To get JSON string, first create a JSON object, then 
 *         insert or remove key-value pairs into or from the 
 *         object. Finally call marshal() to get serialized string. 
 *         (b) To get JSON value, first call unmarshal() or parameterized
 *         constructor to get JSON object, then call get<Type>Val() to get value.
 *  @author Yansong Wang
 */
#include "JSON.h"
#include <iostream>

/** Creates JSON object from string.
 * 
 *  @param s given string JSON object is created from
 */
bbcp_JSON::bbcp_JSON(const string& s) {
    unmarshal(s);
}

/** Creates JSON object from c-style string.
 * 
 *  @param s c-style given string JSON object is created from
 */
bbcp_JSON::bbcp_JSON(const char* s) {
    unmarshal(s);
}

/** Marshals JSON object.
 *
 *  @return serialized JSON object.
 */
const string& bbcp_JSON::marshal() {
    if (iIsNew_) {
        return serialized_;
    }
    serialized_ = "{";
    map<string, string>::const_iterator it = map_.begin();
    for (; it != map_.end(); ++it) {
        if (it != map_.begin()) {
            serialized_ += ",";
        }
        serialized_ += it->first + ":" + toStr_(it->second);
    }
    serialized_ += "}";
    iIsNew_ = 1;
    return serialized_;
}

/** Unmarshal JSON object from c-sytle string.
 *
 *  @param p given c-style string.
 */
void bbcp_JSON::unmarshal(const char* p) {
    if (!p) {
        cerr << "unmarshalling null char array" << endl;
    }
    string s = string(p);
    unmarshal(s);
}

/** Unmarshal JSON object from string.
 *
 *  @param s given string.
 */
void bbcp_JSON::unmarshal(const string &s) {
    clear();
    serialized_ = s;
    int iStart = 0;
    string sKey = "", sVal = "";
    for (int i = 0; i < s.length(); ++i) {
        switch (s[i]) {
            case '{':
                iStart = i + 1;
                break;
            case ':':
                sKey = s.substr(iStart, i - iStart);
                iStart = i + 1;
                break;
            case ',':
            case '}':
                sVal = s.substr(iStart, i - iStart);
                map_[sKey] = sVal;
                sKey = sVal = "";
                iStart = i + 1;
                break;
            default:
                break;
        }
    }
    iIsNew_ = 1;
}

