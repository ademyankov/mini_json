#include <iostream>
#include <vector>
#include <array>
#include <map>

namespace mjson {

//
// Mini Json parser
// Supported values: strings, array of strings
// Lang: C++17
//
class json {
public:
    json(const std::string& s) : s_(s), pos_(0), state_(0), is_done_(false) {
        // Json starts
        pos_ = s_.find('{');
        if (pos_ == std::string::npos) return;

        // Parser
        while (++pos_ < s_.length()) {
            char op = transition_[state_][dictionary_[s_.at(pos_)]];
            if (op < 0) {
                // Format error or done parsing
                is_done_ = op == -2 ? true: false;
                return;
            }

            if (actions_[op]) (this->*actions_[op])();
            state_ = op & 0x0f;
        }
    }

    json() = default;
    ~json() = default;

    using Array = std::vector<std::string>;

    bool is_valid() { return is_done_; };

    std::string const& operator[] (const std::string& key) { return kvm_[key]; }

    bool has(const std::string& key) { return kvm_.find(key) != kvm_.end(); }
    std::string const& get (const std::string& key) { return kvm_[key]; }

    bool has_array(const std::string& key) { return kam_.find(key) != kam_.end(); }
    Array const& get_array(const std::string& key) { return kam_[key]; }

    bool has_object(const std::string& key) { return kom_.find(key) != kom_.end(); }
    json& get_object(const std::string& key) { return kom_[key]; }

    size_t size() { return kvm_.size() + kam_.size() + kom_.size(); };

private:
    std::string s_{};
    size_t pos_{};
    char state_{};
    bool is_done_{};

    using KeyObjectMap = std::map<std::string, json>;
    using KeyValueMap = std::map<std::string, std::string>;
    using KeyArrayMap = std::map<std::string, Array>;
    using Dictionary = std::array<char, 126>;
    using Action = void (json::*)();
    using Actions = std::array<Action, 0x31>;

    std::string k_{};
    std::string v_{};

    bool is_array_{};
    Array array_{};

    KeyObjectMap kom_{};
    KeyArrayMap kam_{};
    KeyValueMap kvm_{};

    void onKeyChar() { k_ += s_.at(pos_); }
    void onValueChar() { v_ += s_.at(pos_); }
    void onArrayBegin() { is_array_ = true; }

    void onObjectBegin() {
        json obj(s_.data() + pos_);
        if (obj.is_valid()) {
            kom_.insert(KeyObjectMap::value_type(k_, std::move(obj)));
        }
    }

    void onArrayItem() {
        array_.push_back(v_);
        v_.clear();
    }

    void onArrayDone() {
        is_array_ = false;
        kam_.insert(KeyArrayMap::value_type(k_, array_));
        array_.clear();
        k_.clear();
    }

    void onKVPairDone() {
        if (is_array_) {
            onArrayItem();
            state_ = 3;
            return;
        }

        kvm_.insert(KeyValueMap::value_type(k_, v_));
        k_.clear();
        v_.clear();
    }

    static constexpr Dictionary dictionary_ = []() {
        Dictionary dic{};

        dic[' '] = 1;
        dic['\t'] = 2;
        dic['\n'] = 3;
        dic['"'] = 4;
        dic[':'] = 5;
        dic[','] = 6;
        dic['{'] = 7;
        dic['}'] = 8;
        dic['['] = 9;
        dic[']'] = 10;

        return dic;
    }();

    const Actions actions_ = []() {
        Actions h{};

        h[0x10] = &json::onKVPairDone;
        h[0x11] = &json::onKeyChar;
        h[0x14] = &json::onValueChar;
        h[0x15] = &json::onValueChar;
        h[0x13] = &json::onArrayBegin;
        h[0x23] = &json::onArrayDone;
        h[0x30] = &json::onObjectBegin;

        return h;
    }();

    // Transition matrix. A primitive state machine with json parsing rules
    // 0xXX - XX byte is an action, where :4 lower bits is a transition to a state;
    // -1 is a format error; -2 is done parsing
    static constexpr char transition_[6][11] = {
        //        code (dictionary)
        //         0     1     2     3     4     5     6     7     8     9    10
        //         *    ' '   \t    \n     "     :     ,     {     }     [     ]      state (transition)
        /*0*/ {   -1,    0,    0,    0,    1,   -1,    3,   -1,   -2,   -1, 0x23 },   // 0 - Main loop: Key Value pair starts/ends
        /*1*/ { 0x11,   -1,   -1,   -1,    2,   -1,   -1,   -1,   -1,   -1,   -1 },   // 1 - Key Name assembling
        /*2*/ {   -1,    2,    2,    2,   -1,    3,   -1,   -1,   -1,   -1,   -1 },   // 2 - Value/Array/Object begins
        /*3*/ { 0x14,    3,    3,    0,    5,   -1,    0, 0x30,   -2, 0x13, 0x23 },   // 3 - Whitespase between dictionary chars
        /*4*/ { 0x14, 0x10, 0x10, 0x10,   -1,   -1, 0x10,   -1,   -1,   -1,   -1 },   // 4 - Value String assembling (w/o whitespace chars)
        /*5*/ { 0x15, 0x15, 0x15, 0x15, 0x10, 0x15, 0x15,   -1, 0x15, 0x15, 0x15 }    // 5 - Value String assembling (with all chars available)
    };


    //static constexpr char tr_[6][11] = {
    //    //        code (dictionary)
    //    //         0     1     2     3     4     5     6     7     8     9    10
    //    //         *    ' '   \t    \n     "     :     ,     {     }     [     ]     state (transition)
    //    /*X*/ {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 }, // X - 

    //    /*0*/ {   -1,    0,    0,    0,   -1,   -1,   -1,    1,   -1,   -1,   -1 }, // 0 - Json header
    //    /*1*/ {   -1,    1,    1,    1, 0x12,   -1,   -1,   -1,   -2,   -1,   -1 }, // 1 - Main loop: 12 - key start
    //    /*2*/ { 0x22,   -1,   -1,   -1, 0x13,   -1,   -1,   -1,   -1,   -1,   -1 }, // 3 - Key; 22 - consume 13 - end
    //    /*3*/ {   -1,    3,    3,    3,   -1, 0x14,   -1,   -1,   -1,   -1,   -1 }, // 3 - Value; 14 - start
    //    /*4*/ { 0x16,    4,    4,    4,    5,   -1,   -1,   -1,   -1,   -1,   -1 }, // 4 - Value; split to 5 or 6
    //    /*5*/ { 0x15, 0x15, 0x15, 0x15, 0x31, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15 }, // 5 - Value; 5 - string in quotes ""; 15,16 - consume
    //    /*6*/ { 0x16, 0x31,   -1,   -1,   -1,   -1, 0x31,   -1,   -1,   -1,   -1 }, // 6 - Value; 6 - start string w/o quotes "" 31 - end


    //}
};

} // namespace tiny
