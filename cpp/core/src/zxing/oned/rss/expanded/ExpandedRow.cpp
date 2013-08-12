//
//  ExpandedRow.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#include <zxing/oned/rss/expanded/ExpandedRow.h>

using zxing::Ref;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::FinderPattern;
using zxing::oned::rss::expanded::ExpandedPair;
using zxing::oned::rss::expanded::ExpandedRow;

ExpandedRow::ExpandedRow(std::vector< Ref<ExpandedPair> > &pairs, int rowNumber, bool wasReversed)
    : _pairs(pairs) {
    _rowNumber = rowNumber;
    _wasReversed = wasReversed;
}

std::vector< Ref<ExpandedPair> > &ExpandedRow::getPairs() {
    return _pairs;
}

int ExpandedRow::getRowNumber() {
    return _rowNumber;
}

bool ExpandedRow::isReversed() {
    return _wasReversed;
}

bool ExpandedRow::isEquivalent(std::vector< Ref<ExpandedPair> > &otherPairs) {
    if ( _pairs.size() != otherPairs.size() ) return false;
    bool equal = true;
    for(std::vector<int>::size_type i = 0; i != _pairs.size(); i++) {
        if (!(_pairs[i]->equals(*(otherPairs[i])))) {
            equal = false;
            break;
        }
    }
    return equal;
}

bool ExpandedRow::operator==(const ExpandedRow &other) const {
    if (_wasReversed != other._wasReversed) return false;
    //return std::equal(_pairs.begin(), _pairs.end(), other._pairs.begin());
    bool equal = true;
    for(std::vector<int>::size_type i = 0; i != _pairs.size(); i++) {
        if (!(_pairs[i]->equals(*(other._pairs[i].object_)))) {
            equal = false;
            break;
        }
    }
    return equal;

}
