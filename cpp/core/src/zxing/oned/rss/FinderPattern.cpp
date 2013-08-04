//
//  FinderPattern.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#include <zxing/oned/rss/FinderPattern.h>

using zxing::Ref;
using zxing::ResultPoint;
using zxing::ArrayRef;
using zxing::oned::rss::FinderPattern;

FinderPattern::FinderPattern(int value, std::vector<int> &startEnd, int start, int end, int rowNumber) {
    _value = value;
    _startEnd = startEnd;    
    Array< Ref<ResultPoint> >* resultPoints = new Array< Ref<ResultPoint> >();
    std::vector< Ref<ResultPoint> >& resultPointValues (resultPoints->values());
    resultPointValues.push_back(Ref<ResultPoint>(new ResultPoint(start, rowNumber)));
    resultPointValues.push_back(Ref<ResultPoint>(new ResultPoint(end, rowNumber)));
    _resultPoints = ArrayRef< Ref<ResultPoint> >(resultPoints);
}

FinderPattern::~FinderPattern() {
    
}

int FinderPattern::getValue() const {
    return _value;
}

std::vector<int> &FinderPattern::getStartEnd() {
    return _startEnd;
}

ArrayRef< Ref<ResultPoint> > FinderPattern::getResultPoints() {
    return _resultPoints;
}

bool FinderPattern::operator==(const FinderPattern &other) const {
    return _value == other._value;
}
