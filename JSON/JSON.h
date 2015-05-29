/** Simple JSON class, which supports only simple JSON object.
 *  Note: INCOMPATIBLE with C++ 11+
 *  Usage: (a) To get JSON string, first create a JSON object, then 
 *         insert or remove key-value pairs into or from the 
 *         object. Finally call marshal() to get serialized string. 
 *         (b) To get JSON value, first call unmarshal() or parameterized
 *         constructor to get JSON object, then call get<Type>Val() to get value.
 *  @author Yansong Wang
 */
#ifndef BBCP_JSON_H
#define BBCP_JSON_H

#include <map>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

class bbcp_JSON {
public:
    bbcp_JSON(){};
    bbcp_JSON(const string& s);
    bbcp_JSON(const char* s);
    
    /** inserts key-value pair into JSON object.
     *  
     *  @param key key name to be inserted.
     *  @param val value to be inserted.
     */
    template<typename T>
    void insert(const string &key, const T &val) {
        map_[key] = toStr_(val);
        iIsNew_ = 0;
    };

    /** Removes key-value pair from JSON object.
     *  
     *  @param key key name to be removed.
     */
    inline void remove(const string &key) {
        map_.erase(key);
    };

    /** Clears JSON object.
     */
    inline void clear() {
        map_.clear();
        serialized_ = "";
    };

    const string& marshal();
    void unmarshal(const string& s);
    void unmarshal(const char* s);

    /** Gets integer value from JSON object.
     *
     *  @param key key associated with which value is returned.
     *  @return returned integer value.
     */
    inline int getIntVal(const string &key) {
        return atoi(map_[key].c_str());
    };

    /** Gets string value from JSON object.
     *
     *  @param key key associated with which value is returned.
     *  @return returned string value.
     */
    string getStrVal(const string &key) {
        return map_[key];
    };

private:
    map<string, string> map_;
    string serialized_;
    int iIsNew_;
    
    /** Converts inserted value to string.
     *
     *  @param s inserted value.
     *  @return converted string.
     */
    inline string toStr_(const string& s) const {
        return s;
    }
    
    /** Converts inserted value to string.
     *
     *  @param pc inserted value.
     *  @return converted string.
     */
    inline string toStr_(char* pc) const {
        string s(pc);
        return s;
    }
 
    /** Converts inserted value to string.
     *
     *  @param i inserted value.
     *  @return converted string.
     */
    inline string toStr_(int i) const {
        stringstream ss;
        string s;
        ss << i;
        ss >> s;
        return s;
    }
};

#endif
