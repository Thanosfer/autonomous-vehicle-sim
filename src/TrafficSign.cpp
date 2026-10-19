#include "TrafficSign.hpp"
#include <iostream>

TrafficSign::TrafficSign(const std::string& id, const Position& pos, SignType signType, const std::string& signText) : StaticObject(id, 'S', pos, "Traffic Sign"), type(signType), text(signText) {
    
    // Αυτόματο κείμενο ανά τύπο
    if (text.empty()) {
        switch (type) {
            case STOP_SIGN: text = "STOP"; break;
            case SPEED_LIMIT: text = "SPEED_LIMIT_50"; break;
            case YIELD: text = "YIELD"; break;
            default: text = "SIGN"; break;
        }
    }
    
    std::cout << "[+] Created sign " << id << " (" << text << ") at (" << pos.getX() << "," << pos.getY() << ")" << std::endl;
}

SignType TrafficSign::getSignType() const {
    return type;
}

std::string TrafficSign::getText() const {
    return text;
}

char TrafficSign::getGlyph() const {
    switch (type) {
        case STOP_SIGN: return 'S';
        case SPEED_LIMIT: return 'L';
        case YIELD: return 'Y';
        default: return '?';
    }
}

bool TrafficSign::affectsMovement() const {
    // Μόνο οι πινακίδες STOP και YIELD επηρεάζουν την κίνηση
    return type == STOP_SIGN || type == YIELD;
}

std::string TrafficSign::getInfo() const {
    std::string info = StaticObject::getInfo();
    info += ", Text: " + text;
    return info;
}