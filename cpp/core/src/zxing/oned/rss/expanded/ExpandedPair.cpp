//
//  ExpandedPair.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#include <zxing/oned/rss/expanded/ExpandedPair.h>

using zxing::Ref;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::FinderPattern;
using zxing::oned::rss::expanded::ExpandedPair;

ExpandedPair::ExpandedPair(Ref<DataCharacter> leftChar, Ref<DataCharacter> rightChar, Ref<FinderPattern> finderPattern, bool mayBeLast) {
    _leftChar = leftChar;
    _rightChar = rightChar;
    _finderPattern = finderPattern;
    _mayBeLast = mayBeLast;
}

bool ExpandedPair::mayBeLast(){
    return _mayBeLast;
}

Ref<DataCharacter> ExpandedPair::getLeftChar() const {
    return _leftChar;
}

Ref<DataCharacter> ExpandedPair::getRightChar() const {
    return _rightChar;
}

Ref<FinderPattern> ExpandedPair::getFinderPattern() const {
    return _finderPattern;
}

bool ExpandedPair::mustBeLast() const {
    return _rightChar.empty();
}

bool ExpandedPair::operator==(const ExpandedPair &other) const {
    return this->equals(other);
}

bool ExpandedPair::equals(const ExpandedPair &other) const {
    if (!(_leftChar.empty() == other._leftChar.empty())) return false;
    if (_leftChar.empty()) return false;
    if (!_leftChar->equals(other._leftChar)) return false;
    
    if (!(_rightChar.empty() == other._rightChar.empty())) return false;
    if (_rightChar.empty()) return false;
    if (!_rightChar->equals(other._rightChar)) return false;
    
    if (!(_finderPattern.empty() == other._finderPattern.empty())) return false;
    if (_finderPattern.empty()) return false;
    if (!_finderPattern->equals(other._finderPattern)) return false;
    return true;
}

bool ExpandedPair::equals(Ref<ExpandedPair> other) const {
    if (other.empty()) return false;
    return this->equals(*(other.object_));
}
