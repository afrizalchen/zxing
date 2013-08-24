//
//  AbstractExpandedDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#include <zxing/IllegalStateException.h>
#include <zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI013103decoder.h>
#include <zxing/oned/rss/expanded/decoders/AI01320xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI01392xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI01393xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI013x0x1xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI01AndOtherAIs.h>
#include <zxing/oned/rss/expanded/decoders/AnyAIDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::oned::rss::expanded::decoders::AbstractExpandedDecoder;
using zxing::oned::rss::expanded::decoders::GeneralAppIdDecoder;

AbstractExpandedDecoder::AbstractExpandedDecoder(Ref<BitArray> information) {
    _information = information;
    _generalDecoder = Ref<GeneralAppIdDecoder>(new GeneralAppIdDecoder(information));
}

AbstractExpandedDecoder::~AbstractExpandedDecoder() {

}

Ref<BitArray> AbstractExpandedDecoder::getInformation() {
    return _information;
}

Ref<GeneralAppIdDecoder> AbstractExpandedDecoder::getGeneralDecoder() {
    return _generalDecoder;
}

Ref<AbstractExpandedDecoder> AbstractExpandedDecoder::createDecoder(Ref<BitArray> information) {
    if (information->get(1)) {
        return Ref<AbstractExpandedDecoder>(new AI01AndOtherAIs(information));
    }
    if (!information->get(2)) {
        return Ref<AbstractExpandedDecoder>(new AnyAIDecoder(information));
    }
    
    int fourBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 4);
    
    switch(fourBitEncodationMethod){
        case 4: return Ref<AbstractExpandedDecoder>(new AI013103decoder(information));
        case 5: return Ref<AbstractExpandedDecoder>(new AI01320xDecoder(information));
    }
    
    int fiveBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 5);
    switch(fiveBitEncodationMethod){
        case 12: return Ref<AbstractExpandedDecoder>(new AI01392xDecoder(information));
        case 13: return Ref<AbstractExpandedDecoder>(new AI01393xDecoder(information));
    }
    
    int sevenBitEncodationMethod = GeneralAppIdDecoder::extractNumericValueFromBitArray(information, 1, 7);
    switch(sevenBitEncodationMethod){
        case 56: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("310")), Ref<String>(new String("11"))));
        case 57: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("320")), Ref<String>(new String("11"))));
        case 58: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("310")), Ref<String>(new String("13"))));
        case 59: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("320")), Ref<String>(new String("13"))));
        case 60: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("310")), Ref<String>(new String("15"))));
        case 61: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("320")), Ref<String>(new String("15"))));
        case 62: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("310")), Ref<String>(new String("17"))));
        case 63: return Ref<AbstractExpandedDecoder>(new AI013x0x1xDecoder(information, Ref<String>(new String("320")), Ref<String>(new String("17"))));
    }
    
    throw new IllegalStateException("unknown decoder: ");// + information);
}