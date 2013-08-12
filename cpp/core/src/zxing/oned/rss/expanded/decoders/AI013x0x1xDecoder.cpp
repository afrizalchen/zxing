//
//  AI013x0x1xDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <sstream>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/expanded/decoders/AI013x0x1xDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI01weightDecoder;
using zxing::oned::rss::expanded::decoders::AI013x0x1xDecoder;

namespace {
    const int HEADER_SIZE = 7 + 1;
    const int WEIGHT_SIZE = 20;
    const int DATE_SIZE = 16;
}

AI013x0x1xDecoder::AI013x0x1xDecoder(Ref<BitArray> information, Ref<String> firstAIdigits, Ref<String> dateCode)
    : AI01weightDecoder(information) {
    _firstAIdigits = firstAIdigits;
    _dateCode = dateCode;
}

AI013x0x1xDecoder::~AI013x0x1xDecoder() {
}

Ref<String> AI013x0x1xDecoder::parseInformation() {
    if (getInformation()->getSize() != HEADER_SIZE + GTIN_SIZE + WEIGHT_SIZE + DATE_SIZE) {
        throw NotFoundException();
    }
    
    Ref<String> buf(new String(""));
    
    encodeCompressedGtin(buf, HEADER_SIZE);
    encodeCompressedWeight(buf, HEADER_SIZE + GTIN_SIZE, WEIGHT_SIZE);
    encodeCompressedDate(buf, HEADER_SIZE + GTIN_SIZE + WEIGHT_SIZE);
    
    return buf;
}

void AI013x0x1xDecoder::encodeCompressedDate(Ref<String> buf, int currentPos) {
    int numericDate = getGeneralDecoder()->extractNumericValueFromBitArray(currentPos, DATE_SIZE);
    if(numericDate == 38400) {
        return;
    }
    
    buf->append('(');
    buf->append(_dateCode->getText());
    buf->append(')');
    
    int day   = numericDate % 32;
    numericDate /= 32;
    int month = numericDate % 12 + 1;
    numericDate /= 12;
    int year  = numericDate;
    
    if (year / 10 == 0) {
        buf->append('0');
    }
    std::ostringstream oss;
    oss << year;
    buf->append(oss.str());
    
    if (month / 10 == 0) {
        buf->append('0');
    }
    oss.clear();
    oss.str("");
    oss << month;
    buf->append(oss.str());
    
    if (day / 10 == 0) {
        buf->append('0');
    }
    oss.clear();
    oss.str("");
    oss << day;
    buf->append(oss.str());
}

void AI013x0x1xDecoder::addWeightCode(Ref<String> buf, int weight) {
    int lastAI = weight / 100000;
    buf->append('(');
    buf->append(_firstAIdigits->getText());
    std::ostringstream oss;
    oss << lastAI;
    buf->append(oss.str());
    buf->append(')');
}

int AI013x0x1xDecoder::checkWeight(int weight) {
    return weight % 100000;
}