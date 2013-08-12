//
//  BlockParsedResult.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 6/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/BlockParsedResult.h>

using zxing::Ref;
using zxing::oned::rss::expanded::decoders::DecodedInformation;
using zxing::oned::rss::expanded::decoders::BlockParsedResult;

BlockParsedResult::BlockParsedResult(bool finished) {
    _decodedInformation = Ref<DecodedInformation>();
    _finished = finished;
}

BlockParsedResult::BlockParsedResult(Ref<DecodedInformation> information, bool finished) {
    _decodedInformation = information;
    _finished = finished;
}

Ref<DecodedInformation> BlockParsedResult::getDecodedInformation() {
    return _decodedInformation;
}

bool BlockParsedResult::isFinished() {
    return _finished;
}