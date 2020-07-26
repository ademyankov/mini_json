//
// Minimalistic Json parser
//
// Copyright(c) 2020 Alex Demyankov <alex.demyankov@gmail.com>
// All rights reserved.
//
// Licensed under the MIT license; A copy of the license that can be
// found in the LICENSE file.
//

//
// Supported values:
//    - "string"
//    - [array of "strings"] (no nested arrays)
//    - {object}
//
// Lang: C++17 (can be easily converted to C++14 by refactoring static inline initialization)
//
// Mini Json parser finite state machine is based on Matthew Endsley's idea for HTTP parser.
//
// The original article can be found at:
// http://mendsley.github.io/2012/12/19/tinyhttp.html
//

#include <iostream>
#include <vector>
#include <array>
#include <map>

namespace mjson {

class json {
public:
    json(const std::string& s) : s_(s), pos_(0), state_(0) {
        if (s_.empty()) return;

        do {
            // Transition matrix gives the next state depending on
            // the current state and the current character
            char move = transition_[state_][dictionary_[s_.at(pos_)]];
            if (move < 0) {
                state_ = move;
                break;
            }

            // Extract the next state before the action on the current move
            // is performed because the action can change the state
            state_ = move & 0x0f;
            if (actions_[move]) (this->*actions_[move])();

        } while(++pos_ < s_.length() && !(state_ < 0));

        state_ = pos_ == s_.length() ? -1 : state_;
        if (state_ == -1) clear();
    }

    json() = default;
    ~json() = default;

    using Array = std::vector<std::string>;

    bool is_valid() { return state_ == -2 ? true : false;  };

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

    void clear() {
        kom_.clear();
        kam_.clear();
        kvm_.clear();
    }

    //
    // Transition matrix.
    //
    //        code (dictionary)
    //       default (if none of the dictionary chars has been found)
    //         0     1     2     3     4     5     6     7     8     9     a     b
    //         *    ' '   \t    \n    \r     "     :     ,     {     }     [     ]     state (transition)
    // -1 is format error; -2 is valid json string is finished
    static constexpr char transition_[9][12] = {
    /*0*/  {   -1,    0,    0,    0,    0,   -1,   -1,   -1,    1,   -1,   -1,   -1 }, // 0 - Json header
    /*1*/  {   -1,    1,    1,    1,    1, 0x12,   -1,   -1,   -1,   -2,   -1,   -1 }, // 1 - Key; 12 - key start; or '}' - object end
    /*2*/  { 0x22, 0x22,   -1,   -1,   -1, 0x13, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 }, // 2 - Key; 22 - consume 13 - end
    /*3*/  {   -1,    3,    3,    3,    3,   -1,    4,   -1,   -1,   -1,   -1,   -1 }, // 3 - ':' separator expected
    /*4*/  {   -1,    4,    4,    4,    4, 0x15,   -1,   -1, 0x19,   -1, 0x18,   -1 }, // 4 - Value; 15 - start, 18 - array start, 19 - object start
    /*5*/  { 0x25, 0x25,   -1,   -1,   -1, 0x16, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25 }, // 5 - Value; 25 - consume 16 - end
    /*6*/  {   -1,    6,    6,    6,    6,   -1,   -1,    7,   -1,   -2,   -1, 0x26 }, // 6 - ',' sequence or '}' - object end
    /*7*/  {   -1,    7,    7,    7,    7, 0x12,   -1,   -1,   -1,   -1,   -1,   -1 }, // 7 - The next key should start
    /*8*/  {   -1,    8,    8,    8,    8, 0x15,   -1,   -1,   -1,   -1,   -1, 0x26 }, // 8 - Array;
    /*X*///{   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 }, // X - 
    };

    void onKeyChar() { k_ += s_.at(pos_); }
    void onValueChar() { v_ += s_.at(pos_); }
    void onArrayBegin() { is_array_ = true; }
    void onKeyStart() { state_ = is_array_ ? 5 : state_; }
    void onKeyEnd() { state_ = (kvm_.find(k_) != kvm_.end()) ? -1 : state_; }
    void onValueStart() { }

    void onObjectBegin() {
        json obj(s_.data() + pos_);
        if (!obj.is_valid()) {
            state_ = -1;
            return;
        }

        kom_.insert(KeyObjectMap::value_type(k_, std::move(obj)));
        k_.clear();
        pos_ += obj.pos_;
        state_ = 6;
    }

    void onArrayEnd() {
        if (!is_array_) {
            state_ = -1;
            return;
        }

        is_array_ = false;
        kam_.insert(KeyArrayMap::value_type(k_, array_));
        array_.clear();
        k_.clear();
    }

    void onValueEnd() {
        if (is_array_) {
            array_.push_back(v_);
            v_.clear();
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
        dic['\r'] = 4;
        dic['"'] = 5;
        dic[':'] = 6;
        dic[','] = 7;
        dic['{'] = 8;
        dic['}'] = 9;
        dic['['] = 10;
        dic[']'] = 11;

        return dic;
    }();

    const Actions actions_ = []() {
        Actions h{};

        h[0x12] = &json::onKeyStart;
        h[0x13] = &json::onKeyEnd;
        h[0x15] = &json::onValueStart;
        h[0x16] = &json::onValueEnd;
        h[0x18] = &json::onArrayBegin;
        h[0x19] = &json::onObjectBegin;
        h[0x22] = &json::onKeyChar;
        h[0x25] = &json::onValueChar;
        h[0x26] = &json::onArrayEnd;

        return h;
    }();
};

} // namespace mjson
